import numpy as np
import matplotlib.pyplot as plt

def initial_condition(x):
    return np.sin(2*np.pi*x)

def flux(u):
    return u


"""
When you use the centered_step function over a longer time period, small numerical errors can accumulate
and lead to instability because there is no form of artificial dissipation in the scheme.

Courant-Friedrichs-Lewy (CFL) condition: dt/dx <= 1

Fails first
"""
def centered_step(u, dt, dx):
    nx=len(u)
    un=np.zeros_like(u)
    for j in range(nx):
        jm=(j-1)%nx
        jp=(j+1)%nx
        un[j]=u[j] - 0.5*(dt/dx)*(flux(u[jp]) - flux(u[jm]))
    return un


"""
The Lax-Friedrichs scheme is a numerical scheme that adds artificial dissipation to the centered scheme, and therefore
make it relatively stable over longer time periods.

Courant-Friedrichs-Lewy (CFL) condition: dt/dx <= 1

Fails last
"""
def lax_friedrichs_step(u, dt, dx):
    nx=len(u)
    un=np.zeros_like(u)
    for j in range(nx):
        jm=(j-1)%nx
        jp=(j+1)%nx
        un[j]=0.5*(u[jp]+u[jm]) - 0.5*(dt/dx)*(flux(u[jp]) - flux(u[jm]))
    return un

"""
Courant-Friedrichs-Lewy (CFL) condition: dt/dx <= 1

Fail second. This is because it has dampening, but still less than the Lax-Friedrichs scheme.
"""
def upwind_step(u, dt, dx):
    nx=len(u)
    un=np.zeros_like(u)
    for j in range(nx):
        jm=(j-1)%nx
        un[j]=u[j] - (dt/dx)*(flux(u[j]) - flux(u[jm]))
    return un

def solve_pde(step_func, nx=100, nt=100, t_end=1.2):
    dx=1.0/nx
    dt=t_end/nt
    x=np.linspace(0,1,nx,endpoint=False)
    u=np.array([initial_condition(xi) for xi in x])
    for _ in range(nt):
        u=step_func(u,dt,dx)
    return x,u

def main():
    nx,nt=100,100
    x1,u1=solve_pde(centered_step,nx,nt)
    x2,u2=solve_pde(lax_friedrichs_step,nx,nt)
    x3,u3=solve_pde(upwind_step,nx,nt)
    plt.subplot(3,1,1)
    plt.plot(x1,u1,".-")
    plt.title("Centered")
    plt.subplot(3,1,2)
    plt.plot(x2,u2,".-")
    plt.title("Lax-Friedrichs")
    plt.subplot(3,1,3)
    plt.plot(x3,u3,".-")
    plt.title("Upwind")
    plt.tight_layout()
    plt.show()

if __name__=="__main__":
    main()
