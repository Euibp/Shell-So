#include "stdio.h"
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

#define MAX_LINE_CHARACTERS 3000
#define MAX_NUMBER_ARG 100
#define MAX_SIZE_PID 100 // 100 characteres
#define BIN_SIZE 6 // size of "/bin/" +1 EOF


typedef char** string_list; // só para não fazer ponteiro de ponteiro de ponteiro.
typedef  enum error_shell{SUCESSO, SAIR, AJUDA_SHELL} error_shell;

int global_filho_pid;

error_shell getstring(char** out_comando, string_list* out_Arguments, char** out_path);
void signal_handler (int sigNumber);
void printhelp();

void signal_handler (int sigNumber){
	char string_pid[MAX_SIZE_PID];	
	sprintf(string_pid,"%i",global_filho_pid);
	if (sigNumber == SIGUSR1){
	 printf("\n#Processo Eliminado PID:%s\n",string_pid);	
	 kill(global_filho_pid,SIGKILL);
	}
}



int main()
{
	//char *comando = (char*)malloc(sizeof(5));
	char *comando = NULL;
	char *caminho_saida = NULL;
	int interador = 0;
	string_list argumentos;
	FILE* arquivo = NULL;
	
	signal(SIGUSR1, signal_handler);
	printhelp();

	while (1)
	{
		printf("Digite o nome do comando e os argumentos:\n");
		switch (getstring(&comando,&argumentos, &caminho_saida)){
			case SAIR:
				printf("Finalizando Shell ...\n\n");
				return SUCESSO;
			case AJUDA_SHELL:
				printhelp();
				continue;
				break;
			defult:
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
			global_filho_pid = filho_pid;
			printf("%i\n",filho_pid);
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
	
	return 0;
}

void printhelp(){
	printf("\n#  Este programa simula uma shell com tratamento de SIGURS1\n");
	printf("#  Para executar um comando você deve digitar todos os argumentos em uma unica linha e caso queira salvar as informacoes em arquivo adicione '>>' (sem aspas) e em seguida o nome e caminho do arquivo\n");
	printf("#  Exemplo:ping 8.8.8.8 -c 10 >> pingfile.out\n");
	printf("#  As limitações do programa são entrada %i characteres para linhas de comando e até %i argumentos \n", MAX_LINE_CHARACTERS, MAX_NUMBER_ARG );
	printf("Os comandos internos são\n    -sair  |encerra o shell\n    -ajuda |exibe as informações acima\n\n");
}

//Código de aquisição de comando e argumentos com tratamento para 
error_shell getstring(char** out_comando, string_list* out_Arguments, char** out_path){
	char inString[MAX_LINE_CHARACTERS];
	char* inArguments;
	int counter = 0;
	
	fgets(inString, MAX_LINE_CHARACTERS, stdin);

	inArguments = strtok(inString, " \n");
	if(strcmp(inArguments,"-sair")==0){
		return SAIR;
	}
	if(strcmp(inArguments,"-ajuda")==0){
		return AJUDA_SHELL;
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
	}
	(*out_Arguments)[counter] = inArguments;

	return SUCESSO; 
}