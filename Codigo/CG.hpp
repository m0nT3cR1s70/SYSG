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
   @brief Se implementa el método numerico CG para la solución
   de sistemas de ecuaciones lineales. En este caso sirve para la matriz
   de tipo CSR.
*/
#ifndef CG_HPP
#define CG_HPP

#include <cmath>
#include <iostream>
#include <chrono>
#include <string>

//#include "Matrix.hpp"
#include "Vector.hpp"
#include "CSR.hpp"
#include "Timer.hpp"

#include <sstream>
#include <fstream>

class CG {
private:
   // Variables necesarias
   int _iterMaxP; /// Iteraciones maximas permitidas 
   int _iterP;  /// Iteraciones totales usadas para la convergencia
   double _tolP; /// Tolerancia de error
   double _errP; // Error calculado
   double _etimeP; // Tiempo utilizado para resolver el problema
   Timer _timerP; // Variable para medir el tiempo utilizado para el problema
   bool precond;
   std::string namep;
public:
   /**
      @brief Constructor vacio del metodo CG, asigna por defecto 
      un numero de 2000 iteaciones maximas, una toleracia de 1e-6.
      @param Ninguno
   */
   CG():_iterMaxP(2000),_tolP(1e-6), _iterP(0),_errP(0),_etimeP(0){};
   /**
      @brief Aplica el metodo de gradiente conjugado por medio, fue realizado
      usando programación generica.
      @param A Matriz de tipo CSR.
      @param x Vector
      @param b Vector
      @param residuo Archivo txt que almacena el residuo del metodo 
      @None
   */
   template<class T>
   void solve(T &A, Vector &x, Vector &b)
   {

      _timerP.tic();
      double tol2 = _tolP*_tolP;
      int n = x.size();
      Vector r(n);
      Vector p(n);
      Vector Ap(n);
      double alpha;
      double beta;
      double rr0,rr;
      int k;
      r = b-A*x;
      rr0 = r*r;
      p=r;
      for(k =0;k<_iterMaxP;++k)
      {
         Ap = A*p;
         alpha =  (rr0) / (Ap*p);
         x = x + alpha*p;
         r = r - alpha*Ap;
         rr = r*r;
         _errP = rr;
         if(_errP < tol2)
            break;
         beta = rr / rr0; 
         p = r + beta*p;
         rr0 = rr;
      }
      _iterP = k+1;
      _errP = sqrt(_errP);
      _timerP.toc();
      _etimeP = _timerP.etime();
      precond = false;
   }
   
   template<class T, class U>
   void solve(T &A, Vector &x, Vector &b,U &M)  //T: tipo de matriz de entrada, 
                                                //U: tipo de precondicinador de entrada
   {

      _timerP.tic();                 //comienza a medir tiempo de ejecucion
      double tol2 = _tolP*_tolP;
      int n = x.size();
      Vector r(n),z(n),p(n),Ap(n);
      double alpha;
      double beta;
      double rr0,rr;
      int k;
      r = b-A*x;                    //primer residual operacion vector = vector - matriz*vector
      M.solve(z,r);                 //resuleve el sistema Mz=r
      p=z;                          //copia de vector
      rr0 = z*r;                    //producto interno de dos vectores
      for(k =0;k<_iterMaxP;++k)
      { 
         Ap = A*p;                  //producto matriz vector
         alpha =  (rr0) / (Ap*p);   //producto interno en el divisor
         x = x + alpha*p;           //operacion vector = vector + escalar*vector (saxpy en BLAS)
         r = r - alpha*Ap;
         M.solve(z,r);              //resuleve el sistema Mz=r
         rr = z*r;                  //producto interno
         _errP = r*r;              //producto interno
         if(_errP < tol2)
            break;
         beta = rr / rr0; 
         p = z + beta*p;            //operacion vector = vector + escalar*vector
         rr0 = rr;
      }
      _iterP = k+1;
      _errP = sqrt(_errP);
      _timerP.toc();                 //termina de medir tiempo
      _etimeP = _timerP.etime();       //guarda en etime el tiempo de ejecucion
      precond = true;
      namep = M.name();            //guarda el nombre del precondicionador
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
      std::cout << "Gradiente conjugado";
      if(precond)
          std::cout << " precondicionado con "<< namep ;
      std::cout << "\n";
      std::cout << "  Iteraciones  "<< _iterP<< std::endl;
      std::cout << "  Error "<< _errP << std::endl;
      std::cout << "  Tiempo de calculo "<< _etimeP << " msec" << std::endl;
   }
};

#endif
