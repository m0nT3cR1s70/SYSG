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
   @brief Se implementa el método numerico BICGSTAB para la solución
   de sistemas de ecuaciones lineales. En este caso sirve para la matriz
   de tipo CSR.
*/

#ifndef BICGSTAB_HPP
#define BICGSTAB_HPP

#include <cmath>
#include <iostream>
#include <string>
#include "CSR.hpp"

#include "Vector.hpp"
#include "Timer.hpp"
#include <sstream>
#include <fstream>


class BICGSTAB {
private:
   // Variables necesarias
   int _iterMaxP; /// Iteraciones maximas permitidas 
   int _iterP;  /// Iteraciones totales usadas para la convergencia
   double _tolP; /// Tolerancia de error
   double _errP; // Error calculado
   double _etimeP; // Tiempo utilizado para resolver el problema
   Timer _timerP; // Variable para medir el tiempo utilizado para el problema
   bool precond; // Si el metodo fue precondicionado
   std::string namep; // Nombre del Precondicionador

public:
   /**
   */
   BICGSTAB(): _iterMaxP(2000),_tolP(1e-6),_iterP(0),_errP(0){};
   
   template<class T>
   void solve(const T &A, Vector &x,const Vector &b,std :: string residuo = "BICGSTAB.txt")
   {

      _timerP.tic();
      double tol2 = _tolP*_tolP;
      int n = x.size();
      Vector r(n),hatr(n),p(n),v(n),s(n),t(n),x0(n);
      double alpha=1;
      double beta=1;
      double omega=1;
      double rho=1,rho0=1;
      double tdt;
      int k;
      // Variable para almacenar el residuo del metodo
      std::ofstream file(residuo,std::ios::app);
      r = b-A*x;
      hatr = r;
      p=0.0;
      v=0.0;
      s=0.0;
      t=0.0;
      
      for(k =0;k<_iterMaxP;++k)
      {
         rho = hatr*r;
         beta = (rho/rho0)*(alpha/omega);
         p = r+beta*(p-omega*v);
         v=A*p;
         alpha =  rho / (hatr*v);
         s = r-alpha*v;
         t = A*s;
         tdt = t*t;
         if(tdt<1e-30) omega =1.;
         else omega = (t*s)/(t*t);         
         
         x = x + alpha*p+omega*s;
         _errP = s*s;
         //std::cout << "# it "<< k << " error " << _errP<< " rho "<< rho<< " (t,t) "<<(t*t) << " omega " << omega << " alpha "<< alpha  <<std::endl;         
         file <<  k+1 << " " << sqrt(_errP) << std :: endl;
         if(_errP < tol2)
            break;
         r = s - omega*t;
         rho0 = rho;
      }
      _iterP = k+1;
      _errP = sqrt(_errP);
      _timerP.toc();
      _etimeP = _timerP.etime();
      precond = false;
      
   }

   template<class T, class U>
   void solve(T &A, Vector &x, Vector &b,U &M,std :: string residuo = "BICGSTABP.txt")
   {

       _timerP.tic();
       double tol2 = _tolP*_tolP;
       int n = x.size();
       Vector r(n),hatr(n),p(n),v(n),s(n),t(n);
       Vector y(n),o(n),tt(n),ss(n);
       double alpha=1;
       double beta=1;
       double omega=1;
       double rho=1,rho0=1;
       double tdt;
       int k;
       r = b-A*x;
       hatr = r;
       p=0.0;
       v=0.0;
       s=0.0;
       t=0.0;
      std::ofstream file(residuo,std::ios::app);
       for(k =0;k<_iterMaxP;++k)
       {
          rho = hatr*r;
          beta = (rho/rho0)*(alpha/omega);
          p = r+beta*(p-omega*v);
          M.solve(y,p);
          v=A*y;
          alpha =  rho / (hatr*v);
          s = r-alpha*v;
          M.solve(o,s);
          t = A*o;

          //M.solve_forward(tt,t);
          //M.solve_forward(ss,s);
          tdt = t*t;
          if(tdt<1e-30)omega = 1.;
          else  omega = (t*s)/tdt;
          x = x + alpha*y+omega*o;

          _errP = s*s;
          file <<  k+1 << " " << sqrt(_errP) << std :: endl;
          //std::cout << "# it "<< k << " error " << _errP<< " rho "<< rho<< " (t,t) "<<(t*t) << " omega " << omega << " alpha "
          //<< alpha << " y*y " << (y*y) << " o*o " << (o*o) << " s*s " << (s*s) << " r*r " << (r*r)  
          //<< " v*v " << (v*v) << " alpha " << alpha
          //<<std::endl;         
          if(_errP < tol2)
             break;
          r = s - omega*t;
          rho0 = rho;
       }
       _iterP = k+1;
       _errP = sqrt(_errP);
       _timerP.toc();
       _etimeP = _timerP.etime();
       precond = true;
       namep = M.name();
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
      std::cout << "BICGSTAB";
      if(precond)
          std::cout << " precondicionado con "<< namep ;
      std::cout << "\n";
      std::cout << "  Iteracinoes  "<< _iterP<< std::endl;
      std::cout << "  Error "<< _errP << std::endl;
      std::cout << "  Tiempo de calculo "<< _etimeP << " msec" << std::endl;
   }
};

#endif
