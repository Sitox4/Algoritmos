/* Autores: Alejandro Camino Pardo        Login: a.camino
            Sergio Vila Riveira                  sergio.vila1 
*/

//Ordenacion Rápida (Quicksort) y Ordenacion por insercion

#include <math.h>
#include <stdio.h>s
#include <time.h>
#include <stdlib.h>
#include <sys/time.h>
#define UMBRAL 1
#define SWAP(x, y, T) do { T SWAP = x; x = y; y = SWAP; } while (0)



typedef void (*f)(int v[], int n);

typedef void (*f_cotas)(int n, double t);



void ord_ins(int v[], int n)
{
    int x, j, i;
    for (i = 1; i < n; i++)
    {
        x = v[i];
        j = i - 1;
        while ((j >= 0) && (v[j] > x))
        {
            v[j + 1] = v[j];
            j = j - 1;
        }
        v[j + 1] = x;
    }
}


void mediana3 (int v[],int i,int j){
int k;
k = (i + j)/2;
if (v[k] > v[j]){
    SWAP(v[k],v[j],int);
}
if (v[k] > v[i]){
    SWAP(v[k],v[i],int);
}
if (v[i] > v[j]){
    SWAP(v[i],v[j],int);
}
}


void ordenarAux(int v[], int izq, int der){
    int pivote,i=0,j=0;
    if (izq + UMBRAL <= der){   /*Umbral >= 1 */
        mediana3(v,izq,der);      /*El pivote estará en la izq y en la der habra*/
		                          /*un valor mayor o igual que el pivote*/
        pivote = v[izq];          
        i = izq;
        j = der;
        do{
            do{i = i + 1;} while (!(v[i] >= pivote));
            do{j = j - 1;} while (!(v[j] <= pivote));
            SWAP(v[i],v[j],int);
        } while (!(j <= i));
                              /*Se deshace el ultimo intercambio*/
        SWAP(v[i],v[j],int);
        SWAP(v[izq],v[j],int);
        ordenarAux (v,izq, j-1);
        ordenarAux (v,j+1, der);
    }
}


void ord_rapida(int v [], int n) {

    ordenarAux(v, 0, n-1);
    if (UMBRAL > 1){
        ord_ins(v, n);
    }
}

//Crea un vector aleatorio a partir de la semilla previamente inicializada

void inicializar_semilla() {
    srand(time(NULL));
   
}


void aleatorio(int v[], int n)
{
    int i, m = 2 * n + 1;
    for (i = 0; i < n; i++)
    {
        v[i] = (rand() % m) - 1;
    }
}

//Crea un vector ordenado ascendentemente

void ascendente(int v[], int n)
{
    int i;
    for (i = 0; i < n; i++)
    {
        v[i] = i;
    }
}

//Crea un vector ordenado descendentemente

void descendente(int v[], int n)
{
    int i;
    for (i = n; i > 0; i--)
    {
       v[n-i] = i;
    }
}


void listar_vector(int v[], int n){
  int i;
  printf("[");
  for (i = 0; i < n; i++)
    printf("%4d", v[i]);
  printf("]\n");
}

//Ordenacion 

int comprobar_ord(int v[], int n)
{
    int i;
    if (n<=1){
        return 1;
    }
    for(i=1; i < n; i++){
        if(v[i-1]>v[i]){
            return 0;
        }
    }
    return 1;
}


void calcular(f func, int v[], int n)
{
    int aux;

    aux = comprobar_ord(v, n);
    if (aux == 0)
    {
        printf("Ordenado?: %d \n", comprobar_ord(v, n));
        printf("Ordenamos");
        func(v, n);

        listar_vector(v, n);
        printf("Ordenado?: %d \n\n", comprobar_ord(v, n));
    }
    else
    {
        printf("ordenado?:%d\n\n", comprobar_ord(v, n));
    }
}

//Test

void test(f func)
{
    int v[25], h[20];

    printf("\tTEST\n");
    printf("Inicialización aleatoria\n");

    aleatorio(v, 25);
    listar_vector(v, 25);
    calcular(func, v, 25);
    aleatorio(h, 20);
    listar_vector(h, 20);
    calcular(func, h, 20);



    printf("Inicialización descendente\n");
    descendente(v, 25);
    listar_vector(v, 25);
    calcular(func, v, 25);

    descendente(h, 20);
    listar_vector(h, 20);
    calcular(func, h, 20);


    printf("Inicialización ascendente\n");
    ascendente(v, 25);
    listar_vector(v, 25);
    calcular(func, v, 25);

    ascendente(h, 20);
    listar_vector(h, 20);
    calcular(func, h, 20);

}

