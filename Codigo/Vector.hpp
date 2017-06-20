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
	@brief Clase para dar tratamiento a los vectores.
*/


#ifndef VECTOR_HPP
#define VECTOR_HPP

//******************************
// Librerias Necesarias
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include "OperVect.hpp"
//******************************

class Vector
{
	public:
		// Vector de elementos
		double *_data;
		// Largo del vector
		int _size;
	private:
		// Solicitamos espacio el vector
		void reserveMemory();
		// Liberamos la memoria utilizada
		void freeMemory();
	public:
		/**
			@brief Constructor vacio de la clase Vector.
		*/
		Vector(){};
		/**
			@brief Solicita espacio en memoria para almacenar
			las componentes de un vector.
			@param n Valor entero del numero total de elementos a ser almacenados
		*/
		Vector(int n):_size(n){reserveMemory();zeros();};
		/**
			@brief Constructor copia.
			@param v Copia el contenido de un vector en otro.
		*/
		Vector(Vector const &v):_size(v._size){reserveMemory();for (int i = 0; i < _size; ++i)_data[i]=v[i];}
		/**
			@brief Devuelve el tamaño de un vector.
			@param Ninguno
			@return size Tamaño del vector.
		*/
		inline int size(){return _size;};
		/**
			@brief Redimensiona un vector.
			@param n Nuevo tamaño de un vector
			@return None
		*/
		void resize(int n);
		/**
			@brief Calcula la norma euclidiana del dado.
			@param Ninguno
			@return euclidana Devuelve la norma euclidiana del vector.
		*/
		double norm();
		/**
			@brief Calcula la norma rms del vector.
			@param Ninguno
			@return rms Norma rms del vector dado.
		*/
		double rms();
		/**
			@brief Llena de ceros un vector.
			@param Ninguno.
			@return None.
		*/
		void zeros();
		/**
			@brief Almacena el contenido de un vector en un archivo
			@param name Nombre de un archivo para almacenar el contenido de un vector
			@return None
		*/
		void saveData(std::string name);
		/**
			@brief Devuelve un elemento del vector dado su localidad de memoria, es 
			una función de tipo get.
			@param i Localidad en donde se encuentra un elemento de un vector.
			@return data componente del vector.
		*/
		double operator  [](int i) const {return _data[i];}; 
		/**
			@brief Devuelve una referencia a un vector de tal manera que 
			su contenido pueda ser modificado.
			@param i Localidad en donde se encuentra un elemento de un vector.
			@return data referencia de un componente del vector.
		*/
		double &operator [](int i) {return _data[i];};
		/**
			@brief Sobrecarga del operador igual.
			@param un vector
		*/
		template<class Expr>
		Vector const &operator = (Expr const &expr) 
		{
			for (int i = 0; i < _size; ++i)
			{
				_data[i] = expr[i];
			}
			return *this;
		};
		// Sobrecarga del operador = para asignar un escalar
		// Inicializa un vector con un valor dado
		void operator =(double escalar);
		// Iguala un vector con otro vector
		void operator =(Vector const &v);
		/**
			@brief Muestra en pantalla el contenido de un vector.
			@param Ninguno.
			@return None.
		*/
		void print();
		//~Vector(){freeMemory();};
		// Operaciones Vectoriales
		/**
			@brief Se implmenta el producto punto dados dos vectores.
		*/
		friend double operator *(Vector const &v,Vector const &v1);
};
#endif // VECTOR_HPP
