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
	@brief Solucion de los problemas tipos Benchmark y de la validacion
*/
//******************************
// Librerias Necesarias
#include "Mesh.hpp"
#include "CVFEM.hpp"
#include "Vector.hpp"
#include "CSR.hpp"
#include "Solve.hpp"
#include "CG.hpp"
#include "BICGSTAB.hpp"
#include "JacobiP.hpp"
#include "ILU.hpp"
#include "MILU.hpp"
//******************************

void laplace()
{
	// Definimos un objeto para el tratamiento de la malla
	Mesh mesh("./GMSH/Laplace/mesh.msh");
	// Cargamos las condiciones de frontera 
	std::vector<int> border = mesh.bordeNode();
	for (int i = 0; i < border.size(); ++i)
	{
		if (mesh.coordY(border[i]) == 0 && (mesh.coordX(border[i]) != 0 && mesh.coordX(border[i]) != 1))
		{
			mesh.bondaryCondition(0,100);
		}
		else
		{
			mesh.bondaryCondition(0,0);
		}
	}
	// Distinguimos los nodos Neumman y Dirichlet
	mesh.typeNodeBorder();
	// Mostramos el valor de las condiciones de frontera
	mesh.showBoundaryCondition();
	// Es necesario determinar los nodos incognita
	mesh.nodeIncongnite();
	mesh.showTotal();
	// Definimos las variables necesarias
	CSR A(mesh.nNodeIncongnite());    // Matriz CVFEM
	Vector x(mesh.nNodeIncongnite()); // Vector x
	Vector b(mesh.nNodeIncongnite()); // Vector b
	Solve solver; // Para el trataiento de las soluciones
	// Objeto para crear el sistema de ecuaciones
	CVFEM cvfem;
	cvfem.Ab(A,b,mesh);
	// Verificamos el valor de b
	b.print();
	// Resolvemos el sistema de ecuaciones lineales
	BICGSTAB bicg; 
	bicg.solve(A,x,b);
	std :: cout << std :: endl;
  	bicg.report();
  	// Usando precondicionador
  	ILU<CSR> milu;                //Precondicionador LU incompleto modificado
	std :: cout << std :: endl << std :: endl;
  	milu.calculate(A);              //calculo del precondicionador
  	x=0.0;
  	bicg.solve(A,x,b,milu);
  	bicg.report();
  	// Usando Gradiente conjugado
  	CG cg;
  	x = 0.0;
  	cg.solve(A,x,b);
	std :: cout << std :: endl;
  	cg.report();
  	// Usando precondicionador
  	x = 0.0;
  	cg.solve(A,x,b,milu);
	std :: cout << std :: endl;
  	cg.report();
  	//x.print();
	// Calculamos la solucion exacta
	Vector x1(mesh.nNode()); 		  // Vector b para la solución final
	Vector error(mesh.nNode());       // Almacena la distribución del error
	Vector exacta(mesh.nNode());
	solver.laplace2D(mesh,"laplace2DE.txt",exacta);
	// Almacenamos la solución aproximada con CVFEM
	solver.solution(mesh,x,x1);
	solver.storage(mesh,x1,"laplace2DA.txt");
	// Calculamos la distribución del error
	error = exacta - x1;
	solver.storageE(mesh,error,"error.txt");
	//x1.print();
	// Calculamos el error
	Vector r(mesh.nNodeIncongnite());
	r = b - A*x;
	std :: cout << std :: endl << std :: endl;
	std :: cout << "#Error ||b-A*x||: " << r.norm() << " " << "rms: " << r.rms() << std :: endl;
	std :: cout << std :: endl << std :: endl;
}

