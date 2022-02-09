/* Autores: Alejandro Camino Pardo        Login: a.camino
            Sergio Vila Riveira                  sergio.vila1 */


#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>            

void ord_sel (int v [], int n){
  int minj,minx, i,j;
  for (i = 0; i < n-1; i++){
    minx = v[i];
    minj = i;
    for(j = i+1; j < n; j++){
        if(v[j] < minx){
            minj = j;
            minx = v[j];
        }
    }
    v[minj] = v[i];
    v[i] = minx;
  }
}

void ord_shell(int v[], int n)
{
    int i, j, tmp, inc = 0;
    int seguir = 1;
    inc = n;
    do
    {
        inc = inc / 2;
        for (i = inc; i < n; i++)
        {
            tmp = v[i];
            j = i;
            seguir = 1;
            while (((j - inc) >= 0) && (seguir == 1))
            {

                if (tmp < v[j - inc])
                {
                    v[j] = v[j - inc];
                    j = j - inc;
                }
                else
                    seguir = 0;
            }
            v[j] = tmp;
        }
    } while (inc != 1);
}


///////////////Funciones auxiliares/////////////////
void listar_vector(int v[], int n){
  int i;
  printf("[");
  for (i = 0; i < n; i++)
    printf("%4d", v[i]);
  printf("]\n");
}

void comprobar_ord(int v[], int n ){
  int i, ord = 1;
  for (i = 1; i < n - 1; i++)
    ord = ord && (v[i] >= v[i-1]);
  printf("Ordenado? = %d\n", ord);
}

void inicializar_semilla() {
    srand(time(NULL));
   
}


void resultados(int n, double t, double tsub, double test, double tsob){
  if (t< 500){
    printf("(*)%7d", n);
  }else{
    printf("%10d", n);
  }
  printf("%15.4f %15.6f %15.6f %15.6f \n", t, t/tsub, t/test, t/tsob);
}

void resultados_simple(int n, double t, double test){
  if (t< 500){
    printf("(*)%7d", n);
  }else{
    printf("%10d", n);
  }
  printf("%15.4f %15.6f \n", t, t/test);
}

double microsegundos(){
  /* obtiene la hora del sistema en microsegundos */
  struct timeval t;
  if (gettimeofday(&t, NULL) < 0 )
    return 0.0;
  return (t.tv_usec + t.tv_sec * 1000000.0);
}


///////////////Funciones de ordenaciÃ³n de vectores/////////////////
void aleatorio (int v[], int n){
    int i, m=2*n+1;
    for (i=0; i < n; i++)
        v[i] = (rand() % m) - n;
}

void ascendente(int v[], int n){
    int i;
    for (i=0; i < n; i++)
        v[i] = i;
}

void descendente(int v[], int n){
    int i;
    for (i = n; i > 0; i--){
        v[n - i] = i - 1;
    }
}


///////////////Funciones matemÃ¡ticas de cotas/////////////////
double exponencial(int n, double t){
  return pow(n,t);
}

double nlogaritmica(int n, double t){
  return n * pow(log(n),t);
}

double logaritmica(int n, double t){
  return pow(log(n),t);
}

double potencia_dos(int n, double t){
  return pow(2,n);
}

char* nombre_funcion(double (*cota) (int,double)){
  if (cota == exponencial){
    return "n^";
  } else if (cota == nlogaritmica){
    return "n*log(n)^";
  } 
  return "NULL";
}


///////////////Funciones de mediciÃ³n/////////////////
void test(void (*f) (int*, int)){
    int n = 15;
    int v[n], w[n];
    aleatorio(v, n);
    descendente(w, n);
    printf("Vector Inicializado Aleatorio\n");
    listar_vector(v,n);
    printf("Vector Inicializado Descendente\n");
    listar_vector(w,n);

    f(v,n);
    f(w,n);

    printf("Vector Aleatorio Ordenado\n");
    listar_vector(v,n);
    comprobar_ord(v, n);
    printf("Vector Descendente Ordenado\n");
    listar_vector(w,n);
    comprobar_ord(v, n);
}

