/******************************************************************************
 * 
 * Archivo: sternsecuencial.cpp 
 *
 * Descripcion: Creacion de un arbol binario de Stern Brocot hasta cierto
 *              nivel ingresado, luego se procede con aproximacion de un       
 *              numero racional con la ayuda de una busqueda binaria     
 * Autores: Nicolas Perez Poblete
 *
 *
 *****************************************************************************/

#include <iostream>
#include <cmath>
#include <cstdlib>
#include <vector>
#include <fstream>
#include <pthread.h>

using namespace std;

vector<vector<float> > arbol(7,vector<float>(67200000)); // Se define una matriz para los datos
                                                          // de 7x135 millones que es el limite si se tiene 4 GB de RAM.

struct timespec tiempo1, tiempo2, latencia;

//*************************************************************************************************************************
void diff_time( timespec *t_fin, timespec *t_ini, timespec *delta ) // Funcion para sacar el tiempo
{                                                                   
    if( ( (*t_fin).tv_nsec - (*t_ini).tv_nsec ) < 0 ){
        if( (*t_fin).tv_sec == (*t_ini).tv_sec ){
            (*delta).tv_sec  = 0;
            (*delta).tv_nsec = 1000000000 + (*t_fin).tv_nsec - (*t_ini).tv_nsec;
        }
        else{
            (*delta).tv_sec  = (*t_fin).tv_sec - (*t_ini).tv_sec - 1;
            (*delta).tv_nsec = 1000000000 + (*t_fin).tv_nsec - (*t_ini).tv_nsec;
        }
    }
    else{
        if( (*t_fin).tv_sec == (*t_ini).tv_sec ){
            (*delta).tv_sec  = 0;
            (*delta).tv_nsec = (*t_fin).tv_nsec - (*t_ini).tv_nsec;
        }
        else{
            (*delta).tv_sec  = (*t_fin).tv_sec - (*t_ini).tv_sec;
            (*delta).tv_nsec = (*t_fin).tv_nsec - (*t_ini).tv_nsec;
        }
    }
}
//*************************************************************************************************************************
unsigned long int pos; // Posiciones avanzadas para crear el arbol;
/*

Filas del arbol:
posicion [0] Numerador
posicion [1] Denominador

otras posiciones:
posicion [2] Izq numerador
posicion [3] Izq Denominaador
posicion [4] Der numerador
posicion [5] Der Denominador
posicion [6] Posicion de la fraccion en el arbol.

*/

void encontrar_hijo_izquierda(){// Funcion que encuentra el hijo de la izquierda segun la fraccion actual del arbol
	unsigned long int posicion;
	posicion = arbol[6][pos] * 2; // Se saca la posicion donde se guardara el hijo
	
	arbol[0][posicion - 1] = arbol[0][pos] + arbol[2][pos];//se saca el numerador del hijo izquierdo
	arbol[1][posicion - 1] = arbol[1][pos] + arbol[3][pos];//se saca el denominador del hijo izquierdo
	
	arbol[2][posicion - 1] = arbol[2][pos]; // se saca el numerador de la fraccion de su izquierda
	arbol[3][posicion - 1] = arbol[3][pos]; // se saca el denominador de su fraccion a la izquierda
	
	arbol[4][posicion - 1] = arbol[0][pos]; // se saca el numerador de la fraccion a su derecha
	arbol[5][posicion - 1] = arbol[1][pos]; // se saca el denominador de la fraccion a su derecha
	
	arbol[6][posicion - 1] = posicion; // se saca la posicion actual de la fraccion (nodo).
	
}

void encontrar_hijo_derecha(){// Funcion que encuentra el hijo de la derecha segun la fraccion actual del arbol
	unsigned long int  posicion;
	posicion = arbol[6][pos] * 2;
	
	arbol[0][posicion] = arbol[0][pos] + arbol[4][pos];//se saca el numerador del hijo derecho
	arbol[1][posicion] = arbol[1][pos] + arbol[5][pos];//se saca el denominador del hio derecho
	
	arbol[4][posicion] = arbol[4][pos];// se saca el numerador de la fraccion de su izquierda
	arbol[5][posicion] = arbol[5][pos];// se saca el denominador de su fraccion a la izquierda
	
	arbol[2][posicion] = arbol[0][pos];// se saca el numerador de la fraccion a su derecha
	arbol[3][posicion] = arbol[1][pos];// se saca el denominador de la fraccion a su derecha
	
	arbol[6][posicion] = posicion + 1;	 // se saca la posicion actual de la fraccion (nodo).
}

