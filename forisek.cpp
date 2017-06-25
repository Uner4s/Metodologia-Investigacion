/******************************************************************************
 * 
 * Archivo: sternsecuencial.cpp 
 *
 * Descripcion: imprime por pantalla el numerador y el denominador de la fraccion
 *              resultante al aproximar un numero racional.      
 *                 
 * Autor: Michal Forisek
 * Implementado por: Ana Alvarez Ojeda - Nicolas Perez Poblete
 *
 *****************************************************************************/
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <vector>
#include <fstream>

using namespace std;

struct timespec tiempo1, tiempo2, latencia;

//*************************************************************************************************************************
void diff_time( timespec *t_fin, timespec *t_ini, timespec *delta ) // funcion para sacar el tiempo de ejecucion
{
    if( ( (*t_fin).tv_nsec - (*t_ini).tv_nsec ) < 0 )
    {
        if( (*t_fin).tv_sec == (*t_ini).tv_sec )
        {
            (*delta).tv_sec  = 0;
            (*delta).tv_nsec = 1000000000 + (*t_fin).tv_nsec - (*t_ini).tv_nsec;
        }
        else
        {
            (*delta).tv_sec  = (*t_fin).tv_sec - (*t_ini).tv_sec - 1;
            (*delta).tv_nsec = 1000000000 + (*t_fin).tv_nsec - (*t_ini).tv_nsec;
        }
    }
    else
    {
        if( (*t_fin).tv_sec == (*t_ini).tv_sec )
        {
            (*delta).tv_sec  = 0;
            (*delta).tv_nsec = (*t_fin).tv_nsec - (*t_ini).tv_nsec;
        }
        else
        {
            (*delta).tv_sec  = (*t_fin).tv_sec - (*t_ini).tv_sec;
            (*delta).tv_nsec = (*t_fin).tv_nsec - (*t_ini).tv_nsec;
        }
    }
}
//*************************************************************************************************************************

int p_a = 0, q_a = 1, p_b = 1, q_b = 1; // se inician los valores de numeradores y denominadores iniciales
int alpha_num, d_num, denum; // se definen lo que es el numero racional (en forma entera), tolerancia, y quien lo divide para que sea un racional

// Comparacion de fraccion
int menor(int a, int b, int c, int d){ // Si retorna un 1 quiere decir que es una aproximacion valida.
	if(a*d < b*c)
		return 1;
	else
		return 0;
}

int menor_o_equivalente(int a, int b, int c, int d){  // Si retorna un 1 quiere decir que no es una aproximacion valida.
	if(a*d <= b*c)                                     // si retorna 0 da la posibilidad de obtener una aproximacion valida por otra funcion.
		return 1;
	else
		return 0;
}

int Determinar_aproximacion_Valida(int a, int b){ // determina si el nodo actual y el nodo anterior son aproximaciones validas
	if(menor_o_equivalente(a, b, alpha_num-d_num, denum) == 1)
		return 0;
	if(menor(a, b, alpha_num + d_num, denum) == 1)
		return 1;
	return 0;
}

void encontrar_solucion_por_la_izquierda(int p_a,int q_a,int p_b,int q_b){ // Cuando se encentra una aproximacion valida e ejecuta esta funcin
	int numerador, denominador;                                              // Saca el numerador y denominador exactos de la fraccion
	int k_num = denum*p_b - (alpha_num + d_num) * q_b;
	int k_denum = (alpha_num + d_num) * q_a - denum * p_a;
	int k = (k_num/k_denum) + 1;
	numerador = p_b + k * p_a; //se obtiene el numerador aproximado
	denominador = q_b + k*q_a; // se obtiene el denominador aproximado
	cout<<" Numerador: "<<numerador<<endl; // numerador de la fraccion
	cout<<" Denominador: "<<denominador<<endl; // denominador de la faccion
}

void encontrar_solucion_por_la_derecha(int p_a,int q_a,int p_b,int q_b){ // Cuando se encentra una aproximacion valida
	int numerador, denominador;                                          
	int k_num = -denum*p_b + (alpha_num - d_num) * q_b;
	int k_denum = -(alpha_num - d_num) * q_a + denum * p_a;
	int k = (k_num/k_denum) + 1;
	numerador = p_b + k * p_a; //se obtiene el numerador aproximado
	denominador = q_b + k*q_a; // se obtiene el denominador aproximado
	cout<<"Numerador: "<<numerador<<endl;
	cout<<"Denominador: "<<denominador<<endl; // denominador de la faccion
}

