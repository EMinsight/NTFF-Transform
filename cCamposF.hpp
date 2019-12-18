/*Copyright 2019 Francisco Lorente Ruiz.

    This file is part of NTFF Transform.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>. 
*/



#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include "cPotenciales.hpp"

using namespace std;

/*Esta clase guarda los campos lejanos para una dirección en las 2 coordenadas esféricas.
Como su estructura es idéntica a la clase cPotenciales, es una clase derivada de la anterior, 
únicamente que aquí se guardan sólo 2 componentes vectoriales (phi y theta) mientras que en la 
anterior hay 3 (x, y , z). Este hecho se modifica en el constructor sin más que modificar el 
valor de la variable componentes. */


class cCamposF: public cPotenciales
{
	public:
		cCamposF();
		~cCamposF();

		void escribir(string fichero); //Escribe todos los valores en un fichero.
};
