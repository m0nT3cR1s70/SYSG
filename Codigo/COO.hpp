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
	la información necesaria, en éste caso se implementa el esquema COO. El 
	esquema COO requiere de tres contenerdores:
		data
		row 
		col
	en data se almacenan los componentes no cero de la matriz, en row se almacenan
	las coordenadas i de la matriz y finalmente en row son almacenadas las coordenadas 
	j. En este caso se usan como contenerdores arreglos estaticos, por tal motivo 
	el usuario debera de indicar de manera aproximada la cantidad de componentes
	totales que almacenara una matriz, a su vez el sistema multiplicara por 30 esta aproximación
	de manera que sea poco factible que se llene.
*/

#ifndef COO_HPP
#define COO_HPP

//***********************
// Librerias necesarias
#include <iostream>
//***********************

class COO
{
	private:
		// Arreglos para almacenar
		double *data; /// Contenedor para almacenar los componentes de una matriz.
		int *row;	 /// Contenedor para almacenar las coordenadas i de una matriz.
		int *col;	 /// Contenedor para almacenar las coordenadas j de una matriz.
		// Variables utiles
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
			@brief Constructor de la clase
		*/
		COO(){};
		/**
			@brief Constructor que inicializa el contenido de las variables
			con una aproximacion de cuanta memomoria se requiere para 
			almacenar una matriz.
			@param m Es un valor entero proporcionado por el usuario 
			que indica de manera aproximada los elementos que son distintos 
			de cero.
		*/
		COO(int m):n(m),nnzMax(m*30),nnz(0){reserveMemory();zeros();};
		/**
			@brief Constructor copia.
			@param COO referencia al objeto a copiar.
		*/
		COO(COO &Acoo);
		/**
			@brief Se redimensiona una matriz para almacenar mas o menos 
			elementos, si la matriz contaba con elementos estos ya no estaran 
			disponibles para el usuario.
			@param m  Es un valor entero proporcionado por el usuario 
			que indica de manera aproximada los elementos que son distintos 
			de cero.
			@return None
		*/
		void resize(int m);
		/**
			@brief Inicializa con ceros todos los elementos de la 
			matriz aproximada.
		*/
		void zeros();
		/**
			@brief Almacena los componentes de una matriz, si se desea almacener
			un elemento en una posicion ya ocupada entonces se suma el valor
			anterior con el valor actual.
			@param i Indice entero i de la matriz.
			@param j Indice entero j de la matriz.
			@param val Valor tipo double con el valor del componente de la matriz
			en el indice i,j.
			@return None
		*/
		void insert(int i, int j, double val);
		/**
			@brief Busca un elemento en la matriz, si este ya se encuentra dentro,
			entonces le suma un valor, si no esta entonces lo almacena de nuevo, esta
			es una operación ineficiente debido que se debe buscar siempre si el 
			valor ya esta dentro de la matriz, su complejidad es del orden O(nnz).
			@param i Indice entero i de la matriz.
			@param j Indice entero j de la matriz.
			@param val Valor tipo double con el valor del componente de la matriz
			en el indice i,j.
			@return None
		*/
		void insertS(int i, int j, double val);
		/**
			@brief Leemos el valor de un elemento de la matriz y lo devolvemos, esta es
			una operación ineficiente su complejidad es del orden O(nnz).
			@param i Indice entero i de la matriz.
			@param j Indice entero j de la matriz.
			@return val Valor tipo double con el valor del componente de la matriz
			en el indice i,j.
		*/
		double value(int i, int j);
		/**
			@brief Devuelve la aproximación dada por el usuario.
			@param Ninguno.
			@return n valor n con la aproximación del usuario.
		*/
		inline int gN()const {return n;};
		/**
			@brief Devuelve el numero de elementos de la matriz.
			@param Ninguno.
			@return nnz
		*/
		inline int gNnz() const {return nnz;};
		/**
			@brief Devuelve el contendio de una localidad de data
			@param l Localidad de data.
			@return data componente del contenedor data.
		*/
		inline double gData(int l) const {return data[l];};
		/**
			@brief Devuelve el contendio de una localidad de row
			@param l Localidad de data.
			@return data componente del contenedor row.
		*/		
		inline int gRow(int l) const {return row[l];};
		/**
			@brief Devuelve el contendio de una localidad de col
			@param l Localidad de data.
			@return data componente del contenedor col.
		*/
		inline int gCol(int l)const {return col[l];};
		/**
			@brief Imprime en pantalla el contenido de la matriz, en formato matriz.
			@param Ninguno.
			@return None.
		*/
		void impMatrix();
		/**
			@brief Destructor de la clase.
		*/
		~COO(){freeMemory();};
};
#endif // COO_HPP