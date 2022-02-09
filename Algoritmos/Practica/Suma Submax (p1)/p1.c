#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>


/* obtiene la hora del sistema en microsegundos */
double microsegundos() { 
    struct timeval t;
    if (gettimeofday(&t, NULL) < 0 )
        return 0.0;
    return (t.tv_usec + t.tv_sec * 1000000.0);
}


 /* se establece la semilla de una nueva serie de enteros pseudo-aleatorios */
void inicializar_semilla() {
    srand(time(NULL));
   
}


/* se generan números pseudoaleatorio entre -n y +n */
void aleatorio(int v [], int n) {
    int i, m=2*n+1;
    for (i=0; i < n; i++)
        v[i] = (rand() % m) - n;
}



int sumaSubMax1(int v[], int n) {
    int i, j;
    int estaSuma, sumaMax = 0;
    for (i = 0; i < n; i++) {
        estaSuma = 0;
        for (j = i; j < n; j++) {
            estaSuma += v[j];
            if (estaSuma > sumaMax) {
                sumaMax = estaSuma;
            }
        }
    }
    return sumaMax;
}


int sumaSubMax2(int v[], int n){
  int estaSuma = 0, sumaMax = 0;
  int j;
  for (j = 0; j < n; j++)
  {
    estaSuma = estaSuma + v[j];
    if (estaSuma > sumaMax)
    {
      sumaMax = estaSuma;
    } else if (estaSuma < 0)
    {
      estaSuma = 0;
    }
  }
  return sumaMax;
}


void listar_vector(int v[], int n){
  int i;
  printf("[");
  for (i = 0; i < n; i++)
    printf("%3d", v[i]);
  printf("  ]");
}


void test1 (){
  int v1[5] = {-9,2,-5,-4,6};
  int v2[5] = {4,0,9,2,5};
  int v3[5] = {-2,-1,-9,-7,-1};
  int v4[5] = {9,-2,1,-7,-8};
  int v5[5] = {15,-2,-5,-4,16};
  int v6[5] = {7,-5,6,7,-7};
  printf("test 1:\n");
  printf("%19s%15s%15s\n", " ", "sumaSubMax", "sumaSubMax2");
  listar_vector(v1, 5);
  printf("%15d%15d\n", sumaSubMax1(v1, 5), sumaSubMax2(v1, 5));
  listar_vector(v2, 5);
  printf("%15d%15d\n", sumaSubMax1(v2, 5), sumaSubMax2(v2, 5));
  listar_vector(v3, 5);
  printf("%15d%15d\n", sumaSubMax1(v3, 5), sumaSubMax2(v3, 5));
  listar_vector(v4, 5);
  printf("%15d%15d\n", sumaSubMax1(v4, 5), sumaSubMax2(v4, 5));
  listar_vector(v5, 5);
  printf("%15d%15d\n", sumaSubMax1(v5, 5), sumaSubMax2(v5, 5));
  listar_vector(v6, 5);
  printf("%15d%15d\n", sumaSubMax1(v6, 5), sumaSubMax2(v6, 5));
}


void test2() {
    int i, a, b;
    int v[9];
    printf("test\n");
    printf("%33s%15s%15s\n", "", "sumaSubMax1", "sumaSubMax2");
    for (i=0; i<10; i++) {
        aleatorio(v, 9);
        listar_vector(v, 9);
        a = sumaSubMax1(v, 9);
        b = sumaSubMax2(v, 9);
        printf("%15d%15d\n", a, b);
    }
}


void resultados(int n, double t, double tsub, double test, double tsob){
  if (t< 500){
    printf("(*)%7d", n);
  }else{
    printf("%10d", n);
  }
  printf("%15.4f %15.6f %15.6f %15.6f \n", t, t/tsub, t/test, t/tsob);
}


double exponencial(int n, double t){
  return pow(n,t);
}


void test3 (int (*fun) (int *, int), double (*cota) (int, double), double sub,
            double est, double sobre, int max, int rep){
  int k;
  int v[32000]; // TamaÃ±o mÃ¡ximo que acepta C
  double ta, tb, t, t1, t2;
    for (int j = 500; j <= max; j *= 2){
      aleatorio(v, j);
      ta = microsegundos();
      fun(v, j);
      tb = microsegundos();
      t = tb - ta;
      if (t < 500){
        ta = microsegundos();
        for (k = 0; k < rep; k++){
          aleatorio(v, j);
          fun(v, j);
        }
        tb = microsegundos();
        t1 = tb - ta;
        ta = microsegundos();
        for (k = 0; k < rep; k++)
          aleatorio(v, j);
        tb = microsegundos();
        t2 = tb - ta;
        t = (t1 - t2) / rep;
      }
      resultados(j, t, cota(j, sub),
      cota(j, est), cota(j, sobre) );
    }
}

int main() {
    inicializar_semilla();
    test1();
    test2();
  //Primer algoritmo sumaSubMax1
  printf("%10s %15s %15s %15s %15s \n","n","t(n)",
  "t(n)/n^1.8","t(n)/n^2.0","t(n)/n^2.2");
  test3(sumaSubMax1, exponencial, 1.8, 2, 2.2, 32000, 100);
  printf("\n");
  test3(sumaSubMax1, exponencial, 1.8, 2, 2.2, 32000, 100);
  printf("\n");
  //Segundo algoritmo sumaSubMax2
  printf("%10s %15s %15s %15s %15s \n","n","t(n)",
  "t(n)/n^0.8","t(n)/n","t(n)/n^1.2");
  test3(sumaSubMax2, exponencial, 0.8, 1, 1.2, 32000,10000);
  printf("\n");
  test3(sumaSubMax2, exponencial, 0.8, 1, 1.2, 32000,10000);
  printf("\n");
  test3(sumaSubMax2, exponencial, 0.8, 1, 1.2, 32000,10000);
  printf("\n");
    return 0;
}
