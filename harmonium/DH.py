import numpy as np
import sounddevice as sd
from pynput import keyboard
from scipy.signal import lfilter

fs = 44100
block_size = 256

# -----------------------------
# Global Bellows Pressure
# -----------------------------
bellows_pressure = 0.6

# -----------------------------
# Key Mapping (Keyboard → Frequency)
# -----------------------------
key_map = {
    'z': 261.63,  # Sa
    'x': 293.66,  # Re
    'c': 329.63,  # Ga
    'v': 349.23,  # Ma
    'b': 392.00,  # Pa
    'n': 440.00,  # Dha
    'm': 493.88,  # Ni
    ',': 523.25   # Sa (upper)
}

# -----------------------------
# Reed Voice Class
# -----------------------------
class ReedVoice:
    def __init__(self, freq):
        self.m = 0.002
        self.k = (2*np.pi*freq)**2 * self.m
        self.c = 0.0008
        self.beta = 800
        
        self.x = 0.0
        self.v = 0.0
        self.active = True
        self.freq = freq
        
        # Slight detune per voice
        self.detune = np.random.uniform(-2, 2)
        self.freq *= 2**(self.detune/1200)

    def process(self, frames):
        global bellows_pressure
        dt = 1/fs
        out = np.zeros(frames)

        for i in range(frames):
            pressure = bellows_pressure + 0.02*np.random.randn()
            F_air = pressure * (1 - self.beta * self.x)
            a = (F_air - self.c*self.v - self.k*self.x) / self.m
            self.v += a * dt
            self.x += self.v * dt
            out[i] = self.v
        
        return out

voices = {}

# -----------------------------
# Resonator Filter
# -----------------------------
def resonator(signal, freq=750, Q=8):
    w0 = 2*np.pi*freq/fs
    alpha = np.sin(w0)/(2*Q)

    b0 = alpha
    b1 = 0
    b2 = -alpha
    a0 = 1 + alpha
    a1 = -2*np.cos(w0)
    a2 = 1 - alpha

    b = np.array([b0/a0, b1/a0, b2/a0])
    a = np.array([1, a1/a0, a2/a0])

    return lfilter(b, a, signal)

# -----------------------------
# Audio Callback
# -----------------------------
def audio_callback(outdata, frames, time, status):
    global voices
    
    mix = np.zeros(frames)
    
    for key in list(voices):
        voice = voices[key]
        mix += voice.process(frames)
    
    if len(voices) > 0:
        mix /= len(voices)
    
    mix = resonator(mix)
    
    mix /= np.max(np.abs(mix)) + 1e-6
    outdata[:] = mix.reshape(-1,1)

# -----------------------------
# Keyboard Controls
# -----------------------------
def on_press(key):
    global voices, bellows_pressure
    
    try:
        k = key.char
        if k in key_map and k not in voices:
            voices[k] = ReedVoice(key_map[k])
        
        if k == 'q':  # increase pressure
            bellows_pressure += 0.05
        if k == 'a':  # decrease pressure
            bellows_pressure -= 0.05
    
    except:
        pass

def on_release(key):
    try:
        k = key.char
        if k in voices:
            del voices[k]
    except:
        pass

# -----------------------------
# Run Engine
# -----------------------------
stream = sd.OutputStream(
    channels=1,
    callback=audio_callback,
    samplerate=fs,
    blocksize=block_size
)

with stream:
    listener = keyboard.Listener(on_press=on_press, on_release=on_release)
    listener.start()
    print("Digital Harmonium Running...")
    print("Keys: z x c v b n m ,")
    print("Increase bellows: q | Decrease bellows: a")
    listener.join()