//Funcion de tiempos

double microsegundos(){

  /* obtiene la hora del sistema en microsegundos */
  struct timeval t;
  if (gettimeofday(&t, NULL) < 0 )
    return 0.0;
  return (t.tv_usec + t.tv_sec * 1000000.0);
}


void tiempos(f alg, f init, f_cotas cotas){
    int i, j = 0;
    double t1, t2 = 0;
    double t_antes, t_despues, t_tot;
    int v[32000], k = 1000;

    for (i=500; i<=32000; i*=2){
        init(v,i);

        t_antes = microsegundos();
        alg(v, i);
        t_despues = microsegundos();
        t_tot = t_despues - t_antes;

        if (t_tot<500) {	//UMBRAL DE CONFIANZA
            t_antes = microsegundos();
            for (j=0; j<k; j++) {
                init(v,i);
                alg(v,i);
            }
            t_despues = microsegundos();
            t1 = t_despues - t_antes;
            t_antes = microsegundos();
            for (j=0; j<k; j++)   //Descontamos la inicializacion del vector
                init(v,i);
            t_despues = microsegundos();
            t2 = t_despues - t_antes;
            t_tot = (t1-t2)/j;
            printf("(*)");
        } else {
            printf("   ");
        }
        cotas(i,t_tot);
    }
}

//Cotas para el algoritmo de inserción

void cota_ins_aleat(int n, double t)
{
    double t_sub, t_ajus, t_sob = 0;

    t_sub = t / pow(n, 1.8);
    t_ajus = t / pow(n, 2);
    t_sob = t / pow(n, 2.2);
    printf("%5d%17.5f%17.6f%17.6f%17.7f\n", n, t, t_sub, t_ajus, t_sob);
}


void cota_ins_asc(int n, double t)
{
    double t_sub, t_ajus, t_sob = 0;

    t_sub = t / pow(n, 0.8);
    t_ajus = t / pow(n, 1);
    t_sob = t / pow(n, 1.2);
    printf("%5d%17.5f%17.6f%17.6f%17.7f\n", n, t, t_sub, t_ajus, t_sob);
}


void cota_ins_desc(int n, double t)
{
    double t_sub, t_ajus, t_sob = 0;

    t_sub = t / pow(n, 1.8);
    t_ajus = t / pow(n, 2);
    t_sob = t / pow(n, 2.2);
    printf("%5d%17.5f%17.6f%17.6f%17.7f\n", n, t, t_sub, t_ajus, t_sob);
}

//Calculo de tiempos para insercion

void calculoTiemposIns(){
    int i;
    f_cotas cotas[3] = {cota_ins_aleat, cota_ins_asc, cota_ins_desc};

    printf("\nInicialización aleatoria\n");
    for (i = 0; i < 3; i++)
    {
        printf("%8s%17s%17s%17s%17s\n", "n", "t(n)",
               "t(n)/n^1.8", "t(n)/n^2", "t(n)/n^2.2");
        tiempos(ord_ins, aleatorio, cotas[0]);
        printf("\n");
    }

    printf("\nInicialización ascendente\n");
    for (i = 0; i < 3; i++)
    {
        printf("%8s%17s%17s%17s%17s\n", "n", "t(n)",
               "t(n)/n^0.8", "t(n)/n", "t(n)/n^1.2");
        tiempos(ord_ins, ascendente, cotas[1]);
        printf("\n");
    }

    printf("\nInicialización descendente\n");
    for (i = 0; i < 3; i++)
    {
        printf("%8s%17s%17s%17s%17s\n\n", "n", "t(n)",
               "t(n)/n^1.8", "t(n)/n^2", "t(n)/n^2.2");
        tiempos(ord_ins, descendente, cotas[2]);
        printf("\n");
    }
}

//Cotas para el algoritmo de seleccion rapida

void cota_rapida_aleat(int n, double t)
{
    double t_sub=0, t_ajus=0, t_sob = 0;
    switch (UMBRAL){
    case 1 :
        t_sub = t / pow(n, 1.09);
        t_ajus = t / (n*log(n));
        t_sob = t / pow(n, 1.5);
        printf("%6d%17.5f%17.6f%17.6f%17.6f\n", n, t, t_sub, t_ajus, t_sob);    
        break;
    case 10 :
        t_sub = t / pow(n, 0.9);
        t_ajus = t / (n*log(n));
        t_sob = t / pow(n, 1.4);
        printf("%6d%17.5f%17.6f%17.6f%17.6f\n", n, t, t_sub, t_ajus, t_sob);
        break;
    case 100 :
        t_sub = t / pow(n, 0.9);
        t_ajus = t / (n*log(n));
        t_sob = t / pow(n, 1.4);
        printf("%6d%17.5f%17.6f%17.6f%17.6f\n", n, t, t_sub, t_ajus, t_sob);
        break;
    default:
        printf("Introduce un umbral 1,10 o 100!");
        break;
    }
}


