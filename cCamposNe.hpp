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

/*En esta clase se almacenan todos los valores temporales y
espaciales de las dos componentes de cada campo. Se trabaja
cada superficie por separado, así que aquí sólo se guardan
los valores de una única superficie. Sin embargo, esta clase
es independiente de la superficie ya que las trata todas por igual.*/

class cCamposNe
{
	public:
		cCamposNe();
		~cCamposNe();

		void inicializar(int Ntimes, int Ncell1, int Ncell2); //Redimensiona la matriz y hace que todos los elementos valgan 0. Se pierden todos los datos anteriores que hubiera.
		void cargar(double valor, int tiempo, int comp, int IX1, int IX2); //Sustituye el valor de matriz indicado por el double valor. Hay que indicar el tiempo, la componente vectorial y la posición en la superficie.
		void sumar(double valor, int tiempo, int comp, int IX1, int IX2); //Idem que el método anterior pero suma el double valor al número previamente almacenado.
		double valor(int tiempo, int comp, int IX1, int IX2); //Devuelve el número guardado.
		void mostrar(int tiempo, int comp); //Muestra los valores del tiempo n.

	private:
		vector<vector<vector<vector<double>>>> matriz; 
		int Nt, N1, N2; //N1 y N2 indican el tamaño de la superficie.
};
