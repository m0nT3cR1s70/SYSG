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
    @brief Construye el precondicionador Jacobi.
*/

#ifndef JACOBI_P_HPP
#define JACOBI_P_HPP

//----------------------------------
// Librerias Necesarias
#include <iostream>
#include <string>
//----------------------------------

template<class P>
class JacobiP
{
	private:
		double *D; 	// Contiene la diagonal de la matriz
		int n; // Numero de elementos de la diagonal
		void reserveMemory(); // Reserva memoria para la diagonal
		std::string namep;
		// Libera memoria en la diagonal
		void freeMemory()
		{
			if(D == nullptr) 
			{
				delete [] D;
			}
		}
	public:
		// Constructor vacio
		JacobiP():namep("Jacobi"){};
		// Libera la memoria utilizada
		~JacobiP(){freeMemory();};
		// Devuelve el nombre del precondicionador
		inline std::string name(){return namep;};
		// Calcula el precondicionador
		void calculate(P  &Mtx);
		// Resuelve el sistema de ecuaciones
		void solve(Vector &z, Vector const &r);
		// Muestra en pantalla el contenido de la diagonal
		void impDiag();
};
// -------------------------------------------------
// Reservamos memoria
template <class P>
void JacobiP<P> :: reserveMemory()
{
	// Seleccionamos espacio para la diagonal
	D = new double[n];
	// Verificamos que la memoria fuera asignada
	if (D == nullptr)
	{
		std::cout << "ERROR: MEMORIA NO ASIGNADA: reserveMemory: JacobiP" << std::endl;
		exit(0);
	}
}
// Calculamos el precondicionador
template<class P>
void JacobiP<P> :: calculate(P &Mtx)
{
	n = Mtx.n;
	reserveMemory();
	// Hacemos una busqueda sobre el arreglo
	for (int i = 0; i < n; ++i)
	{
		D[i] = Mtx.search(i,i);
	}
}
// Calculamos la solucion del sistema
template<class P>
void JacobiP<P> ::solve(Vector &z, Vector const &r)
{
	// Calculamos la nueva z
	for (int i = 0; i < n; ++i)
	{
		z[i] = r[i]/D[i];
	}
}
template <class P>
void JacobiP<P> :: impDiag()
{
	for (int i = 0; i < n; ++i)
	{
		std::cout<<D[i]<<std::endl;
	}
}

#endif // JACOBI_P_HPP