void cota_rapida_asc(int n, double t)
{
       double t_sub, t_ajus, t_sob = 0;
    switch (UMBRAL){
    case 1 :
        t_sub = t / pow(n, 0.9);
        t_ajus = t / pow(n,1.07);
        t_sob = t / pow(n, 1.4);
        printf("%6d%17.5f%17.6f%17.6f%17.6f\n", n, t, t_sub, t_ajus, t_sob);    
        break;
    case 10 :
        t_sub = t / pow(n, 0.9);
        t_ajus = t / (n*log(n));
        t_sob = t / pow(n, 1.4);
        printf("%6d%17.5f%17.6f%17.6f%17.6f\n", n, t, t_sub, t_ajus, t_sob);
        break;
    case 100 :
        t_sub = t / pow(n, 0.9);
        t_ajus = t / pow(n,1.15);
        t_sob = t / pow(n, 1.4);
        printf("%6d%17.5f%17.6f%17.6f%17.6f\n", n, t, t_sub, t_ajus, t_sob);
        break;
    default:
        printf("Introduce un umbral 1,10 o 100!");
        break;
    }
}


void cota_rapida_desc(int n, double t){double t_sub, t_ajus, t_sob = 0;
    switch (UMBRAL){
    case 1 :
        t_sub = t / pow(n, 0.9);
        t_ajus = t / pow(n,1.07);
        t_sob = t / pow(n, 1.4);
        printf("%6d%17.5f%17.6f%17.6f%17.6f\n", n, t, t_sub, t_ajus, t_sob);    
        break;
    case 10 :
        t_sub = t / pow(n, 0.9);
        t_ajus = t / (n*log(n));
        t_sob = t / pow(n, 1.4);
        printf("%6d%17.5f%17.6f%17.6f%17.6f\n", n, t, t_sub, t_ajus, t_sob);
        break;
    case 100 :
        t_sub = t / pow(n, 0.9);
        t_ajus = t / (n*log(n));
        t_sob = t / pow(n, 1.4);
        printf("%6d%17.5f%17.6f%17.6f%17.6f\n", n, t, t_sub, t_ajus, t_sob);
        break;
    default:
        printf("Introduce un umbral 1,10 o 100!");
        break;
    }
}

//Calculo de tiempos para ordenacion rapida

void calculoTiemposRapida(){
    int i;
    f_cotas cotas[3] = {cota_rapida_aleat, cota_rapida_asc, cota_rapida_desc};

    printf("\nInicialización aleatoria\n");
    for (i = 0; i < 3; i++)
    {
        printf("%8s%17s%17s%17s%17s\n\n", "n", "t(n)",
               "t(n)/n^0.9", "t(n)/n*log(n)", "t(n)/n^1.4");
        tiempos(ord_rapida, aleatorio, cotas[0]);
        printf("\n");
    }

    printf("\nInicialización ascendente\n");
    for (i = 0; i < 3; i++)
    {
        printf("%8s%17s%17s%17s%17s\n", "n", "t(n)",
               "t(n)/n^0.9", "t(n)/n*log(n)", "t(n)/n^1.4");
        tiempos(ord_rapida, ascendente, cotas[1]);
        printf("\n");
    }

    printf("\nInicialización descendente\n");
    for (i = 0; i < 3; i++)
    {
        printf("%8s%17s%17s%17s%17s\n\n", "n", "t(n)",
               "t(n)/n^0.9", "t(n)/n*log(n)", "t(n)/n^1.4");
        tiempos(ord_rapida, descendente, cotas[2]);
        printf("\n");
    }
}


int main(){
  inicializar_semilla();
  printf("\nOrdenacion por insercion\n");
  printf("===========================\n");
  test(ord_ins);

  printf("\nOrdenacion Rápida\n");
  printf("===========================\n");
  test(ord_rapida);

  //Calculos de cotas
  //Cotas de insercion
  printf("\nOrdenación por inserción\n");
  printf("===========================\n");
  calculoTiemposIns();
  //Cotas de ordenación rapida
  printf("\nOrdenación por ordenacion rapida\n");
  printf("===========================\n");
  calculoTiemposRapida();
 
  return 0;
}