void laplaceS()
{
	// Definimos un objeto para el tratamiento de la malla
	Mesh mesh("./GMSH/LaplaceS/mesh5.msh");
	// Definimos una tolerancia para el metodo
	double tol = 1e-7;
	// Definimos los radios del los semicirculos
	double ra = 0.5;
	double rb = 1.0;
	// Cargamos las condiciones de frontera 
	std::vector<int> border = mesh.bordeNode();
	for (int i = 0; i < border.size(); ++i)
	{
		double r = sqrt(pow(mesh.coordX(border[i]),2) + pow(mesh.coordY(border[i]),2));
		double theta = atan(mesh.coordY(border[i])/mesh.coordX(border[i]));  //* 180 / PI;
		// Si estamos sobre la pared y = 0
		if (mesh.coordY(border[i]) == 0)
		{
			mesh.bondaryCondition(0,0);
			//std :: cout << border[i] << std :: endl;
			//std :: cout << "2222" << std :: endl;
		}
		// Si estamos sobre la pared y = 0
		if (mesh.coordX(border[i]) == 0)
		{
			mesh.bondaryCondition(0,1/r);
			//std :: cout << border[i] << std :: endl;
		}
		// Si estamos sobre el radio mas cercano al origen
		if (fabs(ra-r) <= tol && mesh.coordY(border[i]) != 0 && mesh.coordX(border[i]) != 0)
		{
			mesh.bondaryCondition(0,sin(theta)/ra);
			//std :: cout << border[i] << std :: endl;
		}
		// Si estamos sobre el radio más legano al origen
		if (fabs(rb-r) <= tol && mesh.coordY(border[i]) != 0 && mesh.coordX(border[i]) != 0)
		{
      		//cout << i+1 << endl;
			mesh.bondaryCondition(0,sin(theta)/rb);
			//std :: cout << border[i] << std :: endl;
      		//cout << i << " " << r << " " << rb << " " << theta << " " << sin(theta)/rb <<endl;
		}
			
	}
	// Distinguimos los nodos Neumman y Dirichlet
	mesh.typeNodeBorder();
	// Mostramos el valor de las condiciones de frontera
	mesh.showBoundaryCondition();
	// Es necesario determinar los nodos incognita
	mesh.nodeIncongnite();
	mesh.showTotal();
	
	// Definimos las variables necesarias
	CSR A(mesh.nNodeIncongnite());    // Matriz CVFEM
	Vector x(mesh.nNodeIncongnite()); // Vector x
	Vector b(mesh.nNodeIncongnite()); // Vector b
	Solve solver; // Para el trataiento de las soluciones
	// Objeto para crear el sistema de ecuaciones
	CVFEM cvfem;
	cvfem.Ab(A,b,mesh);
	// Verificamos el valor de b
	//b.print();
	// Resolvemos el sistema de ecuaciones lineales
	BICGSTAB bicg; 
	bicg.solve(A,x,b);
	std :: cout << std :: endl;
  	bicg.report();
  	// Usando precondicionador
  	ILU<CSR> milu;                //Precondicionador LU incompleto modificado
	std :: cout << std :: endl << std :: endl;
  	milu.calculate(A);              //calculo del precondicionador
  	x=0.0;
  	bicg.solve(A,x,b,milu);
  	bicg.report();
  	// Usando Gradiente conjugado
  	CG cg;
  	x = 0.0;
  	cg.solve(A,x,b);
	std :: cout << std :: endl;
  	cg.report();
  	// Usando precondicionador
  	x = 0.0;
  	cg.solve(A,x,b,milu);
	std :: cout << std :: endl;
  	cg.report();
  	//x.print();
	// Calculamos la solucion exacta
	Vector x1(mesh.nNode()); 		  // Vector b para la solución final
	Vector error(mesh.nNode());       // Almacena la distribución del error
	Vector exacta(mesh.nNode());
	solver.laplace2DS(mesh,"laplace2DE.txt",exacta);
	// Almacenamos la solución aproximada con CVFEM
	solver.solution(mesh,x,x1);
	solver.storage(mesh,x1,"laplace2DA.txt");
	// Calculamos la distribución del error
	error = exacta - x1;
	solver.storageE(mesh,error,"error.txt");
	//x1.print();
	// Calculamos el error
	Vector r(mesh.nNodeIncongnite());
	r = b - A*x;
	std :: cout << std :: endl << std :: endl;
	std :: cout << "#Error ||b-A*x||: " << r.norm() << " " << "rms: " << r.rms() << std :: endl;
	std :: cout << std :: endl << std :: endl;
}

