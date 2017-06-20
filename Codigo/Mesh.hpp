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
	@brief Los objetos tipos mesh son contenedores para el dominio de 
	interes generado con la herramienta gmsh. 
*/

#ifndef MESH_HPP
#define MESH_HPP

//******************************************
// Librerias Necesarias para la malla
#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
//******************************************

class Mesh
{
	private:
		// Variables Necesarias
		int _nnodes;     // Almacena el numero de nodos totales de una malla
		int _nelements; // Almacena el numero de elementos triangulares totales de una malla
		std::vector<std::pair<double,double>> _coordOrder; // Coordenadas x y y de cada nodo de la malla
		std::vector<std::vector<int>> _elemntsOrder; // Se almacenan los elementos y que nodos lo conforman
		std::vector<std::vector<int>> _supportNodeElemnt; // Indica que elementos rodean a un nodo
		std::vector<int> _nodeBoundary; // Almacena que nodos son de frontera
		std::vector<int> _nodeFrontera; // Almacena que nodos son de frontera
		std::vector<int> _nodeInterior; // Almacena los nodos interiores
		std::vector<double> _bondaryCondition; // Almacena el valor de un nodo de frontera tipo Neumman o Dirchlet
		std::vector<int> _bondaryType; // Para cada nodo frontera se indica de que tipo es
		std::vector<int> _nodeDirichlet; // Almacena los nodos de tipo Dirichlet
		std::vector<int> _nodeNeumman; // Almacena los nodos tipo Neumann
		std::vector<int> _nodeIncogniteP; // Almacena en orden los nodos que son incognitas
		std::vector<double> _area; // Almacena el area de cada elemento triangular
	public:
		/**
			@brief Constructor vacio, inicializa los atributos a valores por defecto.
		*/
		Mesh();
		/**
			@brief Constructor para cargar un archivo <nombre>.msh y encargado de llamar
			a initialize(filename), para dar el tratamiento basico a una geometria 
			obtenida con la herramienta gmsh.
			@param filename  Ruta de un archivo <nombre>.msh para leer el dominio de trabajo. 
			@oaram vertices  Vertices del dominio.
			@see initialize
		*/
		Mesh(std::string filename, std::vector<double> &vertices){initialize(filename,vertices);};
		/**
			@brief Constructor para cargar un archivo <nombre>.msh y encargado de llamar
			a initialize(filename), para dar el tratamiento basico a una geometria, que
			especifica con elmentos lineales su frontera.
		*/
		Mesh(std::string filename){initialize(filename);};
		/**
			@brief 
			@param Ruta de la localización de un archivo gmsh.
			@param ra Radio 
			@param rb Radio
		*/
		void initialize(std::string filename);
		/**
			@brief 
			@param filename  Ruta de la localización de un archivo gmsh.
			@oaram vertices  Vertices del dominio.
			@return None
		*/
		void initialize(std::string filename, std::vector<double> &vertices);
		/**
			@brief Lee un archivo *.msh generado con la herramienta gmsh y almacena las 
			coordenadas y los elementos triangulares que lo conforman.
			@param filename Ruta de la localización de un archivo gmsh.
			@param coord Matriz de tipo std::vector<std::vector<double>> para almacenar las 
			coordenadas de cada nodo de la malla segun la numeracion por gmsh.
			@param elemn Matriz de tipo std::vector<std::vector<int>> para almacenar los
			elementos triangulares y su nodos que lo conforman segun la numeración de gmsh.
			@return None
		*/
		void readFileGMSH(std::string filename, std::vector<std::vector<double>> &coord, std::vector<std::vector<int>> &elemn);
		/**
			@brief Lee un archivo *.msh generado con la herramienta gmsh y almacena las 
			coordenadas, los elementos triangulares que lo conforman y los nodos de frontera.
			@param filename Ruta de la localización de un archivo gmsh.
			@param coord Matriz de tipo std::vector<std::vector<double>> para almacenar las 
			coordenadas de cada nodo de la malla segun la numeracion por gmsh.
			@param elemn Matriz de tipo std::vector<std::vector<int>> para almacenar los
			elementos triangulares y su nodos que lo conforman segun la numeración de gmsh.
			@return None		
		*/
		void readFileGMSHF(std::string filename, std::vector<std::vector<double>> &coord, std::vector<std::vector<int>> &elemn);
		/**
			@brief Toma los nodos leidos del archivo y cambia el orden dado por
			GMSH, ordena primero con respecto a Y y despues con respecto a X de
			forma tal que se tendra todos los nodos con la misma componente en Y
			y despues se ordena las x en forma asecendente
			@param coord Recibe una matriz de la forma std::vector<std::vector<double>>
			que debera contener las coordenada X y Y de cada nodo de la malla
			@return None
		*/
		void reorderNode(std::vector<std::vector<double>> &coord);
		/**
			@brief Toma los nodos ordenados y los desordenados y devuelve
			una relacion entre ellos, donde el indice de relation se refiere
			a la numeración de gmsh y el contenido indica a que lugar se 
			movio en los nodos.
			@param Recibe una matriz de la forma std::vector<std::vector<double>>
			que debera contener las coordenada X y Y de cada nodo de la malla
			@param Vector de tipo std::vector<int>, en donde será almacenada 
			temporalmente la relacion entre la numeración de gmsh y la numeración
			de los nodos ordenados.
			@return None
		*/
		void relationNodes(std::vector<std::vector<double>> &coord, std::vector<int> &relation);
		/**
			@brief Se reordenan los elementos segun la nueva numeracion de 
			los nodos.
			@param elemn Vector del tipo std::vector<int>> que contiene cada elemento y 
			los nodos que contiene
			@return None
		*/
		void reorderElement(std::vector<std::vector<int>> &elemn,std::vector<int> &relation);
		/**
			@brief Para un dominio rectangular determina que nodos se encuentran sobre la 
			frontera, cuales son interiores y los almacena.
			@param vertices Es un vector del tipo std::vector<double> y debe contener 4 los
			vertices de un dominio rectangular para hacer.
			@return None
		*/
		void borderInteriorNode(std::vector<double> &vertices);
		/**
			@brief Para un dominio rectangular determina que nodos se encuentran sobre la 
			frontera, cuales son interiores y los almacena.
			@param vertices Es un vector del tipo std::vector<double> y debe contener 4 los
			vertices de un dominio rectangular para hacer.
			@return None
		*/
		void borderInteriorNode(std::vector<int> &relation);
		/**
			@brief Para cada nodo de frontera se almacena en memoria el tipo de condición de
			frontera y su valor, solo acepta dos tipos distintos de condiciones de frontera
			Dirchelet y Neumman.
			@param type Valor tipo int, recibe un 0 si se trata de un nodo tipo Dirichlet y 1 si se trata de
			un nodo tipo Neumman, en cualquier otro caso termina la ejecución.
			@param value Tipo double, recibe el valor de frontera de un nodo dado.
			@return None
		*/
		void bondaryCondition(int type, double value);
		/**
			@brief Distingue los nodos de frontera en nodos tipo Dirichlet y nodos tipo Neumann
			@param Ninguno
			@return None
		*/
		void typeNodeBorder();
		/**
			@brief Para cada elemento triangular almacenado calcula su área según las coordenadas
			de sus vertices usando un determinantes, ésta área es almacenada en memoria para su 
			uso.
			@param Ninguno
			@return None
		*/
		void areaElement();
		/**
			@brief Para cada nodo de la malla se calcula sus elementos soporte;
			es decir se almacena para cada nodo los elementos en donde aparace.
			@param Ninguno
			@return None
		*/
		void elementSupportNode();
		/**
			@brief Dado un nodo devuelve los nodos más proximos a el que lo rodean es 
			decir sus nodos vecinos.
			@param node Identificador del nodo del que se desea conocer sus vecinos
			@return un vector de tipo std::vector<int> que contiene los nodos vecinos
		*/
		std::vector<int> nodeNeighborh(int node);
		/**
			@brief Segun el tipo de cada nodo se determina que nodos son incognita y para 
			que nodos se conoce la solución en ese punto, y son almacenados en memoria.
			@param Ninguno
			@return None
		*/
		void nodeIncongnite();
		/**
			@brief Se implementa el algoritmo PointInPolygon, el cual determina
			para un poligono convexo si un punto se encuentra en su interior o en 
			su exterior.
			@param vertices arreglo de tipo std::vector<std::pair<double,double>> 
			almacena los vertices de un poligono convexo.
			@return nodes regresa un arreglo de tipo std::vector<int> el cual
			indica que nodos están dentro del poligono.
		*/
		std::vector<int> piP(std::vector<std::pair<double,double>> &vertices);
		/**
			@brief Devuelve el numero de nodos incognta que se tienen en la malla
			@param Ninguno.
			@return None.
		*/
		inline int nNodeIncongnite(){return _nodeIncogniteP.size();};
		/**
			@brief Devuelve un vector con los nodos de frontera
			@param Ninguno
			@return Nodos de frontera
		*/
		inline std::vector<int> &bordeNode(){return _nodeBoundary;};
		/**
			@brief Devuelve un vector con los nodos de frontera
			tipo Dirichelet
			@param Ninguno
			@return Nodos tipo dirichelet
		*/
		inline std::vector<int> &dirichletNode(){return _nodeDirichlet;};
		/**
			@brief Devuelve los elementos que rodean a un nodo.
			@param Node nodo
			@return _supportNodeElemnt Arreglo de tipo std::std::vector<int> en donde 
			se almacenan los elementos que rodean a un nodo.
		*/
		inline std::vector<int> &elementSupport(int node){return _supportNodeElemnt[node-1];};
		/**
			@brief Dado un nodo cualquiera de la malla devuelve su coordenada en x
			@param node Identificador del nodo que se desea su coordenada x
			@return double valor entero con la coordenada x de un nodo dado
		*/
		inline double coordX(int node){return _coordOrder[node-1].first;};
		/**
			@brief Dado un nodo cualquiera de la malla devuelve su coordenada en y
			@param node Identificador del nodo que se desea su coordenada y
			@return double valor entero con la coordenada y de un nodo dado
		*/
		inline double coordY(int node){return _coordOrder[node-1].second;};
		/**
			@brief Dado un nodo frontera devuelve su valor.
			@param node Recibe un identificador de nodo de frontera.
			@return valor de la condición de frontera de un nodo.
		*/
		inline double value(int node)
		{
			for (int i = 0; i < _nodeBoundary.size(); ++i)
			{
				if (node == _nodeBoundary[i])
				{
					return _bondaryCondition[i];
				}
			}
		}
		/**
			@brief Dado un nodo frontera devuelve su tipo.
			@param node Recibe un identificador de nodo de frontera.
			@return devuelve 0 si es Dirichlet y 1 si es Neumman
		*/
		inline int type(int node)
		{
			for (int i = 0; i < _nodeBoundary.size(); ++i)
			{
				if (node == _nodeBoundary[i])
				{
					return _bondaryType[i];
				}
			}
		}
		/**
			@brief Devuelve los nodos incognita.
			@param Ninguno
			@return std::vector<int> Devuelve los nodos incognita.
		*/
		inline std::vector<int> &Incognite(){return _nodeIncogniteP;};
		/**
			@brief Devuelve el numero total de nodos de una malla
			@param Ninguno
			@return _nnodes Numero total de nodos de la malla
		*/
		inline int nNode(){return _nnodes;};
		/**
			@brief Devuelve el numero de nodos incognita de la malla.
			@param Ninguno
			@return size Devuelve el número de nodos incognita de la malla.
		*/
		inline int nIncognite(){return _nodeIncogniteP.size();}
		/**
			@brief Devuelve el área de cada elemento triangular del dominio
			de interés discretizado.
			@param element Elemento del cual se quiere conocer su área.
			@return area Devuelve el área de un elemento triangular.
		*/
		inline double K(int element){return _area[element-1];};
		/**
			@brief Muestra en pantalla los nodos y los elementos leidos del archivo gmsh 
			segun la numeracion por defecto.
			@param coord Matriz de tipo std::vector<std::vector<double>> que contiene las coordenadas
			de cada nodo de la malla.
			@param elemn Matriz de tipo std::vector<std::vector<int>> que contiene los elementos y 
			los nodos que lo conforman.
			@return None
		*/
		void showGmsh(std::vector<std::vector<double>> &coord, std::vector<std::vector<int>> &elemn);
		/**
			@brief Muestra en pantalla los nodos con sus coordenadas ordenados.
			@param Ninguno
			@return None
		*/
		void showNode();
		/**
			@brief Muestra en pantalla la nueva composición de los elementos
			según la nueva numeración
			@param Ninguno
			@return None
		*/
		void showElement();
		/**
			@brief Muestra en pantalla la cantidad de nodos y elementos totales de la malla.
			@param Ninguno
			@return None
		*/
		void showNodeElement();
		/**
			@brief Mostramos en pantalla la relacion de la numeración ordenada con la numeración
			de gmsh.
			@param Ninguno
			@return None
		*/
		void showRelation(std::vector<int> &relation);
		/**
			@brief Muestra en pantalla que nodos son interiores y que nodos son frontera.
			@param Ninguno.
			@return None.
		*/
		void showNodeIF();
		/**
			@brief Muestra en pantalla el area de cada elemento triangular que fue calculado
			@param Ninguno
			@return None
		*/
		void showArea();
		/**
			@brief Mostramos en pantalla los elementos soporte de cada nodo.
			@param Ninguno.
			@return None.
		*/
		void showElementSupportNode();
		/**
			@brief Muestra en pantalla los nodos frontera con su valor asignado y 
			de que tipo son.
			@param Ninguno
			@return None
		*/
		void showBoundaryCondition();
		/**
			@brief Muestra en pantalla los nodos de frontera tipo Dirchlet.
			@param Ninguno
			@return None
		*/
		void shownodeDirichlet();
		/**
			@brief Muestra en pantalla los nodos de frontera tipo Dirchlet.
			@param Ninguno
			@return None
		*/
		void shownodeNeumman();
		/**
			@brief Muestra en pantalla los nodos vecinos de cada nodo.
			@param Ninguno
			@return None
		*/
		void showNeighborh();
		/**
			@brief Muestra en pantalla el numero total de nodos 
			y de elementos de un dominio de interes
			@param Ninguno
			@return None
		*/
		void showTotal();
		/**
		*/
		void shownodeIncongnite();
};




#endif