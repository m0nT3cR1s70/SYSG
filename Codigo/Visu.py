import numpy as np
from mpl_toolkits.mplot3d import axes3d
import matplotlib.pyplot as plt
from matplotlib import cm
import time

# Generamos los contornos
# Para la solucion Analitica y la Calculada
def ambas():
	nodos = 3000
	#Leemos el contenido de del archivo exacto
	xe = np.zeros(nodos)
	ye = np.zeros(nodos)
	ve = np.zeros(nodos)

	xa = np.zeros(nodos)
	ya = np.zeros(nodos)
	va = np.zeros(nodos)

	ex = np.zeros(nodos)
	ey = np.zeros(nodos)
	ev = np.zeros(nodos)


	i = 0
	j = 0
	k = 0

	exacta = open('laplace2DE.txt')
	for linea in exacta:
		elementos = linea.strip().split()
		xe[i] = elementos[0]
		ye[i] = elementos[1]
		ve[i] = elementos[2]
		i = i+1
		
	aprox = open('laplace2DA.txt')
	for linea in aprox:
	    elementos = linea.strip().split()
	    xa[j] = elementos[0]
	    ya[j] = elementos[1]
	    va[j] = elementos[2]
	    j = j+1


	error = open('error.txt')
	for linea in error:
		elementos = linea.strip().split()
		ex[i] = elementos[0]
		ey[i] = elementos[1]
		ev[i] = elementos[2]
		i = i+1


	fig = plt.figure()

	ax1 = fig.add_subplot(2,2,1)
	im = ax1.tricontourf(xe, ye, ve)
	plt.tricontour(xe, ye, ve, colors = 'w')  
	ax1.set_aspect('equal')
	ax1.set_axis_off()
	ax1.set_title('Exacta')

	ax2 = fig.add_subplot(2,2,2)
	im = ax2.tricontourf(xa, ya, va)
	plt.colorbar(im, drawedges=True, orientation='vertical')
	plt.tricontour(xa, ya, va, colors = 'w')
	ax2.set_aspect('equal')
	ax2.set_axis_off()
	ax2.set_title('Aproximada')


	ax3 = fig.add_subplot(2,2,4)
	im1 = ax3.tricontourf(ex, ey, ev)
	plt.colorbar(im1, drawedges=True, orientation='vertical')
	plt.tricontour(ex, ey, ev, colors = 'w')
	ax3.set_aspect('equal')
	ax3.set_axis_off()
	ax3.set_title('ERROR')


	plt.show()


# Solo para la Calculada
def calculada():
	nodos = 5000

	xa = np.zeros(nodos)
	ya = np.zeros(nodos)
	va = np.zeros(nodos)

	j = 0
	aprox = open('solx.txt')
	for linea in aprox:
	    elementos = linea.strip().split()
	    xa[j] = elementos[0]
	    ya[j] = elementos[1]
	    va[j] = elementos[2]
	    j = j+1

	fig = plt.figure()


	ax2 = fig.add_subplot(1,1,1)
	im = ax2.tricontourf(xa, ya, va)
	plt.colorbar(im, drawedges=True, orientation='vertical')
	plt.tricontour(xa, ya, va, colors = 'w')
	ax2.set_aspect('equal')
	ax2.set_axis_off()
	ax2.set_title('Aproximada')

	plt.show()
def calculada1():
	nodos = 5000

	xa = np.zeros(nodos)
	ya = np.zeros(nodos)
	va = np.zeros(nodos)

	j = 0
	aprox = open('sol.txt')
	for linea in aprox:
	    elementos = linea.strip().split()
	    xa[j] = elementos[0]
	    ya[j] = elementos[1]
	    va[j] = elementos[2]
	    j = j+1

	fig = plt.figure()


	ax2 = fig.add_subplot(1,1,1)
	im = ax2.tricontourf(xa, ya, va)
	plt.colorbar(im, drawedges=True, orientation='vertical')
	plt.tricontour(xa, ya, va, colors = 'w')
	ax2.set_aspect('equal')
	ax2.set_axis_off()
	ax2.set_title('Aproximada')

	plt.show()
# Grafica 3D 
def superficie():
	nodos = 5000

	xa = np.zeros(nodos)
	ya = np.zeros(nodos)
	va = np.zeros(nodos)

	j = 0
	aprox = open('solx.txt')
	for linea in aprox:
	    elementos = linea.strip().split()
	    xa[j] = elementos[0]
	    ya[j] = elementos[1]
	    va[j] = elementos[2]
	    j = j+1

	fig = plt.figure()
	ax = fig.gca(projection='3d')
	ax.plot_trisurf(xa, ya, va, cmap=cm.jet, linewidth=0.2)
	ax.set_aspect('equal')
	#ax.set_axis_off()
	ax.set_title('Aproximada')
	plt.show()
def superficie1():
	nodos = 5000

	xa = np.zeros(nodos)
	ya = np.zeros(nodos)
	va = np.zeros(nodos)

	j = 0
	aprox = open('sol.txt')
	for linea in aprox:
	    elementos = linea.strip().split()
	    xa[j] = elementos[0]
	    ya[j] = elementos[1]
	    va[j] = elementos[2]
	    j = j+1

	fig = plt.figure()
	ax = fig.gca(projection='3d')
	ax.plot_trisurf(xa, ya, va, cmap=cm.jet, linewidth=0.2)
	ax.set_aspect('equal')
	#ax.set_axis_off()
	ax.set_title('Aproximada')
	plt.show()

ambas()
#calculada1()
#superficie1()
#calculada()
#superficie()