void poisson()
{
	// Definimos un objeto para el tratamiento de la malla
	Mesh mesh("./GMSH/Poisson/mesh2.msh");
	
	// Cargamos las condiciones de frontera 
	std::vector<int> border = mesh.bordeNode();
	for (int i = 0; i < border.size(); ++i)
	{
		// Nodos tipo Neumann frontera izquierda
		if (mesh.coordX(border[i]) == 0 && mesh.coordY(border[i]) != 0 && mesh.coordY(border[i]) != 1)
		{
			mesh.bondaryCondition(1,0);
		}
		// Nodos tipo Neumann frontera izquierda
		if (mesh.coordX(border[i]) == 1 && mesh.coordY(border[i]) != 0 && mesh.coordY(border[i]) != 1)
		{
			mesh.bondaryCondition(1,0);
		}
		if (mesh.coordY(border[i]) == 0)
		{
			mesh.bondaryCondition(0,cos(PI*mesh.coordX(border[i])));
		}
		if (mesh.coordY(border[i]) == 1)
		{
			mesh.bondaryCondition(0,-cos(PI*mesh.coordX(border[i])));
		}
	}
	// Distinguimos los nodos Neumman y Dirichlet
	mesh.typeNodeBorder();
	// Mostramos el valor de las condiciones de frontera
	mesh.showBoundaryCondition();
	// Es necesario determinar los nodos incognita
	mesh.nodeIncongnite();
	mesh.showTotal();
	// Definimos las variables necesarias
	CSR A(mesh.nNodeIncongnite());    // Matriz CVFEM
	Vector x(mesh.nNodeIncongnite()); // Vector x
	Vector b(mesh.nNodeIncongnite()); // Vector b
	Solve solver; // Para el trataiento de las soluciones
	// Objeto para crear el sistema de ecuaciones
	CVFEM cvfem;
	cvfem.Ab(A,b,mesh);
	// Verificamos el valor de b
	b.print();
	// Resolvemos el sistema de ecuaciones lineales
	BICGSTAB bicg; 
	bicg.solve(A,x,b);
	std :: cout << std :: endl;
  	bicg.report();
  	// Usando precondicionador
  	ILU<CSR> milu;                //Precondicionador LU incompleto modificado
	std :: cout << std :: endl << std :: endl;
  	milu.calculate(A);              //calculo del precondicionador
  	x=0.0;
  	bicg.solve(A,x,b,milu);
  	bicg.report();
  	// Usando Gradiente conjugado
  	CG cg;
  	x = 0.0;
  	cg.solve(A,x,b);
	std :: cout << std :: endl;
  	cg.report();
  	// Usando precondicionador
  	x = 0.0;
  	cg.solve(A,x,b,milu);
	std :: cout << std :: endl;
  	cg.report();
  	//x.print();
	// Calculamos la solucion exacta
	Vector x1(mesh.nNode()); 		  // Vector b para la solución final
	Vector error(mesh.nNode());       // Almacena la distribución del error
	Vector exacta(mesh.nNode());
	solver.poisson(mesh,"laplace2DE.txt",exacta);
	// Almacenamos la solución aproximada con CVFEM
	solver.solution(mesh,x,x1);
	solver.storage(mesh,x1,"laplace2DA.txt");
	// Calculamos la distribución del error
	error = exacta - x1;
	solver.storageE(mesh,error,"error.txt");
	//x1.print();
	// Calculamos el error
	Vector r(mesh.nNodeIncongnite());
	r = b - A*x;
	std :: cout << std :: endl << std :: endl;
	std :: cout << "#Error ||b-A*x||: " << r.norm() << " " << "rms: " << r.rms() << std :: endl;
	std :: cout << std :: endl << std :: endl;
}


