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
	@brief Cosnstruye la matriz A y el vector b del sistema de ecuaciones
    AX=b
*/

#ifndef CVFEM_HPP
#define CVFEM_HPP

//****************************************
// Librerias Necesarias
#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <string>
#include <sstream>
#include <cmath>
#include <fstream>
#include <algorithm>
#include <random>
#include "Mesh.hpp"
#include "CSR.hpp"
#include "COO.hpp"
// Variable util
#define PI  3.14159265358979323846
//****************************************

class CVFEM
{
    private:
        // Variables Necesarias
        double _tolP = 1e-10;
        // Valores utiles
        double _permeabilidadP;
        double _compresibilidadP;
        double _porosidadP;
        double _densidadP;
        double _gravedadP;
        double _fuenteP;
        std::vector<std::vector<double>> Tv;
        // Para la permeabilidad
        Vector _k11;
        Vector _k22;
        // Coeficientes de la matriz
        CSR _coefM;
    public:
        /**
            @brief Inicializa los atributos oir defecto da a la permeabilidad un valor de 1, a la compresibilidad 1.0, 
            a la fuente -10, a la porosidad 0.2 a la densidad 1 y a la gravedad 0
        */
        CVFEM():_permeabilidadP(1),_compresibilidadP(1),_fuenteP(-10),_porosidadP(0.2),_densidadP(1),_gravedadP(0){};
        /**
            @brief Inicializa los atributos oir defecto da a la permeabilidad un valor de 1, a la compresibilidad 1.0, 
            a la fuente -10, a la porosidad 0.2 a la densidad 1 y a la gravedad 0
        */
        CVFEM(double perme, double compre, double fuen, double poro, double dens, double grav):_permeabilidadP(perme),_compresibilidadP(compre),_fuenteP(fuen),_porosidadP(poro),_densidadP(dens),_gravedadP(grav){};
        /**
            @brief Constructor copia
            @param cvfem Recibe un onbeto tipo CVFEM para copiarlo en otro
        */
        CVFEM(CVFEM &cvfem);
        /**
            @brief Construye el sistema Ax = b segun una permeabilidad homogenea
            @param mesh Recibe toda la información de la geometria.
            @param A matriz CSR para dar un almacenamiento minimo.
            @param b Vector V para su construcción.
        */
        void Ab(CSR &A, Vector &b, Mesh &mesh);
        /**
            @brief Construye el sistema Ax = b segun una permeabilidad homogenea
            @param mesh Recibe toda la información de la geometria.
            @param A matriz CSR para dar un almacenamiento minimo.
            @param b Vector V para su construcción.
        */
        void Ab(CSR &A, Vector &b, Mesh &mesh, std::vector<std::pair<double,double>> &alta, std::vector<std::pair<double,double>> &baja);
        /**
            @brief Construye la matriz de transmisibilidad usando cada nodo de la malla.
            @param mesh Recibe toda la información de la geometria.
            @param A matriz CSR para dar un almacenamiento minimo.
        */
        void matrixCFVEM(Mesh &mesh, CSR &csr);
        /**
            @brief Construye el vector para la ecuación de laplace, considerando las condiciones de 
            frontera y los coeficientes de la matriz
            @param mesh Objeto tipo mesh, con la información de la geometría de interés.
            @param b Vector para almacenar el contendio del vector calculado
            @return None
        */
        void laplaceVector(Mesh &mesh, Vector &b);
        /**
        */
        void poissonVector(Mesh &mesh, Vector &b);
        /**
            @brief Calcula la permeabilidad para distintos lugares del dominio.
            @param mesh Recibe la malla discretizada para asignar a cada nodo una permeabilidad aleatoria.
            @return None.
        */
        void permeabilidad(Mesh &mesh, std::vector<std::pair<double,double>> &alta, std::vector<std::pair<double,double>> &baja);
        /**
            @brief Calcula un promedio armonico de una propiedad dados dos valores distintos.
            @param node1 Recibe el valor de una propiedad en un nodo.
            @param node2 Recibe el valor de una propoiedad en un nodo.
            @return Promedio armonico de una propiedad dados dos nodos.
        */
        inline double permehar(double node1, double node2){return (2*node1*node2)/(node1+node2);};
        /**
            @brief Calcula un promedio de una propiedad dados dos valores distintos.
            @param node1 Recibe el valor de una propiedad en un nodo.
            @param node2 Recibe el valor de una propoiedad en un nodo.
            @return Promedio de una propiedad dados dos nodos.
        */
        inline double permeAverage(double node1, double node2){return (node1+node2)/2;};
        /**
            @brief Devuelve la permeabilidad
            @param Ninguno
            @return Permeabilidad
        */
        inline double gPerme(){return _permeabilidadP;};
        /**
            @brief Modifica el valor de la permeabilidad
            @param permeabilidad Valor real con el nuevo valor de permeabilidad
            @return None
        */
        inline void sPerme(double permeabilidad){_permeabilidadP = permeabilidad;};
        /**
            @brief Devuelve la compresibilidad
            @param Ninguno
            @return compresibilidad
        */
        inline double gCompre(){return _compresibilidadP;};
        /**
            @brief Modifica el valor de la compresibilidad
            @param Ninguno
            @return None
        */
        inline void sCompre(double compresibilidad){_compresibilidadP = compresibilidad;};
        /**
            @brief Devuelve la densidad
            @param Ninguno
            @return densidad
        */
        inline double gDensidad(){return _densidadP;};
        /**
            @brief Modifica el valor de la densidad
            @param Ninguno
            @return None
        */
        inline void sDensidad(double densidad){_densidadP = densidad;};
        /**
            @brief Devuelve la gravedad
            @param Ninguno
            @return gravedad  
        */
        inline double gGravedad(){return _gravedadP;};
        /**
            @brief Modifica el valor de la gravedad
            @param gravedad Nuevo valor para la gravedad
            @return None
        */
        inline void sGravedad(double gravedad){_gravedadP = gravedad;};
        /**
            @brief Devuelve la fuente
            @param Ninguno
            @return fuente  
        */
        inline double gFuente(){return _fuenteP;};
        /**
            @brief Modifica el valor de la fuente
            @param fuente Nuevo valor para la fuente
            @return None
        */
        inline double sFuente(double fuente){_fuenteP = _fuenteP;};
        /**
            @brief Devueve el vector k11.
            @param Ninguno.
            @return k11 vector tipo K11 con la permeabilidad en cada nodo
            de la malla.
        */
        inline Vector& gK11(){return _k11;};
        /**
            @brief Devueve el vector k22.
            @param Ninguno.
            @return k22 vector tipo K22 con la permeabilidad en cada nodo
            de la malla.
        */
        inline Vector& gK22(){return _k22;};
        /**
            @brief Devueve el un elemento del vector k11.
            @param Node.
            @return k11 Valor del tensor de permeabilidad en un punto.
        */
        inline double K11(int node){return _k11[node-1];};
        /**
            @brief Devueve el un elemento del vector k22.
            @param Node.
            @return k22 Valor del tensor de permeabilidad en un punto.
        */
        inline double K22(int node){return _k22[node-1];};
        /**
            @brief Funcion para comprobar las funciones base de cada elemento triangular
            @param a Coeficiente a de la función de forma.
            @param b Coeficiente b de la función de forma.
            @param c Coeficiente c de la función de forma.
            @param x1 Coordenada x1 en donde se evalua.
            @param x2 Coordenada x2 en donde se evalua.
            @param K Area del elemeto triagular.
            @return eval Valor real producido por evaluar la función de forma.
        */
        inline double baseFunction(double a, double b, double c, double x1, double x2, double K){return (0.5)*(1/K)*(a*x1+b*x2+c);};
        //~CVFEM();
        void matrixCVFEMW(Mesh &mesh, CSR &A); // Para nodos extraños
};



#endif