#!/usr/bin/python 
"""
  This file is part of SYSG.

    SYSG is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    SYSG is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with SYSG.  If not, see <http://www.gnu.org/licenses/>.
"""

"""
	@package docstring
	Se encarga de la visualizacion del simulador SYSG.

"""
from mpl_toolkits.mplot3d import Axes3D
from matplotlib import cm
import matplotlib.pyplot as plt
import numpy as np
import sys

def velocidad(numero,file1,file2,ejex,ejey):
	''' Dados dos archivos con la comparacion de 
	tiempos entre dos metodos este se muestra
	en pantalla'''
	nodos = numero
	# Declaramos el contenido de los dos archivos
	x1 = np.zeros(nodos)
	y1 = np.zeros(nodos)
	# Declaramos dos variables para almacenar
	x2 = np.zeros(nodos)
	y2 = np.zeros(nodos)
	# Leemos las coordenadas del primer archivo
	file1l = open(file1)
	i = 0
	for linea in file1l:
		elementos = linea.strip().split()
		x1[i] = elementos[0]
		y1[i] = elementos[1]
		i = i+1
	# Leemos las coordenadas del segundo archivo
	file2l = open(file2)
	i = 0
	for linea in file2l:
		elementos = linea.strip().split()
		x2[i] = elementos[0]
		y2[i] = elementos[1]
		i = i+1
	#Dibujamos las graficas
	plt.plot(x1, y1,'*-',label = file1)
	plt.plot(x2, y2,'*-',label = file2)
	# Leyenda del grafico
	plt.legend(loc = 2)
	# Para las fuentes
	# Nombre del eje x
	plt.xlabel(ejex)
	# Nombre del eje y
	plt.ylabel(ejey) 
	#Mostramos las graficas
	plt.show()

def residuo(numero,file1,file2,ejex,ejey):
	''' Dados dos archivos con la comparacion de 
	tiempos entre dos metodos este se muestra
	en pantalla'''
	nodos = numero
	# Declaramos el contenido de los dos archivos
	x1 = np.zeros(nodos)
	y1 = np.zeros(nodos)
	# Declaramos dos variables para almacenar
	x2 = np.zeros(nodos)
	y2 = np.zeros(nodos)
	# Leemos las coordenadas del primer archivo
	file1l = open(file1)
	i = 0
	for linea in file1l:
		elementos = linea.strip().split()
		x1[i] = elementos[0]
		y1[i] = elementos[1]
		i = i+1
	# Leemos las coordenadas del segundo archivo
	file2l = open(file2)
	i = 0
	for linea in file2l:
		elementos = linea.strip().split()
		x2[i] = elementos[0]
		y2[i] = elementos[1]
		i = i+1
	#Indicamos que sera de tipo Semi-log el eje y
	plt.semilogy()
	#Dibujamos las graficas
	plt.plot(x1, y1,label = file1)
	plt.plot(x2, y2,label = file2)
	# Leyenda del grafico
	plt.legend()
	# Nombre del eje x
	plt.xlabel(ejex)
	# Nombre del eje y
	plt.ylabel(ejey) 
	#Mostramos las graficas
	plt.show()

def solucion(nodos,file1,file2,file3):
	''' Muestra en pantalla la solucion para un problema tipo Benchmark'''

	#Coordenadas para leer el contenido de tres archivos
	x1 = np.zeros(nodos)
	y1 = np.zeros(nodos)
	z1 = np.zeros(nodos)

	x2 = np.zeros(nodos)
	y2 = np.zeros(nodos)
	z2 = np.zeros(nodos)

	x3 = np.zeros(nodos)
	y3 = np.zeros(nodos)
	z3 = np.zeros(nodos)

	#Leemos el contenido de los archivos
	file1l = open(file1)
	i = 0
	for linea in file1l:
		elementos = linea.strip().split()
		x1[i] = elementos[0]
		y1[i] = elementos[1]
		z1[i] = elementos[2]
		i = i+1
	#Leemos el contenido del segundo archivo
	file2l = open(file2)
	i = 0
	for linea in file2l:
		elementos = linea.strip().split()
		x2[i] = elementos[0]
		y2[i] = elementos[1]
		z2[i] = elementos[2]
		i = i+1
	#Leemos el contenido de tercer archivo
	file3l = open(file3)
	i = 0
	for linea in file3l:
		elementos = linea.strip().split()
		x3[i] = elementos[0]
		y3[i] = elementos[1]
		z3[i] = elementos[2]
		i = i+1

	#Dibujamos los resultados
	fig = plt.figure()

	ax1 = fig.add_subplot(2,2,1)
	im = ax1.tricontourf(x1, y1, z1)
	plt.tricontour(x1, y1, z1, colors = 'w')  
	ax1.set_aspect('equal')
	ax1.set_axis_off()
	ax1.set_title('Exacta')

	ax2 = fig.add_subplot(2,2,2)
	im = ax2.tricontourf(x2, y2, z2)
	plt.colorbar(im, drawedges=True, orientation='vertical')
	plt.tricontour(x2, y2, z2, colors = 'w')
	ax2.set_aspect('equal')
	ax2.set_axis_off()
	ax2.set_title('Aproximada')


	ax3 = fig.add_subplot(2,2,4)
	im1 = ax3.tricontourf(x3, y3, z3)
	plt.colorbar(im1, drawedges=True, orientation='vertical')
	plt.tricontour(x3, y3, z3, colors = 'w')
	ax3.set_aspect('equal')
	ax3.set_axis_off()
	ax3.set_title('ERROR')

	plt.show(



		)

def solucion1(nodos,file,title):

	xa = np.zeros(nodos)
	ya = np.zeros(nodos)
	va = np.zeros(nodos)

	j = 0
	aprox = open(file)
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
	ax2.set_title(title


		)

	plt.show()

def superficie(nodos,file,title):

	xa = np.zeros(nodos)
	ya = np.zeros(nodos)
	va = np.zeros(nodos)

	j = 0
	aprox = open(file)
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
	ax.set_title(title)
	plt.show()


def main():
	nodos = 10000
	#Mostramos la visualizacion de datos
	solucion(nodos,"laplace2DE.txt","laplace2DA.txt","error.txt")
	#solucion1(nodos,"solx.txt","Medios Porosos")
	#solucion1(nodos,"K11.txt","Permeabilidad Aleatoria K11")
	#solucion1(nodos,"mediosPorsos.txt","MediosPorsos")
	#ejex = "iteraciones"
	#ejey = "log del residuo"
	#residuo(nodos,"CG.txt","CGP.txt",ejex,ejey)
	#residuo(nodos,"BICGSTAB.txt","BICGSTABP.txt",ejex,ejey)
	#solucion1(nodos,"K22.txt","Permeabilidad Aleatoria K22")
	




#Para poder hacer la ejecucion
if __name__ == '__main__':
	main()
