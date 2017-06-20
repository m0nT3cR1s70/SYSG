#include "Solve.hpp"
/*
	Devuelve un archivo de salida con la solucion de cada
	nodo
*/
void Solve :: storage(Mesh &mesh, Vector &b, std::string filename)
{
	std::ofstream file(filename.c_str(),std::ios::app);
	for (int i = 0; i < mesh.nNode(); ++i)
	{
		file << mesh.coordX(i+1) << " " << mesh.coordY(i+1) << " " << b[i] << std :: endl;
	}
}
/*
	Devuelve un archivo de salida con el error de cada
	nodo
*/
void Solve :: storageE(Mesh &mesh, Vector &b, std::string filename)
{
	std::ofstream file(filename.c_str(),std::ios::app);
	for (int i = 0; i < mesh.nNode(); ++i)
	{
		file << mesh.coordX(i+1) << " " << mesh.coordY(i+1) << " " << fabs(b[i]) << std :: endl;
	}
}
/**/
void Solve :: solution(Mesh &mesh, Vector &x, Vector &x1)
{
	// Variables temporales
	int tmp = 0;
	int tmp1 = 0;
	std::vector<int> incognite = mesh.Incognite();
	std::vector<int> border = mesh.bordeNode();
	for (int i = 0; i < mesh.nNode(); ++i)
	{
		if (std::binary_search(border.begin(),border.end(),i+1))
		{
			x1[i] = mesh.value(i+1);
			tmp1++;
		}
		if (std::binary_search(incognite.begin(),incognite.end(),i+1))
		{
			x1[i] = x[tmp];
			tmp++;
		}
	}
}
/*
	Implementamos la solucion exacta del problema de laplace 
	en 2D
*/
void Solve :: laplace2D(Mesh &mesh, std::string filename, Vector &exacta)
{
	int n = 100;
	double T = 100.0;
	std::ofstream file(filename.c_str(),std::ios::app);
	// Calculamos la solución exacta de la ecuacion de calor
	for (int i = 0; i < mesh.nNode(); ++i)
	{
		for (int j = 1; j < n; ++j)
		{
			exacta[i] += ((1-pow(-1,j))/(PI*j))*(sinh(2.0*PI*j-mesh.coordY(i+1)*j*PI)/sinh(2.0*j*PI))*sin(j*PI*mesh.coordX(i+1));
			//std :: cout << exacta[i] << std:: endl;
		}
		exacta[i] *= 2.0*T;
		file << mesh.coordX(i+1) << " " << mesh.coordY(i+1) << " " << exacta[i] << std :: endl;
	}
	//exacta = 2.0*T*exacta;
}
/*
	Implementamos la solucion exacta para el problema de Laplace
	en la semicircunferencia.
*/
void Solve :: laplace2DS(Mesh &mesh, std::string filename, Vector &exacta)
{
	// Variables tmporales
	std::vector<double> r;
	std::vector<double> theta;
	r.resize(mesh.nNode());
	theta.resize(mesh.nNode());
	std::ofstream file(filename.c_str(),std::ios::app);
	// Transformamos las coordenadas a su valor polar
	for (int i = 0; i < mesh.nNode(); ++i)
	{
		r[i] = sqrt(pow(mesh.coordX(i+1),2) + pow(mesh.coordY(i+1),2));
		theta[i] = atan(mesh.coordY(i+1)/mesh.coordX(i+1));
		// std :: cout << r[i] << " " << theta[i] << " " << mesh.nNode() << std :: endl;
		//std :: cout << mesh.coordX(i) << " " << mesh.coordY(i) << " " << atan(mesh.coordY(i)/mesh.coordX(i)) << std :: endl;
	}
	// Calculamos la funcion dados los puntos polares
	for (int i = 0; i < mesh.nNode(); ++i)
	{
		exacta[i] = sin(theta[i])/r[i];
		file << mesh.coordX(i+1) << " " << mesh.coordY(i+1) << " " << exacta[i] << std :: endl;
	}
}
/*
	Implementamos la solucion exacta para el problema de Poisson
	en 2D.
*/
void Solve :: poisson(Mesh &mesh, std::string filename, Vector &exacta)
{
	std::ofstream file(filename.c_str(),std::ios::app);
	for (int i = 0; i < mesh.nNode(); ++i)
	{
		exacta[i] = cos(PI*mesh.coordX(i+1))*cos(PI*mesh.coordY(i+1));
		// Almacenamos en un archivo
		file << mesh.coordX(i+1) << " " << mesh.coordY(i+1) << " " << exacta[i] << std :: endl;
	}
}
/*
	Almacena la permeabilidad k11
*/
void Solve :: permeabilidad(Mesh &mesh, Vector &b, std::string filename)
{
	std::ofstream file(filename.c_str(),std::ios::app);
	for (int i = 0; i < mesh.nNode(); ++i)
	{
		file << mesh.coordX(i+1) << " " << mesh.coordY(i+1) << " " << b[i] << std :: endl;
	}
}