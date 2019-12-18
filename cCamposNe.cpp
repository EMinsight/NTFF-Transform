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



#include "cCamposNe.hpp"

cCamposNe::cCamposNe(){}

cCamposNe::~cCamposNe(){}

void cCamposNe::inicializar(int Ntimes, int Ncell1, int Ncell2)
{
	int i, j, k, l;

	Nt=Ntimes;
	N1=Ncell1;
	N2=Ncell2;

	matriz.resize(Nt);
	for(i=0; i<Nt; i++)
	{
		matriz[i].resize(2);
		for(j=0; j<2; j++)
		{
			matriz[i][j].resize(N1);
			for(k=0; k<N1; k++)
			{
				matriz[i][j][k].resize(N2, 0.0);
				for(l=0; l<N2; l++)
					matriz[i][j][k][l]=0.0; //Forzamos que todos los elementos valgan 0.
			}
		}
	}
	
	

	return;
}

void cCamposNe::cargar(double valor, int tiempo, int comp, int IX1, int IX2)
{	
	matriz[tiempo][comp][IX1][IX2]=valor;
	
	return;
}

void cCamposNe::sumar(double valor, int tiempo, int comp, int IX1, int IX2)
{
	matriz[tiempo][comp][IX1][IX2]+=valor;

	return;
}

double cCamposNe::valor(int tiempo, int comp, int IX1, int IX2)
{
	if(tiempo>=0 && tiempo<Nt) return matriz[tiempo][comp][IX1][IX2];
	else return 0.0;
}

void cCamposNe::mostrar(int tiempo, int comp)
{
	int i, j;

	for(i=0; i<N1; i++)
	{
		for(j=0; j<N2; j++)
			cout<<matriz[tiempo][comp][i][j]<<" ";
		cout<<endl;
	}

	return;
}
