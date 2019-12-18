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



#include "cNTFF.hpp"

cNTFF::cNTFF(){}


cNTFF::~cNTFF(){}

bool cNTFF::calcularCampos(string direccion, string fichero, double theta, double phi)
{
	int i, j, k, l, X[2], signo;
	double r2[3], Wtheta, Wphi, Utheta, Uphi;
	string cadena;
	ifstream fich;

	fich.open(direccion+"/"+fichero);
	if(!fich.is_open()) 
	{
		cout<<"Error: No existe el fichero "+fichero<<endl;
		return false;
	}

	//Leemos r1 y r2 (posiciones de las esquinas del paralelepípedo)
	getline(fich, cadena); //Para !huygensBox
	for(i=0; i<3; i++) fich>>r1[i];
	for(i=0; i<3; i++) fich>>r2[i];

	//Leemos la dimensión de las celdas.
	for(i=0; i<3; i++) getline(fich, cadena); //Para saltar la lína, para la línea vacía y para !dimensionInCells.
	for(i=0; i<3; i++) fich>>Ncel[i];
	for(i=0; i<3; i++) incr_x[i]=abs(r2[i]-r1[i])/Ncel[i]; //Calculamos los incrementos de longitud.

	//Leemos el incremento de tiempo y pasos temporales.
	for(i=0; i<3; i++) getline(fich, cadena); //Para saltar la lína, para la línea vacía y para !time.
	fich>>incr_t;
	fich>>Ntimes;
	for(i=0; i<2; i++) getline(fich, cadena);

	//Calculamos r1 para ajustar el sistema de referencia al centro del cubo de Huygens.
	for(i=0; i<3; i++) r1[i]=(r1[i]-r2[i])/2.0;

	//Damos el valor de r al centro del cubo de Huygens.
	r=0.0;

	//Inicializamos los potenciales con tamaño cero. No es necesario, pero si se llama a esta función
	//sucesivas veces se sumarán los potenciales de diferentes direcciones.
	W.inicializar(0, 0);
	U.inicializar(0, 0);

	//Transformamos la dirección de esféricas a cartesianas.
	direcc[0]=cos(phi)*sin(theta);
	direcc[1]=sin(phi)*sin(theta);
	direcc[2]=cos(theta);
	
	//Leemos los binarios y calculamos cada superficie.
	for(l=0; l<3; l++) //Recorremos cada superficie.
	{
		for(signo=+1; signo>=-1; signo=signo-2)
		{
			X[0]=1;
			X[1]=2;
			rotarDer(X[0], X[1], l); //Establecemos la superficie.
			//Inicializamos los campos cercanos.
			Ene.inicializar(Ntimes, Ncel[X[0]]+1, Ncel[X[1]]+1);
			Hne.inicializar(Ntimes, Ncel[X[0]]+1, Ncel[X[1]]+1);
			for(j=0; j<2; j++)
				for(k=0; k<2; k++)
				{
					for(i=0; i<2; i++) getline(fich, cadena); //Para saltar la línea vacía, !files y !bound.
					getline(fich, cadena); //Nombre fichero binario.
					cadena=direccion+"/"+cadena;
					if(!cargarBinario(cadena, X[0], X[1], j, k)) return false; //Campos EX1, EX2, HX1, HX2, en ese orden.
				}
			if(l==0 && signo==-1) calcPotencialesSup(l, signo);
		}
	}
	
	//Establecemos el periodo temporal mayor que abarque todos los tiempos de W y U.
	j=W.tiempMin();
	if(j>U.tiempMin()) j=U.tiempMin();
	k=W.tiempMax();
	if(k<U.tiempMax()) k=U.tiempMax();

	//Calculamos los campos lejanos. Primero se transformas W y U a esféricas y luego
	//se calculan los campos lejanos también en esféricas.
	for(i=j; i<=k; i++)
	{
		//Calculamos W sub theta
		Wtheta=W.valor(i, 0)*cos(theta)*cos(phi);
		Wtheta+=W.valor(i, 1)*cos(theta)*sin(phi);
		Wtheta+=-W.valor(i, 2)*sin(theta);
		Wtheta/=4*Pi*c*incr_t;

		//Calculamos W sub phi
		Wphi=-W.valor(i, 0)*sin(phi);
		Wphi+=W.valor(i, 1)*cos(phi);
		Wphi/=4*Pi*c*incr_t;

		//Calculamos U sub theta
		Utheta=U.valor(i, 0)*cos(theta)*cos(phi);
		Utheta+=U.valor(i, 1)*cos(theta)*sin(phi);
		Utheta+=-U.valor(i, 2)*sin(theta);
		Utheta/=4*Pi*c*incr_t;

		//Calculamos U sub phi
		Uphi=-U.valor(i, 0)*sin(phi);
		Uphi+=U.valor(i, 1)*cos(phi);
		Uphi/=4*Pi*c*incr_t;

		//Calculamos por último los campos lejanos en coordenadas esféricas.
		Ef.cargar(-imp*Wtheta-Uphi, i, 0);
		Ef.cargar(-imp*Wphi+Utheta, i, 1);
		Hf.cargar(Wphi-Utheta/imp, i, 0);
		Hf.cargar(-Wtheta-Uphi/imp, i, 1);
	}

	fich.close();

	return true;
}


