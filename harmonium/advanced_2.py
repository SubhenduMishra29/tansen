import numpy as np
import sounddevice as sd
from scipy.signal import lfilter

fs = 44100
duration = 5
N = int(fs * duration)
dt = 1/fs

# --- Physical Constants ---
rho = 1.2  # air density kg/m3
slot_width = 0.0005
x_max = 0.0006
k_collision = 2e6

# Reed parameters
m = 0.002
f0 = 261.63
k = (2*np.pi*f0)**2 * m
c = 0.001

# Bellows pressure
P_base = 600  # Pascals (realistic scale)

x = 0.0
v = 0.0

output = np.zeros(N)

def airflow_force(P, x):
    opening = max(slot_width - x, 0)
    if opening <= 0:
        return 0
    
    U = opening * np.sqrt(2 * abs(P) / rho)
    return P * U * 0.0001

def collision_force(x):
    if x > x_max:
        return -k_collision * (x - x_max)**2
    return 0

for n in range(N):
    t = n/fs
    
    # Realistic pressure modulation
    P = P_base + 50*np.sin(2*np.pi*1.5*t)
    
    # RK2 Integration
    def acceleration(xi, vi):
        F_air = airflow_force(P, xi)
        F_col = collision_force(xi)
        return (F_air + F_col - c*vi - k*xi)/m
    
    a1 = acceleration(x, v)
    x_mid = x + v*dt/2
    v_mid = v + a1*dt/2
    a2 = acceleration(x_mid, v_mid)
    
    x += v_mid*dt
    v += a2*dt
    
    output[n] = v

# --- Multi-Mode Wooden Resonance ---
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

# Parallel resonances
res1 = resonator(output, 650, 6)
res2 = resonator(output, 1200, 4)
res3 = resonator(output, 2400, 3)

output = (res1 + res2 + res3) / 3

# Normalize
output /= np.max(np.abs(output))

sd.play(output, fs)
sd.wait()
