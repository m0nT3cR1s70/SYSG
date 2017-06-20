#include "Vector.hpp"

// Solicitamos espacio para los datos
void Vector :: reserveMemory()
{
	if (_size <= 0)
	{
		std::cout<<"ERROR: LARGO NO VALIDO"<<std::endl;
		exit(-1);
	}
	// Si el largo es valido entonces
	_data = new double[_size];
	// Verificamos que el espacio fue asignado
	if (_data == nullptr)
	{
		std::cout<<"ERROR: ESPACIO NO ASIGNNADO"<<std::endl;
		exit(-1);
	}
}
// Liberamos la memoria utilizada
void Vector :: freeMemory()
{
	delete [] _data;
}
// Funcion para obtener la norma de un vector
double Vector :: norm()
{
	// Calculamos la norma
	double euclidiana = 0.0;
	// Calculamos el cuadrado de los elementos
	for (int i = 0; i < _size; ++i)
	{
		euclidiana += _data[i]*_data[i];
	}
	// Enviamos el resultado
	return sqrt(euclidiana);
}
/*
	Función para calcular la norma rms de
	un vector
*/
double Vector :: rms()
{
	double rms = 0;
	for (int i = 0; i < _size; ++i)
	{
		rms += pow(_data[i],2)/_size;
	}
	return sqrt(rms);
}
/*
	Redimensiona un vector
*/
void Vector :: resize(int n)
{
	_size = n;
	reserveMemory();
	zeros();
}
// Funcion para llenar un vector de ceros
void Vector :: zeros()
{
	// Llenamos el vector con ceros
	for (int i = 0; i < _size; ++i)
	{
		_data[i] = 0.0;
	}
}
// Almacena el contenido de un vector en un archivo
void Vector :: saveData(std::string name)
{
	// Almacenar en un archivo
	std::ofstream file;
	// Nombre del archivo
	file.open(name.c_str());
	// Recorremos el vector
	for (int i = 0; i < _size; ++i)
	{
		file.width(9);
		file.fill(' ');
		file.setf( std::ios::fixed, std:: ios::floatfield );
		file.precision(12);
		file << _data[i] << std::endl;
	}
}
// Imprime en pantalla el contenido de un vector
void Vector :: print()
{
	for (int i = 0; i < _size; ++i)
	{
		std::cout<<_data[i]<<std::endl;
	}
}

// Producto interno
double operator *(Vector const &v,Vector const &v1)
{
	// Largo del vector
	int nt = v._size;
	double suma = 0.0;
	// Verificamos que ambos vectores sean del mismo largo
	if (nt != v1._size)
	{
		std::cout << "ERROR: vectores de distinto largo" << std::endl;
		exit(0);
	}
	// Realizamos el producto interno
	for (int i = 0; i < nt; ++i)
	{
		suma += v1._data[i]*v._data[i];
	}
	// Devolvemos el vector
	return suma;
}

// Sobrecargamos el operador =
// Inicializa un vector 
void Vector :: operator =(double escalar)
{
	for (int i = 0; i < _size; ++i)
	{
		_data[i] = escalar;
	}
}
//Copia un vector en otro
void Vector :: operator =(Vector const &v)
{
	int nt = v._size;
	// Revisamos que los indices sean los mismos
	if(nt != _size) 
	{
		std::cout<<"ERROR: vectores de distinto largo: =" << std::endl;
		exit(0);
	}
	for (int i = 0; i < _size; ++i)
	{
		_data[i] = v[i];
	}
} 