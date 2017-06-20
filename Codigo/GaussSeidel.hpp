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
	@brief Se implementa el método numerico Gauss-Seidel para la solución
	de sistemas de ecuaciones lineales. En este caso sirve para la matriz
	de tipo CSR.
*/

#ifndef GaussSeidel_HPP
#define GaussSeidel_HPP

//*************************
// Librerias necesarias
#include <iostream>
#include <cmath>
#include "CSR.hpp"
#include "Vector.hpp"
#include "Timer.hpp"
//*************************

class GaussSeidel
{
	private:
		// Variables necesarias
		int _iterMaxP; /// Iteraciones maximas permitidas 
		int _iterP;  /// Iteraciones totales usadas para la convergencia
		double _tolP; /// Tolerancia de error
		double _errP; // Error calculado
		double _etimeP; // Tiempo utilizado para resolver el problema
		Timer _timerP; // Variable para medir el tiempo utilizado para el problema
	public:
		// Constructor de la clase
		GaussSeidel():_iterMaxP(2000),_tolP(1e-6),_iterP(0),_errP(0),_etimeP(0){};
		template<class T>
		void solve(T &A, Vector &x, Vector &b)
		{
			// Variables
			_timerP.tic(); // Comenzamos a medir el tiempo
			int j = 0;
			double diag = 0.0;
			double suma = 0.0;
			double tol2 = _tolP*_tolP;
			int n = x.size();
			// Comenzamos con el metodo de GaussSeidel
			for (int k = 0; k < _iterMaxP; ++k)
			{
				for (int i = 0; i < n; ++i)
				{
					
					suma = 0.0;
					for (int l = A.gIrow(i); l < A.gIrow(i+1); ++l)
					{
						j = A.gCol(l);
						if (i != j)
						{
							suma +=  A.gData(l) * x[j];
						}
						else
						{
							diag = A.gData(l);
						}
					}
					_errP += (b[i]-suma-diag*x[i]) * (b[i]-suma-diag*x[i]);
					x[i] = (b[i] - suma) /diag;
				}
				// Iteraciones realizadas
				_iterP++;
				// Revisamos la tolerancia de paro
				if (_errP < tol2) break;

			}
			// Terminamos de medir el tiempo usado
			_timerP.toc();
			_errP = sqrt(_errP);
      		_etimeP = _timerP.etime();
		}
		/**
			@brief Devuelve el numero de iteraciones realizadas.
			@param Ninguno.
			@return _iterP Numero de iteraciones utilizadas para la 
			convergencia del metodo.
		*/
		inline int its(){return _iterP;};
		/**
			@brief Devuelve el error calculado de la matriz.
			@param Ninguno.
			@return _errP Error alcanzado con el método Jacobi.
		*/
   		inline double error() {return _errP;};
   		/**
   			@brief Tiempo utilizado por el algoritmo de Jacobi.
   			@param Ninguno.
   			@return _etimeP Tiempo usado por el algoritmo de Jacobi.
   		*/
   		double tim(){return _etimeP;};
   		/**
   			@brief Define un nuevo numero de iteraciones maximas permitidas.
   			@param i Iteraciones nuevas permitidas.
   			@return None.
   		*/
   		void maxIts(int it){ _iterMaxP = it;};
   		/**
   			@brief Define una nueva tolerancia para el método de Jacobi.
   			@param t Valor double con la nueva tolerancia.
   			@return None.
   		*/
   		void tole(double t) {_tolP=t;};
   		/**
   			@brief Muestra en pantalla un reporte del algoritmo Jacobi.
   			@param Ninguno.
   			@return None.
   		*/
   		void report()
   		{
      		std::cout << "GaussSeidel";
      		std::cout << "\n";
      		std::cout << "  Iteraciones  "<< _iterP << std::endl;
      		std::cout << "  Error "<< _errP << std::endl;
      		std::cout << "  Tiempo de calculo "<< _etimeP << " msec" << std::endl;
   		};
};

#endif // GaussSeidel_HPP
