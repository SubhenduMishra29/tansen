import torch
import torch.nn as nn
import torch.fft as fft
import numpy as np
import sounddevice as sd
import mido
import threading

DEVICE = "cuda" if torch.cuda.is_available() else "cpu"

SR = 44100
BLOCK = 256
OS = 2
DT = 1 / (SR * OS)
MAX_VOICES = 16

# =========================================================
# POLYPHASE FIR OVERSAMPLER
# =========================================================

class PolyphaseOversampler:
    def __init__(self, factor=2, taps=64):
        self.factor = factor
        self.taps = taps
        self.kernel = torch.hamming_window(taps).to(DEVICE)
        self.kernel /= self.kernel.sum()

    def upsample(self, x):
        up = torch.zeros(len(x)*self.factor, device=DEVICE)
        up[::self.factor] = x
        return torch.conv1d(
            up.view(1,1,-1),
            self.kernel.view(1,1,-1),
            padding=self.taps//2
        ).view(-1)

    def downsample(self, x):
        filtered = torch.conv1d(
            x.view(1,1,-1),
            self.kernel.view(1,1,-1),
            padding=self.taps//2
        ).view(-1)
        return filtered[::self.factor]

# =========================================================
# PARTITIONED CONVOLUTION (LOW LATENCY)
# =========================================================

class PartitionedConvolver:
    def __init__(self, ir, partition_size=512):
        self.partition = partition_size
        self.ir = ir.to(DEVICE)
        self.ir_parts = []
        for i in range(0, len(ir), partition_size):
            part = ir[i:i+partition_size]
            self.ir_parts.append(fft.rfft(part, n=partition_size*2))

        self.buffer = torch.zeros(partition_size*2, device=DEVICE)

    def process(self, x):
        n = self.partition
        self.buffer = torch.roll(self.buffer, -n)
        self.buffer[-n:] = x
        X = fft.rfft(self.buffer)

        Y = torch.zeros_like(X)
        for i, H in enumerate(self.ir_parts):
            Y += X * H

        y = fft.irfft(Y)
        return y[n:2*n]

# =========================================================
# FREQUENCY-DEPENDENT WAVEGUIDE
# =========================================================

class Waveguide:
    def __init__(self, length=2048):
        self.buffer = torch.zeros(length, device=DEVICE)
        self.idx = 0
        self.length = length
        self.lowpass = 0.98

    def process(self, input_sample):
        delayed = self.buffer[self.idx]
        reflected = self.lowpass * delayed
        self.buffer[self.idx] = input_sample + reflected
        self.idx = (self.idx + 1) % self.length
        return delayed

# =========================================================
# BELLOWS ODE SYSTEM
# =========================================================

class Bellows:
    def __init__(self):
        self.pressure = torch.tensor(1.0, device=DEVICE)
        self.compliance = 0.01
        self.pump_rate = 0.5

    def step(self, airflow):
        dP = (self.pump_rate - airflow) / self.compliance
        self.pressure += DT * dP
        return self.pressure

# =========================================================
# REED WITH IMPEDANCE FEEDBACK
# =========================================================

class ReedVoice:
    def __init__(self, freq):
        self.freq = freq
        self.x = torch.zeros(1, device=DEVICE)
        self.v = torch.zeros(1, device=DEVICE)
        self.mass = 1e-4
        self.k = (2*np.pi*freq)**2 * self.mass
        self.r = 0.001

    def step(self, P_b, P_c):

        force = P_b - P_c - self.k*self.x - self.r*self.v
        a = force / self.mass
        self.v += DT * a
        self.x += DT * self.v

        airflow = torch.relu(self.x) * self.freq * 0.001
        return torch.tanh(10*self.x), airflow

# =========================================================
# PROFESSIONAL VOICE ALLOCATION
# =========================================================

class VoiceManager:
    def __init__(self):
        self.voices = {}

    def note_on(self, note):
        freq = 440 * 2 ** ((note - 69) / 12)
        if len(self.voices) >= MAX_VOICES:
            oldest = list(self.voices.keys())[0]
            del self.voices[oldest]
        self.voices[note] = ReedVoice(freq)

    def note_off(self, note):
        if note in self.voices:
            del self.voices[note]

# =========================================================
# MAIN ENGINE
# =========================================================

class HarmoniumEngine:
    def __init__(self):
        self.voice_manager = VoiceManager()
        self.waveguide = Waveguide()
        self.bellows = Bellows()
        self.oversampler = PolyphaseOversampler(OS)

        ir = torch.zeros(2048)
        ir[0] = 1.0
        self.convolver = PartitionedConvolver(ir)

    def generate_block(self):
        block = torch.zeros(BLOCK, device=DEVICE)

        for i in range(BLOCK):
            cavity_pressure = torch.tensor(0.0, device=DEVICE)
            airflow_total = torch.tensor(0.0, device=DEVICE)

            P_b = self.bellows.pressure

            for voice in self.voice_manager.voices.values():
                reed_out, airflow = voice.step(P_b, cavity_pressure)
                airflow_total += airflow
                cavity_pressure += reed_out

            cavity_pressure = self.waveguide.process(cavity_pressure)
            self.bellows.step(airflow_total)

            block[i] = cavity_pressure

        block = self.oversampler.upsample(block)
        block = self.convolver.process(block[:512])
        block = self.oversampler.downsample(block)

        return block.detach().cpu().numpy()

# =========================================================
# REALTIME AUDIO + MIDI
# =========================================================

engine = HarmoniumEngine()

def audio_callback(outdata, frames, time, status):
    audio = engine.generate_block()
    outdata[:] = audio.reshape(-1,1)

def midi_thread():
    with mido.open_input() as port:
        for msg in port:
            if msg.type == "note_on" and msg.velocity > 0:
                engine.voice_manager.note_on(msg.note)
            elif msg.type == "note_off":
                engine.voice_manager.note_off(msg.note)

threading.Thread(target=midi_thread, daemon=True).start()

with sd.OutputStream(
    samplerate=SR,
    blocksize=BLOCK,
    channels=1,
    callback=audio_callback,
):
    print("Full Research Harmonium Engine Running...")
    while True:
        pass
