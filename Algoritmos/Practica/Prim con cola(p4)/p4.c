/* Autores: Alejandro Camino Pardo        Login: a.camino
            Sergio Vila Riveira                  sergio.vila1 
*/

//Práctica 4: El Algoritmo de Prim

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>
#define TAM_MAX 1600
typedef int ** matrix;

typedef struct {
    int x, y, peso;
} arista;

typedef arista tipo_elemento;

typedef struct {
    int cabeza, final, tamano;
    tipo_elemento vector[TAM_MAX];
} cola;

//Funciones de cola

void crear_cola(cola *c)
{
    c->tamano = 0;
    c->cabeza = 0;
    c->final = -1;
}

int cola_vacia(cola c)
{
    return (c.tamano == 0);
}

void incrementar(int *x){ 
    if (++(*x) == TAM_MAX)
        *x = 0;
}

void insertar(tipo_elemento x, cola *c)
{
    if (c->tamano == TAM_MAX)
    {
        printf("error: cola llena: %d\n", c->tamano);
        exit(EXIT_FAILURE);
    }
    c->tamano++;
    incrementar(&(c->final));
    c->vector[c->final] = x;
}

tipo_elemento primero(cola c)
{
    if (cola_vacia(c))
    {
        printf("error: cola vacia\n");
        exit(EXIT_FAILURE);
    }
    return (c.vector[c.cabeza]);
}

tipo_elemento quitar_primero(cola *c)
{
    tipo_elemento x;
    if (cola_vacia(*c))
    {
        printf("error: cola vacia\n");
        exit(EXIT_FAILURE);
    }
    c->tamano--;
    x = c->vector[c->cabeza];
    incrementar(&(c->cabeza));
    return x;
}

void mostrar_cola(cola *c){
    int i;
    int peso= 0;
    printf("\nAristas:");
    for(i = 0; i < c->tamano; i++){
        printf("(%d,%d)", c->vector[i].x, c->vector[i].y);
    }
    printf("\n");
    for (i = 0; i < c->tamano; i++) {
        peso += c->vector[i].peso; 
    }
    printf("\nPeso: %d\n", peso);
}


//Algoritmo de Prim

void Prim(matrix m, int nodos, cola *aristas){
    int min, i, j, k=0;
    int contador = 0;
    arista a;
    int *masProximo = (int *) malloc(nodos*sizeof(int));
    int *distanciaMinima = (int *) malloc(nodos*sizeof(int));
    crear_cola(aristas);
    distanciaMinima[0] = -1;
    for(i = 1;i < nodos; i++){
        masProximo[i] = 0;
        distanciaMinima[i] = m[i][0];
    }
    do{
            min =__INT_MAX__;
            for(j = 1; j < nodos; j++){
                if (0 <= distanciaMinima[j] && distanciaMinima[j] < min){
                    min = distanciaMinima[j];
                    k = j;
                }
            }
        a.x = masProximo[k];
        a.y = k;
        a.peso = distanciaMinima[k];
        insertar(a,aristas);
        distanciaMinima[k] = -1;
        for(j = 1; j < nodos; j++){
            if(m[j][k] < distanciaMinima[j]){
                distanciaMinima[j] = m[j][k];
                masProximo[j] = k;
                
            }
        }contador++;
    
    }while(contador < nodos -1);

    free(masProximo);
    free(distanciaMinima);
}

//Funciones para la creacion de la matriz

matrix crear_matriz(int n) {
    int i;
    matrix aux;
    if ((aux = malloc(n*sizeof(int *))) == NULL)
        return NULL;
    for (i=0; i<n; i++)
        if ((aux[i] = malloc(n*sizeof(int))) == NULL)
        return NULL;
    return aux;
}

void inicializar_matriz(matrix m, int n) {
/* Crea un grafo completo no dirigido con valores aleatorios entre 1 y n */
    int i, j;
    for (i=0; i<n; i++)
        for (j=i+1; j<n; j++)
            m[i][j] = rand() % n + 1;
    for (i=0; i<n; i++)
        for (j=0; j<=i; j++)
        if (i==j)
            m[i][j] = 0;
        else
        m[i][j] = m[j][i];
}

void liberar_matriz(matrix m, int n) {
    int i;
    for (i=0; i<n; i++)
        free(m[i]);
    free(m);
}

void inicializar_semilla() {
    srand(time(NULL));
   
}

//Funcion de tiempos

double microsegundos(){

  /* obtiene la hora del sistema en microsegundos */
  struct timeval t;
  if (gettimeofday(&t, NULL) < 0 )
    return 0.0;
  return (t.tv_usec + t.tv_sec * 1000000.0);
}

