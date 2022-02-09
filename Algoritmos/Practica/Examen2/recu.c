#include <math.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <sys/time.h>
struct nodo {
int elem;
int num_repeticiones;
struct nodo *izq, *der;
};
typedef struct nodo *posicion;
typedef struct nodo *arbol;
typedef void (*f)(int e,arbol a);
typedef void (*f_cotas)(int n, double t);

/*----------FUNCIONES DADAS POR LA PRÁCTICA--------------*/
static struct nodo *crearnodo(int e) {
struct nodo *p = malloc(sizeof(struct nodo));
    if (p == NULL) {
    printf("memoria agotada\n"); exit(EXIT_FAILURE);
    }
    p->elem = e;
    p->num_repeticiones = 1;
    p->izq = NULL;
    p->der = NULL;
    return p;
}
arbol insertar(int e, arbol a){ 
    if (a == NULL)
        return crearnodo(e);
    else if (e < a->elem)
        a->izq = insertar(e, a->izq);
    else if (e > a->elem)
        a->der = insertar(e, a->der);
    else if (e==a->elem)
        a->num_repeticiones++;
    return a;
}
/*--------------- FUNCIONES AUXILIARES -----------------*/
int max(int a,int b){
    int aux=0;
    if (a<b)
        return aux=b;//b>a
    else return aux=a;;//a>=b
    /* En caso de ser a mayor sale a, y si
        a y b son iguales sale a también*/    
}
//Inicializa la semilla para los números aleatorios.
void inicializar_semilla()
{
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

/*----------------- FUNCIONES BASE  -------------------*/
arbol creararbol(){
    return NULL;
}
int esarbolvacio(arbol a){
    return a==NULL;
}
posicion buscar(int e, arbol a){
    if (a == NULL){
        return NULL;
    }
    else{
        if (e == a->elem){
            return a;
        }else{
            if (e < a->elem){
                return buscar(e, a->izq);
            }else return buscar(e, a->der);
        }
    }
}
arbol eliminararbol(arbol a){
    if(a!=NULL){
        eliminararbol(a->izq);
        eliminararbol(a->der);
        free(a);
        a = NULL;
    }
    return a;
}
posicion hijoizquierdo(arbol a){
    if (a!=NULL)
        return a->izq;
    else return 0;
}
posicion hijoderecho(arbol a){
  if (a!=NULL)
        return a->der;
    else return 0;
}
int elemento(posicion p){
    return p->elem;    
}
int numerorepeticiones(posicion p){
    if (p!=NULL)
        return p->num_repeticiones;
    else return 0;
}
int altura(arbol a){
    if (a==NULL){
        return -1;
    }else if (a->der==NULL && a->izq==NULL){
        return 0;
    } else return 1 + max(altura(a->izq),altura(a->der));
}
void visualizar(arbol a){ //no saca el arbol
    if(a!=NULL){
        printf("(");
        visualizar(a->izq);
        printf(" %d ",a->elem);
        visualizar(a->der);
        printf(")");
        //printf(" (%d)\n",a->elem);
    }//else printf("().");   
}
/*------- MEDICION DE TIEMPOS ----------*/
/*ALGORITMOS ALTURA MEDIA*/

void altura_media_aux2(arbol a, int *numero_nodos, int *suma_alturas){

    int nodos_izq, suma_izq;
    int nodos_der, suma_der;
    int altura_arbol;

    if(esarbolvacio(a)){
        *numero_nodos = 0;
        *suma_alturas = 0;
    }else{

            altura_media_aux2(a->izq,&nodos_izq,&suma_izq);
            altura_media_aux2(a->der,&nodos_der,&suma_der);
            altura_arbol= altura(a);
            *numero_nodos= nodos_izq + nodos_der + 1;
            *suma_alturas= suma_der+suma_izq + altura_arbol;
    }
}

double altura_media(arbol a){
    int numero_nodos=0 , suma_alturas=0 ;

    altura_media_aux2(a,&numero_nodos, &suma_alturas);
    
    if(numero_nodos > 0){
        printf(" sum altura %d num nodos %d\n ",suma_alturas,numero_nodos);
        return (suma_alturas / numero_nodos);
    }else
        return -1;
}

double microsegundos()
{
    struct timeval t;
    if (gettimeofday(&t, NULL) < 0)
        return 0.0;
    return (t.tv_usec + t.tv_sec * 1000000.0);
}

double tiemposAlt(arbol a,int v[],int n){
    double ttotal,t_alt,tantes,tdespues=0;
    int k=10000,j=0,i=0;
    aleatorio(v,n);
    tantes=microsegundos();
    for ( i = 0; i < n; i++)
    {
       altura_media(a);
       
    }
    tdespues=microsegundos();
    ttotal=tdespues-tantes;
      if(ttotal<500){
        tantes=microsegundos();
        for(j=0; j<k; j++){
            altura_media(a);
        }
        tdespues=microsegundos();
        t_alt=tdespues-tantes;
        ttotal=t_alt/k;
        printf("*");
    }
        
    return ttotal;
}
void calculoTiempos(){
    arbol a=creararbol();
    int i,j=0,n=256000,v[n];
    double arraytAlt[6];
    printf("     n\t\t     t_altura\t\n");
    for ( i = 8000; i <=256000 ; i=i*2)
    {   
        arraytAlt[j]=tiemposAlt(a,v,i);
        a=eliminararbol(a);
        printf("%6d %20.2f \n", i, arraytAlt[j] );
        j++;
    }
    printf("\n");
    j=0;
    
    printf("Altura de n elementos: \n");
    printf("     n\t\t   t\t      t/n  \t  t/n^1.41\t t/n^1.7\n");
    for ( i = 8000; i <= 256000; i=i*2)
    {
        printf("%6d%14.2f%15.6f%16.8f%14.8f\n",i,arraytAlt[j],
        arraytAlt[j]/i,
        arraytAlt[j]/pow(i,1.3),
        arraytAlt[j]/pow(i,1.7));
        j++;
    }
    printf("\n");
}

/*---------TEST PRUEBAS MINIMAS--------*/
void test(){
    arbol a= creararbol();
    printf("arbol vacio? :%d\n",esarbolvacio(a));
    visualizar(a);
    printf(" altura  del arbol %d\n",altura(a));
    printf(" altura  promedio del nodo %f\n",altura_media(a));
    printf(" inserto un 3\n"); 
    a=insertar(3,a);
    visualizar(a);
    printf(" altura  del arbol %d\n",altura(a));
    printf(" altura  promedio del nodo %f\n",altura_media(a));
    printf(" inserto un 1\n"); 
    a=insertar(1,a);
    visualizar(a);
    printf(" altura  del arbol %d\n",altura(a));
    printf(" altura  promedio del nodo %f\n",altura_media(a));
    printf(" inserto un 2\n inserto un 5\n inserto un 4\n inserto un 5\n"); 
    a=insertar(2,a);
    a=insertar(5,a);
    a=insertar(4,a);
    a=insertar(5,a);
    visualizar(a);
    printf(" altura  del arbol %d\n",altura(a));
    printf(" altura  promedio del nodo %f\n",altura_media(a));
    a=eliminararbol(a);
    visualizar(a);
    visualizar(a);
    printf(" altura del arbol %d\n",altura(a));
}
int main(){
    
    test();
    inicializar_semilla();
    calculoTiempos();
    calculoTiempos();
    calculoTiempos();
    
}