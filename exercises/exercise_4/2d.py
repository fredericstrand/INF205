import numpy as np
import matplotlib.pyplot as plt

gamma = 1.4

nx, ny = 400, 80
dx = 1.0 / nx
dy = 0.2 / ny
x_centres = 0.5*dx + np.arange(nx)*dx
y_centres = 0.5*dy + np.arange(ny)*dy
X, Y = np.meshgrid(x_centres, y_centres, indexing='ij')

U = np.zeros((4, nx+2, ny+2))

rho = np.where(X < 0.5, 1.0, 0.125)
u   = np.zeros_like(rho)
v   = np.zeros_like(rho)
p   = np.where(X < 0.5, 1.0, 0.1)
E   = p/(gamma-1) + 0.5*rho*(u**2 + v**2)

U[0,1:-1,1:-1] = rho
U[1,1:-1,1:-1] = rho*u
U[2,1:-1,1:-1] = rho*v
U[3,1:-1,1:-1] = E

def apply_bc(U):
    # x‑walls
    U[:,0,:]       = U[:,1,:]
    U[1,0,:]       = -U[1,1,:]        # flip ρu at left wall
    U[:,nx+1,:]    = U[:,nx,:]
    U[1,nx+1,:]    = -U[1,nx,:]       # flip ρu at right wall
    # y‑walls
    U[:,:,0]       = U[:,:,1]
    U[2,:,0]       = -U[2,:,1]        # flip ρv at bottom wall
    U[:,:,ny+1]    = U[:,:,ny]
    U[2,:,ny+1]    = -U[2,:,ny]       # flip ρv at top wall

def prim(W):
    r = W[0]
    u = W[1] / r
    v = W[2] / r
    p = (gamma-1)*(W[3] - 0.5*r*(u**2 + v**2))
    return r, u, v, p

def Fx(W):
    r,u,v,p = prim(W)
    return np.array([W[1],                # ρu
                     W[1]*u + p,          # ρu² + p
                     W[2]*u,              # ρuv
                     u*(W[3]+p)])         # u(E+p)

def Gy(W):
    r,u,v,p = prim(W)
    return np.array([W[2],                # ρv
                     W[1]*v,              # ρuv
                     W[2]*v + p,          # ρv² + p
                     v*(W[3]+p)])         # v(E+p)

t, tf, CFL = 0.0, 0.25, 0.6
apply_bc(U)                       # populate ghost cells once

while t < tf:
    r,u,v,p = prim(U[:,1:-1,1:-1])
    a = np.sqrt(gamma*p/r)
    dt = CFL * min(dx, dy) / np.max(np.abs(u) + np.abs(v) + a)
    if t + dt > tf:
        dt = tf - t

    # x‑faces (vertical)
    Fx_face = np.zeros((4, nx+1, ny))
    for i in range(nx+1):
        for j in range(ny):
            UL = U[:, i,     j+1]
            UR = U[:, i+1,   j+1]
            rL,uL,vL,pL = prim(UL)
            rR,uR,vR,pR = prim(UR)
            aL = np.sqrt(gamma*pL/rL)
            aR = np.sqrt(gamma*pR/rR)
            SL = min(uL - aL, uR - aR)
            SR = max(uL + aL, uR + aR)
            FL = Fx(UL)
            FR = Fx(UR)
            if   SL >= 0:   Fx_face[:,i,j] = FL
            elif SR <= 0:   Fx_face[:,i,j] = FR
            else:           Fx_face[:,i,j] = (SR*FL - SL*FR + SL*SR*(UR-UL)) / (SR-SL)

    # y‑faces (horizontal)
    Gy_face = np.zeros((4, nx, ny+1))
    for i in range(nx):
        for j in range(ny+1):
            UB = U[:, i+1, j]
            UT = U[:, i+1, j+1]
            rB,uB,vB,pB = prim(UB)
            rT,uT,vT,pT = prim(UT)
            aB = np.sqrt(gamma*pB/rB)
            aT = np.sqrt(gamma*pT/rT)
            SB = min(vB - aB, vT - aT)
            ST = max(vB + aB, vT + aT)
            GB = Gy(UB)
            GT = Gy(UT)
            if   SB >= 0:   Gy_face[:,i,j] = GB
            elif ST <= 0:   Gy_face[:,i,j] = GT
            else:           Gy_face[:,i,j] = (ST*GB - SB*GT + SB*ST*(UT-UB)) / (ST-SB)

    # update conserved variables
    U[:,1:-1,1:-1] -= dt/dx * (Fx_face[:,1:,:] - Fx_face[:,:-1,:])
    U[:,1:-1,1:-1] -= dt/dy * (Gy_face[:,:,1:] - Gy_face[:,:,:-1])

    apply_bc(U)
    t += dt

rho, *_ = prim(U[:,1:-1,1:-1])
plt.figure(figsize=(6,2))
plt.imshow(rho.T, origin='lower', extent=(0,1,0,0.2), aspect='auto', cmap='viridis')
plt.colorbar(label='Density')
plt.xlabel('x'); plt.ylabel('y')
plt.title(f'Density at t={t:.3f} s')
plt.tight_layout(); plt.show()
