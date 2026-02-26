import torch
import torch.nn as nn
import torch.fft as fft
import sounddevice as sd
import numpy as np
import mido
import threading

# ==============================
# CONFIGURATION
# ==============================

CONFIG = {
    "sample_rate": 44100,
    "block_size": 256,
    "oversample": 2,
    "max_voices": 16,
    "integrator": "rk2",  # "rk2" or "rk4"
    "device": "cuda" if torch.cuda.is_available() else "cpu",
    "ir_path": None
}

SR = CONFIG["sample_rate"]
OS = CONFIG["oversample"]
DEVICE = CONFIG["device"]

# ==============================
# INTEGRATORS
# ==============================

def rk2(f, y, dt):
    k1 = f(y)
    k2 = f(y + dt * k1)
    return y + dt * 0.5 * (k1 + k2)

def rk4(f, y, dt):
    k1 = f(y)
    k2 = f(y + dt * 0.5 * k1)
    k3 = f(y + dt * 0.5 * k2)
    k4 = f(y + dt * k3)
    return y + dt * (k1 + 2*k2 + 2*k3 + k4) / 6

# ==============================
# REED MODEL
# ==============================

class ReedVoice:
    def __init__(self, freq):
        self.freq = freq
        self.phase = torch.zeros(1, device=DEVICE)
        self.velocity = torch.zeros(1, device=DEVICE)
        self.pressure = torch.tensor(1.0, device=DEVICE)
        self.mass = 1e-4
        self.stiffness = (2*np.pi*freq)**2 * self.mass
        self.damping = 0.0005

    def step(self, dt):

        def dynamics(state):
            x = state[0]
            v = state[1]
            force = self.pressure - self.stiffness*x - self.damping*v
            dx = v
            dv = force / self.mass
            return torch.stack([dx, dv])

        state = torch.stack([self.phase, self.velocity])
        if CONFIG["integrator"] == "rk4":
            state = rk4(dynamics, state, dt)
        else:
            state = rk2(dynamics, state, dt)

        self.phase, self.velocity = state
        return torch.tanh(10 * self.phase)

# ==============================
# WAVEGUIDE CAVITY
# ==============================

class Waveguide:
    def __init__(self, length=2048):
        self.buffer = torch.zeros(length, device=DEVICE)
        self.idx = 0
        self.length = length

    def process(self, x):
        out = self.buffer[self.idx]
        self.buffer[self.idx] = x + 0.98 * out
        self.idx = (self.idx + 1) % self.length
        return out

# ==============================
# IR CONVOLUTION
# ==============================

class IRConvolver:
    def __init__(self, ir):
        self.ir = ir.to(DEVICE)
        self.ir_fft = fft.rfft(self.ir)

    def process(self, x):
        X = fft.rfft(x)
        Y = X * self.ir_fft
        return fft.irfft(Y)

# ==============================
# NEURAL RESIDUAL MODEL
# ==============================

class ResidualNet(nn.Module):
    def __init__(self):
        super().__init__()
        self.net = nn.Sequential(
            nn.Conv1d(1, 16, 9, padding=4),
            nn.ReLU(),
            nn.Conv1d(16, 16, 9, padding=4),
            nn.ReLU(),
            nn.Conv1d(16, 1, 9, padding=4)
        )

    def forward(self, x):
        return x + 0.1 * self.net(x)

# ==============================
# POLYPHONIC ENGINE
# ==============================

class HarmoniumEngine:
    def __init__(self):
        self.voices = []
        self.waveguide = Waveguide()
        self.residual = ResidualNet().to(DEVICE)
        self.dt = 1.0 / (SR * OS)

    def note_on(self, midi_note):
        freq = 440 * 2 ** ((midi_note - 69) / 12)
        if len(self.voices) < CONFIG["max_voices"]:
            self.voices.append(ReedVoice(freq))

    def note_off(self, midi_note):
        self.voices = self.voices[:-1]

    def generate_block(self):

        block = torch.zeros(CONFIG["block_size"] * OS, device=DEVICE)

        for i in range(len(block)):
            sample = torch.zeros(1, device=DEVICE)
            for v in self.voices:
                sample += v.step(self.dt)
            sample /= max(len(self.voices), 1)
            sample = self.waveguide.process(sample)
            block[i] = sample

        block = block.view(1,1,-1)
        block = self.residual(block)
        block = block.view(-1)

        # Downsample
        block = block[::OS]
        return block.detach().cpu().numpy()

# ==============================
# REAL-TIME AUDIO CALLBACK
# ==============================

engine = HarmoniumEngine()

def audio_callback(outdata, frames, time, status):
    audio = engine.generate_block()
    outdata[:] = audio.reshape(-1,1)

# ==============================
# MIDI THREAD
# ==============================

def midi_thread():
    with mido.open_input() as port:
        for msg in port:
            if msg.type == 'note_on' and msg.velocity > 0:
                engine.note_on(msg.note)
            elif msg.type == 'note_off':
                engine.note_off(msg.note)

threading.Thread(target=midi_thread, daemon=True).start()

# ==============================
# START AUDIO
# ==============================

with sd.OutputStream(
    samplerate=SR,
    blocksize=CONFIG["block_size"],
    channels=1,
    callback=audio_callback,
):
    print("Harmonium Engine Running...")
    while True:
        pass
