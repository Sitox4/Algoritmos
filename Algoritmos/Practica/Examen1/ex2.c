/*
Pablo Longueira Failde : pablo.longueira.failde@udc.es
*/

#include <math.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <sys/time.h>

/* ========== ALGORITMOS ========== */
#define SWAP(x, y, T) do { T SWAP = x; x = y; y = SWAP; } while (0)
typedef void (*f)(int v[], int n);
typedef void (*f_cotas)(int n, double t);

// Ordenación Shell
void auxord(int v[], int izq, int der){
    int j;
    int i;
    if (izq < der) {
        j = der;
        for (i = der -1 ; i >= izq; i--) {
            if (v[i] > v[der]) {
                j = j - 1;;
                SWAP(v[i], v[j], int);
            }
        }
        SWAP(v[j], v[der], int);
        auxord(v, izq, j - 1);
        auxord(v, j + 1, der);
    }
}

void ordenar(int v[], int n){
    auxord(v, 0, n);
}



/* =========  FUNCIONES AUXILIARES ========== */

//Inicializa la semilla para los números aleatorios.
void inicializar_semilla()
{
    srand(time(NULL));
}

//Crea un vector aleatorio a partir de la semilla previamente inicializada
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

//Muestra un vector bien formateado por pantalla
void listar_vector(int v[], int n)
{
    int i = 0;
    printf("[");
    for (i = 0; i < n; i++)
    {
        printf("%d ", v[i]);
    }
    printf("]\n");
}

//Comprueba si nuestro vector está ordenado o no.
int ordenado(int v[], int n)
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
    listar_vector(v, n);
    aux = ordenado(v, n);
    if (aux == 0)
    {
        printf("Ordenado?: %d \n", ordenado(v, n));
        printf("Ordenamos");
        func(v, n);

        listar_vector(v, n);
        printf("Ordenado?: %d \n\n", ordenado(v, n));
    }
    else
    {
        printf("ordenado?:%d\n\n", ordenado(v, n));
    }
}
/* =========== TEST ========== */
void test(f func)
{
    int v[15];

    printf("\tTEST\n");
    printf("Inicialización aleatoria\n");
    aleatorio(v, 15);
    calcular(func, v, 15);

    printf("Inicialización descendente\n");
    descendente(v, 15);
    calcular(func, v, 15);

    printf("Inicialización ascendente\n");
    ascendente(v, 15);
    calcular(func, v, 15);
}

/* ============== FUNCIONES DE COTAS ============== */



//Cotas para el Algoritmo de ordenación

void cota_algo_aleat(int n, double t)
{
    double t_sub, t_ajus, t_sob;

    t_sub = t / pow(n,0.9);
    t_ajus = t / pow(n,1.15);
    t_sob = t / pow(n,1.3);
    printf("%5d%17.5f%17.6f%17.6f%17.6f\n", n, t, t_sub, t_ajus, t_sob);
}

void cota_algo_asc(int n, double t)
{
    double t_sub, t_ajus, t_sob;

    t_sub = t / pow(n,1.7);
    t_ajus = t / pow(n,2);
    t_sob = t / pow(n,2.2);
    printf("%5d%17.5f%17.6f%17.6f%17.6f\n", n, t, t_sub, t_ajus, t_sob);
}

void cota_algo_desc(int n, double t)
{
    double t_sub, t_ajus, t_sob;

    t_sub = t / pow(n,1.3);
    t_ajus = t / pow(n, 2);
    t_sob = t / pow(n,2.4);
    printf("%5d%17.5f%17.6f%17.6f%17.6f\n", n, t, t_sub, t_ajus, t_sob);
}

/* =========== MEDICIÓN DE TIEMPOS ========== */
double microsegundos()
{
    struct timeval t;
    if (gettimeofday(&t, NULL) < 0)
        return 0.0;
    return (t.tv_usec + t.tv_sec * 1000000.0);
}
void tiempos(f alg, f init, f_cotas cotas){
    int i, j;
    double t1, t2;
    double t_antes, t_despues, t_tot;
    int v[32000], k = 1000;

    for (i=500; i<=10000; i*=2){
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

void calculoTiemposAlgo()
{
    int i;
    f_cotas cotas[3] = {cota_algo_aleat, cota_algo_asc, cota_algo_desc};

    printf("\nInicialización aleatoria\n");
    for (i = 0; i < 3; i++)
    {
        printf("%8s%17s%17s%17s%17s\n\n", "n", "t(n)",
               "t(n)/n^0.9", "t(n)/n^1.15", "t(n)/n^1.3");
        tiempos(ordenar, aleatorio, cotas[0]);
        printf("\n");
    }

    printf("\nInicialización ascendente\n");
    for (i = 0; i < 3; i++)
    {
        printf("%8s%17s%17s%17s%17s\n", "n", "t(n)",
               "t(n)/n^1.7", "t(n)/n^2", "t(n)/n^2.2");
        tiempos(ordenar, ascendente, cotas[1]);
        printf("\n");
    }

    printf("\nInicialización descendente\n");
    for (i = 0; i < 3; i++)
    {
        printf("%8s%17s%17s%17s%17s\n\n", "n", "t(n)",
               "t(n)/n^1.3", "t(n)/n^2", "t(n)/n^2.4");
        tiempos(ordenar, descendente, cotas[2]);
        printf("\n");
    }
}

//Generador de guiones
/* =========== MAIN ========== */

int main()
{
    inicializar_semilla();
    printf("\nOrdenación por inserción\n");
    printf("===========================\n");
    test(ordenar);

  /*  //Calculamos las cotas
    printf("\nOrdenación por inserción\n");
    printf("===========================\n");
    calculoTiemposAlgo();

*/
    return 0;
}