int main(int argc, char *argv[]){
	alpha_num = atoi(argv[1]); // representacion del numero racional en entero
	d_num = atoi(argv[2]); // tolerancia de busqueda, generalmente = 5
	denum = atoi(argv[3]); // numero que transforma a alpha_num a decimal 
	int condicion = 0;
	long int tiempoComputo; // tiempo de ejecucion.
	int x, x_num, x_denum, aa, bb, new_p_a, new_p_b, new_q_a, new_q_b; // se definen variables de x hablados en el articulo, y otras auxiliares.
	clock_gettime(CLOCK_MONOTONIC, &tiempo1);
	while(condicion == 0){

		x_num = denum * p_b - alpha_num * q_b;
		x_denum = -denum * p_a + alpha_num * q_a;
		x = (x_num+x_denum-1) / x_denum; // se obtiene la cantidad de pisos que debe saltar el algoritmo
		aa = Determinar_aproximacion_Valida(p_b+x*p_a, q_b+x*q_a); // determinar si es aproximacion validad el nodo actual desues de haber saltado
		bb = Determinar_aproximacion_Valida(p_b+(x-1)*p_a, q_b+(x-1)*q_a); // determinar si es aproximacion valida el nodo anterior del que se encuentra despues del salto
		if(aa || bb){ // si alguno de ellos es distinto de 0 quiere decir que se encontro una aproximacion valida
			encontrar_solucion_por_la_izquierda(p_a, q_a, p_b, q_b); // se busca una aproximacion valida por la izquierda
			condicion = 1; // condicion para no seguir iterando
		}
		if(condicion != 1){		// si no se encontro una aproximacion valida, seguimos pero por la derecha

			new_p_a = p_b + (x-1) * p_a; // Todos los new son variables auxiliares
			new_q_a = q_b + (x-1) * q_a; // para actualizar los vertices de los cuales se empezaran denuevo
			new_p_b = p_b + x*p_a;       // que son el nodo obtenido despues de haber saltado, y el anterior a ese. 
			new_q_b = q_b + x*q_a;
			//-----------------------------//
			p_a = new_p_a;
			q_a = new_q_a;
			p_b = new_p_b;
			q_b= new_q_b;

			x_num = alpha_num * q_b - denum * p_b;    // variables para obtener la cantidad de saltos de nivel  
			x_denum = -alpha_num * q_a + denum * p_a; // para encontrar soluciones validas por la derecha, se cambiaron los signos
			x = (x_num + x_denum - 1) / x_denum; // Cantidad de saltos que se deben hacer pero esta vez por la derecha
			aa = Determinar_aproximacion_Valida(p_b+x*p_a, q_b+x*q_a);// determinar si es aproximacion validad el nodo actual desues de haber saltado
			bb = Determinar_aproximacion_Valida(p_b+(x-1)*p_a, q_b+(x-1)*q_a);// determinar si es aproximacion valida el nodo anterior del que se encuentra despues del salto
			if(aa || bb){ // Si alguno de ambos es distinto de cero quiere decir que se encontro una aproximacion valida
				encontrar_solucion_por_la_derecha(p_a, q_a, p_b, q_b);// Encontramos la solucion pero esta vez por la derecha
				condicion = 1; // Condicion para no seguir iterando
			}
		}
		if(condicion !=1){		 // si nuevamente no se encontro una aproximacion valida, se vuelven a actualizar los intervalos.
			new_p_a = p_b + (x-1) * p_a;
			new_q_a = q_b + (x-1) * q_a;
			new_p_b = p_b + x*p_a;
			new_q_b = q_b + x*q_a;
			//----------------------------------//
			p_a = new_p_a;
			q_a = new_q_a;
			p_b = new_p_b;
			q_b= new_q_b;

		}
	}	
	clock_gettime(CLOCK_MONOTONIC, &tiempo2);
    diff_time(&tiempo2, &tiempo1, &latencia);
    tiempoComputo = latencia.tv_sec*1000000000 + latencia.tv_nsec;
    cout << "Tiempo en segundos:" << (float)tiempoComputo/1000000000.0 << endl;
	cout<<"Presione Enter para continuar..."<<endl;
	getchar();
	system("clear");
	
	return 0;
}