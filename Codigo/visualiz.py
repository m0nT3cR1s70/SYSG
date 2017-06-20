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
import numpy as np
import matplotlib.pyplot as plt
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

def velocidada(numero,file1,file2,file3,ejex,ejey):
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
	x3 = np.zeros(nodos)
	y3 = np.zeros(nodos)
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

	file3l = open(file3)
	i = 0
	for linea in file3l:
		elementos = linea.strip().split()
		x3[i] = elementos[0]
		y3[i] = elementos[1]
		i = i+1
	plt.semilogy()
	plt.semilogx()
	#Dibujamos las graficas
	plt.plot(x1, y1,'*-',label = file1)
	plt.plot(x2, y2,'*-',label = file2)
	plt.plot(x3, y3,'*-',label = file3)
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
	x2 = np.zeros(nodos)
	y2 = np.zeros(nodos)
	x3 = np.zeros(nodos)
	y3 = np.zeros(nodos)

	#Leemos el contenido de los archivos
	file1l = open(file1)
	i = 0
	for linea in file1l:
		elementos = linea.strip().split()
		x1[i] = elementos[0]
		y1[i] = elementos[1]
		i = i+1
	#Leemos el contenido del segundo archivo
	file2l = open(file2)
	i = 0
	for linea in file2l:
		elementos = linea.strip().split()
		x1[i] = elementos[0]
		y1[i] = elementos[1]
		i = i+1
	#Leemos el contenido de tercer archivo
	file3l = open(file3)
	i = 0
	for linea in file3l:
		elementos = linea.strip().split()
		x1[i] = elementos[0]
		y1[i] = elementos[1]
		i = i+1

	#Dibujamos los resultados
	fig = plt.figure()

	ax1 = fig.add_subplot(2,2,1)


def main():
	nodos = 4176
	ejex = "log de las iteraciones"
	ejey = "log del Tiempo medido en ms"
	#velocidada(nodos,"CGE.txt","CGEILU.txt","CGEMILU.txt",ejex,ejey)
	#velocidada(nodos,"BICGSTABE.txt","BICGSTABEILU.txt","BICGSTABEMILU.txt",ejex,ejey)
	#velocidad(nodos,"CGE.txt","CGA.txt",ejex,ejey)
	#velocidad(nodos,"CGEJAC.txt","CGAJAC.txt",ejex,ejey)
	#velocidad(nodos,"CGEILU.txt","CGAILU.txt",ejex,ejey)
	#velocidad(nodos,"CGEMILU.txt","CGAMILU.txt",ejex,ejey)
	#velocidad(nodos,"BICGSTABE.txt","BICGSTABA.txt",ejex,ejey)
	#velocidad(nodos,"BICGSTABEJAC.txt","BICGSTABAJAC.txt",ejex,ejey)
	#velocidad(nodos,"BICGSTABEILU.txt","BICGSTABAILU.txt",ejex,ejey)
	#velocidad(nodos,"BICGSTABEMILU.txt","BICGSTABAMILU.txt",ejex,ejey)
	# Graficamos los residuos para distintos de los metodos de Krylov
	ejex = "Iteraciones"
	ejey = "log del residuo"
	residuo(nodos,"CG.txt","CGP.txt",ejex,ejey)
	residuo(nodos,"BICGSTAB.txt","BICGSTABP.txt",ejex,ejey)
	#residuo(nodos,"CG.txt","CGJAC.txt",ejex,ejey)
	#residuo(nodos,"CG.txt","CGPILU.txt",ejex,ejey)
	#residuo(nodos,"CG.txt","CGPMILU.txt",ejex,ejey)
	#residuo(nodos,"BICGSTAB.txt","BICGSTABJACOBI.txt",ejex,ejey)
	#residuo(nodos,"BICGSTAB.txt","BICGSTABILU.txt",ejex,ejey)
	#residuo(nodos,"BICGSTAB.txt","BICGSTABMILU.txt",ejex,ejey)
	#Mostramos la visualizacion de datos
	#solucion(nodos,"laplace2DA.txt","laplace2DE.txt","error.txt")
	




#Para poder hacer la ejecucion
if __name__ == '__main__':
	main()
