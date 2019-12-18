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



#include "cCamposF.hpp"

cCamposF::cCamposF()
{
	Ntpos=0;
	Ntneg=0;
	componentes=2;
}

cCamposF::~cCamposF(){}

void cCamposF::escribir(string fichero)
{
	int n, i;
	ofstream fich;

	fich.open(fichero);

	fich<<"#n  Coord Theta  Coord Phi"<<endl;

	for(n=1-Ntneg; n<Ntpos; n++)
	{
		fich<<n;
		for(i=0; i<componentes; i++) fich<<" "<<valor(n, i);
		fich<<endl;
	}

	fich.close();

	return;
}
