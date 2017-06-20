#include "CSR.hpp"

// Asignamos memoria a los arreglos
void CSR :: reserveMemory()
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
	irow = new int[n+1];
	// Verificamos que el espacio fue asignado
	if (irow == nullptr)
	{
		std::cout << "ERROR: ESPACIO NO ASIGNADO: CSR: irow" << std::endl;
		exit(0);
	}
	// Solicitamos espacio para col
	col = new int[nnzMax];
	// Verificamos que el espacio fue asignado
	if (col == nullptr)
	{
		std::cout << "ERROR: ESPACIO NO ASIGNADO: CSR: col" << std::endl;
		exit(0);
	}
	// Solicitamos espacio para idiag
	idiag = new int[n];
	// Verificamos que el espacio fue asignado
	if (idiag == nullptr)
	{
		std::cout << "ERROR: ESPACIO NO ASIGNADO: CSR: idiag" << std::endl;
		exit(0);
	}
}
// Redimensionamos una matriz
void CSR :: resize(int m)
{
	// Liberamos el espacio en memoria
	freeMemory();
	// Nueva aproximacion
	n = m;
	nnzMax = m * 30;
	// Solicitamos el nuevo espacio en memoria
	reserveMemory();
	// Inicializamos con ceros
	zeros();
}
// Liberamos el espacio asignado en memoria
void CSR :: freeMemory()
{
	// Vaciamos el contenido de una matriz
	delete [] data;
	delete [] irow;
	delete [] col;
	delete [] idiag;
	// Limpiamos los valores
	n = 0;
	nnz = 0;
	nnzMax = 0;
}
// Llenamos de ceros los arreglos
void CSR :: zeros()
{
	for (int i = 0; i < nnzMax; ++i)
	{
		data[i] = 0.0;
		col[i] = 0;
	}
	// Llenamos a irow de ceros
	for (int i = 0; i < n+1; ++i)
	{
		irow[i] = 0;
	}
}
void CSR :: convert(COO const &coo)
{
	// Tomamos el nnz de COO
	nnz = coo.gNnz();
	// Contamos el numero de entradas no cero
	for (int l = 0; l < nnz; ++l)
	{
		irow[coo.gRow(l)] = irow[coo.gRow(l)] + 1;
	}
	//******************************************************
	// Se imprime el numero de elementos no cero de cada row
	/*for (int i = 0; i < n+1; ++i)
	{
		std::cout <<"* " << i <<" " << irow[i] << std::endl;
	}*/
	//******************************************************
	// Ensamble del arreglo irow
	int sum = 0;
	int tmp = 0;
	for (int i = 0; i < n; ++i)
	{
		tmp = irow[i];
		irow[i] = sum;
		sum += tmp;
	}
	irow[n] = nnz;
	//******************************************************
	// Ensamble de irow
	/*for (int i = 0; i < n+1; ++i)
	{
		std::cout <<"* " << i <<" " << irow[i] << std::endl;
	}*/
	//******************************************************
	// Ensamble de data y col de CSR
	int i = 0;
	int j = 0;
	int d = 0;
	double val = 0.0;
	for (int l = 0; l < nnz; ++l)
	{
		// Obtenemos los indices 
		i = coo.gRow(l);
		j = coo.gCol(l);
		if (i == j)
		{
			idiag[i] = l;
		}
		// Valor del datos asociado
		val = coo.gData(l);
		d = irow[i];
		data[d] = val;
		col[d] = j;
		irow[i] = irow[i] + 1;
	}
	//******************************************************
	// Revismos el contenido de data y col
	/*for (int i = 0; i < n+1; ++i)
	{
		std::cout <<"* " << i <<" " << data[i] << " " << col[i] << std::endl;
	}*/
	//******************************************************
	// Reconstruimos irow
	int last = 0;
	for (int i = 0; i < n; ++i)
	{
		tmp = irow[i];
		irow[i] = last;
		last = tmp;
	}
	//******************************************************
	// Reconstruccion de irow
	/*for (int i = 0; i < n+1; ++i)
	{
		std::cout <<"* " << i <<" " << irow[i] << std::endl;
	}*/
	//******************************************************
}
// Utilizamos la busqueda binaria para obtener un elemento
int CSR :: binarySearch(int lmin, int lmax, int j)
{
	for (int i = lmin; i < lmax; ++i)
	{
		if (j == col[i]) return i;
	}
}
// Buscamos un elemento dentro de data
double CSR :: search(int i ,int j)
{
	int lmin = irow[i];
	int lmax = irow[i+1];
	int l = binarySearch(lmin,lmax,j);
	return data[l];
}
/*
// Definimos una iteracion para el metodo de Jacobi
double CSR :: iterJacobi(int i, Vector &x)
{
	// Variables necesarias
	int j = 0;
	double diag = 0.0;
	double error = 0;
	for (int l = irow[i]; l < irow[i + 1]; ++l)
	{
		j = col[l];
		// Si no es parte de la diagonal se suman y multiplican
		if( i != j) 
		{
			x[i] = x[i] - data[l] * x[j];
		}
		else 
		{
			diag = data[l];
		}
	}
	x[i] = x[i]/diag;
	// Devolvemos el error calculuado
	return error;
}
// Definimos una iteracion para el metodo de Gauss-seidel
void CSR :: iterGaussSeidel(int i, Vector &x, Vector const &b)
{
	// Variables necesrias
	double sum = 0.0;
	double diag = 0.0;
	int j = 0;
	// Comenzamos con la iteracion
	for (int l = irow[i]; l < irow[i+1]; ++l)
	{
		j = col[l];
		// Si no es parte de la diaginal se suman y multiplican
		if (i!=j)
		{
			sum = sum + data[l]*x[j];
		}
		else
		{
			diag = data[l];
		}
	}
	// Actualizamos el valor de x[i]
	x[i] = (b[i]-sum)/diag;
}
*/
// Sobrecargamos el operador = con matrizes CSR para copiar matrices
void CSR :: operator =(CSR const &Mtx)
{
	// Comprobamos que se tengan el mismo largo
	n = Mtx.n;
	nnzMax = Mtx.nnzMax;
	nnz = Mtx.nnz;
	reserveMemory();
	// Copiamos el arreglo data y col
	for (int i = 0; i < nnzMax; ++i)
	{
		data[i] = Mtx.data[i];
		col[i] = Mtx.col[i];
	}
	// Copiamos el arreglo irow
	for (int i = 0; i < n+1; ++i)
	{
		irow[i] = Mtx.irow[i];
		if (i < n)
		{
			idiag[i] = Mtx.idiag[i];
		}
	}
}