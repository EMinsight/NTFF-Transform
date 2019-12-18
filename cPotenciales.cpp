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



#include "cPotenciales.hpp"

cPotenciales::cPotenciales()
{
	Ntpos=0;
	Ntneg=0;
	componentes=3; //Esta variable aparentemente no es necesaria pero la clase cCamposF deriva de ésta y sólo necesita dos componentes vectoriales. Así se puede manipular fácilmente.
}

cPotenciales::~cPotenciales(){}

void cPotenciales::inicializar(int Ntpos, int Ntneg)
{
	int i, j;

	this->Ntpos=Ntpos;
	this->Ntneg=Ntneg;

	mpos.resize(Ntpos);
	for(i=0; i<Ntpos; i++)
	{	
		mpos[i].resize(componentes, 0.0);
		for(j=0; j<componentes; j++) mpos[i][j]=0.0; //Forzamos que todos los elementos valgan cero.
	}

	mneg.resize(Ntneg);
	for(i=0; i<Ntneg; i++)
	{
		mneg[i].resize(componentes, 0.0);
		for(j=0; j<componentes; j++) mneg[i][j]=0.0;
	}

	return;
}

void cPotenciales::cargar(double valor, int tiempo, int comp)
{	
	int i;

	//Para tiempos positivos guardamos en mpos.
	if(tiempo>=0) 
	{
		if(tiempo>=Ntpos)
		{
			mpos.resize(tiempo+1);
			for(i=Ntpos; i<tiempo+1; i++) mpos[i].resize(componentes, 0.0);
			Ntpos=tiempo+1;
		}
		mpos[tiempo][comp]=valor;
	}
	else //Para tiempos negativos guardamos en mneg.
	{
		if(abs(tiempo)>=Ntneg)
		{
			mneg.resize(abs(tiempo)+1);
			for(i=Ntneg; i<abs(tiempo)+1; i++) mneg[i].resize(componentes, 0.0);
			Ntneg=abs(tiempo)+1;
		}
		mneg[abs(tiempo)][comp]=valor;
	}
	
	return;
}

void cPotenciales::sumar(double valor, int tiempo, int comp)
{	
	int i;

	if(tiempo>=0) 
	{
		if(tiempo>=Ntpos)
		{
			mpos.resize(tiempo+1);
			for(i=Ntpos; i<tiempo+1; i++) mpos[i].resize(componentes, 0.0);
			Ntpos=tiempo+1;
		}
		mpos[tiempo][comp]+=valor;
	}
	else
	{
		if(abs(tiempo)>=Ntneg)
		{
			mneg.resize(abs(tiempo)+1);
			for(i=Ntneg; i<abs(tiempo)+1; i++) mneg[i].resize(componentes, 0.0);
			Ntneg=abs(tiempo)+1;
		}
		mneg[abs(tiempo)][comp]+=valor;
	}
	
	return;
}

double cPotenciales::valor(int tiempo, int comp)
{
	if(tiempo<=-Ntneg || tiempo>=Ntpos) return 0.0;
	else
	{
		if(tiempo>=0) return mpos[tiempo][comp];
		else return mneg[abs(tiempo)][comp];
	}
}

int cPotenciales::tiempMin()
{
	return -(Ntneg-1);
}

int cPotenciales::tiempMax()
{
	return Ntpos-1;
}