void medir (void (*fun) (int *, int),void (*init) (int *, int),
            double (*cota) (int, double), double est, int max, int rep){
  double sobre = est * 0.9;
  double sub = est * 1.1;
  int k, j;
  int vector[32000]; // TamaÃ±o mÃ¡ximo que acepta C
  double ta, tb, t, t1, t2;
  char * name = nombre_funcion(cota);
  printf("%10s %10s %13s%s%0.2f %7s%s%0.2f %7s%s%0.2f \n","n","t(n)",
  "t(n)/",name,sub,"t(n)/",name,est,"t(n)/",name,sobre);
    for (j = 500; j <= max; j *= 2){
      init(vector, j);
      ta = microsegundos();
      fun(vector, j);
      tb = microsegundos();
      t = tb - ta;
      if (t < 500){
        ta = microsegundos();
        for (k = 0; k < rep; k++){
          init(vector, j);
          fun(vector, j);
        }
        tb = microsegundos();
        t1 = tb - ta;
        ta = microsegundos();
        for (k = 0; k < rep; k++)
          init(vector, j);
        tb = microsegundos();
        t2 = tb - ta;
        t = (t1 - t2) / rep;
      }
      resultados(j, t, cota(j, sub),
      cota(j, est), cota(j, sobre) );
    }
    printf("\n");
}

void probar_cotas(){
  printf("Exponencial\n");
  medir(ord_shell,aleatorio,exponencial,1,32000,1000);
  medir(ord_shell,aleatorio,exponencial,1,32000,1000);

  printf("Nlogaritmica\n");
  medir(ord_shell,aleatorio,nlogaritmica,1.12,32000,1000);
  medir(ord_shell,aleatorio,nlogaritmica,1.12,32000,1000);
}

int main(int argc, char const *argv[]){
  inicializar_semilla();
  printf("----------%s----------\n", "Test Ordenacion Seleccion");
  test(ord_sel);
  printf("\n");
  printf("----------%s----------\n", "Test Ordenacion Shell");
  test(ord_shell);
  printf("----------%s----------\n", "Ordenacion Seleccion Aleatorio");
  medir(ord_sel, aleatorio, exponencial, 2, 32000, 1000);
  medir(ord_sel, aleatorio, exponencial, 2, 32000, 1000);
  medir(ord_sel, aleatorio, exponencial, 2, 32000, 1000);
  printf("----------%s----------\n", "Ordenacion Seleccion Ascendente");
  medir(ord_sel, ascendente, exponencial, 2, 32000, 1000);
  medir(ord_sel, ascendente, exponencial, 2, 32000, 1000);
  medir(ord_sel, ascendente, exponencial, 2, 32000, 1000);
  printf("----------%s----------\n", "Ordenacion Seleccion Descendente");
  medir(ord_sel, descendente, exponencial, 2, 32000, 1000);
  medir(ord_sel, descendente, exponencial, 2, 32000, 1000);
  medir(ord_sel, descendente, exponencial, 2, 32000, 1000);
  printf("----------%s----------\n", "Ordenacion Shell Aleatorio");
  medir(ord_shell, aleatorio, exponencial, 1.2, 32000, 1000);
  medir(ord_shell, aleatorio, exponencial, 1.2, 32000, 1000);
  medir(ord_shell, aleatorio, exponencial, 1.2, 32000, 1000);
  printf("----------%s----------\n", "Ordenacion Shell Ascendente");
  medir(ord_shell, ascendente, nlogaritmica, 1.12, 32000, 1000);
  medir(ord_shell, ascendente, nlogaritmica, 1.12, 32000, 1000);
  medir(ord_shell, ascendente, nlogaritmica, 1.12, 32000, 1000);
  printf("----------%s----------\n", "Ordenacion Shell Descendente");
  medir(ord_shell, descendente, exponencial, 1.12, 32000, 1000);
  medir(ord_shell, descendente, exponencial, 1.12, 32000, 1000);
  medir(ord_shell, descendente, exponencial, 1.12, 32000, 1000);
  return 0;
}

