/*
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
*/

/**
	@version 1.0
	@date 6/12/2016
	@author Mario Arturo Nieto Butrón
	@title SYSG
	@brief El CVFEM devuelve matrices con un gran contenido de ceros 
	en sus entradas que no son significativas en su calculo por lo tanto se
	requieren utilizar esquemas de almacenamiento, que consideren unicamiente 
	la información necesaria, en éste caso se implementa el esquema CSR. 
	El esquema requiere CSR de tres arreglos:
		data
		col
		irow
	en data se almacenan los elementos reales no cero de la matriz, en col
	se almacenan la coordenada j de la matriz y en irow se almacenan los indices 
	de inicio y fin de cada renglón del arreglo.
*/

#ifndef CSR_HPP
#define CSR_HPP

//**********************
// Librerias necesarias
#include <iostream>
#include "COO.hpp"
#include "Vector.hpp"
//**********************

class CSR
{
	public:
		// Arreglos necesarios
		double *data; /// Contenedor para almacenar los elementos no cero de una matriz
		int *col; /// Contenedor para almacenar las coordenadas j de cada componente.
		int *irow; /// Contenedor para almacenar los indices de inicio y fin de cada renglon de la matriz
		int *idiag; /// Contendor para almacenar la diagonal de una matriz.
		// Variables necesarias
		int nnz; /// Número de componentes distintos de cero de una matriz
		int nnzMax; /// Valor aproximado por 30
		int n; // Valor aproximado proporcionado por el usuario
		// Metodos utiles
		/**
			@brief Método privado, solicita espacio en memoria para los contenedores.
			@param Ninguno. 
			@return None.
		*/
		void reserveMemory();
		/**
			@brief Método privado, libera el espacio de los contenedores.
			@param Ninguno.
			@return None.
		*/
		void freeMemory();
	public:
		/**
			@brief Constructor vacio.
		*/
		CSR(){};
		/**
			@brief Constructor que inciaiza las variables dada una aproximación
			inicial.
			@param m Aproximación del número total de elementos de la matriz.
		*/
		CSR(int m):n(m),nnzMax(m*30),nnz(0){reserveMemory();zeros();};
		/**
			@brief Construtor copia de la matriz.
		*/
		CSR(CSR  const &Mtx):n(Mtx.n),nnzMax(Mtx.nnzMax),nnz(Mtx.nnz)
		{
			reserveMemory();
			// Copiamos los elementos de data y col
			for (int i = 0; i < nnz; ++i)
			{
				data[i] = Mtx.data[i];
				col[i] = Mtx.col[i];
			}
			// Copiamos los elementos de irow
			for (int i = 0; i < n; ++i)
			{
				irow[i] = Mtx.irow[i];
			}
		};
		/**
			@brief Destructor.
		*/
		~CSR(){freeMemory();};
		/**
			@brief Redimensionamos el tamaño de una matriz CSR en caso
			que se tuvieran elementos en ella estos seran eliminados.
			@param m Aproximación del número total de elementos de la matriz.
			@return None
		*/
		void resize(int m);
		/**
			@brief Se llenan de ceros los conedores data, col y irow.
			@param Ninguno.
			@return None.
		*/
		void zeros();
		/**
			@brief Dada una matriz de tipo COO esta es convertida a 
			una matriz de tipo CSR y almacenada en memoria.
			@param COO matriz de tipo COO.
			@return None
		*/
		void convert(COO const &coo);
		/**
			@brief Realiza una busqueda binaria sobre los contenedores 
			de la matriz para localizar un elemento.
			@param lmin 
			@param lmax
			@return j 
		*/
		int binarySearch(int lmin,int lmax,int j);
		/**
			@brief Devuelve el valor de una componente de la matriz 
			almacenada en este formato.
			@param i Valor entero i del componente buscado.
			@param j Valor entero j del componente buscado.
			@return data Valor real almacenado en la matriz.
		*/
		double search(int i, int j);
		/**
			@brief Devuelve el valor de la variable n.
			@param Ninguno.
			@return n Regresa el contenido de la variable n.
		*/
		inline int gN(){return n;};
		/**
			@brief Devuelve el numero total de elementos
			no cero almacenados en la matriz.
			@param Ninguno.
			@return nnz.
		*/
		inline int gNnz(){return nnz;};
		/**
			@brief Devuelve el valor de data según las coordenadas
			i y j de la matriz. La complejidad de este álgoritmo 
			es de O(log(n)), donde n es el numero maximo de 
			elementos del renglon i-esimo.
			@param l Una localidad del arreglo data.
			@return Contenido de la localidad i del arreglo data.
		*/
		inline double gData(int i)const{return data[i];};
		/**
			@brief Devuelve el valor de col.
			@param l Una localidad del arreglo col.
			@return Contenido de la localidad i del arreglo col.
		*/
		inline int gCol(int i)const{return col[i];};
		/**
			@brief Devuelve el valor de irow.
			@param l Una localidad del arreglo irow.
			@return Contenido de la localidad i del arreglo irow.
		*/
		inline int gIrow(int i)const{return irow[i];};
		/**
			@brief Sobrecarda del operador =, para permitir almacenar 
			una matriz en otra de manera sencilla.
			@param CSR matriz de tipo CSR.
			@return None.
		*/
		void operator =(CSR const &Mtx);
};

#endif // CSR_HPP
