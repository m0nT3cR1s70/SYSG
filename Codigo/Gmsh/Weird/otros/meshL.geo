//——————————————————————————————————————————————————————————
//            Universidad Nacional Autonoma de Mexico
//               	MALLA CUADRADA
//               	Nieto Butron Mario Arturo
// —————————————————————————————————————————————————————————
// Descripcion:
//	Definimos un dominio cuadrado para la solución de
// problema el cual tiene un tamaño de 100
// —————————————————————————————————————————————————————————

lc = 25; //longitud carecteristica
// Definimos cuatro puntos del dominio por medio de sus 
// coordenadas x,y,z
Point(1) = {0, 0, 0, lc}; 
Point(2) = {100, 0, 0, lc};
Point(4) = {0, 100, 0, lc};
Point(3) = {100, 100, 0, lc};

// Unimos el punto 1 y el punto 2 con una linea recta
Line(1) = {1, 2};
// Unimos el punto 2 y el punto 3 con una linea recta
Line(2) = {2, 3};
// Unimos el punto 3 y el punto 4 con una linea recta
Line(3) = {3, 4};
// Cerramos el cuadrado uniendo el punto final y el inicial
Line(4) = {4, 1};

Line Loop(5) = {4, 1, 2, 3};
Plane Surface(6) = {5};

