from tkinter import *
import numpy as np 


Data = np.loadtxt('../Computational Physics Project B/data/SSF/Rand/1billion 0-5/0.800000.txt',skiprows=2)
print(Data)

size=100
UnitSize=5
window = Tk()
window.title("Configuration")
canvas=Canvas(window,bg='white',width=UnitSize*size,height=UnitSize*size)

for i in range(0,size):
    for j in range(0,size):
        if Data[i][j]==1:
            canvas.create_rectangle(i*UnitSize,j*UnitSize,(i+1)*UnitSize,(j+1)*UnitSize,fill="white")
        else:
            canvas.create_rectangle(i*UnitSize,j*UnitSize,(i+1)*UnitSize,(j+1)*UnitSize,fill="black")
canvas.pack()
window.mainloop()