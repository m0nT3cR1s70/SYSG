//——————————————————————————————————————————————————————————
//            Universidad Nacional Autonoma de Mexico
//               	MALLA CUADRADA
//               	Nieto Butron Mario Arturo
// —————————————————————————————————————————————————————————
// Descripcion:
//	Definimos un dominio cuadrado para la solución de
// problema el cual tiene un tamaño de 100
// —————————————————————————————————————————————————————————

lc = 3.5; //longitud carecteristica
// Definimos cuatro puntos del dominio por medio de sus 
// coordenadas x,y,z
Point(1) = {0, 0, 0, lc}; 
Point(2) = {100, 0, 0, lc};
Point(3) = {0, 100, 0, lc};
Point(4) = {100, 100, 0, lc};
Point(5) = {0, 80, 0, lc}; 
Point(6) = {50, 80, 0, lc};
Point(7) = {0, 50, 0, lc};
Point(8) = {50, 50, 0, lc};
Point(9) = {15, 10, 0, lc}; 
Point(10) = {15, 30, 0, lc};
Point(11) = {45, 30, 0, lc};
Point(12) = {45, 10, 0, lc};

Line(1) = {3, 5};
Line(2) = {5, 7};
Line(3) = {7, 1};
Line(4) = {1, 2};
Line(5) = {2, 4};
Line(6) = {4, 3};
Line(7) = {7, 8};
Line(8) = {8, 6};
Line(9) = {6, 5};
Line(10) = {9, 12};
Line(11) = {12, 11};
Line(12) = {11, 10};
Line(13) = {10, 9};



Physical Line(14) = {1, 2, 3, 4, 5, 6};
Line Loop(15) = {1, -9, -8, -7, 3, 4, 5, 6};
Line Loop(16) = {12, 13, 10, 11};
Plane Surface(17) = {15, 16};
Line Loop(18) = {9, 2, 7, 8};
Plane Surface(19) = {18};
Plane Surface(20) = {16};
Physical Surface(21) = {17};
Physical Surface(22) = {19};
Physical Surface(23) = {20};
