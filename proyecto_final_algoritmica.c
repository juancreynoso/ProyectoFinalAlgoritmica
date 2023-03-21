#include<stdio.h>
#include<stdlib.h>
#define Max 400

typedef struct {
    int ddmmyyyy;
    int tmax;
    int tmin;
    int HUM;
    int PNM;
    int DV;
    int FF;
    int borrado;
    int PP;
} RegDiario;

typedef struct {
    RegDiario a[Max];
    int cant;
} TData;

typedef struct TNodo {
    struct TNodo* next;
    RegDiario info; 
} TNodo;


int Buscar(TData s, int i, int fecha);
int esBisiesto(int x);
void DeArchivoAArreglo(FILE *f,TData *s);
void DeArchivoALista(FILE* f, TNodo *p);
void SolicitarFecha( int *fecha);
void AltaRegDiario(FILE* f);
void SuprRegDiario(FILE* f);
void MostrarRegAct(FILE *f);
void ModifRegDiario(FILE *f);
void Inicializar(TNodo* p);
void Insertar(TNodo* p, RegDiario elem);
void CopiaDeSeguridad(FILE *f, FILE *g);
void ListarMaxTemp(TNodo *p, FILE* f);
void MostrarPP(TData *s, FILE *f);
void OrdenarPP(TData *s);
void InfoParametros(FILE *f); 
void OrdenarFF(TData *s);
void MostrarFF(TData s);

