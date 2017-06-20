#include "COO.hpp"

// Constructor copia
COO :: COO(COO &Acoo)
{
	freeMemory();
	// Aproximacion del usuario
	n = Acoo.gN();
	nnz = Acoo.gNnz();
	nnzMax = n*30;
	// Solicitamos espacio en memoria
	reserveMemory();
	// Llenamos los contenedores
	for (int i = 0; i < nnz; ++i)
	{
		data[i] = Acoo.gData(i);
		row[i] =  Acoo.gRow(i);
		col[i] = Acoo.gCol(i);
	}
}
// Se redimensiona el tamaño de una matriz
void COO :: resize(int m)
{
	// Liberamos la memoria utilizada
	freeMemory();
	// Nueva aproximacion de las componentes a ser almacenadas
	n = m;
	nnzMax = n*30;
	// Solicitamos el nuevo espacio en memoria
	reserveMemory();
	// Llenamos con ceros los valores de la matriz
	zeros();
}
// Reservamos el espacio en memoria suficiente
void COO :: reserveMemory()
{
	// Solicitamos espacio para data
	data = new double[nnzMax];
	// Verificamos que el espacio fue asignado
	if (data == nullptr)
	{
		std::cout << "ERROR: ESPACIO NO ASIGNADO: COO: data" << std::endl;
		exit(0);
	}
	// Solicitamos espacio para row
	row = new int[nnzMax];
	// Verificamos que el espacio fue asignado
	if (row == nullptr)
	{
		std::cout << "ERROR: ESPACIO NO ASIGNADO: COO: row" << std::endl;
	}
	// Solicitamos espacio para col
	col = new int[nnzMax];
	// Verificamos que el espacio fue asignado
	if (col == nullptr)
	{
		std::cout << "ERROR: ESPACIO NO ASIGNADO: COO: row" << std::endl;
	}
}
// Liberamos el espacio asignado en memoria
void COO :: freeMemory()
{
	// Vaciamos el contenido de una matriz
	delete [] data;
	delete [] row;
	delete [] col;
	// Limpiamos los valores
	n = 0;
	nnz = 0;
	nnzMax = 0;
}
// Llenamos de ceros los arreglos
void COO :: zeros()
{
	for (int i = 0; i < nnzMax; ++i)
	{
		data[i] = 0.0;
		row[i] = 0;
		col[i] = 0;
	}
}
// Insertamos los elementos en los arreglos
void COO :: insert(int i, int j, double val)
{
	// Verificamos que aun se tenga espacio para almacenar elementos
	if (nnz > nnzMax)
	{
		std::cout << "ERROR: MEMORIA ESTIMADA NO SUFICENTE" << std::endl;
		exit(0);
	}
	// Insertamos en caso de tener espacio
	data[nnz] = val;
	row[nnz] = i;
	col[nnz] = j;
	nnz++;
}
// Suma a un valor en la matriz otro valor si no esta entonces 
// lo inserta.
void COO :: insertS(int i, int j, double val)
{
	// Buscamos si el valor ya esta dentro de 
	// la matriz
	for (int l = 0; l < nnz; ++l)
	{
		if (row[l] == i && col[l] == j)
		{
			data[l] += val;
			break;
		}
	}
	// Si no esta entonces lo insertamos 
	// Verificamos que aun se tenga espacio para almacenar elementos
	if (nnz > nnzMax)
	{
		std::cout << "ERROR: MEMORIA ESTIMADA NO SUFICENTE" << std::endl;
		exit(0);
	}
	// Insertamos en caso de tener espacio
	data[nnz] = val;
	row[nnz] = i;
	col[nnz] = j;
	nnz++;
}
// Devolvemos el valor de una matriz
double COO :: value(int i, int j)
{
	double val = 0;
	// Buscamos el componente solicitado
	for (int l = 0; l < nnz; ++l)
	{
		if (row[l] == i && col[l] == j)
		{
			val = data[l];
			break;
		}
	}
	// Devolvemos un componente de la matriz
	return val;	
}
// Se imprime en forma de matriz para verficar 
void COO :: impMatrix()
{
	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			bool x = false;
			for (int l = 0; l < nnz; ++l)
			{
				//std::cout << i << "," << j << "," << row[i] << "," << col[l] << std::endl;
				if ((row[l] == i) and (col[l] == j))
				{
					std::cout << data[l] << "\t";
					x = true;
					continue;
				}
			}
			if(x == false)std::cout << 0 << "\t";
		}
		std::cout << std::endl;
	}
}