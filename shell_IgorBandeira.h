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
typedef  enum error_shell{SUCESSO, SAIR, AJUDA_SHELL} error_shell; // enum de errors

int global_filho_pid;

error_shell getstring(char** out_comando, string_list* out_Arguments, char** out_path);
void signal_handler (int sigNumber);
void printhelp();