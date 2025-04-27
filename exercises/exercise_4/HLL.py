import numpy as np
import matplotlib.pyplot as plt

gamma=1.4
nx=500
x0,x1=0.0,1.0
dx=(x1-x0)/nx
x=np.linspace(x0-dx*0.5,x1+dx*0.5,nx+2)

rho=np.where(x<0.5,1.0,0.125)
u=np.zeros_like(rho)
p=np.where(x<0.5,1.0,0.1)
E=p/(gamma-1)+0.5*rho*u**2
U=np.stack((rho,rho*u,E))

def primitive_values(V):
    r=V[0]
    v=V[1]/r
    q=(gamma-1)*(V[2]-0.5*r*v**2)
    return r,v,q

def flux(V):
    r,v,q=primitive_values(V)
    F0=V[1]
    F1=V[1]*v+q
    F2=v*(V[2]+q)
    return np.array((F0,F1,F2))

t,tf,CFL=0.0,0.2,0.8
while t<tf:
    r,v,q=primitive_values(U)
    a=np.sqrt(gamma*q/r)
    dt=CFL*dx/np.max(np.abs(v)+a)
    if t+dt>tf:
        dt=tf-t
    F=np.zeros((3,nx+1))
    for j in range(nx+1):
        UL=U[:,j]
        UR=U[:,j+1]
        rL,uL,pL=primitive_values(UL)
        rR,uR,pR=primitive_values(UR)
        aL=np.sqrt(gamma*pL/rL)
        aR=np.sqrt(gamma*pR/rR)
        SL=min(uL-aL,uR-aR)
        SR=max(uL+aL,uR+aR)
        FL=flux(UL)
        FR=flux(UR)
        if SL>=0:
            F[:,j]=FL
        elif SR<=0:
            F[:,j]=FR
        else:
            F[:,j]=(SR*FL-SL*FR+SL*SR*(UR-UL))/(SR-SL)
    U[:,1:-1]-=dt/dx*(F[:,1:]-F[:,:-1])
    U[:,0]=U[:,1]
    U[:,-1]=U[:,-2]
    t+=dt

rho,u,p=primitive_values(U)
plt.figure()
plt.plot(x[1:-1],rho[1:-1])
plt.xlabel('x')
plt.ylabel('Density')
plt.figure()
plt.plot(x[1:-1],u[1:-1])
plt.xlabel('x')
plt.ylabel('Velocity')
plt.figure()
plt.plot(x[1:-1],p[1:-1])
plt.xlabel('x')
plt.ylabel('Pressure')
plt.show()
