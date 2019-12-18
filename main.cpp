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
#include <sstream>
#include "cNTFF.hpp"

using namespace std;

int main(int argc, char* argv[])
{
	const double Pi=3.14159265358979323846;
	ifstream fichero;
	string direccion, archivo;
	cNTFF sim;

	if(argc!=3)
	{
		cout<<"Error: el programa necesita 2 argumentos."<<endl;
	}
	else
	{
		fichero.open("Dirección datos.dat");
		do{
			getline(fichero, direccion);
		} while((direccion[0]=='#' || direccion.empty()) && !fichero.eof());

		do{
			getline(fichero, archivo);
		} while((archivo[0]=='#' || archivo.empty()) && !fichero.eof());

		/*Se calculan los campos con el fichero .dat con los nombres de los binarios, 
		la dirección donde está, y los dos ángulos con la dirección.*/
		if(!sim.calcularCampos(direccion, archivo, atof(argv[1])*Pi/180.0, atof(argv[2])*Pi/180.0))
		{
			cout<<"Error"<<endl;
		}
	
		//Se escriben los resultados en dos ficheros .dat.
		sim.Ef.escribir("Datos Ef.dat");
		sim.Hf.escribir("Datos Hf.dat");

		fichero.close();
	}
	
	return 0;
}