void weird()
{
	// Definimos las zona de baja permeabilidad
	std::vector<std::pair<double,double>> baja;
	baja.push_back(std::make_pair(15,10));
	baja.push_back(std::make_pair(45,10));
	baja.push_back(std::make_pair(45,30));
	baja.push_back(std::make_pair(15,30));
	// Definimos las zona de alta permeabilidad
	std::vector<std::pair<double,double>> alta;
	alta.push_back(std::make_pair(0,50));
	alta.push_back(std::make_pair(50,50));
	alta.push_back(std::make_pair(50,80));
	alta.push_back(std::make_pair(0,80));
	// Definimos la malla a utilizar
	Mesh mesh("./GMSH/Weird/mesh.msh");
	// Cargamos las condiciones Iniciales 
	std::vector<int> border = mesh.bordeNode();
	for (int i = 0; i < border.size(); ++i)
	{
		//std :: cout << mesh.coordX(border[i]) << " " << mesh.coordY(border[i]) << std :: endl;
		// Si se esta sobre la pared x = 0
		if (mesh.coordX(border[i]) == 0 && (mesh.coordY(border[i]) != 0 && mesh.coordY(border[i]) != 100))
		{
			mesh.bondaryCondition(0,10);
		}
		// si esta sobre la pared x = 1
		if (mesh.coordX(border[i]) == 100 && (mesh.coordY(border[i]) != 0 && mesh.coordY(border[i]) != 100))
		{
			mesh.bondaryCondition(0,1);
		}
		// Si esta sobre la pared y = 0
		if (mesh.coordY(border[i]) == 0 && (mesh.coordX(border[i]) != 0 && mesh.coordX(border[i]) != 100))
		{
			mesh.bondaryCondition(0,0);
		}
		// Si esta sobre la pared y = 1
		if (mesh.coordY(border[i]) == 100 && (mesh.coordX(border[i]) != 0 && mesh.coordX(border[i]) != 100))
		{
			mesh.bondaryCondition(0,0);
			std :: cout << border[i] << std :: endl;
		}
		// Si es un nodo de una esquina
		if (mesh.coordX(border[i]) == 0 && mesh.coordY(border[i]) == 0)
		{
			mesh.bondaryCondition(0,0);
		}
		// Si es un nodo de una esquina
		if (mesh.coordX(border[i]) == 0 && mesh.coordY(border[i]) == 100)
		{
			mesh.bondaryCondition(0,0);
		}
		// Si es un nodo de una esquina
		if (mesh.coordX(border[i]) == 100 && mesh.coordY(border[i]) == 0)
		{
			mesh.bondaryCondition(0,0);
		}
		// Si es un nodo de una esquina
		if (mesh.coordX(border[i]) == 100 && mesh.coordY(border[i]) == 100)
		{
			mesh.bondaryCondition(0,0);
		}
	}
	mesh.showBoundaryCondition();
	// Distinguimos los nodos Neumman y Dirichlet
	mesh.typeNodeBorder();
	// Mostramos el valor de las condiciones de frontera
	// Es necesario determinar los nodos incognita
	mesh.nodeIncongnite();
	// Muestra los nodos incognita
	mesh.shownodeIncongnite();
	// Mostramos
	mesh.showNodeIF();
	// Mostramos la informacion total de la malla
	mesh.showTotal();
	std :: cout << mesh.nNodeIncongnite() << std :: endl;
	// Definimos las variables necesarias
	CSR A(mesh.nNodeIncongnite());    // Matriz CVFEM
	Vector x(mesh.nNodeIncongnite()); // Vector x
	Vector b(mesh.nNodeIncongnite()); // Vector b
	Solve solver; // Para el trataiento de las soluciones
	// Objeto para crear el sistema de ecuaciones
	CVFEM cvfem;
	cvfem.Ab(A,b,mesh,alta,baja);
	// Verificamos el valor de b
	b.print();
	// Resolvemos el sistema de ecuaciones lineales
	BICGSTAB bicg; 
	bicg.solve(A,x,b);
	std :: cout << std :: endl;
  	bicg.report();
  	// Usando precondicionador
  	ILU<CSR> milu;                //Precondicionador LU incompleto modificado
	std :: cout << std :: endl << std :: endl;
  	milu.calculate(A);              //calculo del precondicionador
  	x=0.0;
  	bicg.solve(A,x,b,milu);
  	bicg.report();
  	// Medimos el precondicionado 
  	//x.print();
	// Calculamos la solucion exacta
	Vector x1(mesh.nNode()); 		  // Vector b para la solución final
	// Almacenamos la solución aproximada con CVFEM
	solver.solution(mesh,x,x1);
	solver.storage(mesh,x1,"solx.txt");
	// Calculamos el error
	Vector r(mesh.nNodeIncongnite());
	r = b - A*x;
	std :: cout << std :: endl << std :: endl;
	std :: cout << "#Error ||b-A*x||: " << r.norm() << " " << "rms: " << r.rms() << std :: endl;
	std :: cout << std :: endl << std :: endl;
}
