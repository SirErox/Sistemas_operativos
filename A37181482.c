/* CODIGO BASE PARA EL SHELL DE LA MATERIA DE SISTEMAS OPERATIVOS
   Autor: Tec. David Lozano Bañuelos.

*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>

#define NUM_VARS_AMBIENTE 4
#define FALSE 0
#define TRUE 1

/* Constantes para representar el indice de cada una de las
variables de ambiente que el shell debe manejar */
enum vars_ambiente {
	SHELL,
	PATH,
	HOME,
	PWD
};
/* Variable que contiene el valor de cada una de las
variables de ambiente del shell */
char valores_vars_ambiente[NUM_VARS_AMBIENTE][1000];

/*En esta funcion obtenemos las rutas de "trabajo"
  se hace uso de la funcion getenv(), la cual se encarga
  de obtener la ruta de cada una de las variables que
  le otorgemos, seguidamente de un strcpy, para guardar dicho valor
*/
void inicializa(char *nombreprograma){
     strcpy( valores_vars_ambiente[SHELL],getenv("SHELL") );
     strcpy( valores_vars_ambiente[PATH],getenv("PATH") );
     strcpy( valores_vars_ambiente[HOME],getenv("HOME") );
     strcpy( valores_vars_ambiente[PWD],getenv("PWD") );
}

/*En esta variable quedan los elementos encontrados
por la funcion separa */
char elementos[10][40];

/* la funcion separa consiste en separar una cadena de texto muy larga
  en cadenas mas chicas, mediante los espacios logramos hacer
  esa separacion, como si fuera por palabras.

  se obtiene la separacion con la funcion strtok y se guarda ese apuntador
  que nos retorno la funcion en otro apuntador, para poder manejarlo mas facil
*/
void separa(char *cad, char delim){
	char *comando=strtok(cad," ");
	int i=0;
	while(comando){
	   strcpy(elementos[i],comando);
	   comando=strtok(NULL," ");
	   i++;
	}
}

void dir(char *args){
     DIR *pDir;
     struct dirent *pDirent;
     pDir = opendir(valores_vars_ambiente[PWD]);
  if(args!=NULL){
     if(pDir==NULL){
	printf("Error:Unable to open directory.\n");
     }
     while( (pDirent = readdir(pDir)) != NULL){
       printf("[%s]\n",pDirent->d_name);
     }
     closedir(pDir);
  }
}

/*En esta funcion hacemos la llamada a clear del sistema, para poder limpiar la pantalla */
void clr(void){
    system("clear");
}

/*Esta funcion lo que hace es enlistar las variables de entorno
  como se estaran utilizando constantemente, se me hizo facil hacerlo asi
*/
void environ(void){
     printf("SHELL:%s\n",valores_vars_ambiente[SHELL]);
     printf("PATH:%s\n",valores_vars_ambiente[PATH]);
     printf("HOME:%s\n",valores_vars_ambiente[HOME]);
     printf("PWD:%s\n",valores_vars_ambiente[PWD]);
}
/*En la funcion cd, se estara actualizando la variable PWD
  que es el directorio donde estaremos moviendonos*/
void cd(char *args){
   /*corregir la forma de ingresar el comando con comillas a chdir */
     char tmp[150]=" \" ";
     printf("tmp:%s\n",tmp);
     strcat(tmp,args);
     printf("tmp:%s\n",tmp);
     strcat(tmp,"\"");
     printf("tmp:%s \n",tmp);
     if(chdir(tmp) != -1){
      getcwd(valores_vars_ambiente[PWD],sizeof(valores_vars_ambiente[PWD]));
     } else
      printf("No se pudo cambiar de directorio,flag:%d\n",chdir(tmp));

}
/*Esta funcion imprime el valor de la variable "PWD" en cualquier momento que se llame */
void pwd(void){
     printf("PWD:%s\n",valores_vars_ambiente[PWD]);
} /*En esta funcion lo que se hace es obtener por partes la cadena que se le pasa a echo
  para distinguir si hay algun $ en la cadena y asi imprimir dicho valor que se pida*/
void echo(char *args){
	puts(args);
}
void inicializa(char *nombreprograma);
void separa(char *cad, char delim);
void clr(void);
void environ(void);
void dir(char *args);
void cd(char *args);
void echo(char *args);
void pwd(void);

int main(int argc, char *argv[]) {
	int salir = FALSE, pid,i;
	char linea_comando[200];
	char comando[40],argumentos[160];

	/* Se manda llamar a inicializa con el nombre del programa
	  	que se esta ejecutando */
   	 inicializa(argv[0]);

	/* Se entra en un ciclo para esperar que el usuario
	   teclee un comando, el cual e	s guardado en linea_comando */
	do {
		/* Se muestra el indicador del sistema */
		printf("%s> ",valores_vars_ambiente[PWD]);
		fgets(linea_comando,sizeof(linea_comando),stdin);

		separa(linea_comando,' ');

		strcpy(comando,elementos[0]);

		i = 1;
		strcpy(argumentos,"");
		while(elementos[i][0]) {
			strcat(argumentos,elementos[i++]);
			strcat(argumentos," ");
 		}
		argumentos[strlen(argumentos)-1]=0;

		if (!strcmp(comando,"cd")==1) {
			cd(argumentos);
		}
		else if (!strcmp(comando,"dir")==1) {
			dir(argumentos);
		}
		else if (!strcmp(comando,"clr")==1) {
			clr();
		}
		else if (!strcmp(comando,"environ")==1) {
			environ();
		}
		else if (!strcmp(comando,"echo")==1) {
			echo(argumentos);
		}
		else if (!strcmp(comando,"pwd")==1) {
			pwd();
		}
		else if (!strcmp(comando,"quit")==1) {
			return 0;
		}
		else{
			/*
			   CODIGO PARA CREAR UN NUEVO PROCESO Y EJECUTAR EL COMANDO EXTERNO 
			   AL CUAL SE  LE DEBEN PASAR LOS ARGUMENTOS/PARAMETROS QUE
			   SE HAYAN TECLEADO POR PARTE DEL USUARIO 
			*/
		}
	} while (!salir);
	exit(0);
}
