#include "Mesh.hpp"

// Funcion para comparar los elmentos de un pair
// Para ordenar con respecto a la variable y
bool comparatorY(const std::pair<double,double> &A,const std::pair<double,double> &B)
{

	return A.second < B.second;
}
// Para preguntar si son iguales
bool isEqual(const std::pair<double,double> &A,const std::pair<double,double> &B)
{
	// Definimos una tolerancia
	double tol = 1e-10;
	// Comparamos las primeras coordenadas
	double resta = A.first - B.first;
	// Comparamos las segundas coordenadas
	double resta1 = A.second - B.second;
	// Verificamos que sean iguales
	return resta < tol && resta1 && tol;
}
// Inicializa la geometria para el semicirculo
void Mesh :: initialize(std::string filename)
{
	// Variables utiles
	std::vector<std::vector<int>> elmen;
	std::vector<std::vector<double>> coord;
	std::vector<int> relation;
	// Leemos un archivo *.msh
	readFileGMSHF(filename,coord,elmen);
	std::vector<std::vector<double>> coor = coord;
	showGmsh(coord,elmen);
	// Reordenamos los nodos
	reorderNode(coord);
	showNode();
	// Obtenermos la relacion entre los nodos
	relationNodes(coor,relation);
	showRelation(relation);
	// Reordenamos los nodos
	reorderElement(elmen,relation);
	showElement();
	std :: stable_sort(_nodeFrontera.begin(),_nodeFrontera.end());

	// Eliminamos los repetidos
	for (int i = 0; i < _nodeFrontera.size(); ++i)
	{
		std::vector<int>::iterator it;
  		it = std::unique (_nodeFrontera.begin(), _nodeFrontera.end()); 
  		_nodeFrontera.resize( std::distance(_nodeFrontera.begin(),it));
	}
	// Eliminamos los elementos que no existan
	_elemntsOrder.erase(_elemntsOrder.begin(),_elemntsOrder.begin()+_nodeFrontera.size());
	_nelements = _elemntsOrder.size();
	// mostramos el contenido de los nodos frontera
	std :: cout << _nodeFrontera.size() << std :: endl;
	// Calculamos los nodos frontera
	borderInteriorNode(relation);
	showNodeIF();
	// Calculamos los nodos soporte de cada nodo
	elementSupportNode();
	//showElementSupportNode();
	// Se calcula el area de cada elemento triangular
	areaElement();
	showArea();
}
// Inicializa toda la geometria dada
void Mesh :: initialize(std::string filename, std::vector<double> &vertices)
{
	// Variables utiles
	std::vector<std::vector<int>> elmen;
	std::vector<std::vector<double>> coord;
	std::vector<int> relation;
	// Leemos un archivo *.msh
	readFileGMSH(filename,coord,elmen);
	std::vector<std::vector<double>> coor = coord;
	showGmsh(coord,elmen);
	// Reordenamos los nodos
	reorderNode(coord);
	showNode();
	// Obtenermos la relacion entre los nodos
	relationNodes(coor,relation);
	showRelation(relation);
	// Reordenamos los nodos
	reorderElement(elmen,relation);
	showElement();
	// Calculamos los nodos frontera y los nodos interiores
	borderInteriorNode(vertices);
	showNodeIF();
	// Calculamos los nodos soporte de cada nodo
	elementSupportNode();
	showElementSupportNode();
	// Se calcula el area de cada elemento triangular
	areaElement();
	showArea();
}
/*
// Lee un archivo *.msh y almacena las coordenadas x y y de cada nodo
// y de cada elemento almacena que nodos lo componen segun la numeracion
// de gmsh
*/
void Mesh :: readFileGMSH(std::string filename, std::vector<std::vector<double>> &coord, std::vector<std::vector<int>> &elemn)
{
	// Variables Necesarias
	std :: string line;  // Almacena el contenido de una linea actual
	std :: string lineB; // Almacena el contenido de una linea anterior
	std :: string tmp;   // Almacena el valor buscado

	int count = 0;
	int contT = 0;
	int contT1 = 0;

	std::ifstream file(filename); // Abrimos el archivo para la lectura
	// Comprobamos que el archivo fue abierto
	if (file.is_open())
	{
		// Comenzamos la lectura del archivo
		do
		{
			// Leemos linea a linea el archivo
			getline(file,line);
			// Si la frase contiene la palabra &Nodes entonces comienza 
			// la informacion de los nodos y comenzamos a capturarla
			if (line == "$Nodes" || lineB == "$Nodes")
			{
				// Si la linea actual contiene la palabra buscada
				// entonces la siguiente linea tiene el numero de
				// nodos totales
				if (line == "$Nodes")
				{
					// Tomamos el numero de nodos que contiene la malla
					getline(file,line);
					_nnodes = std :: stoi(line);
					// Solicitamos espacio para almacenar los nodos con sus coordenadas
					coord.resize(_nnodes);
					for (int i = 0; i < _nnodes; ++i)
					{
						coord[i].resize(2);
					}
					// Leemos la siguiente linea del archivo
					getline(file,line);
				}
				// Asignamos una variable temporal que impide que salgamos 
				// hasta reccorrer todoa a informacion de los nodos
				lineB = "$Nodes";
				// Cortamos la linea para obtener las coordenadas x y y
				std::istringstream isstream(line);
				count=0;
				// 
				// Dividimos la frase para extraer las coordenadas
				while(!isstream.eof())
				{
					// Almacenamos las coordenadas
					isstream >> tmp;
					if (count != 0 && count != 3)
					{
						coord[contT][count-1] = std::stod(tmp);
					}
					count++;
					//cout << tmp << endl;
				}
				// Si llegamos a EndNodes entonces termina
				if (line == "$EndNodes")
				{
					lineB = "$EndNodes";
				}
				contT++;
			}
			// Leemos cada elemento triangular de la malla
			if (line == "$Elements" || lineB == "$Elements")
			{
				// Si comienza con $Elements la siguiente linea contiene
				// la informacion acerca del numero de elementos totales
				if (line == "$Elements")
				{
					// Leemos el numero de elementos totales
					getline(file,line);
					_nelements = std :: stoi(line);
					// Reservamos espacio para almacenar cada elemento
					// con los nodos que lo conforman
					elemn.resize(_nelements);
					for (int i = 0; i < _nelements; ++i)
					{
						elemn[i].resize(3);
					}
					// Leemos la siguiente linea 
					getline(file,line);
				}
				// Asignamos una variable temporal que impida que salgamos hasta 
				// recorrer todos los elementos con sus nodos
				lineB = "$Elements";
				std :: istringstream isstream(line);
				count=0;
				// Dividimos la frase para extraer las coordenadas
				while(!isstream.eof())
				{
					// Almacenamos las coordenadas
					isstream >> tmp;
					if (count == 1 && tmp != "2")
					{
						//cout << count << endl;
						//cout << tmp << endl;
						break;
					}
					//cout << "*************************" << endl;
					if (count > 4)
					{
						//cout << tmp << ",";
						//cout << contT1 << " : "<< count - 5 << ",";
						//cout << contT1 << " , " << count-5 << ",";
						elemn[contT1-1][count-5] = std::stoi(tmp);
						//cout << _elem[contT][count-1] << endl;
					}
					if (count == 1)
					{
						contT1++;
					}
					count++;
					//cout << tmp << endl;
				}
				_nelements = contT1;
				//cout << endl;
				if (line == "$EndElements")
				{
					lineB = "$EndElements";
				}
			}

		}while(!file.eof());
	}
	else
	{
		std :: cout << "ERROR: FILE NOT FOUND" << std :: endl;
		exit(-1);
	}
}
/*
// Lee un archivo *.msh y almacena las coordenadas x y y de cada nodo
// y de cada elemento almacena que nodos lo componen segun la numeracion
// de gmsh
*/
void Mesh :: readFileGMSHF(std::string filename, std::vector<std::vector<double>> &coord, std::vector<std::vector<int>> &elemn)
{
	// Variables Necesarias
	std :: string line;  // Almacena el contenido de una linea actual
	std :: string lineB; // Almacena el contenido de una linea anterior
	std :: string tmp;   // Almacena el valor buscado

	int count = 0;
	int contT = 0;
	int contT1 = 0;

	std::ifstream file(filename); // Abrimos el archivo para la lectura
	// Comprobamos que el archivo fue abierto
	if (file.is_open())
	{
		// Comenzamos la lectura del archivo
		do
		{
			// Leemos linea a linea el archivo
			getline(file,line);
			// Si la frase contiene la palabra &Nodes entonces comienza 
			// la informacion de los nodos y comenzamos a capturarla
			if (line == "$Nodes" || lineB == "$Nodes")
			{
				// Si la linea actual contiene la palabra buscada
				// entonces la siguiente linea tiene el numero de
				// nodos totales
				if (line == "$Nodes")
				{
					// Tomamos el numero de nodos que contiene la malla
					getline(file,line);
					_nnodes = std :: stoi(line);
					// Solicitamos espacio para almacenar los nodos con sus coordenadas
					coord.resize(_nnodes);
					for (int i = 0; i < _nnodes; ++i)
					{
						coord[i].resize(2);
					}
					// Leemos la siguiente linea del archivo
					getline(file,line);
				}
				// Asignamos una variable temporal que impide que salgamos 
				// hasta reccorrer todoa a informacion de los nodos
				lineB = "$Nodes";
				// Cortamos la linea para obtener las coordenadas x y y
				std::istringstream isstream(line);
				count=0;
				// 
				// Dividimos la frase para extraer las coordenadas
				while(!isstream.eof())
				{
					// Almacenamos las coordenadas
					isstream >> tmp;
					if (count != 0 && count != 3)
					{
						coord[contT][count-1] = std::stod(tmp);
					}
					count++;
					//cout << tmp << endl;
				}
				// Si llegamos a EndNodes entonces termina
				if (line == "$EndNodes")
				{
					lineB = "$EndNodes";
				}
				contT++;
			}
			// Leemos cada elemento de la malla
			if (line == "$Elements" || lineB == "$Elements")
			{
				// Si comienza con $Elements la siguiente linea contiene
				// la informacion acerca del numero de elementos totales
				if (line == "$Elements")
				{
					// Leemos el numero de elementos totales
					getline(file,line);
					_nelements = std :: stoi(line);
					// Reservamos espacio para almacenar cada elemento
					// con los nodos que lo conforman
					elemn.resize(_nelements);
					for (int i = 0; i < _nelements; ++i)
					{
						elemn[i].resize(3);
					}
					// Leemos la siguiente linea 
					getline(file,line);
				}
				// Asignamos una variable temporal que impida que salgamos hasta 
				// recorrer todos los elementos con sus nodos
				lineB = "$Elements";
				std :: istringstream isstream(line);
				count=0;
				// Almacenamos de manera temporal el tipo de elemento
				std :: string tipo;
				// Dividimos la frase para extraer las coordenadas
				while(!isstream.eof())
				{
					// Almacenamos las coordenadas
					isstream >> tmp;
					if (count == 1)
					{
						tipo = tmp;	
					}
					// Almacenamos los elementos para el tipo 1
					if (count > 4 && tipo == "1")
					{
						_nodeFrontera.push_back(std::stoi(tmp));
					}
					// Almacenamos los elementos para el tipo 2
					//cout << "*************************" << endl;
					if (count > 4 && tipo == "2")
					{
						//cout << tmp << ",";
						//cout << contT1 << " : "<< count - 5 << ",";
						//cout << contT1 << " , " << count-5 << ",";
						elemn[contT1-1][count-5] = std::stoi(tmp);
						//cout << _elem[contT][count-1] << endl;
					}
					if (count == 1)
					{
						contT1++;
					}
					count++;
					//cout << tmp << endl;
				}
				_nelements = contT1;
				//cout << endl;
				if (line == "$EndElements")
				{
					lineB = "$EndElements";
				}
			}

		}while(!file.eof());
	}
	else
	{
		std :: cout << "ERROR: FILE NOT FOUND" << std :: endl;
		exit(-1);
	}
}
void Mesh :: showGmsh(std::vector<std::vector<double>> &coord, std::vector<std::vector<int>> &elemn)
{
	// Verificamos que se tenga algo que mostrar
	if (coord.size() == 0)
	{
		std::cout << "ERROR: coord empty" << std::endl;
		exit(-1);
	}
	std::cout << "TABLA NODOS-COORDENADAS" << std::endl << std::endl;
	for (int i = 0; i < _nnodes; ++i)
	{
		std::cout << i+1 << " :\t\t" << coord[i][0] << " ,\t\t" << coord[i][1] << std::endl;
	}
	//*************************
	// Verificamos que se tenga algo que mostrar 
	if (elemn.size() == 0)
	{
		std::cout << "ERROR: elemn empty" << std::endl;
		exit(-1);
	}
	// Mpstramos en pantalla los elmentos
	std::cout<<"TABLA ELEMENTOS-NODOS SOPORTE" << std::endl << std::endl;
	for (int i = 0; i < _nelements; ++i)
	{
		std::cout << i+1 << ":\t" << elemn[i][0] << ",\t" << elemn[i][1] << ",\t" << elemn[i][2] << std::endl;
	}
}

