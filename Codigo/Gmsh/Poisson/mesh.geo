//——————————————————————————————————————————————————————————
//            Universidad Nacional Autonoma de Mexico
//               	MALLA CUADRADO
//               	Nieto Butron Mario Arturo
// —————————————————————————————————————————————————————————
// Descripcion:
//	Definimos un dominio cuadrado para la solución de
// problema el cual tiene un tamaño de 100
// —————————————————————————————————————————————————————————

lc = 0.04;
Point(1) = {0, 0, 0, lc};
Point(2) = {1, 0, 0, lc};
Point(4) = {0, 1, 0, lc};
Point(3) = {1, 1, 0, lc};



Line(1) = {1, 2};
Line(2) = {2, 3};
Line(3) = {3, 4};
Line(4) = {4, 1};
Physical Line(5) = {4, 1, 2, 3};
Line Loop(6) = {4, 1, 2, 3};
Plane Surface(7) = {6};
Physical Surface(8) = {7};
