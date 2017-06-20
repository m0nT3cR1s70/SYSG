//——————————————————————————————————————————————————————————
//            Universidad Nacional Autonoma de Mexico
//               	MALLA CUADRADO
//               	Nieto Butron Mario Arturo
// —————————————————————————————————————————————————————————
// Descripcion:
//	Definimos un dominio limitado por dos semicirculos  
// —————————————————————————————————————————————————————————

lc = 0.014; // Longitud caracteristica

// Entidades geometricas basicas para la construccion
// del dominio
Point(1) = {0.5, 0, 0, lc};   // Punto del dominio 1
Point(2) = {1.0, 0.0, 0, lc}; // Punto del dominio 2
Point(3) = {0, 1, 0, lc};     // Punto del dominio 3
Point(4) = {0, 0.5, 0, lc};   // Punto del dominio 4
Point(5) = {0, 0, 0, lc};     // Punto auxiliar 

Line(1) = {1, 2};            // Linea entre el punto 1 y 2
Circle(2) = {2, 5, 3};       // Semicircunferencia con centro en 5
Line(3) = {3, 4};			 // Linea entre el punto 3 y 4
Circle(4) = {1, 5, 4};       // Semicircunferencia con centro en 5



Physical Line(5) = {3, 4, 1, 2}; // Fronteras del dominio
Line Loop(6) = {3, -4, 1, 2};    //
Plane Surface(7) = {6};          // 
Physical Surface(8) = {7};       // Dominio a ser mallado