bool cNTFF::cargarBinario(string fichero, int X1, int X2, int campo, int comp)
{
	int i, j, n, borde[2];
	signed short int tamano;
	double aux;
	ifstream fich; //ifstream para leer (in) ofstream para escribir (out). fstream para ambos.
 
    	fich.open (fichero, ios::in | ios::binary);

	if(!fich.is_open()) 
	{
		cout<<"Error: No existe el binario."<<endl;
		return false;
	}

	//Leemos el primer número que indica el tamaño de los datos en bytes.
	fich.read(reinterpret_cast<char *>(&tamano), sizeof(tamano));

	//Establecemos la variable borde para ajustar los bucles. Tienen que tomar un dato más en los bordes.
	borde[0]=0;
	borde[1]=0;
	//Campo E	
	if(campo==0)
	{
		//Componente EX1
		if(comp==0) borde[1]=1;
		//Componente EX2
		if(comp==1) borde[0]=1;
	}
	//Campo H	
	if(campo==1)
	{
		//Componente HX1
		if(comp==0) borde[0]=1;
		//Componente HX2
		if(comp==1) borde[1]=1;
	}

	//Leemos los datos de los campos.
	for(n=0; n<Ntimes; n++)
		for(i=0; i<Ncel[X1]+borde[0]; i++)
			for(j=0; j<Ncel[X2]+borde[1]; j++)
			{
				fich.read(reinterpret_cast<char *>(&aux), sizeof(aux));
				if(i+borde[1]==0 || j+borde[0]==0 || i*borde[0]==Ncel[X1] || j*borde[1]==Ncel[X2])
					if(campo==0) aux/=2.0; //Dividimos entre 2 los campos E de las aristas porque están duplicados.
				if(campo==0) Ene.cargar(aux, n, comp, i, j);
				if(campo==1) Hne.cargar(aux, n, comp, i, j);
			}
	
	if(fich.eof()) 
	{
		fich.close();
		cout<<"Error: Faltan datos en el binario."<<endl;
		return false;
	}
	else
	{
		fich.close();
		return true;
	}
}

void cNTFF::calcPotencialesSup(int X3, int signo)
{
	int n, nn, X1, X2, I[3];
	double a, aux;

	//Calculamos X1, X2, sabiendo el vector normal a la superficie (X3 y signo)
	X1=1;
	X2=2;
	rotarDer(X1, X2, X3);

	//Asignamos el valor de la 3º componente que es fija para toda la superficie.
	if(signo>0) I[X3]=Ncel[X3];
	if(signo<0) I[X3]=0;


	//Recorremos todos los tiempos.
	for(n=0; n<Ntimes; n++)
	{
		//Recorremos la superficie. Recorremos una celda más para incluir los bordes, que están dados dos veces.
		for(I[X1]=0; I[X1]<=Ncel[X1]; I[X1]++)
			for(I[X2]=0; I[X2]<=Ncel[X2]; I[X2]++)
			{
				//Calculamos W[][X1].
				calc_nnya(n, nn, a, I, X1, false);
				aux=Hne.valor(n, 1, I[X1], I[X2])-Hne.valor(n-1, 1, I[X1], I[X2]);
				aux*=-signo*incr_x[X1]*incr_x[X2];
				W.sumar((1-a)*aux, nn, X1);
				W.sumar(a*aux, nn+1, X1);

				//Calculamos W[][X2].
				calc_nnya(n, nn, a, I, X2, false);
				aux=Hne.valor(n, 0, I[X1], I[X2])-Hne.valor(n-1, 0, I[X1], I[X2]);
				aux*=signo*incr_x[X1]*incr_x[X2];
				W.sumar((1-a)*aux, nn, X2);
				W.sumar(a*aux, nn+1, X2);


				//Calculamos U[][X1].
				calc_nnya(n, nn, a, I, X2, true);
				aux=Ene.valor(n+1, 1, I[X1], I[X2])-Ene.valor(n, 1, I[X1], I[X2]);
				aux*=signo*incr_x[X1]*incr_x[X2];
				U.sumar((1-a)*aux, nn, X1);
				U.sumar(a*aux, nn+1, X1);

				//Calculamos U[][X2].
				calc_nnya(n, nn, a, I, X1, true);
				aux=Ene.valor(n+1, 0, I[X1], I[X2])-Ene.valor(n, 0, I[X1], I[X2]);
				aux*=-signo*incr_x[X1]*incr_x[X2];
				U.sumar((1-a)*aux, nn, X2);
				U.sumar(a*aux, nn+1, X2);
			}
	}

	return;
}

void cNTFF::calc_nnya(int n, int &nn, double &a, int* I, int X, bool n_prima)
{
	int i;
	double r_prima[3], aux=0.0;

	//Calculamos r', vector posición del punto.
	for(i=0; i<3; i++) r_prima[i]=r1[i]+incr_x[i]*I[i];
	r_prima[X]+=0.5*incr_x[X];

	//Calculamos el producto escalar de r' con la dirección de observación.
	for(i=0; i<3; i++) aux-=r_prima[i]*direcc[i];
	aux+=r;
	aux/=(c*incr_t); //Velocidad de la luz en m/s por el incremento de t.
	aux+=n;
	if(n_prima) aux+=0.5; //Si se trata de los tiempos desfasados n' sumamos 1/2 para que U esté con el mismo tiempo que W.

	nn=aux; //nn y nn+1 son los enteros que entre los que está aux. nn es el más pequeño.
	if(aux<0) nn--; //Si aux es negativo debemos restar uno a nn para que sea el más pequeño.
	a=(double) aux-nn;

	return;
}

void cNTFF::rotarDer(int &X1, int &X2, int Nrotaciones)
{
	int i, X3, aux;

	if(X1==0 && X2==1) X3=2;
	if(X1==1 && X2==2) X3=0;
	if(X1==2 && X2==0) X3=1;

	for(i=0; i<Nrotaciones; i++)
	{
		aux=X1;
		X1=X2;
		X2=X3;
		X3=aux;
	}

	return;
}
