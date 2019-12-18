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

using namespace std;

/*En esta clase se almacenan todos los valores temporales y espaciales de las tres 
componentes de cada potencial. Trabaja con tiempos negativos y positivos. Cuando se 
intenta añadir un valor fuera del rango temporal, se agranda automáticamente.*/


class cPotenciales
{
	public:
		cPotenciales();
		~cPotenciales();

		void inicializar(int Ntpos, int Ntneg); //Redimensiona la matriz y hace que todos los elementos valgan 0. Se pierden todos los datos anteriores que hubiera.
		void cargar(double valor, int tiempo, int comp);
		void sumar(double valor, int tiempo, int comp);
		double valor(int tiempo, int comp);//Comp indica la componente del vector.
		int tiempMin(); //Indica el tiempo mínimo que hay almacenado.
		int tiempMax(); //Indica el tiempo máximo que hay almacenado.

	protected:
		vector<vector<double>> mpos;
		vector<vector<double>> mneg;
		int Ntpos, Ntneg, componentes; //Número de tiempos positivos (empieza en cero), número de tiempos negativos (empieza en cero), y número de componentes del potencial (3 en este caso).
};
