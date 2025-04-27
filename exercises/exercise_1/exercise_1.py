import numpy as np
import matplotlib.pyplot as plt

def velocity(t, x):
    x1, x2 = x
    v1 = t + x1
    v2 = t + x2
    return np.array([v1, v2])

def euler_step(x, t, dt):
    return x + dt * velocity(t, x)

def simulation(x0, t0, t_end, dt):
    N = int((t_end - t0) / dt)
    t_values = np.zeros(N+1)
    x_values = np.zeros((N+1, 2))
    t_values[0] = t0
    x_values[0] = x0
    for i in range(N):
        t_value = t_values[i]
        x_value = x_values[i]
        t_values[i+1] = t_value + dt
        x_values[i+1] = euler_step(x_value, t_value, dt)
    return t_values, x_values

if __name__ == "__main__":
    x0 = np.array([0, 0])
    t0 = 0
    t_end = 10
    dt = 0.01
    t_values, x_values = simulation(x0, t0, t_end, dt)
    plt.plot(t_values, x_values[:, 0], label='x')
    plt.plot(t_values, x_values[:, 1], label='y')
    plt.legend()
    plt.show()
