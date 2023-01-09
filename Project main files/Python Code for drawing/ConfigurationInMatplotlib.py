import matplotlib.pyplot as plt
from matplotlib.patches import Rectangle
import numpy as np 

fig = plt.figure()
ax = fig.add_subplot(1, 1, 1)

#define Matplotlib figure and axis
#fig,ax = plt.subplots()

Add1='../Computational Physics Project B/data/Wolff/2000/rand/'

Add3='.txt'
for x in range(4,50):
    Add2='1.500000 '+str(x)
    Data = np.loadtxt(Add1+Add2+Add3,skiprows=2)
    print(Data)

    size=100
    UnitSize=1
    fig.set_size_inches(UnitSize*size,UnitSize*size)

    for i in range(0,size):
        for j in range(0,size):
            if Data[i][j]==1:
                ax.add_patch(Rectangle((i*UnitSize, j*UnitSize), UnitSize, UnitSize,facecolor = 'black',fill=True))
            else:
                ax.add_patch(Rectangle((i*UnitSize, j*UnitSize), UnitSize, UnitSize,facecolor = 'white',fill=True))
    my_x_ticks = np.arange(0, 110, 10)
    my_y_ticks = np.arange(0, 110, 10)
    plt.xticks(my_x_ticks,fontsize=100)
    plt.yticks(my_y_ticks,fontsize=100)

    ax.autoscale()
    Add2='KbT='+Add2
    ax.set_title(Add2,fontsize=200)
    plt.savefig(fname=Add2+".png",format="png")