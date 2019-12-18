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
#include <fstream>
#include <vector>
#include <cmath>
#include "cCamposNe.hpp"
#include "cCamposF.hpp"

using namespace std;

class cNTFF
{
	public:
		cNTFF();
		~cNTFF();

		cCamposF Ef, Hf; //Campos lejanos (far). Son públicos para poder acceder a ellos desde fuera.
		
		bool calcularCampos(string direccion, string fichero, double theta, double phi); //Calcula los campos lejanos en una dirección dada.

	private:
		const double imp=376.73031346177, c=299792458.0, Pi=3.14159265358979323846;
		int Ncel[3], Ntimes;
		double r, direcc[3], r1[3], incr_x[3], incr_t;
		cCamposNe Ene, Hne; //Campos cercanos (near).
		cPotenciales W, U;
		
		bool cargarBinario(string fichero, int X1, int X2, int campo, int comp); //Devuelve false si el fichero no existe.
		void calcPotencialesSup(int X3, int signo); //Calcula los potenciales de la superficie y dirección cargados en memoria.
		void calc_nnya(int n, int &nn, double &a, int* I, int X, bool n_prima); //Recibe nn y a por referencia, X indicará a qué componente le suma incr_x/2, y n_prima indica si se trata de los tiempos desfasados n' (potencial U).
		void rotarDer(int &X1, int &X2, int Nrotaciones); 
};
