// ConsoleApplication2.cpp : Defines the entry point for the console application.

#include "stdio.h"
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

#define MAX_COUTER 1000
#define MAX_NUMBER_ARG 100
#define BIN_SIZE 6 // size of "/bin/" +1 EOF


typedef char** string_list; // só para não fazer ponteiro de ponteiro de ponteiro.
typedef  enum error_shell{SUCESSO, SAIR} error_shell;


error_shell getstring(char** out_comando, string_list* out_Arguments, char** out_path);
void signal_handler (int sigNumber);

void signal_handler (int sigNumber){
	if (sigNumber == SIGUSR1)
	printf("Recebi um SIGUSR1");
}



int main()
{
	signal(SIGUSR1, signal_handler);
	//char *comando = (char*)malloc(sizeof(5));
	char *comando = NULL;
	char *caminho_saida = NULL;
	int interador = 0;
	string_list argumentos;
	FILE* arquivo = NULL;
	while (1)
	{
		printf("Digite o nome do comando e os argumentos:\n");
		if (getstring(&comando,&argumentos, &caminho_saida) == SAIR){
			break;
		};
		printf("\n# Iniciando Processo \n");

	 	
		int filho_pid = fork();
		if (filho_pid == 0) {
			if(caminho_saida != NULL){
				arquivo = fopen(caminho_saida, "w");
				dup2(fileno(arquivo), fileno(stdout));
			 }			
			execvp(comando,argumentos);
			if(arquivo != NULL) fclose(arquivo);
			return SUCESSO;
		} else {
			wait(NULL);
		}
			
		printf("# Processo Finalizado\n\n");
		// Area de limpesa de Alocações;		
		free(comando);
		comando=NULL; 
		free(argumentos);
		if(caminho_saida != NULL){
			free(caminho_saida);
			caminho_saida = NULL;
		}
	}
	
	printf("Finalizando Shell ...\n\n");
	 // string = (char*)malloc(sizeof(MAX_COUTER));
	// fgets(comando, MAX_COUTER, stdin);

	
	return 0;
}

//Código de aquisição de comando e argumentos com tratamento para 
error_shell getstring(char** out_comando, string_list* out_Arguments, char** out_path){
	char inString[MAX_COUTER];
	char* inArguments;
	int counter = 0;
	
	fgets(inString, MAX_COUTER, stdin);

	inArguments = strtok(inString, " \n");
	if(strcmp(inArguments,"sair")==0){
		return SAIR;
	}
	
	*out_comando = (char*)malloc( strlen(inArguments) + BIN_SIZE);
	//realloc(out_comando, strlen(inArguments) + BIN_SIZE);
	strcpy(*out_comando, "/bin/");
	strcat(*out_comando, inArguments);
	//printf("%s\n", out_comando);
	*out_Arguments = (char**)malloc(MAX_NUMBER_ARG);	
	while (inArguments != NULL)
	{
		(*out_Arguments)[counter] = (char*)malloc(strlen(inArguments)+1); //aloca espaço para string +1 para EOF
		strcpy((*out_Arguments)[counter],inArguments);
		counter++;
		inArguments = strtok(NULL, " \n");
		
		// area para receber path de saida
		if(inArguments!=NULL){  // evita problema de comparação
			if(strcmp(inArguments,">>") == 0 ){
				inArguments = strtok(NULL, " \n");
				*out_path = (char*)malloc(strlen(inArguments)+1);
				strcpy(*out_path,inArguments);
				inArguments = (char*)NULL;
			}
		}
		printf(" -- end \n");
	}
	(*out_Arguments)[counter] = inArguments;

	return SUCESSO; 
}