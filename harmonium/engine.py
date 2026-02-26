import numpy as np
import sounddevice as sd
from pynput import keyboard
from scipy.signal import lfilter

fs = 44100
block = 128
dt = 1/fs

# -----------------------------
# GLOBAL BELLOW SYSTEM
# -----------------------------
class Bellows:
    def __init__(self):
        self.P = 600       # Initial pressure (Pa)
        self.C = 0.0005    # Compliance
        self.pump = 650    # Pump input

    def update(self, total_flow):
        dP = (self.pump - total_flow) / self.C
        self.P += dP * dt
        return self.P

bellows = Bellows()

# -----------------------------
# REED VOICE
# -----------------------------
class ReedVoice:
    def __init__(self, freq):
        self.m = 0.002
        self.c = 0.001
        self.rho = 1.2
        self.slot = 0.0005
        self.x_max = 0.0006
        self.k_col = 2e6
        
        self.freq = freq * 2**(np.random.uniform(-2,2)/1200)
        self.k = (2*np.pi*self.freq)**2 * self.m
        
        self.x = 0
        self.v = 0
        self.active = True

    def airflow(self, P):
        opening = max(self.slot - self.x, 0)
        if opening <= 0:
            return 0
        return opening * np.sqrt(2 * abs(P) / self.rho)

    def collision(self):
        if self.x > self.x_max:
            return -self.k_col * (self.x - self.x_max)**2
        return 0

    def process(self, frames):
        out = np.zeros(frames)
        total_flow = 0

        for i in range(frames):
            P = bellows.P
            
            def accel(x, v):
                flow = self.airflow(P)
                F_air = P * flow * 0.0001
                F_col = self.collision()
                return (F_air + F_col - self.c*v - self.k*x)/self.m

            # RK2 integration
            a1 = accel(self.x, self.v)
            x_mid = self.x + self.v*dt/2
            v_mid = self.v + a1*dt/2
            a2 = accel(x_mid, v_mid)
            self.x += v_mid*dt
            self.v += a2*dt
            
            out[i] = self.v
            total_flow += self.airflow(P)

        return out, total_flow

# -----------------------------
# BODY RESONATOR (Multi-mode)
# -----------------------------
def resonator(signal, freq, Q):
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

def body_filter(signal):
    r1 = resonator(signal, 650, 6)
    r2 = resonator(signal, 1200, 4)
    r3 = resonator(signal, 2400, 3)
    return (r1 + r2 + r3)/3

# -----------------------------
# VOICE MANAGEMENT
# -----------------------------
voices = {}

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

# Drone stops (Sa–Pa–Sa)
drones = []

def toggle_drone():
    global drones
    if drones:
        drones = []
    else:
        drones = [
            ReedVoice(261.63),
            ReedVoice(392.00),
            ReedVoice(523.25)
        ]

# -----------------------------
# AUDIO CALLBACK
# -----------------------------
def callback(outdata, frames, time, status):
    mix = np.zeros(frames)
    total_flow = 0

    for k in list(voices):
        sig, flow = voices[k].process(frames)
        mix += sig
        total_flow += flow

    for d in drones:
        sig, flow = d.process(frames)
        mix += sig
        total_flow += flow

    # Update bellows pressure globally
    bellows.update(total_flow)

    mix = body_filter(mix)

    mix /= np.max(np.abs(mix)) + 1e-6
    outdata[:] = mix.reshape(-1,1)

# -----------------------------
# KEYBOARD CONTROL
# -----------------------------
def on_press(key):
    try:
        k = key.char
        if k in key_map and k not in voices:
            voices[k] = ReedVoice(key_map[k])
        if k == 'd':
            toggle_drone()
    except:
        pass

def on_release(key):
    try:
        k = key.char
        if k in voices:
            del voices[k]
    except:
        pass

stream = sd.OutputStream(
    channels=1,
    callback=callback,
    samplerate=fs,
    blocksize=block
)

with stream:
    listener = keyboard.Listener(on_press=on_press,
                                 on_release=on_release)
    print("Research-Grade Digital Harmonium Running")
    print("Keys: z x c v b n m ,")
    print("Press 'd' to toggle Sa-Pa-Sa drone")
    listener.start()
    listener.join()
