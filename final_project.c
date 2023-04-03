/*
Proyecto Final Algoritmica y Programacion
Realizado por: Palacios, Matías y Reynoso, Juan Cruz.

El siguiente programa tiene como fin, el almacenamiento persistente de registros meteorologicos 
del año en curso, que se pueden ingresar diariamente. Al ejecutarse permite cargar un archivo con
datos ya cargados o crear uno nuevo. El programa permite cargar datos como: temperaturas, precipitaciones
velocidades y direccion del viento y presion atmosferica. Ademas tiene funciones como, mostrar todos
los registros cargados, borrar y dar de alta nuevos registros, mostrar mayores precipitaciones, mayores
velocidades de viento y temperaturas. 
Al probar el archivo nov2022.dat obtenido de EVELIA, se observo que el programa y sus funciones trabajaron 
correctamente, solo que se mostraron 69 registros. Casualmente ninguno con precipatciones. De todas maneras,
al cargar un registro con precipitacion distinta de 0, esta si se mostrara al listar.
Se adjunta tambien un archivo (archivoPropio.dat), el cual contiene 12registros con los que se realizaron pruebas.
El programa fue compilado en el sistema operativo Linux.
Version de gcc utilizada para compilar: gcc (Ubuntu 9.4.0-1ubuntu1~20.04.1) 9.4.0.
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define Max 400

//Lexico Global 
typedef struct {
    int ddmmyyyy;   //Fecha 
    int tmax;   //Temperatura maxima 
    int tmin;   //Temperatura minima 
    int HUM;    //Humedad registrada 
    int PNM;    //Presion atmosferica 
    int DV;     //Direccion viento 
    int FF;     //Velocidad de viento 
    int borrado;    //Este campo se utiliza para saber si el registro esta borrado 
    int PP;     //Precipitacion pluvial 
} RegDiario; //Declaracion del registro 

typedef struct {
    RegDiario a[Max]; //Declaramos el arreglo de registro
    int cant; 
} TData;   

typedef struct TNodo {  //Declaracion del TNodo para lista.
    RegDiario info; 
    struct TNodo *next;
} TNodo;

char nomArch[20]; //Se guarda el año con extension .dat
char arch[20]; // Se guarda el año en string
char copy[20]; //Copia del nombre del archivo
int anioEnCurso;

//Declaracion de Funciones 
int Buscar(TData s, int i, int fecha);      //Funcion Recursiva Buscar, esta funcion toma como pre condicion i= 0
int esBisiesto(int x);      //Funcion para llevar el control de la entrada de dias en años que sean bisiestos
int Vacia(TData s);    

//Declaracion de Acciones
void DeArchivoAArreglo(FILE *f,TData *s);
void DeArchivoALista(FILE* f, TNodo *p);
void SolicitarFecha( int *fecha);
void AltaRegDiario(FILE* f);
void SuprRegDiario(FILE* f);
void MostrarRegAct(FILE *f);
void ModifRegDiario(FILE *f);
void inicializar(TNodo** list);
void Insertar(TNodo* p, RegDiario elem);
void CopiaDeSeguridad(FILE *f, FILE *g);
void ListarMaxTemp(TNodo *p, FILE* f);
void MostrarPP(TData *s, FILE *f);
void OrdenarPP(TData *s);
void InfoParametros(FILE *f); 
void OrdenarFF(TData *s);
void MostrarFF(FILE *f, TData s);
void cargarReg(RegDiario *reg);
void CrearArch(char arch[20]);

int main() {
    FILE* g;
    FILE* h; //Archivo de respaldo
    TData s;
    TNodo *list;
    int eleccion;  
    RegDiario registro;

    while ((eleccion != 1) && (eleccion != 2)) {
        printf("Elija una opcion: \n");
        printf("1. Cargar nuevo archivo (ATENCION: ¡¡¡Si ya existe se sobrescribira!!!)\n");
        printf("2. Cargar archivo existente (Debe existir el archivo en el directorio actual)\n");
        scanf("%d", &eleccion);
    }
    
    if (eleccion == 1) {
        printf("Escribe el nombre del nuevo archivo (sin formato): ");
        scanf("%s", arch);
        strcpy(copy, arch);
        strcpy(nomArch, strcat(arch, ".dat"));
        CrearArch(nomArch);
        printf("El archivo %s ha sido creado!\n", nomArch);    

        do { 
            printf("Escribe el año con el que desea trabajar: ");
            scanf("%d", &anioEnCurso);
        } while (!(anioEnCurso >= 1));

    } else {
        printf("Escribe el nombre del archivo existente (sin formato): ");
        scanf("%s", arch);
        strcpy(copy, arch);
        strcpy(nomArch, strcat(arch, ".dat"));
        printf("El archivo %s fue cargado!\n", nomArch);

        g = fopen(nomArch, "rb");
        fread(&registro, sizeof(registro), 1, g);
        while (!(feof(g))){
            fread(&registro, sizeof(registro), 1, g);
        }
        anioEnCurso = registro.ddmmyyyy % 10000;
        fclose(g);
    } 

    while (eleccion != 10) {
        fflush(stdin);
        printf("\nQue accion desea realizar?\n1. Alta de un registro diario\n2. Suprimir un registro diario\n3. Modificar un registro\n4. Mostrar todos los registros activos\n5. Buscar registro y mostrar parametros\n6. Listar dias de maxima temperatura en el año\n7. Listar dias de maxima precipitacion en el año\n8. Listar dias de mayor velocidad del viento\n9. Realizar copia de seguridad\n10. Salir\n");
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
                                inicializar(&list);
                                ListarMaxTemp(list, g);
                            } 
                            else {
                                if(eleccion == 7) { //Listar el día o días de máxima precipitación en lo que va del año
                                    MostrarPP(&s, g);
                                } 
                                else {
                                    if(eleccion == 8) { //Listar las fechas de mayor a menor velocidad de viento
                                        MostrarFF(g, s);
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
    int dia; 
    int anio;
    //Inicio
    *fecha = 0; 

    do {
        printf("Anio: ");
        scanf("%d", &anio);
        if(anio != anioEnCurso){
            printf("Recuerde que esta trabajando con el año %d.\n", anioEnCurso);
        }
    } while (anio != anioEnCurso);

    do {
        printf("Mes: ");
        scanf("%d",&mes);
    } while (!((1 <= mes) && (mes <= 12 )));    //Controlamos que el mes ingresado sea un mes valido 

    if (mes == 2 ) {
        if (esBisiesto(anio)==0) {      //El siguiente bloque de acciones se ejecutaran si el año ingresado NO es bisiesto     
            do {
                printf("Dia: ");
                scanf("%d",&dia);
            } while (!((1 <= dia) && (dia <= 28)));     //Controlamos que el dia ingresado sea el correcto en el mes de febrero  
        }                           
        else {      //El siguiente bloque de acciones se ejecutaran si el año ingresado es bisiesto
            do {
                printf("Dia: ");
                scanf("%d",&dia);
            } while (!((1 <= dia) && (dia <= 29)));     //Controlamos que el dia ingresado sea el correcto en el mes de febrero 
        }
    }
    else 
    {
        if ((mes == 6)||(mes == 9)||(mes == 11)) { 
            do {
                printf("Dia: ");
                scanf("%d",&dia);
            } while (!((1 <= dia) && (dia <= 30)));    
        }       
        else {
            do {
                printf("Dia: ");
                scanf("%d", &dia);
            } while (!((1 <= dia) && (dia <= 31)));
        }
    } 
    //Las siguientes operaciones se realizan para una representacion convencional en el ambito meteorologico 
    dia = dia * 1000000;
    mes = mes * 10000;
    *fecha = dia + mes + anio ;
}

int esBisiesto(int x) {     //Funcion que devuelve si el año es bisiesto o no 
    //Inicio
    if (((x % 4 == 0) && (x % 100 != 0)) || ((x % 100 == 0) && (x % 400 == 0))) {
        return 1;      
    } else {
        return 0;
    }
}

void cargarReg(RegDiario *reg) {    //Esta accion nos permite cargar con datos un registro 
    //Lexico Local
    int fecha;
    //Inicio
    printf("Ingrese los siguientes datos: \n");
    SolicitarFecha(&fecha);
    reg->ddmmyyyy = fecha; 
    
    printf("Temperatura maxima: ");
    scanf("%d", &reg->tmax);
    fflush(stdin);     // Vacía el buffer de escritura  
    
    do {
        printf("Temperatura minima: ");
        scanf("%d", &reg->tmin);
        fflush(stdin);       
    } while (!((reg->tmin)<(reg->tmax)));   //La temperatura minima debe ser menor que la maxima
    
    do {
        printf("Humedad: ");
        scanf("%d", &reg->HUM);
        fflush(stdin); 
    } while (!((0 <= reg->HUM) && (reg->HUM <= 100)));    //La humedad se mide de 0 a 100%

    do {
        printf("Presion atmosferica (Desde 900 hasta 3500): "); 
        scanf("%d", &reg->PNM);
        fflush(stdin);
    } while(!((900 <= reg->PNM) && (reg->PNM <= 3500)));  //900 a 3500

    do {
        printf("Direccion del viento: "); // 0< dv< 360
        scanf("%d", &reg->DV);
        fflush(stdin);
        
    } while (!((reg->DV >= 0)&&(reg->DV <= 360)));
    
    do {
        printf("Velocidad del viento: ");
        scanf("%d", &reg->FF);
        fflush(stdin); 
        
    } while (!(reg->FF >= 0));
    
    do{ 
        printf("Precipitacion pluvial: ");
        scanf("%d", &reg->PP);
    } while (!(reg->PP >= 0));

    reg->borrado = 0; // Asignamos 0 por que es un r
}

void AltaRegDiario(FILE* f) {
    //Lexico Local
    RegDiario reg;  //Registro en donde se guardan los datos a cargar
    RegDiario reg2; //Registro utilizado para leer el archivo
    //Inicio
    f = fopen(nomArch, "rb");
    cargarReg(&reg);    //Se carga el registro
    
    fread(&reg2, sizeof(reg2), 1, f);
    while (!(feof(f)) && ((reg.ddmmyyyy != reg2.ddmmyyyy) || reg2.borrado == 1)) {  
        fread(&reg2, sizeof(reg2), 1, f);
    }
    fclose(f);
    f = fopen(nomArch, "ab");
    if (reg.ddmmyyyy != reg2.ddmmyyyy) {    //Se verifica si el while termino por que llego al final del archivo
        fwrite(&reg, sizeof(reg), 1, f);    //o porque coincidieron las fechas con un registro no borrado
        printf("Registro cargado!\n");
    } else {
        printf("Ya existe un registro con esta fecha.\n");
    }
    fclose(f); 
}

//PreCond: i=0
int Buscar(TData s, int i, int fecha) { //Esta funcion busca un registro a partir de una fecha dada
    //Inicio
    if (i > s.cant) {                   //Si encuentra devuelve la posicion, de lo contrario devuelve -1
        return(-1);
    }
    else {
        if ((s.a[i].ddmmyyyy == fecha) && (s.a[i].borrado == 0)) {
            return(i);
        }
        else {
            return (Buscar(s, (i+1),fecha));    //Llamada recursiva
        }
    } 
}

void SuprRegDiario(FILE* f) {  
    //Lexico Local
    RegDiario reg;
    int fecha;
    //Inicio
    f = fopen(nomArch, "r+b");
    printf("Ingrese la fecha del registro que quiere eliminar: \n");
    SolicitarFecha(&fecha);

    while (!(feof(f)) && ((reg.ddmmyyyy != fecha) || (reg.borrado == 1))) {    
        fread(&reg, sizeof(reg), 1, f);     
    }

    if (reg.ddmmyyyy == fecha) {    //Al terminar el ciclo verificamos si encontro la fecha solicitada
        reg.borrado = 1;    //Se cambia el valor del registro para actualizarlo como no activo 
        fseek(f, ftell(f)-sizeof(reg), 0);  
        fwrite(&reg, sizeof(reg), 1, f);
        printf("Registro eliminado!\n");  //Se informa que el registro se elimino con exito 
    } else {
        printf("No existe el registro.\n");
    }
    fclose(f);
}

void MostrarRegAct(FILE *f) {
    //Lexico Local
    RegDiario reg;
    int i;
    //Inicio
    f = fopen(nomArch, "rb");   //Se abre el archivo en lectura binaria
    fseek(f, 0, SEEK_END);

    if (ftell(f) == 0) { 
        printf("Archivo sin registros cargados.\n"); 
    } else {    
        fseek(f, 0, SEEK_SET);
        fread(&reg, sizeof(reg), 1, f);
        i = 1;
        while (!(feof(f))) {
            if (reg.borrado == 0) { //Se verifica que el registro a mostrar se encuentre activo 
                printf("\nRegistro %d: ", i);
                printf("\nFecha: %d\n", reg.ddmmyyyy);
                printf("Temperatura maxima: %d\n", reg.tmax);
                printf("Temperatura minima: %d\n", reg.tmin);
                printf("Direccion del viento: %d\n", reg.DV);
                printf("Velocidad del viento: %d\n", reg.FF);
                printf("Presion atmosferica: %d\n", reg.PNM);
                printf("Precipitacion pluvial: %d\n", reg.PP);
                printf("Humedad: %d\n", reg.HUM);
                i++;
            }
            fread(&reg, sizeof(reg), 1, f); //Se avanza al siguiente registro dentro del archivo 
        }
    }
    fclose(f); 
}

void ModifRegDiario(FILE *f) {
    //Lexico Local
    RegDiario reg;
    TData s;
    int pos;
    int fecha;
    //Inicio
    DeArchivoAArreglo(f, &s);   //Se pasa el archivo a arreglo para buscar la posicion del registro a modificar 
    SolicitarFecha(&fecha);     //Se obtiene la fecha del dia a modificar
    pos = Buscar(s, 0, fecha);  //Buscamos su posicion dentro del arreglo
    
    if (pos == -1) {    
        printf("No existe el registro.\n");
    } else {        //Si se encuentra el archivo realizamos el siguiente bloque de acciones 
        printf("\nSe encontro el registro.\n");
        f = fopen(nomArch, "r+b");  //Se abre el archivo en modo lectura/escritura binaria 
        cargarReg(&reg);    //Se invoca la accion que carga el registro
        fseek(f, pos*(sizeof(reg)), 0);     //Esta funcion nos permite posicionarnos en la ubicacion del registro a modificar
        fwrite(&reg, sizeof(reg), 1, f);    //Se realiza la escritura sobre ese mismo registro modificando sus campos
        printf("El registro ha sido modificado.\n");
        fclose(f); 
    }
}

void DeArchivoAArreglo(FILE *f, TData *s) {
    //Lexico Local
    int i;
    RegDiario reg;
    //Inicio
    f = fopen(nomArch, "rb");   //Se abre en modo lectura binaria 

    s->cant = 0;   
    i = 0;
    fread(&reg, sizeof(reg), 1, f);
    while (!(feof(f))) {    //Se cicla en modo lectura binaria hasta que se llega al final del archivo 
        s->a[i] = reg;  //Se asigna en la posicion "i" el registro correspondiente 
        s->cant++;  //Se actualiza la cantidad de registros cargados
        i++;    
        fread(&reg, sizeof(reg), 1, f); //Se lee el siguiente registro
    }
    fclose(f);
}

void inicializar(TNodo** list) {
    //lexico local
    TNodo* aux;
    //inicio
    aux = malloc(sizeof(TNodo));    //Se obtiene un doblete 
    aux->next = NULL;   //Se le asigna lo apuntado por aux.next a null generando asi el elemento ficticio 
    (*list) = aux;  
}

void Insertar(TNodo* p, RegDiario elem) {
    //Lexico Local
    TNodo* aux;
    TNodo* a2;
    //Inicio
    aux = p;
    while (aux->next != NULL) {      
        aux = aux->next;
    }
    a2 = (TNodo*)malloc(sizeof(TNodo)); //Se genera un doblete 
    a2->info = elem;    //Se asigna al campo info el contenido del registro 
    a2->next = aux->next;   
    aux->next = a2;
}

void DeArchivoALista(FILE* f, TNodo *p) {
    //Lexico Local
    RegDiario reg;
    int max;
    //Inicio 
    f = fopen(nomArch, "rb");
    max = 0;   
    
    while (!(feof(f))) {    //Se obtiene el valor maximo de temperatura y se almacena en la variable max
        fread(&reg, sizeof(reg), 1, f);
        if ((reg.tmax >= max) && (reg.borrado == 0)) {  
            max = reg.tmax;
        }
    }
    fseek(f, 0, SEEK_SET );
    fread(&reg, sizeof(reg), 1, f);
    while (!(feof(f))) {    //Se pasan a una lista solo los registros que tengan la maxima temperatura
        if (reg.tmax == max) {
            Insertar(p, reg);
        }
        fread(&reg, sizeof(reg), 1, f);
    }
    fclose(f);
}

void ListarMaxTemp(TNodo *p, FILE* f) { 
    //Lexico Local
    TNodo* rec;
    //Inicio
    DeArchivoALista(f, p);
    rec = p->next;

    f = fopen(nomArch, "rb");
    fseek(f, 0, SEEK_END);

    if (ftell(f) == 0) { //Se verifica si el no archivo esta cargado  
        printf("Archivo sin registros cargados.\n"); 
    } else {    
        printf("Mostrando maximas temperaturas...\n");
        while (rec != NULL) {   //Se avanza en la lista hasta que esta sea igual a null 
            if (rec->info.borrado == 0) {   // Se verifica que el registro este activo
                printf("Maxima temperatura (%d°), fecha: %d\n",rec->info.tmax, rec->info.ddmmyyyy); 
            } 
            rec = rec->next; //Se obtiene la siguiente posicion
        }
    }  
}

void OrdenarPP(TData* s) { //Ordenamiento de precipitaciones con InsertionSort
    //Lexico Local
    int i, j;
    RegDiario aux; 
    //Inicio
    i = 1 ;
    while (i < s->cant){
        aux = s->a[i]; 
        j = i - 1;
        while ((j >= 0 ) && (s->a[j].PP > aux.PP)){ //Comparacion precicpitaciones
            s->a[j+1] = s->a[j]; //Insercion de registro
            j--;
        }
        s->a[j+1] = aux; 
        i++;        
    } 
}

void MostrarPP(TData *s, FILE *f) { //Muestra las precipitaciones
    //Lexico Local
    int i, j, k;
    //Inicio
    DeArchivoAArreglo(f, s);    //Obtiene un arreglo con los registros activos 
    OrdenarPP(&(*s));   //Se ordena el arreglo con el metodo InsertionSort

    f = fopen(nomArch, "rb"); //Se abre el archivo en modo lectura binaria 
    fseek(f, 0, SEEK_END); //Nos posicionamos al final para corroborar que tenga archivos o no. 

    if (ftell(f) == 0) {
        printf("Archivo sin registros cargados.\n");      
    } else { 
        i = s->cant-1;
        j = 0;
        k = 10; 
        printf("Mostrando maximas precipitaciones...\n");   
        while ((j < k) && (i >= 0)) {   //Se cicla hasta poder informar 10 maximas precipitaciones  
            if (s->a[i].borrado == 0) {     //Esta condicion cumple la funcion de corroborar si el registro se encuentra activo
                printf("Maxima precipitacion [%d](%d mm), fecha: %d \n", j+1, s->a[i].PP, s->a[i].ddmmyyyy);
                j++;   
            }
            i--; 
        }
    } 
}

void OrdenarFF(TData *s) { //Ordenamiento de viento por BubbleSort
    //Lexico Local
    int i, j;
    RegDiario aux; 
    //Inicio
    i = 0;
    while (i < s->cant) {
        j = s->cant;
        while (j >= i) {
            if ((s->a[j].FF) > (s->a[j-1].FF)) {   
                aux = s->a[j];          //Utilizamos una variable auxiliar para guardar el registro
                s->a[j] = s->a[j-1];    //Intercambiamos los registros
                s->a[j-1] = aux;        
            }
            j--;
        }
        i++; 
    }    
}                           

void MostrarFF(FILE *f, TData s) { //Muestra los 10 mayores valores de velocidad de viento de mayor a menor
    //Lexico Local
    int i, j;
    int k;
    //Inicio
    DeArchivoAArreglo(f, &s);
    OrdenarFF(&s);
    i = 0;
    j = 1;

    f = fopen(nomArch, "rb");
    fseek(f, 0, SEEK_END);  //Posicionamiento al final del archivo para ver si esta vacio

    if (ftell(f) == 0) {
        printf("Archivo sin registros cargados.\n");
    } else 
        {
            printf("Mostrando maximas velocidades de viento...\n");
            k = i;
            while (((k < 10) && (i < s.cant))) {    //No se muestran mas de 10 registros
                if (s.a[i].borrado == 0) {
                    printf("Maxima velocidad de viento [%d]: %d km/h\n", j, s.a[i].FF);
                    j++;
                    k++;
                }
                i++;
            }
        }
}


void InfoParametros(FILE *f) { //Se informan todos los parametros de una fecha dada
    //Lexico Local
    TData s; 
    int i; 
    int pos; 
    int fecha;
    //Inicio
    DeArchivoAArreglo(f, &s);
    printf("Digite la fecha del registro que quiere buscar: \n");
    SolicitarFecha(&fecha);
    i = 0; 
    pos = Buscar(s,i,fecha);

    if(pos != -1) {     //Se pasa a informar los campos si la recursiva devuelve un valor distinto a -1 
        printf("\nSe encontro el registro:\n");
        printf("Fecha: %d\n", s.a[pos].ddmmyyyy);
        printf("Direccion del viento: %d\n", s.a[pos].DV);
        printf("Velocidad del viento: %d\n", s.a[pos].FF);
        printf("Presion: %d\n", s.a[pos].PNM);
        printf("Temperatura maxima: %d\n", s.a[pos].tmax);
        printf("Temperatura minima: %d\n", s.a[pos].tmin);
        printf("Precipitacion pluvial: %d\n", s.a[pos].PP);
        printf("Humedad: %d\n", s.a[pos].HUM);
    }
    else {
        printf("El registro no existe.\n"); 
    }    
}

void CopiaDeSeguridad(FILE *f, FILE *g) {   //Crea un archivo de respaldo con los mismos datos
    //Lexico Local
    RegDiario reg; 
    char copiaArch[20];
    //Inicio
    strcpy(copiaArch, strcat(copy, "Copia.dat"));   //Concatenacion del nombre del archivo con Copia.dat
    f = fopen(nomArch, "rb");
    g = fopen(copiaArch, "wb");
    fread(&reg, sizeof(reg), 1, f);
        while (!(feof(f))){ //Copia de registros de un archivo a otro
            fwrite(&reg, sizeof(reg), 1, g);
            fread(&reg, sizeof(reg), 1, f);
        }
        printf("Copia de seguridad realizada!\n");
    fclose(f);
    fclose(g); 
}

void CrearArch(char arch[20]) { //Crea nuevo archivo
    //Lexico Local
    FILE *f;
    //Inicio
    f = fopen(nomArch, "wb");   //Se abre un archivo en modo "wb" que permite la lectura/escritura. Se crea un archivo nuevo o se sobreescribe si ya existe 
    if (f == NULL) {
        exit(1);
    }
    fclose(f);
}
