import matplotlib.pyplot as plt
from matplotlib import cm
from matplotlib.ticker import LinearLocator
import numpy as np

fig, ax = plt.subplots(subplot_kw={"projection": "3d"})

DataPath = '../Computational Physics Project B/single.txt'

Data = np.loadtxt(DataPath,skiprows=1)

with open(DataPath, 'r', encoding='utf-8') as f:
    comment = f.readline().rstrip()
    print(comment)

print(Data)

#x=np.linspace(0,100000,1000,endpoint=False)
x=np.linspace(0,100000000,1001,endpoint=True)#lower limit, upper limit, how many points
temperature=np.linspace(2,3,101)


X,Y=np.meshgrid(x,temperature)

print(X)
print(Y)

colors =plt.cm.magma( (Y-Y.min())/float((Y-Y.min()).max()) )
#https://matplotlib.org/stable/tutorials/colors/colormaps.html different color map to choose
ax.plot_surface(X,Y,Data ,facecolors=colors, linewidth=0, shade=False )

ax.set_xlabel('Step')
ax.set_ylabel('Temperature')
ax.set_zlabel('Z-axis')
ax.set_title(comment)

#plt.savefig(fname="3D.png",format="png")
plt.show()
