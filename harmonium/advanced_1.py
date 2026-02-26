import numpy as np
import sounddevice as sd
from pynput import keyboard
from scipy.signal import lfilter

fs = 44100
block_size = 256
solver_type = "rk2"   # Options: "euler", "semi", "rk2", "rk4"

bellows_pressure = 0.6

key_map = {
    'z': 261.63,
    'x': 293.66,
    'c': 329.63,
    'v': 349.23,
    'b': 392.00,
    'n': 440.00,
    'm': 493.88,
    ',': 523.25
}

# -----------------------------
# Reed Voice with Selectable Solver
# -----------------------------
class ReedVoice:
    def __init__(self, freq):
        self.m = 0.002
        self.c = 0.0008
        self.beta = 800
        
        self.detune = np.random.uniform(-2, 2)
        freq *= 2**(self.detune/1200)
        self.k = (2*np.pi*freq)**2 * self.m
        
        self.x = 0.0
        self.v = 0.0
    
    def acceleration(self, x, v, pressure):
        F_air = pressure * (1 - self.beta * x)
        return (F_air - self.c*v - self.k*x) / self.m

    def step(self, dt, pressure):
        if solver_type == "euler":
            a = self.acceleration(self.x, self.v, pressure)
            self.x += self.v * dt
            self.v += a * dt

        elif solver_type == "semi":  # Semi-Implicit Euler
            a = self.acceleration(self.x, self.v, pressure)
            self.v += a * dt
            self.x += self.v * dt

        elif solver_type == "rk2":
            a1 = self.acceleration(self.x, self.v, pressure)
            x_mid = self.x + self.v * dt/2
            v_mid = self.v + a1 * dt/2
            a2 = self.acceleration(x_mid, v_mid, pressure)
            self.x += v_mid * dt
            self.v += a2 * dt

        elif solver_type == "rk4":
            a1 = self.acceleration(self.x, self.v, pressure)
            k1x = self.v
            k1v = a1

            a2 = self.acceleration(self.x + k1x*dt/2,
                                   self.v + k1v*dt/2, pressure)
            k2x = self.v + k1v*dt/2
            k2v = a2

            a3 = self.acceleration(self.x + k2x*dt/2,
                                   self.v + k2v*dt/2, pressure)
            k3x = self.v + k2v*dt/2
            k3v = a3

            a4 = self.acceleration(self.x + k3x*dt,
                                   self.v + k3v*dt, pressure)
            k4x = self.v + k3v*dt
            k4v = a4

            self.x += dt*(k1x + 2*k2x + 2*k3x + k4x)/6
            self.v += dt*(k1v + 2*k2v + 2*k3v + k4v)/6

    def process(self, frames):
        global bellows_pressure
        dt = 1/fs
        out = np.zeros(frames)

        for i in range(frames):
            pressure = bellows_pressure + 0.02*np.random.randn()
            self.step(dt, pressure)
            out[i] = self.v
        
        return out

voices = {}

# -----------------------------
# Wooden Resonator
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
    mix = np.zeros(frames)

    for key in list(voices):
        mix += voices[key].process(frames)

    if len(voices) > 0:
        mix /= len(voices)

    mix = resonator(mix)
    mix /= np.max(np.abs(mix)) + 1e-6
    outdata[:] = mix.reshape(-1,1)

# -----------------------------
# Keyboard Controls
# -----------------------------
def on_press(key):
    global bellows_pressure
    try:
        k = key.char
        if k in key_map and k not in voices:
            voices[k] = ReedVoice(key_map[k])

        if k == 'q':
            bellows_pressure += 0.05
        if k == 'a':
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
# Run
# -----------------------------
stream = sd.OutputStream(
    channels=1,
    callback=audio_callback,
    samplerate=fs,
    blocksize=block_size
)

with stream:
    listener = keyboard.Listener(on_press=on_press,
                                 on_release=on_release)
    listener.start()
    print("Solver:", solver_type)
    listener.join()
