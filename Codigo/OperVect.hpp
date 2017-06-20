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
   @brief Operaciones Vectoriales con Expressions Templates.
*/
#ifndef OPER_VECT_HPP
#define OPER_VECT_HPP

// Implementamos la Suma de dos vectores
template <class L, class R>
class Add
{
	private:
		L const &_l;
		R const &_r;
	public:
		Add(L const &l, R const &r):_l(l),_r(r){};
		double operator[](int n) const {return _l[n]+_r[n];};
};
template <class L, class R>
Add<L,R> operator +(L const &l, R const &r) { return Add<L,R>(l,r); };

// Implementamos la resta de dos vectores
template <class L, class R>
class Sub
{
	private:
		L const &_l;
		R const &_r;
	public:
		Sub(L const &l, R const &r):_l(l),_r(r){};
		double operator[](int n) const {return _l[n]-_r[n];};
};
template <class L, class R>
Sub<L,R> operator -(L const &l, R const &r) { return Sub<L,R>(l,r); };

// Implementamos la multiplicacion de un escalar por un vector
template <class L>
class Mult
{
	private:
		L const &_l;
		double _r;
	public:
		Mult(L const &l, double r):_l(l),_r(r){};
		double operator[](int n)const{return _l[n]*_r;};
};
// Multiplica al vector por la derecha
template <class L>
Mult<L> operator *(L const &l, double r){return Mult<L>(l,r);};
template <class L>
// Multiplica al vector por la izquierda
Mult<L> operator *(double r,L const &l){return Mult<L>(l,r);};

// Implemantamos la multiplicacion Matriz-Vector
template<class M,class L>
class MV
{
	M const  & _m;
	L const  & _l;
	public:
		MV(const M &m,const L &l):_m(m),_l(l){};
		//int size(void)const{return _l.size();};
		double operator[](int i)const
		{
			double tmp = 0.0;
			for (int l = _m.gIrow(i); l < _m.gIrow(i+1); ++l)
			{
				tmp = tmp + _m.gData(l)*_l[_m.gCol(l)];
			}
			return tmp;
        }
};
template<class M,class L>
MV<M,L> operator*(const M &m,const L &l){return MV<M,L>(m,l);};



// Producto Interno de dos vectores
/*
template <class L, class R>
double operator*(L const &l, R const &r)
{
	double tmp = 0.0;
	for (int i = 0; i < l._size; ++i)
	{
		tmp = l[i]*r[i];
	}
	return tmp;
}*/
/*
template <class L, class R>
class PI
{
	private:
		L const _l;
		R const _r;
	public:
		PI(L const &l, R const &r):_l(l),_r(r){};
		double operator[](int n) const {
			double d;
			for (int i = 0; i < l._size; ++i)
			{
				d += _l[i]*_r[i];
			}
		};
};
*/
// Obtiene el producto punto
//template <class L, class R>
//double operator *(L const &l, R const &r){return PI<L,R>(l,r);};


#endif // OPER_VECT_HPP
