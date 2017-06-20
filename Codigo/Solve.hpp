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
	@brief Construye la salida, según lo necesita un archivo externo
*/
#ifndef SOLVE_HPP
#define SOLVE_HPP

//**********************************
// Librerias Necesarias
#include "Mesh.hpp"
#include "Vector.hpp"
#include <string>
#include <sstream>
#include <cmath>
#include <fstream>
// Variable util
#define PI  3.14159265358979323846
//**********************************

class Solve
{
    private:

    public:
        /**
            @brief Constructor vacio para inicializar la clase.
        */
        Solve(){};
        /**
            @brief Almacena en un archivo de texto la solución 
            para cada nodo de la malla segun el formato 
            x y resultado.
            @param Ninguno
            @return None
        */
        void storage(Mesh &mesh, Vector &b, std::string filename);
        /**
            @brief Almacena en un archivo de texto la solución 
            para cada nodo de la malla segun el formato 
            x y resultado.
            @param Ninguno
            @return None
        */
        void storageE(Mesh &mesh, Vector &b, std::string filename);
        /**
            @brief Genera un vector con la solución final, incluyendo 
            los valores conocidos en la malla.
            @param mesh Objeto de tipo mesh con toda la información 
            de la malla usada en la aproximación.
            @param x Vector con la solución aproximada por medio 
            del método CVFEM.
            @param x1 Contenedor para almacenar la solución total
        */
        void solution(Mesh &mesh, Vector &x, Vector &x1);
        /**
            @brief Obtiene la solución analitica para la ecuacion
            de Laplace en 2D y la cual es almacenada en un vector
            y en un archivo de salida con el formato x,y, solución.
            @param mesh Objeto de tipo mesh con toda la información 
            de la malla usada en la aproximación.
            @param filename nombre del archivo en donde se almacenara
            la solución.
            @param exacta Almacena la solución exacta del problema
            en un vector
            @param None
        */
        void laplace2D(Mesh &mesh, std::string filename, Vector &exacta);
        /**
            @brief Obtiene la solución analitica para la ecuacion
            de Laplace en 2D en un semicirculo y la cual es almacenada 
            en un vector y en un archivo de salida con el 
            formato x,y, solución.
            @param mesh Objeto de tipo mesh con toda la información 
            de la malla usada en la aproximación.
            @param filename nombre del archivo en donde se almacenara
            la solución.
            @param exacta Almacena la solución exacta del problema
            en un vector
            @param None
        */
        void laplace2DS(Mesh &mesh, std::string filename, Vector &exacta);
        /**
            @brief Obtiene la solución analitica para la ecuacion
            de Poisson en 2D y la cual es almacenada en un vector
            y en un archivo de salida con el formato x,y, solución.
            @param mesh Objeto de tipo mesh con toda la información 
            de la malla usada en la aproximación.
            @param filename nombre del archivo en donde se almacenara
            la solución.
            @param exacta Almacena la solución exacta del problema
            en un vector
            @param None
        */
        void poisson(Mesh &mesh, std::string filename, Vector &exacta);
        /**
            @brief Almacena la permeabilidad de cada punto de la malla, 
            la componente k11 del vector.
            @param mesh Objeto de tipo mesh con toda la información 
            de la malla usada en la aproximación.
            @param exacta recibe la el vector de permeabildad de la malla.
            @param filename nombre del archivo en donde se almacenara
            la solución.
            @param None
        */
        void permeabilidad(Mesh &mesh, Vector &b, std::string filename);
        //~Solve(); 
};


#endif