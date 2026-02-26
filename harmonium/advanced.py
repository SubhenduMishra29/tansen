import numpy as np
import sounddevice as sd
from scipy.signal import lfilter

fs = 44100
duration = 5.0
N = int(fs * duration)

# Reed physical parameters
m = 0.002       # mass
k = (2*np.pi*261.63)**2 * m  # stiffness tuned to Sa
c = 0.0008      # damping
beta = 800      # airflow nonlinearity factor

dt = 1/fs

# State variables
x = 0.0
v = 0.0

output = np.zeros(N)

# Bellows pressure model
P_base = 0.6
pressure = P_base

for n in range(N):
    t = n / fs
    
    # Bellows micro-variation
    pressure = P_base + 0.05*np.sin(2*np.pi*1.5*t) + 0.02*np.random.randn()
    
    # Nonlinear airflow force
    F_air = pressure * (1 - beta * x)
    
    # Reed ODE: m*a = F - c*v - k*x
    a = (F_air - c*v - k*x) / m
    
    # Integrate (Euler method)
    v += a * dt
    x += v * dt
    
    # Output is proportional to velocity (airflow modulation)
    output[n] = v

# Normalize
output /= np.max(np.abs(output))

# --- Wooden Resonator (2-pole filter) ---
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

output = resonator(output, 750, 8)

# Add slight airflow noise
output += 0.01*np.random.randn(N)

# Final normalize
output /= np.max(np.abs(output))

sd.play(output, fs)
sd.wait()
