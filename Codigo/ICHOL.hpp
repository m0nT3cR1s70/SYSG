#ifndef ICHOL_HPP
#define ICHOL_HPP

//-----------------------------
// Librerias Necesarias
#include <iostream>
#include "CSR.hpp"
//-----------------------------

template<class P>
class ICHOL
{
    private:
        // Objeto que contiene la matriz L
        P L;
        // Almacena del nombre del precondicionador
        std::string namep;
    public:
        // Constructor vacio
        ICHOL():namep("ICHOL"){};
        // Devuelve el nombre del precondicionador
        inline std::string name(){return namep;};
        // Calcula el precondicionador
        void calculate(P const &Mtx);
        // Resuelve un sistema de ecuaciones usando sustitucion hacia adelante y hacia atras
        void solve(Vector &x, Vector const &b);
        // Imprimir resultados
        void imchol();
};
template<class P>
void ICHOL<P> :: calculate(P const &Mtx)
{
    // Copiamos el contenido de la matriz
    L = Mtx;
    // Variables utiles
    int j = 0;
    double dot = 0.0;
    int lli = 0;
    int llj = 0;
    int ji = 0;
    int jj = 0;
    // indices de la diagonal
    L.data[L.idiag[0]] = sqrt(Mtx.data[L.idiag[0]]);
    // Comenzamos con la factorizacion
    for (int i = 0; i < L.n; ++i)
    {
        for (int l = L.irow[i]; l < L.idiag[i]; ++l)
        {
            j = L.col[l];
            lli = L.irow[i];
            llj = L.irow[j];
            ji = L.col[lli];
            jj = L.col[llj];
            L.data[l] = Mtx.data[l];
            while(lli < L.irow[i+1] && llj < L.irow[j+1]) 
            {
                if (ji == jj)
                {
                    if (ji < j && jj < j)
                    {
                        L.data[l] = L.data[l]-L.data[lli]*L.data[llj];
                    }
                    lli = lli + 1;
                    llj = llj + 1;
                    ji = L.col[lli];
                    jj = L.col[llj];
                }
                else if (ji < jj)
                {
                    lli = lli + 1;
                    ji = L.col[lli];
                }
                else
                {
                    llj = llj + 1;
                    jj = L.col[llj];
                }
            }
            L.data[l] = L.data[l]/L.data[L.idiag[j]];
        }
        dot = 0.0;
        for (int l = L.irow[i]; l < L.idiag[i]; ++l)
        {
            dot = dot + L.data[l]*L.data[l];
        }
        L.data[L.idiag[i]] = sqrt(Mtx.data[L.idiag[i]]-dot);
    }
}
// Solve de ICHOL
template<class P>
void ICHOL<P> :: solve(Vector &x, Vector const &b)
{

    Vector aux(x._size);
    double taux = 0.0;
    // Sustitucion hacia adelante
    //x = b;
    for (int i = 0; i < x._size; ++i)
    {
        aux[i] = b[i];
        for (int l = L.irow[i]; l < L.idiag[i]; ++l)
        {
            aux[i] = aux[i] - L.data[l]*aux[L.col[l]];
        }
        aux[i] = aux[i]/L.data[L.idiag[i]];
    }
    // Sustitucion hacia atras
    x = aux;
    for (int j = 0; j <= 0; ++j)
    {
        taux = aux[j]/L.data[L.idiag[j]];
        for (int l = 0; l < L.idiag[j]; ++l)
        {
            x[L.col[l]] = x[L.col[l]] - taux*L.data[l];
        }
        x[j] = taux;
    }
}
template<class P>
void ICHOL<P> :: imchol()
{
    std::cout << "Data" << std::endl;
    for (int i = 0; i < L.nnz; ++i)
    {
        std::cout<<L.data[i]<<" ";
    }
    std::cout<<std::endl;
    std::cout << "col" << std::endl;
    for (int i = 0; i < L.nnz; ++i)
    {
        std::cout<<L.col[i]<<" ";
    }
    std::cout<<std::endl;
    std::cout << "irow" << std::endl;
    for (int i = 0; i < L.n+1; ++i)
    {
        std::cout<<L.irow[i]<<" ";
    }
    std::cout<<std::endl;
}

#endif // ICHOL_HPP