double medir_tiempo ( int n, int k){
    cola aristas;
    double t_inicio = 0.0;
    double t_fin = 0.0;
    double t_test = 0.0;
    double t_test_init = 0.0;
    double t_init = 0.0;
    int i;
    matrix m;
    m = crear_matriz(n);
    inicializar_matriz(m,n);
    t_inicio = microsegundos();
    Prim(m,n,&aristas);
    t_fin = microsegundos();
    t_test = t_fin - t_inicio;
    if (t_test < 500){
        printf("(*)");
        t_inicio = microsegundos();
        for(i = 0; i < k; i++){
            inicializar_matriz(m,n);
            Prim(m,n,&aristas);
        }
        t_fin = microsegundos();
        t_test_init = t_fin - t_inicio;
        t_inicio = microsegundos();
        for(i = 0;i < k; i++){
            inicializar_matriz(m,n);
        }
        t_fin = microsegundos();
        t_init = t_fin - t_inicio;
        t_test = (t_test_init - t_init)/ k;
    }
    liberar_matriz(m,n);
    return t_test;
}

void print_prim(){
    int k = 1000;
    double tiempo = 0.0;
    int n;
    printf("Algoritmo de Prim\n");
    printf("\t\tn\t\t\t  t(n)\t\t  t(n)/(n^1.85)"
            "\t\t  t(n)/(n^2.08)\t\t  t(n)/(n^2.3)\n");
    for(n = 15; n <= 1000; n*=2){
        tiempo = medir_tiempo(n,k);
        printf("\t% 8d\t\t% 7f\t\t% 7f\t\t% 7f\t\t% 7f\n",
        n, tiempo, tiempo/(pow(n,1.85)),
        tiempo/(pow(n,2.08)),
        tiempo/(pow(n,2.3)));

    }
    printf("\n");
    printf("\n\n (*) Tiempo promedio en %d ejecuciones del algoritmo\n\n",k);

}

void test_prim(){
    cola aristas;
    matrix m = crear_matriz(5);
    m[0][0] = 0; m[0][1] = 1; m[0][2] = 8; m[0][3] = 4; m[0][4] = 7;
    m[1][0] = 1; m[1][1] = 0; m[1][2] = 2; m[1][3] = 6; m[1][4] = 5;
    m[2][0] = 8; m[2][1] = 2; m[2][2] = 0; m[2][3] = 9; m[2][4] = 5;
    m[3][0] = 4; m[3][1] = 6; m[3][2] = 9; m[3][3] = 0; m[3][4] = 3;
    m[4][0] = 7; m[4][1] = 5; m[4][2] = 5; m[4][3] = 3; m[4][4] = 0;
    Prim(m,5,&aristas);
    printf("Arbol de recumbirmiento minimo de la prueba 1:\n");
    mostrar_cola(&aristas);
}

void test_prim1(){
    cola aristas;
    matrix m = crear_matriz(4);
    m[0][0] = 0; m[0][1] = 1; m[0][2] = 4; m[0][3] = 7;
    m[1][0] = 1; m[1][1] = 0; m[1][2] = 2; m[1][3] = 8;
    m[2][0] = 4; m[2][1] = 2; m[2][2] = 0; m[2][3] = 3;
    m[3][0] = 7; m[3][1] = 8; m[3][2] = 3; m[3][3] = 0;
    Prim(m,4,&aristas);
    printf("Arbol de recumbirmiento minimo de la prueba 2:\n");
    mostrar_cola(&aristas);
}

void test_prim2(){
    cola aristas;
    matrix m = crear_matriz(7);
    m[0][0] = 0; m[0][1] = 7; m[0][2] = 99; m[0][3] = 5; m[0][4] = 99; m[0][5] = 99; m[0][6] = 99;
    m[1][0] = 7; m[1][1] = 0; m[1][2] = 8; m[1][3] = 9; m[1][4] = 7; m[1][5] = 99; m[1][6] = 99;
    m[2][0] = 99; m[2][1] = 8; m[2][2] = 0; m[2][3] = 99; m[2][4] = 5; m[2][5] = 99; m[2][6] = 99;
    m[3][0] = 5; m[3][1] = 9; m[3][2] = 99; m[3][3] = 0; m[3][4] = 15; m[3][5] = 6; m[3][6] = 99;
    m[4][0] = 99; m[4][1] = 7; m[4][2] = 5; m[4][3] = 15; m[4][4] = 0; m[4][5] = 8; m[4][6] = 9;
    m[5][0] = 99; m[5][1] = 99; m[5][2] = 99; m[5][3] = 6; m[5][4] = 8; m[5][5] = 0; m[5][6] = 11;
    m[6][0] = 99; m[6][1] = 99; m[6][2] = 99; m[6][3] = 99; m[6][4] = 9; m[6][5] =11; m[6][6] = 0;
    Prim(m,7,&aristas);
    printf("Arbol de recumbirmiento minimo de la prueba 3:\n");
    mostrar_cola(&aristas);
}


int main(int argc , char const *argv[]){
    inicializar_semilla();
    test_prim();
    test_prim1();
    test_prim2();
    printf("\n");
    print_prim();
    print_prim();
    print_prim();
    return 0;
}



