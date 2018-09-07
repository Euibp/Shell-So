// ConsoleApplication2.cpp : Defines the entry point for the console application.

#include "stdio.h"
#include <string.h>
#include <stdlib.h>

#define MAX_COUTER 1000
#define MAX_NUMBER_ARG 100
#define BIN_SIZE 6 // size of "/bin/" +1 EOF

typedef char** string_list;

int getstring(char** out_comando, string_list* out_Arguments, char** out_path);

// void signal_handler (int sigNumber){
	// if (sigNumber == SIGTERM)
	// printf(“Recebi um SIGTERM”);
// }



int main()
{
	//char *comando = (char*)malloc(sizeof(5));
	char *comando = NULL;
	char *caminho_saida = NULL;
	int interador;
	string_list argumentos;
	FILE* arquivo;
	while (1)
	{
		printf("Digite os argumentos o nome da comando e os argumentos:\n");
		if (getstring(&comando,&argumentos, &caminho_saida) == 1){
			break;
		};
		printf("Voce escreveu %i letras: %s \n", strlen(comando), comando);

		/*
		int child_pid = fork();
		if (child_pid == 0) {
			execvp(comando,argumentos);
		} else {
			wait(NULL);
			return 0;
		}	
		
		*/
			 if(caminho_saida != NULL){
					// arquivo = fopen(caminho_saida, "w");
					// dup2(fileno(arquivo), fileno(stdout));
					free(caminho_saida);
					caminho_saida = NULL;
					printf("MOL %d \n", (caminho_saida==NULL));

			 }
		
			while(argumentos[interador] != NULL){
			printf("%d -- %s\n", &argumentos[interador],argumentos[interador]);
			free(argumentos[interador]);
			interador++;
		}interador=0;
		
		printf("isso ai\n\n");
		free(comando);
		comando=NULL; 
	}
	
	printf("Ending\n\n");
	 // string = (char*)malloc(sizeof(MAX_COUTER));
	// fgets(comando, MAX_COUTER, stdin);

	
	return 0;
}

//Código de aquisição de comando e argumentos com tratamento para 
int getstring(char** out_comando, string_list* out_Arguments, char** out_path){
	char inString[MAX_COUTER];
	char* inArguments;
	char* token;
	int counter = 0;
	
	fgets(inString, MAX_COUTER, stdin);
	char* listArguments[MAX_NUMBER_ARG];

	inArguments = strtok_s(inString, " \n", &token);
	if(strcmp(inArguments,"sair")==0){
		return 1;
	}
	
	*out_comando = (char*)malloc( strlen(inArguments) + BIN_SIZE);
	//realloc(out_comando, strlen(inArguments) + BIN_SIZE);
	strcpy_s(*out_comando, BIN_SIZE, "/bin/");
	strcat_s(*out_comando, strlen(inArguments) + BIN_SIZE, inArguments);
	//printf("%s\n", out_comando);	
	while (inArguments != NULL)
	{
		listArguments[counter] = (char*)malloc(strlen(inArguments)+1); //aloca espaço para string +1 para EOF
		strcpy_s(listArguments[counter],strlen(inArguments)+1,inArguments);
		printf("%d -- %s", &listArguments[counter],listArguments[counter]);
		counter++;
		inArguments = strtok_s(NULL, " \n", &token);
		
		// area para receber path de saida
		if(inArguments!=NULL){  // evita problema de comparação
			if(strcmp(inArguments,">>") == 0 ){
				inArguments = strtok_s(NULL, " \n", &token);
				*out_path = (char*)malloc(strlen(inArguments)+1);
				
				//realloc(out_path,strlen(inArguments)+1);
				strcpy_s(*out_path,strlen(inArguments)+1,inArguments);
				//printf("%s", out_path );
				inArguments = (char*)NULL;
			}
		}
		printf(" -- end \n");
	}
	listArguments[counter] = inArguments;
	*out_Arguments = listArguments;	
	//printf("%s\n", out_comando);	
	return 0; 
}