/*
	Reordenamos los nodos
*/
void Mesh :: reorderNode(std::vector<std::vector<double>> &coord)
{
	// Variables utiles para reordenar
	int tmp = 0;
	int tmp2 = 0;
	int count;
	int suma;
	std::vector<std::pair<double,double>> pairTmp; // Almacenamos de forma temporar los nodos desordenados
	// Ordenamos los nodos desordenados con respecto a Y
	stable_sort(coord.begin(),coord.end());
	for (int i = 0; i < coord.size(); ++i)
	{
		pairTmp.push_back(std::make_pair(coord[i][0],coord[i][1])); 
	}
	// Ordenamos los nodos con respecto a la variable y
	stable_sort(pairTmp.begin(),pairTmp.end(),&comparatorY);
	for (int i = 0; i < _nnodes; ++i)
	{
		std::vector<std::pair <double, double>> tmp1;
		// Tomamos un elemnto de y para comenzar a contar
		double num = pairTmp[tmp].second;
		// Comenzamos a contar
		count = 0;
		// Recorremos los demas elementos
		for (int j = tmp; j < _nnodes; ++j)
		{
			if (fabs(pairTmp[tmp].second - num) < 1e-8)
			{
				// Avanzamos a la siguiente coordenada
				tmp++;
				// contamos un elemento
				count++;
			}
			else
			{
				continue;
			}
		}
		// Almacenamos temporalmente los nodos que tienen misma y
		for (int k = tmp2; k < tmp2+count; ++k)
		{
			tmp1.push_back(std::make_pair(pairTmp[k].first, pairTmp[k].second));
		}
		// Ordenamos con respecto a x
		stable_sort(tmp1.begin(),tmp1.end());
		// Agregamos al arreglo final
		for (int l = 0; l < count; ++l)
		{
			_coordOrder.push_back(std::make_pair(tmp1[l].first,tmp1[l].second));
		}
		tmp2 = tmp;
	}
}
/*
	Definimos una relación entre la numeración nueva y la numeración anterior
*/
void Mesh :: relationNodes(std::vector<std::vector<double>> &coord, std::vector<int> &relation)
{
	// Definimos un iterador para recorrer los nodos
	std::vector<std::pair<double,double>> :: iterator it;
	// Recorremos todos los nodos
	for (int i = 0; i < _nnodes; ++i)
	{
		// Leemos nodo a nodo de la lista desordenada
		auto finder = std :: make_pair(coord[i][0],coord[i][1]);
		//std :: cout << coord[i][0] << " " << coord[i][1] << std :: endl;
		// Buscamos si este se encuentra dentro de las coordenadas ordenadas
		it = std::find(_coordOrder.begin(),_coordOrder.end(),finder);
		// Si si se encuentra entonces
		if (it != _coordOrder.end())
		{
			relation.push_back((it-_coordOrder.begin())+1);
		}
	}
}
/*
	Se reordenan los nodos según la nueva numeración de los nodos
	que conforman la malla
*/
void Mesh :: reorderElement(std::vector<std::vector<int>> &elemn,std::vector<int> &relation)
{
	// Solicitamos espacio para almacenar los elementos
	_elemntsOrder.resize(_nelements);
	for (int i = 0; i < _nelements; ++i)
	{
		_elemntsOrder[i].resize(3);
	}

	// Recorremos cada elemento
	for (int i = 0; i < _nelements; ++i)
	{
		// Recorremos los nodos que forman cada elemento triangular
		for (int j = 0; j < 3; ++j)
		{
			// Tomamos cada nodo que conforma el elemento
			int node = elemn[i][j];
			auto it = std::find(relation.begin(),relation.end(),node);
			// Si el numero fue encontrado entonces se busca la posicion
			// en donde se encontraba en relation y me dira a donde se fue
			if (it != relation.end())
			{
				//std :: cout << "AQUI MURIO " << relation[node-1] << " " << relation.size() << _elemntsOrder[0][0] <<std :: endl;
				//std :: cout << node-1 << " ";
				_elemntsOrder[i][j] = relation[node-1];
			}
		}
		std :: cout << std :: endl;
	}
}
/*
	Almacena en memoria el valor de una condicion y su tipo
*/
void Mesh :: bondaryCondition(int type, double value)
{
	// Revisamos que el tipo de nodo sea valido
	if (type < 0 || type > 1)
	{
		std :: cout << "ERROR: NODE TYPE NOT VALIDATE" << std :: endl;
		exit(-1);
	}
	_bondaryType.push_back(type);
	_bondaryCondition.push_back(value);
}
/*
	Se determina cuales nodos son frontera y cuales son interiores para dominios rectangulares
*/
void Mesh :: borderInteriorNode(std::vector<double> &vertices)
{
	if (vertices.size() == 0)
	{
		std :: cout << "ERROR: vertices empty" << std :: endl;
		exit(-1);
	}
	// Recorremos todos los nodos
	for (int i = 0; i < _nnodes; ++i)
	{
		// Comparamos la coordenada de cada nodo con los vertices del dominio rectangular
		if (_coordOrder[i].first == vertices[0] || _coordOrder[i].second == vertices[1] || _coordOrder[i].first == vertices[2] || _coordOrder[i].second == vertices[3])
		{
			_nodeBoundary.push_back(i+1);
		}
		else
		{
			_nodeInterior.push_back(i+1);
		}
	}
}
/*
	Calculamos los nodos interiores
*/
void Mesh :: borderInteriorNode(std::vector<int> &relation)
{
	if (_nodeFrontera.size() == 0)
	{
		std :: cout << "ERROR: _nodeFrontera empty" << std :: endl;
		exit(-1);
	}
	// Recorremos todos los nodos
	for (int i = 0; i < _nnodes+1; ++i)
	{
		// Buscamos el nodo en los nodos frontera
		if (!std::binary_search(_nodeFrontera.begin(),_nodeFrontera.end(),i))
		{
			auto it = std::find(relation.begin(),relation.end(),i);
			if (it != relation.end())
			{
				_nodeInterior.push_back(relation[i-1]);
			}
		}
		else
		{
			auto it = std::find(relation.begin(),relation.end(),i);
			if (it != relation.end())
			{
				_nodeBoundary.push_back(relation[i-1]);
			}
		}
	}
	std::stable_sort(_nodeInterior.begin(),_nodeInterior.end());
	std::stable_sort(_nodeBoundary.begin(),_nodeBoundary.end());
}
/*
	Distingue los nodos frontera en nodos tipo Dirichlet y nodos tipo Neumman
*/
void Mesh :: typeNodeBorder()
{
	// Recorremos todos los nodos frontera
	for (int i = 0; i < _nodeBoundary.size(); ++i)
	{
		// Nodos tipo Dirichlet
		if (_bondaryType[i] == 0)
		{
			_nodeDirichlet.push_back(_nodeBoundary[i]);
		}
		else
		{
			_nodeNeumman.push_back(_nodeBoundary[i]);
		}
	}
}
/*
	Calculamos los nodos soporte de cada elelemnto
*/
void Mesh :: elementSupportNode()
{
	// Variables termporales
	std::vector<int> nodeFinder;
	std::vector<int> indices;
	// Recorremos todos los nodos
	for (int i = 0; i < _nnodes; ++i)
	{
		// Contamos cuantos elementos contienen al nodo
		int tmp = 0;
		// Recorremos todos los elementos buscando el nodo
		for (int j = 0; j < _nelements; ++j)
		{
			// Buscamos el nodo en cada vertice que compone al elemento triangular
			for (int k = 0; k < 3; ++k)
			{
				if (i+1 == _elemntsOrder[j][k])
				{
					nodeFinder.push_back(j+1);
					tmp++;
				}
			}
		}
		// Almacenamos para cada nodo cuantos elementos soporte tiene
		indices.push_back(tmp);
	}
	// Solicitamos espacio para almacenar los nodos soporte
	_supportNodeElemnt.resize(_nnodes);
	for (int i = 0; i < _nnodes; ++i)
	{
		_supportNodeElemnt[i].resize(indices[i]);
	}
	// Almacenamos los elementos soporte de cada nodo
	int tmp = 0;
	for (int i = 0; i < _nnodes; ++i)
	{
		int count = 0;
		for (int j = tmp; j < tmp+indices[i]; ++j)
		{
			_supportNodeElemnt[i][count] = nodeFinder[j];
			count++;
		}
		tmp += indices[i];
	}
}
/*
	Calcula el area de cada elemento triángular usando las coordenadas
	de los vertices que lo conforman
*/
void Mesh :: areaElement()
{
	// Solicitamos espacio en memoria para el area
	_area.resize(_nelements);
	// Vector temporal para obtener sus coordenadas
	std::vector<double> tmp;
	// Calculamos para cada elemento
	for (int i = 0; i < _nelements; ++i)
	{
		// Limipiamos el vector
		tmp.clear();
		// Recorremos para cada nodo
		for (int j = 0; j < 3; ++j)
		{
			// Almacenamos la lista de nodos
			tmp.push_back(_coordOrder[_elemntsOrder[i][j]-1].first);
			tmp.push_back(_coordOrder[_elemntsOrder[i][j]-1].second);
			//std :: cout << _elemntsOrder[i][j]-1 << " ";
		}
		_area[i] = 0.5*(tmp[2]*tmp[5] - tmp[4]*tmp[3] - tmp[0]*(tmp[5]-tmp[3]) + tmp[1]*(tmp[4] - tmp[2]));
		//std :: cout << std :: endl;
	}
}
/*
	Según su tipo se determina que nodos son nodos 
	incognita y cuales son nodos conocidos
*/
void Mesh :: nodeIncongnite()
{
	// Si no hay ningun nodo Neuman todos los nodos frontera
	//son conocidos
	if (_nodeNeumman.size() == 0)
	{
		_nodeIncogniteP = _nodeInterior;
	}
	else
	{
		// Agregamos los nodos interiores
		for (int i = 0; i < _nodeInterior.size(); ++i)
		{
			_nodeIncogniteP.push_back(_nodeInterior[i]);
		}
		// Agregamos los nodos frontera
		for (int i = 0; i < _nodeNeumman.size(); ++i)
		{
			_nodeIncogniteP.push_back(_nodeNeumman[i]);
		}
	}
	std :: stable_sort(_nodeIncogniteP.begin(),_nodeIncogniteP.end());
}
/*
	Se determinan los nodos vecinos dado un nodo central
*/
std::vector<int> Mesh :: nodeNeighborh(int node)
{
	// Definimos un vector temporal
	std::vector<int> tmp;	
	// Recorremos todos elementos buscando al nodo
	for (int i = 0; i < _nelements; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			if (_elemntsOrder[i][j] == node && j == 0)
			{
				tmp.push_back(_elemntsOrder[i][1]);
				tmp.push_back(_elemntsOrder[i][2]);
			}
			else if (_elemntsOrder[i][j] == node && j ==1 )
			{
				tmp.push_back(_elemntsOrder[i][2]);
				tmp.push_back(_elemntsOrder[i][0]);
			}
			else if (_elemntsOrder[i][j] == node && j == 2)
			{
				tmp.push_back(_elemntsOrder[i][0]);
				tmp.push_back(_elemntsOrder[i][1]);
			}
		}
	}
	// Devolvemos el valor 
	return tmp;
}
/*
	Determina si un nodo esta dentro de un contorno
*/
std::vector<int> Mesh :: piP(std::vector<std::pair<double,double>> &vertices)
{
	// Arreglo temporal
	std::vector<int> nodes;
	// Variable utiles
	int i = 0;
	int j = vertices.size() - 1; // Almacenamos el indice del ultimo vertice
	bool inside = false; // Determina si un nodo esta dentro o sobre la frontera 
	// Para cada nodo de la malla debemos verificar que este dentro
	/*
	for (int k = 0; k < _nnodes; ++k)
	{
		for (int i = 0; i < vertices.size(); ++i)
		{
			// Determinamos primero si el punto esta entre los vertices que forman una linea
			if ((vertices[i].second < _coordOrder[k].second && vertices[j].second >= _coordOrder[k].second) || ( vertices[j].second < _coordOrder[k].second && vertices[i].second >= _coordOrder[k].second))
			{
				if ((vertices[i].first + (_coordOrder[k].second - vertices[i].second/(vertices[j].second - vertices[i].second))*(vertices[j].first - vertices[i].first)) < _coordOrder[k].first)
				{
					inside = !inside;
				}
			}
			j = i;
			if (inside)
			{
				nodes.push_back(k+1);
			}
		}
	}*/
	for (int i = 0; i < _nnodes; ++i)
	{
		if ((coordX(i+1) >= vertices[0].first && coordX(i+1) <= vertices[1].first) && (coordY(i+1) >= vertices[0].second && coordY(i+1) <= vertices[3].second))
		{
			nodes.push_back(i+1);
		}

	}

	return nodes;
}
/*
	Mostramos los nodos incognita
*/
void Mesh :: shownodeIncongnite()
{
	if (_nodeIncogniteP.size() == 0)
	{
		std :: cout << "ERROR: empty: _nodeIncogniteP" << std :: endl;
	}
	// Mostramos en pantalla los nodos incognita
	std :: cout << std :: endl << std :: endl;
	std :: cout << "NODOS INCOGNITA" << std :: endl;
	for (int i = 0; i < _nodeIncogniteP.size(); ++i)
	{
		std :: cout << _nodeIncogniteP[i] << " ";
	}
	std :: cout << std :: endl << std :: endl;
}
/*
	Muestra en pantalla los nodos con sus coordenada x y Y 
	de forma ordenada
*/
void Mesh :: showNode()
{
	// Comprobamos que se tengan nodos
	if (_coordOrder.size() == 0)
	{
		std :: cout << "ERROR: _coordOrder empty" << std :: endl;
		exit(-1);
	}
	std :: cout << "TABLA NODOS-COORDENADAS-ORDENADAS" << std :: endl;
	for (int i = 0; i < _coordOrder.size(); ++i)
	{
		std :: cout  << i+1 << ":\t\t" << _coordOrder[i].first << "\t\t" << _coordOrder[i].second << std :: endl;
	}
}
/*
	Muestra en pantalla la nueva numeración de los elementos
*/
void Mesh :: showElement()
{
	if (_elemntsOrder.size() == 0)
	{
		std :: cout << "ERROR: _elemntsOrder: empty " << std :: endl;
		exit(-1);
	}
	// Mostramos los nodos componentes
	std :: cout << "TABLA DE ELEMENTOS-NODOS NUEVA NUMERACION" << std :: endl;
	for (int i = 0; i < _nelements; ++i)
	{
		std::cout << i+1 << ":\t" << _elemntsOrder[i][0] << ",\t" << _elemntsOrder[i][1] << ",\t" << _elemntsOrder[i][2] << std::endl;
	}
}
/*
	Muestra en pantalla el numero de nodos y elementos totales
	de una malla
*/
void Mesh :: showNodeElement()
{
	std :: cout << "NUMERO TOTAL DE MALLAS Y ELEMENTOS " << std :: endl;
	std :: cout << "Nodos totales: " << _nnodes << std :: endl;
	std :: cout << "Elementos totales: " << _nelements << std :: endl;
}
/*
	Mostramos en pantalla la relación entre la numeración de gmsh
	con la numeración ordenada
*/
void Mesh :: showRelation(std::vector<int> &relation)
{
	// Revisamos que se tenga contenido de el vector 
	if (relation.size() == 0)
	{
		std :: cout << "ERROR: relation empty" << std :: endl;
		exit(-1);
	}
	// Mostramos en pantalla la relacion
	std :: cout << "RELACION ENTRE LA NUMERACION DE GMSH Y LA NUMERACION ORDENADA" << std :: endl;
	for (int i = 0; i < relation.size(); ++i)
	{
		std :: cout << i+1 << " en la lista ordenada: " << relation[i] << std :: endl;
	}
}
/*
	Mostramos en pantalla los nodos que son de frontera y los que son
	interiores
*/
void Mesh :: showNodeIF()
{
	// Vemos su contenido
	if (_nodeBoundary.size() == 0 || _nodeInterior.size() == 0)
	{
		std :: cout << "ERROR: _nodeInterior y _nodeBoundary are empty" << std :: endl;
		exit(-1);
	}
	std :: cout << std :: endl << std :: endl;
	std :: cout << "NODOS DE CONTORNO" << std :: endl;
	for (int i = 0; i < _nodeBoundary.size(); ++i)
	{
		std :: cout << _nodeBoundary[i] << " ";
	}
	std :: cout << std :: endl << std :: endl;
	std :: cout << "NODOS INTERIORES" << std :: endl;
	for (int i = 0; i < _nodeInterior.size(); ++i)
	{
		std :: cout << _nodeInterior[i] << " ";
	}
}
/*
	Mostramos en pantalla el área de cada elemento triángular según 
	las coordenadas de sus vertices
*/
void Mesh :: showArea()
{
	if (_area.size() == 0)
	{
		std :: cout << "ERROR: _area empty" << std :: endl;
		exit(-1);
	}
	// Recorremos los elementos
	std :: cout << "ÁREA DE CADA ELEMENTO TRIÁNGULAR" << std :: endl;
	for (int i = 0; i < _area.size(); ++i)
	{
		std :: cout << "Elemento " << i+1 << ": area: \t" <<  _area[i] << std :: endl;
	}
}
/*
	Mostramos los elementos que soportan a cada nodo
*/
void Mesh :: showElementSupportNode()
{
	// Verificamos que no se encuentre vacio
	if (_supportNodeElemnt.size() == 0)
	{
		std :: cout << "ERROR: _supportNodeElemnt: empty" << std :: endl;
		exit(-1);
	}
	// Mostramos en pantalla lo requerido
	std :: cout << "TABLA NODOS-ELEMENTOS-SOPORTE" << std :: endl;
	for (int i = 0; i < _nnodes; ++i)
	{
		std :: cout << i+1;
		for (int j = 0; j < _supportNodeElemnt[i].size(); ++j)
		{
			std :: cout << "\t" << _supportNodeElemnt[i][j];
		}
		std :: cout << std :: endl;
	}
}
/*
	Muestra en pantalla informacion sobre los nodos de frontera 
*/
void Mesh :: showBoundaryCondition()
{
	if (_nodeBoundary.size() == 0)
	{
		std :: cout << "ERROR: _nodeBoundary are empty" << std :: endl;
		exit(-1);
	}
	// Mostramos en pantalla el valor de los nodos
	for (int i = 0; i < _nodeBoundary.size(); ++i)
	{
		std :: cout << "NODO " << _nodeBoundary[i] << " ";
		if (_bondaryType[i] == 0)
		{
			std :: cout << " TIPO DIRICHLET, valor: "; 
		}
		else if (_bondaryType[i] == 1)
		{
			std :: cout << " TIPO NEUMANN, valor: "; 
		}
		std :: cout << _bondaryCondition[i] << std :: endl << std :: endl;
	}
}
/*
	Muestra los nodos tipo Dirichlet
*/
void Mesh :: shownodeDirichlet()
{
	if (_nodeDirichlet.size() == 0)
	{
		std :: cout << "ERROR: _nodeDirichlet empty" << std :: endl;
		exit(-1);
	}
	std :: cout << "NODOS TIPO DIRICHLET" << std :: endl;
	for (int i = 0; i < _nodeDirichlet.size(); ++i)
	{
		std :: cout << _nodeDirichlet[i] << " ";
	}
	std :: cout << std :: endl;
}
/*
	Muestra los nodos tipo Neumman
*/
void Mesh :: shownodeNeumman()
{
	if (_nodeNeumman.size() == 0)
	{
		std :: cout << "ERROR: _nodeNeumman empty" << std :: endl;
		exit(-1);
	}
	std :: cout << "NODOS TIPO NEUMANN" << std :: endl;
	for (int i = 0; i < _nodeNeumman.size(); ++i)
	{
		std :: cout << _nodeNeumman[i] << " ";
	}
	std :: cout << std :: endl;
}
/*
	Muestra los nodos vecinos de cada nodo
*/
void Mesh :: showNeighborh()
{
	for (int i = 0; i < _nnodes; ++i)
	{
		std::vector<int> vecino = nodeNeighborh(i+1);
		std :: cout << "EL NODO " << i+1 << " tiene como vecinos a los nodos: " << std :: endl;
		for (int j = 0; j < vecino.size(); ++j)
		{
			std :: cout << vecino[j] << " ";
		}
		std :: cout << std :: endl;
	}
}
/*
	Muestra el total de nodos y elementos de una malla de interes
*/
void Mesh :: showTotal()
{
	if (_nnodes == 0 || _nelements == 0)
	{
		std :: cout << "ERROR: File empty" <<std :: endl;
		exit(-1);
	}
	std::cout << "EL NUMERO TOTAL DE NODOS DE LA MALLA ES DE: " << _nnodes << std :: endl;
	std::cout << "EL NUMERO TOTAL DE ELEMENTOS DE LA MALLA ES DE: " << _nelements << std :: endl;
}