int main(){
	unsigned long int cantidad_nodos; // Numero de nodos que podra crear el programa.
	int k; // Numero ingresado por el usuario que representa la altura del arbol que se desea crear.
	int i = 0; // Variable auxiliar para realizar busqueda binaria
	float numero_buscar; // numero decimal ingresado por el usuario.
	float tolerancia; // Porcentaje de error de la busqueda.
	int nivel = 1; // Variable auxiliar para salir de la busqueda binaria.
	int encontrado = 0; // Variable que si cambia de valor, quiere decir que se encontro la aproximacion
	ifstream Archivo;
	Archivo.open("ArbolStern.txt");
	Archivo>>k;
	Archivo>>numero_buscar;
	Archivo>>tolerancia;
	Archivo.close();
	ofstream Archivo2;
	Archivo2.open("ArbolSecuencial.dat",std::ios_base::app);

	cantidad_nodos = pow(2,k-1) - 1;
	long int tiempoComputo;
	arbol[0][0] = 1; // numerador del primer nodo el arbol
	arbol[1][0] = 2; // denominado del primer nodo del arbol
	arbol[2][0] = 0; // numerador de la fraccion de su izquierda
	arbol[3][0] = 1; // denominado de la fraccion de su izquierda
	arbol[4][0] = 1; // numerador de la fraccion de su derecha
	arbol[5][0] = 1; // denominador de la fraccion de su derecha
	arbol[6][0] = 1; // posicion de la fraccion en el arbol. - nodo 1
	clock_gettime(CLOCK_MONOTONIC, &tiempo1); // Se saca el tiempo de inicio.
	for(pos = 0 ; pos < cantidad_nodos; pos ++){ // Se crean los hijos de cada posicion del arbol.
		encontrar_hijo_izquierda(); // Funcion que genera el hijo de la izquierda de un nodo
		encontrar_hijo_derecha(); // Funcion que genera el hijo de la derecha de un nodo.

		Archivo2<<arbol[0][pos]<<endl; // numerador del primer nodo el arbol
		Archivo2<<arbol[1][pos]<<endl; // denominado del primer nodo del arbol
		Archivo2<<arbol[2][pos]<<endl; // numerador de la fraccion de su izquierda
		Archivo2<<arbol[3][pos]<<endl; // denominado de la fraccion de su izquierda
		Archivo2<<arbol[4][pos]<<endl; // numerador de la fraccion de su derecha
		Archivo2<<arbol[5][pos]<<endl; // denominador de la fraccion de su derecha
		Archivo2<<arbol[6][pos]<<endl; // posicion de la fraccion en el arbol. - nodo 
		Archivo2<<endl;
	}
	cout<<endl;
	clock_gettime(CLOCK_MONOTONIC, &tiempo2);// Se saca tiempo despues de que termina el ciclo.
    diff_time(&tiempo2, &tiempo1, &latencia); // Se hace una restas de ambos tiempos para obtener solamente el tiempo del ciclo for
    tiempoComputo = latencia.tv_sec*1000000000 + latencia.tv_nsec;
    cout << "Tiempo en segundos:" << (float)tiempoComputo/1000000000.0 << endl;// Se imprime en pantalla el tiempo transcurrido en segundos
	cout<<"Arbol Creado..."<<endl; // Se avisa que el arbol fue creado.

	clock_gettime(CLOCK_MONOTONIC, &tiempo1); // Se saca el tiempo de inicio para la busqueda
	while(encontrado == 0){
		if(numero_buscar >= arbol[0][i]/arbol[1][i] - tolerancia && numero_buscar <= arbol[0][i]/arbol[1][i] + tolerancia){//Si el numero que se esta buscando es menor que la fraccion actual del arbol mas la toleracia y si es mayor que la fraccion actual del arbol menos la tolerancia 
			encontrado = 1; // Si se cumple la condicion anterior, quiere decir que se encontro
		}                   // una aproximacion valida y no es necesario seguir buscando.
		else{
			if(numero_buscar < arbol[0][i]/arbol[1][i]){ // como es busqueda binaria se compara, si el numero a buscar es menor que el actual, se va a la izquierda, en caso contrario se va a la derecha 
				i = (arbol[6][i] * 2) - 1; // el i toma el valor de la posicion de la siguiente fraccion a la izquierda
				nivel++; // se aumenta el nivel para saber en que piso del arbol nos encontramos
			}
			else{
				i = arbol[6][i] * 2; // En cao contrario, el i toma el valor de la posicion de la fraccion siguiente a la derecha
				nivel++; // se aumenta el nivel para saber en que piso del arbol nos encontramos
			}
		}
		if(nivel>k)// si la busqueda supera el nivel maximo del arbol.
			encontrado = 2; // se cambia el valor encontrado a uno que indica que no se puede seguir buscando
	}
	if(encontrado == 1){// Si se encuentra la solucion, se imprime su numerado y su denominador
		cout<<"La fraccion exacta o que mas se aproxima a la solucion es: ";
		cout<<arbol[0][i]<<"/"<<arbol[1][i]<<endl;// Numeradr y denominador validos aproximados
		cout<<"Nivel donde se encuentra la solucion: "<<nivel<<endl; // Se imprime el nivel del arbol donde se encuentra la solucion
	}
	else// En caso contrario se indica que no se encontro una solucion valida.
		cout<<"No se encontro una solucion aproximada en el arbol Stern Brocot con la tolerancia ingresada.."<<endl;
	clock_gettime(CLOCK_MONOTONIC, &tiempo2); // Se saca tiempo de inicio despues del ciclo.
	diff_time(&tiempo2, &tiempo1, &latencia); // Se realiza una resta de ambos tiempos para sacar solo el que corresponde al ciclo while
	tiempoComputo = latencia.tv_sec*1000000000 + latencia.tv_nsec;
	cout << "Tiempo en segundos de la busqueda:" << (float)tiempoComputo/1000000000.0 << endl;// Se imprime el tiempo en segundos.
	return 0;// Programa finalizado
}
