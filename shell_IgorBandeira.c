#include "shell_IgorBandeira.h"

int main()
{
	//char *comando = (char*)malloc(sizeof(5));
	char *comando = NULL;
	char *caminho_saida = NULL;
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
			case SUCESSO:
				break;
			
		};
		printf("\n# Iniciando Processo ");

	 	
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
			printf("PID: %i\n",filho_pid);
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

void signal_handler (int sigNumber){
	char string_pid[MAX_SIZE_PID];	
	sprintf(string_pid,"%i",global_filho_pid);
	if (sigNumber == SIGUSR1){
	 printf("\n#Processo Eliminado PID:%s\n",string_pid);	
	 kill(global_filho_pid,SIGKILL);
	}
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