int main() {
    FILE* g;
    FILE* h; //Archivo de respaldo
    RegDiario registro; 
    TData s;
    TNodo *list;
    int eleccion;

    while (eleccion != 10) {

        printf("\nQue accion desea realizar?\n1. Alta de un registro diario\n2. Suprimir un registro diario\n3. Modificar un registro\n4. Mostrar todos los registros activos\n5. Buscar registro y mostrar parametros\n6. par dias de maxima temperatura en el año\n7. par dias de maxima precipitacion en el año\n8. par fechas de mayor a menor velocidad del viento\n9. Realizar copia de seguridad\n10. Salir\n");
        scanf("\n%d", &eleccion);

        if (eleccion == 1) { //Se agrega un registro al archivo. 
            AltaRegDiario(g);
        } 
        else {
            if(eleccion == 2) { //Se elimina un registro del archivo. 
                SuprRegDiario(g);
            }
            else {
                if(eleccion == 3) { //Modifica un archivo, se busca por fecha 
                    ModifRegDiario(g);
                }
                else {
                    if(eleccion == 4) { // Mostrar todos todos los registros activos
                        MostrarRegAct(g);
                    }
                    else {
                        if (eleccion == 5) {  //Buscar registro de un día dado y mostrar todos los parámetros
                            InfoParametros(g);
                        }
                        else {
                            if(eleccion == 6) { //Listar el día o días de máxima temperatura en lo que va del año
                                ListarMaxTemp(&list, g);
                            } 
                            else {
                                if(eleccion == 7) { //Listar el día o días de máxima precipitación en lo que va del año
                                    MostrarPP(&s, g);
                                } 
                                else {
                                    if(eleccion == 8) { //Listar las fechas de mayor a menor velocidad de viento
                                        MostrarFF(s);
                                    } 
                                    else {
                                        if(eleccion == 9) { //Realizar una copia de seguridad del archivo del año en curso
                                            CopiaDeSeguridad(g, h);
                                        }
                                        else {
                                            if(eleccion == 10) { //Salir 
                                                printf("Usted salio del menu con exito.\n");
                                                exit(-1);
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return 0;
}

void SolicitarFecha(int *fecha) {
    //Lexico Local 
    int mes; 
    int anio; 
    int dia; 
    //Inicio 
    fecha = 0; 
    do {
        printf("Ingrese el anio: ");
        scanf("%d",&anio);
    } while (!(anio >= 1700));

    do {
        printf("Ingrese el mes: ");
        scanf("%d",&mes);
    } while (!((1 <= mes) && (mes <= 12 )));

    if (mes == 2 ) {
        if (esBisiesto(anio)==0) {
            do {
                printf("Ingrese el dia: ");
                scanf("%d",&dia);
            } while (!((1 <= dia) && (dia <= 28)));
        } 
        else {
            do {
                printf("Ingrese el dia: ");
                scanf("%d",&dia);
            } while (!((1 <= dia) && (dia <= 29)));
        }
    }
    else 
    {
        if ((mes == 6)||(mes == 9)||(mes == 11)) {
            do {
                printf("Ingrese el dia: ");
                scanf("%d",&dia);
            } while (!((1 <= dia) && (dia <= 30)));
        }       
        else {
            do {
                printf("Ingrese el dia: ");
                scanf("%d",&dia);
            } while (!((1 <= dia) && (dia <= 31)));
        }
    }
    
    dia = dia * 1000000;
    mes = mes * 10000;
    *fecha = dia + mes + anio ;
}

int esBisiesto(int x) {
    if (((x % 4 == 0) && (x % 100 != 0)) || ((x % 100 == 0) && (x % 400 == 0))) {
        return 1;
    } else {
        return 0;
    }
}

void AltaRegDiario(FILE* f) {
    RegDiario reg;
    int fecha;

    f = fopen("registros.dat", "w");

    //SolicitarFecha(&fecha);
    //reg.ddmmyyyy = fecha;
    do {
        printf("Ingrese temperatura maxima: \n");
        scanf("%d", &reg.tmax);
 
        printf("Ingrese temperatura minima: \n");
        scanf("%d", &reg.tmin);
            
    } while (!((reg.tmax > 0) && (reg.tmin > 0)));
    
    int fflush(FILE* f);
    
    do {
        printf("Ingrese humedad: \n");
        scanf("%d", &reg.HUM);
    } while (!((0 < reg.HUM) && (reg.HUM < 100)));
    
    int fflush(FILE* f);

    do {
        printf("Ingrese presion (Desde 350 hasta 900): \n"); //900 y 350
        scanf("%d", &reg.PNM);
    } while(!((350 < reg.PNM) && (reg.PNM < 900)));
        
    int fflush(FILE* f);

    do {
        printf("Ingrese direccion del viento: \n"); // 0< dv< 360
        scanf("%d", &reg.DV);
        
    } while (!((reg.DV>0)&&(reg.DV<360)));
    
    
    do {
        printf("Ingrese velocidad del viento: \n");
        scanf("%d", &reg.FF);
        
    } while (!(reg.FF > 0));
    
    
    printf("Ingrese precipitacion pluvial: \n");
    scanf("%d", &reg.PP);
    reg.borrado = 0;
    int fflush(FILE* f);

    printf("%d", reg.ddmmyyyy);
    printf("%d", reg.DV);
    printf("%d", reg.FF);
    printf("%d", reg.PNM);
    printf("%d", reg.tmax);
    printf("%d", reg.tmin);
    printf("%d", reg.PP);
    printf("%d", reg.HUM);

    fwrite(&reg, sizeof(reg), 1, f);
    printf("Registro cargado");
}

//PreC: i<-1 
int Buscar(TData s, int i, int fecha) {
    //inicio 
    if(i > s.cant) {
        return(-1);
    }
    else {
        if (s.a[i].ddmmyyyy == fecha) {
                return(i);
        }
        else {
            return (Buscar(s,(i+1),fecha));
        }
    } 
}

void SuprRegDiario(FILE* f) {
    RegDiario reg;
    int fecha;
    int pos;

    f = fopen("registros.dat", "l");
    SolicitarFecha(&fecha);
    pos = 0;

    while (!(feof(f))) {
        fread(&reg, sizeof(reg), 1, f);
        if (reg.ddmmyyyy == fecha) {
            pos = ftell(f) - 1;
            reg.borrado = 1;
        }
    }
    fclose(f);

    f = fopen("registros.dat", "a");
    fseek(f, -sizeof(&reg), pos);
    fwrite(&reg, sizeof(reg), 1, f);
}

void MostrarRegAct(FILE *f) {
    RegDiario reg;

    f = fopen("registros.dat", "l");
    
    while (!(feof(f))) {
        fread(&reg, sizeof(reg), 1, f);
        if (reg.borrado == 0) {
            printf("%d", reg.ddmmyyyy);
            printf("%d", reg.DV);
            printf("%d", reg.FF);
            printf("%d", reg.PNM);
            printf("%d", reg.tmax);
            printf("%d", reg.tmin);
            printf("%d", reg.PP);
            printf("%d", reg.HUM);
        }
    }
    fclose(f);
    
}

void ModifRegDiario(FILE *f) {
    RegDiario reg;
    TData s;
    int pos;
    int fecha;
    
    DeArchivoAArreglo(f, &s);
    SolicitarFecha(&fecha);
    pos = Buscar(s, 1, fecha);
    
    if (pos == -1) {
        printf("No existe el registro");
    } else {
        f = fopen("registros.dat", "a");

        SolicitarFecha(&fecha);
        reg.ddmmyyyy = fecha;
        printf("Ingrese temperatura maxima: \n");
        scanf("%d", &reg.tmax);
        printf("Ingrese temperatura minima: \n");
        scanf("%d", &reg.tmin);
        printf("Ingrese humedad: \n");
        scanf("%d", &reg.HUM);
        printf("Ingrese presion: \n");
        scanf("%d", &reg.PNM);
        printf("Ingrese direccion del viento: \n");
        scanf("%d", &reg.DV);
        printf("Ingrese velocidad del viento: \n");
        scanf("%d", &reg.FF);
        printf("Ingrese precipitacion pluvial: \n");
        scanf("%d", &reg.PP);
        reg.borrado = 0;
        
        fseek(f, -sizeof(&reg), ftell(f));  //ver si hay que poner ftell(f)-1
        fwrite(&reg, sizeof(reg), 1, f);
        fclose(f);
    }
}

void DeArchivoAArreglo(FILE *f, TData *s) {
    int i;
    RegDiario reg;

    f = fopen("registros.dat", "l");

    s->cant = 0;
    i = 1;

    while (!(feof(f))) {
        fread(&reg, sizeof(reg), 1, f);
        s->a[i] = reg;
        s->cant++;
        i++;
    }
    fclose(f);
}

void Inicializar(TNodo* p) {
    //inicio
    p = (TNodo*)malloc(sizeof(TNodo)); //Creamos un doblete
    p->next = NULL;
}

void Insertar(TNodo* p, RegDiario elem) {
    TNodo* aux;

    while (p->next != NULL) {
        p = p->next;
    }
    aux = malloc(sizeof(TNodo));
    aux->info = elem;
    aux->next = p->next;
    p->next = aux;
}

void ListarMaxTemp(TNodo *p, FILE* f) {
    TNodo* aux;

    DeArchivoALista(f, p);
    aux = p->next;
    while (aux != NULL) {
        printf("Maxima temperatura, fecha: %d", p->info.ddmmyyyy);
    }
}

void MostrarPP(TData *s, FILE *f) {
    int i;
    
    DeArchivoAArreglo(f, s);
    OrdenarPP(&(*s));
    i = (s->cant)-10;

    while (i <= s->cant) {
        printf("Maxima precipitacion, fecha: %d", s->a[i].ddmmyyyy);
        i++;
    }
}

void OrdenarFF(TData *s) {
    int i, j;
    RegDiario aux; 
    i = 0;
    while (i < s->cant) {
        j = s->cant;
        while (j > i) {
            if ((s->a[j].FF) > (s->a[j-1].FF)) {   
                aux = s->a[j];
                s->a[j] = s->a[j-1]; 
                s->a[j-1] = aux;
            }
            j--;
        }
        i++;
    }
}                           

void MostrarFF(TData s) { //Muestra los 10 mayores valores de velocidad de viento de mayor a menor
    int i;

    for (i = 0; i < 10; i++) {
        printf("Maxima velocidad de viento: %d", s.a[i].FF);
    }
}

/*
void Intercambiar(RegDiario* x, RegDiario* y ) {
    RegDiario *aux;

    *aux = *x;
    *x = *y;
    *y = *aux;
}
*/

void CopiaDeSeguridad(FILE *f, FILE *g) {
    //Lexico Local 
    RegDiario reg; 
    //Inicio 
    f = fopen("registros.dat", "l");
    g = fopen("registrosCopia.dat", "w");
        while (feof(f)){
            fread(&reg, sizeof(reg), 1, f);
            fwrite(&reg, sizeof(reg), 1, g);
        }
    fclose(f);
    fclose(g); 
}

void DeArchivoALista(FILE* f, TNodo *p) {
    RegDiario reg;
    int max;

    f = fopen("registros.dat", "l");
    Inicializar(p);
    max = 0;
    
    while (!(feof(f))) {
        fread(&reg, sizeof(reg), 1, f);
        if (reg.tmax >= max) {
            max = reg.tmax;
        }
    }
    while (!(feof(f))) {
        if (reg.tmax == max) {
            Insertar(p, reg);
        }
    }
    fclose(f);
}

void OrdenarPP(TData* s) {
//Lexico 
    int i, j, aux; 
//Inicio 
    i = 1 ;
    while (i <= s->cant){
        aux = s->a[i].PP; 
        j = i - 1;
        while ((j > 0 )&&(s->a[j].PP > aux)){
            s->a[j+1].PP = s->a[j].PP;
            j--;
        }
        s->a[j+1].PP = aux; 
        i++;        
    } 
}

void InfoParametros(FILE *f) {
    //Lexico local 
    TData s; 
    int i; 
    int pos; 
    int *fecha;
    //inicio 

    DeArchivoAArreglo(f, &s);
    SolicitarFecha(&fecha);
    i = 1;
    pos = Buscar(s,i,fecha); 

    if(pos != -1) {
        printf("%d", s.a[pos].ddmmyyyy);
        printf("%d", s.a[pos].DV);
        printf("%d", s.a[pos].FF);
        printf("%d", s.a[pos].PNM);
        printf("%d", s.a[pos].tmax);
        printf("%d", s.a[pos].tmin);
        printf("%d", s.a[pos].PP);
        printf("%d", s.a[pos].HUM);
    }
    else{
        printf("El registro no existe"); 
    }    
}

