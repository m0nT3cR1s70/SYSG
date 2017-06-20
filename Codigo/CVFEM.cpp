#include "CVFEM.hpp"

/*
	Se implementa el constructor copia
*/
CVFEM :: CVFEM(CVFEM &cvfem)
{
	_permeabilidadP = cvfem.gPerme();
	_compresibilidadP = cvfem.gCompre();
}
/*
	Inicializa para la matriz para una misma permeabilidad
	en todos los puntos
*/
void CVFEM :: Ab(CSR &A, Vector &b, Mesh &mesh)
{
	// Redimensionamos los vectores para almacenar 
	_k11.resize(mesh.nNode());
	_k22.resize(mesh.nNode());
	// Llamamos a la permeabilidad
	_k11 = _permeabilidadP;
	_k22 = _permeabilidadP;
	// Construimos la matriz de permeabilidad
	matrixCFVEM(mesh,A);
	// Construimos el vector de salida
	//laplaceVector(mesh,b);
	poissonVector(mesh,b);
}
/*
	Para dos regiones de distinta permeabilidad

*/
void CVFEM :: Ab(CSR &A, Vector &b, Mesh &mesh, std::vector<std::pair<double,double>> &alta, std::vector<std::pair<double,double>> &baja)
{
	// Cambiamos la viscosidad
	_compresibilidadP = 0.5;
	// Llamamos a la permeabilidad
	permeabilidad(mesh,alta,baja);
	// Construimos la matriz de permeabilidad
	matrixCFVEM(mesh,A);
	// Construimos el vector de salida
	laplaceVector(mesh,b);
}
/*
	Construye la matriz CVFEM
*/
void CVFEM :: matrixCFVEM(Mesh &mesh, CSR &A)
{
	// Definimos las variables Necesarias
	double ai,bi,ci = 0;
	double aj,bj,cj = 0;
	double ak,bk,ck = 0;
	// Coeficientes de transmibilidad
	double Tij, Tik;
	// Informacion de la malla
	std::vector<int> neigh;   // Nodos vecinos a otro
	std::vector<int> elemnt; // Elemento de soporte
	std::vector<int> incognite = mesh.Incognite();
	std::vector<int> border = mesh.bordeNode();
	std::vector<int> dirichelet = mesh.dirichletNode();
	//std :: cout << incognite.size() << std :: endl;
	// Creamos matrices temporales para el almacenamiento
	COO Acoo(incognite.size());
	COO coef(mesh.nNode());
	// Redimensionamos el tamaño de la matriz
	_coefM.resize(mesh.nNode());
	// Creamos un vector temporal para almacenar todos los elementos de la lista
	// Definimos una relacion entre los nodos interiores 
	for (int i = 0; i < incognite.size(); ++i)
	{
		// Para recorrer las i y las j de un triangulo
		int tmp = 0;
		// Para cada nodo leemos a sus vecinos y sus elementos de soporte
		neigh = mesh.nodeNeighborh(incognite[i]);
		elemnt = mesh.elementSupport(incognite[i]);
		// Creamos un renglon para almacenar los elementos
		std::vector<double> renglon(mesh.nNode(),0.0);
		//std :: cout << incognite[i] << std :: endl;
		//std::vector<double> coefTemp = ;
		for (int j = 0; j < elemnt.size(); ++j)
		{
			//std :: cout << elemnt.size() << " ";
			// Calculamos los coeficientes de las funciones de forma
			// Nodo i
			ai = mesh.coordY(neigh[tmp]) - mesh.coordY(neigh[tmp+1]);
			bi = -(mesh.coordX(neigh[tmp]) - mesh.coordX(neigh[tmp+1]));
			ci = mesh.coordX(neigh[tmp])*mesh.coordY(neigh[tmp+1]) - mesh.coordY(neigh[tmp])*mesh.coordX(neigh[tmp+1]);
			// Nodo j
			aj = mesh.coordY(neigh[tmp+1]) - mesh.coordY(incognite[i]);
			bj = -(mesh.coordX(neigh[tmp+1]) - mesh.coordX(incognite[i]));
			cj = mesh.coordX(neigh[tmp+1])*mesh.coordY(incognite[i]) - mesh.coordY(neigh[tmp+1])*mesh.coordX(incognite[i]);
			// Nodo k
			ak = mesh.coordY(incognite[i]) - mesh.coordY(neigh[tmp]);
			bk = -(mesh.coordX(incognite[i]) - mesh.coordX(neigh[tmp]));
			ck = mesh.coordX(incognite[i])*mesh.coordY(neigh[tmp]) - mesh.coordY(incognite[i])*mesh.coordX(neigh[tmp]);
			// Calculamos los coeficientes de transmisibilidad
			double a11 = permehar(K11(incognite[i]),K11(neigh[tmp]));
			double a22 = permehar(K22(incognite[i]),K22(neigh[tmp]));
			Tij = (-0.25)*(1/_compresibilidadP)*(1/mesh.K(elemnt[j]))*(a11*aj*ai+a22*bj*bi);
			double b11 = permehar(K11(incognite[i]),K11(neigh[tmp+1]));
			double b22 = permehar(K22(incognite[i]),K22(neigh[tmp+1]));
			Tik = (-0.25)*(1/_compresibilidadP)*(1/mesh.K(elemnt[j]))*(b11*ak*ai+b22*bk*bi);
			//std :: cout << Tij << " " << Tik << std :: endl;
			//******************************************************************
			// Comprobacion de las propiedades de forma
			//std :: cout << "PARA EL NODO i" << std :: endl;
			//std :: cout << baseFunction(ai,bi,ci,mesh.coordX(incognite[i]),mesh.coordY(incognite[i]),mesh.K(elemnt[j])) << std :: endl;
			//std :: cout << baseFunction(ai,bi,ci,mesh.coordX(neigh[tmp]),mesh.coordY(neigh[tmp]),mesh.K(elemnt[j])) << std :: endl;
			//std :: cout << baseFunction(ai,bi,ci,mesh.coordX(neigh[tmp+1]),mesh.coordY(neigh[tmp+1]),mesh.K(elemnt[j])) << std :: endl;
			//std :: cout << "PARA EL NODO j"  << std :: endl;
			//std :: cout << baseFunction(aj,bj,cj,mesh.coordX(incognite[i]),mesh.coordY(incognite[i]),mesh.K(elemnt[j])) << std :: endl;
			//std :: cout << baseFunction(aj,bj,cj,mesh.coordX(neigh[tmp]),mesh.coordY(neigh[tmp]),mesh.K(elemnt[j])) << std :: endl;
			//std :: cout << baseFunction(aj,bj,cj,mesh.coordX(neigh[tmp+1]),mesh.coordY(neigh[tmp+1]),mesh.K(elemnt[j])) << std :: endl;
			//std :: cout << "PARA EL NODO k" << std :: endl;
			//std :: cout << baseFunction(ak,bk,ck,mesh.coordX(incognite[i]),mesh.coordY(incognite[i]),mesh.K(elemnt[j])) << std :: endl;
			//std :: cout << baseFunction(ak,bk,ck,mesh.coordX(neigh[tmp]),mesh.coordY(neigh[tmp]),mesh.K(elemnt[j])) << std :: endl;
			//std :: cout << baseFunction(ak,bk,ck,mesh.coordX(neigh[tmp+1]),mesh.coordY(neigh[tmp+1]),mesh.K(elemnt[j])) << std :: endl;
			//double sumaa = ai + aj + ak;
			//double sumab = bi + bj + bk;
			//double sumac = (0.5)*(1/mesh.K(elemnt[j]))*(ci + cj + ck);
			//std :: cout << sumaa  << "\t\t" << sumab << "\t\t" << sumac << "\t\t" << sumaa+sumab+sumac << std :: endl;
			//std :: cout << "Tij: " << Tij << "Tik: " << Tik << std :: endl;
			//******************************************************************
			// Agregamos los nodos al renglon
			renglon[neigh[tmp]-1] += Tij;
			renglon[neigh[tmp+1]-1] += Tik; 
			renglon[incognite[i] - 1] += Tij + Tik;
			//std :: cout << neigh[tmp] << " " << neigh[tmp+1] << std :: endl;
			// Movemos el indice para los triangulos
			tmp += 2;
		}
		int rec = 0; // Indice j para el almacenamiento de la matriz
		// Almacenamos en  la matriz de coeficientes
		for (int j = 0; j < mesh.nNode(); ++j)
		{
			// LLenamos los coeficientes de la matriz 
			// para ser usados más adelante
			if (fabs(renglon[j]) > 1e-8)
			{
				coef.insert(i,j,renglon[j]);
			}
			// Llenamos la matriz A para resolver el
			// problema
			if (!std::binary_search(dirichelet.begin(),dirichelet.end(),j+1))
			{	
				if (fabs(renglon[j]) > 1e-8)
				{
					double dato = (i == rec) ?renglon[j]:-renglon[j];
					Acoo.insert(i,rec,dato);
					//std::cout << "(" << i << "," <<rec << ") : " << renglon[j] << std::endl;
				}
				rec++;
			}
			//std :: cout << renglon[j] << " ";
		}
		//std :: cout << std :: endl;
		//std :: cout << std :: endl;
	}
	//coef.impMatrix();
	//std :: cout << std :: endl << std :: endl;
	//Acoo.impMatrix();
	//std :: cout << std :: endl << std :: endl;
	// Transformamos la matriz con los coeficientes 
	_coefM.convert(coef);
	A.convert(Acoo);
	/*
	std :: cout << std :: endl << std :: endl << std :: endl;
	std :: cout << dirichelet.size() << std :: endl;
	for (int i = 0; i < dirichelet.size(); ++i)
	{
		std :: cout << dirichelet[i] << std :: endl;
	}
	*/
}
/*
	Vector para la ecucion de laplace
*/
void CVFEM :: laplaceVector(Mesh &mesh, Vector &b)
{
	// Inicializamos con ceros
	b = 0.0;
	// Nodos que estan sobre el contorno del dominio
	std::vector<int> border = mesh.bordeNode();
	// Nodos ingognita
	std::vector<int> incognite = mesh.Incognite();
	// Nodos vecinos
	std::vector<int> neigh;
	// Recorremos todos los nodos para los cuales 
	// se requiere un elemento en el vector
	for (int i = 0; i < mesh.nIncognite(); ++i)
	{
		// Obtenemos los nodos vecinos de nodo incognita
		neigh = mesh.nodeNeighborh(incognite[i]);
		// Ordenamos los nodos
		std::sort(neigh.begin(),neigh.end());
		// Recorremos los nodos vecinos
		int rec = 0;
		// Ordenamos los nodos
		for (int j = 0; j < neigh.size()/2; ++j)
		{
			if (std::binary_search(border.begin(), border.end(),neigh[rec]))
			{
				b[i] += _coefM.search(i,neigh[rec]-1)*mesh.value(neigh[rec]);
				//std :: cout << neigh[rec] << " ";
			}
			// Aumentamos 2 para el siguiente nodo
			rec +=2;
		}
		//std :: cout << std :: endl;
	}
}
/*
	Vector para la ecuacion de poisson
*/
void CVFEM ::  poissonVector(Mesh &mesh, Vector &b)
{
	// Llamamos a los vectores incognita
	std::vector<int> incognite = mesh.Incognite();
	b = 0.0; // Limpiamos el vector
	for (int i = 0; i < incognite.size(); ++i)
	{
		std::vector<int> element = mesh.elementSupport(incognite[i]);
		for (int j = 0; j < element.size(); ++j)
		{
			b[i] = b[i] + (1/3.0*mesh.K(element[j]))*2*pow(PI,2)*cos(PI*mesh.coordX(incognite[i]))*cos(PI*mesh.coordY(incognite[i]));
			//std :: cout << 1/3.0 << " " << j << std :: endl;
		}
	}
	// Aplicamos las condiciones de frontera
	std::vector<int> dirichelet = mesh.dirichletNode();
	for (int i = 0; i < incognite.size(); ++i)
	{
		// Obtenemos los nodos vecinos de cada nodo incognita
		std::vector<int> neigh = mesh.nodeNeighborh(incognite[i]);
		// Ordenamos los nodos
		std :: sort(neigh.begin(),neigh.end());
		// Recorremos los nodos vecinos
		int rec = 0;
		// Ordenamos los nodos
		for (int j = 0; j < neigh.size()/2; ++j)
		{
			if (std::binary_search(dirichelet.begin(), dirichelet.end(),neigh[rec]))
			{
				b[i] += _coefM.search(i,neigh[rec]-1)*mesh.value(neigh[rec]);
				//std :: cout << neigh[rec] << " ";
			}
			// Aumentamos 2 para el siguiente nodo
			rec +=2;
		}
	}
}
/*
	Calcula la permeabilidad de una zona
*/
void CVFEM :: permeabilidad(Mesh &mesh, std::vector<std::pair<double,double>> &alta, std::vector<std::pair<double,double>> &baja)
{
	// Redimensionamos el tamaño de los vectores
	_k11.resize(mesh.nNode());
	_k22.resize(mesh.nNode());
	// Definimos los nodos que tienen un alta permeabilidad
	std::vector<int> Alta = mesh.piP(alta);
	std::vector<int> Baja = mesh.piP(baja);

	double lower_bound = 0;
   	double upper_bound = 1;
   	std::uniform_real_distribution<double> unif(lower_bound,upper_bound);
   	std::default_random_engine re;

	// Permeabilidad homogenea
	_k11 = _permeabilidadP;
	_k22 = _permeabilidadP;

	// Definimos para cada nodo un valor de permeabilidad
	for (int i = 0; i < mesh.nNode(); ++i)
	{
		double k11 = unif(re);
		double k22 = unif(re);

		// Permeabilodad Variable
		_k11[i] = _permeabilidadP * ((double)rand()/ RAND_MAX);
		_k22[i] = _permeabilidadP * ((double)rand()/ RAND_MAX);
		//_k11[i] = _permeabilidadP * (k11);
		//_k22[i] = _permeabilidadP * (k22);
		// Multiplocamos la zona de altar permeabilodad
		if (std::binary_search(Alta.begin(),Alta.end(),i+1))
		{
			_k11[i] *= 100;
			_k22[i] *= 100;
		}
		// Multiplicamos la zona de permeabilidas por una baja
		if (std :: binary_search(Baja.begin(),Baja.end(),i+1))
		{
			_k11[i] *= 0.001;
			_k22[i] *= 0.001;
		}
		//std :: cout << _k11[i] << " " << _k22[i] << std :: endl;
	}
}
/*
	Funcion para combrobar la funcion base
*/
