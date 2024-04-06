#include "exec.h"

// sets "key" with the key part of "arg"
// and null-terminates it
//
// Example:
//  - KEY=value
//  arg = ['K', 'E', 'Y', '=', 'v', 'a', 'l', 'u', 'e', '\0']
//  key = "KEY"
//
static void
get_environ_key(char *arg, char *key)
{
	int i;
	for (i = 0; arg[i] != '='; i++)
		key[i] = arg[i];

	key[i] = END_STRING;
}

// sets "value" with the value part of "arg"
// and null-terminates it
// "idx" should be the index in "arg" where "=" char
// resides
//
// Example:
//  - KEY=value
//  arg = ['K', 'E', 'Y', '=', 'v', 'a', 'l', 'u', 'e', '\0']
//  value = "value"
//
static void
get_environ_value(char *arg, char *value, int idx)
{
	size_t i, j;
	for (i = (idx + 1), j = 0; i < strlen(arg); i++, j++)
		value[j] = arg[i];

	value[j] = END_STRING;
}

// sets the environment variables received
// in the command line
//
// Hints:
// - use 'block_contains()' to
// 	get the index where the '=' is
// - 'get_environ_*()' can be useful here
static void
set_environ_vars(char **eargv, int eargc)
{
	// Your code here
}

// opens the file in which the stdin/stdout/stderr
// flow will be redirected, and returns
// the file descriptor
//
// Find out what permissions it needs.
// Does it have to be closed after the execve(2) call?
//
// Hints:
// - if O_CREAT is used, add S_IWUSR and S_IRUSR
// 	to make it a readable normal file
// O_CREAT -> si no existe el file path lo crea como file regular
// S_IRUSR -> el user tiene permisos de lectura
// S_IWUSR -> el user tiene permisos de escritura
static int
open_redir_fd(char *file, int flags)
{
	/*
	int fd;
	//O_CLOEXEC,
	fd = open(file, flags | O_CREAT | O_CLOEXEC , S_IWUSR | S_IRUSR);
	return fd;
	*/
	return open(file, flags | O_CREAT | O_CLOEXEC , S_IWUSR | S_IRUSR);
}

// executes a command - does not return
//
// Hint:
// - check how the 'cmd' structs are defined
// 	in types.h
// - casting could be a good option

/*
struct cmd {
	int type;
	pid_t pid; -> the process id
	char scmd[BUFLEN]; -> a string representing the command before being parsed
};

struct execcmd {
	int type;
	pid_t pid;
	char scmd[BUFLEN];
	int argc;
	int eargc;
	char *argv[MAXARGS];
	char *eargv[MAXARGS];
	char out_file[FNAMESIZE];
	char in_file[FNAMESIZE];
	char err_file[FNAMESIZE];
};

*/
void
exec_cmd(struct cmd *cmd)
{
	// To be used in the different cases
	struct execcmd *e;
	struct backcmd *b;
	struct execcmd *r;
	struct pipecmd *p;

	switch (cmd->type) {
	case EXEC: {
		e = cmd;
		execvp(e -> argv[0], e -> argv); //int execvp(const char *file, char *const argv[]);
		perror("execvp");
	}
	
	case BACK: {
		// runs a command in background
		//
		// Your code here
		printf("Background process are not yet implemented\n");
		_exit(-1);
		break;
	}

	case REDIR: {
		// changes the input/output/stderr flow
		//
		// To check if a redirection has to be performed
		// verify if file name's length (in the execcmd struct)
		// is greater than zero
		//
		// Your code here

		/*
		En pseudocodigo:
		verificar que se deba hacer un redirecionamiento
		nuveo_fd = open_redir_fd();
		revisar que no haya un error al abrir el nuevo fd
		dup2(nuevo_fd, std_in || std_out || std_err); <- El segundo parametro depende del caso
		*/

		r = cmd;
		
		// Caso de test para std_out
		if (strlen(r -> out_file) > 0) {
			int nuevo_out_fd = open_redir_fd(r -> out_file, 0); // [Gero] -> No tengo idea de donde tengo que sacar las flags, asi que a priori voy a fingir demencia y voy a decir que es 0
			if (nuevo_out_fd >= 0) {
				if (dup2(nuevo_out_fd, 1) < 0) { // reemplazar el 1 por STDOUT_FD o algo asi
					perror("Error al ejecutar dup2");
				}
			} else {
				perror("Error al ejecutar open_redir_fd");
			}
		}

		printf("Redirections are not yet implemented\n");
		_exit(-1);
		break;
	}

	case PIPE: {
		// pipes two commands
		//
		// Your code here
		printf("Pipes are not yet implemented\n");

		// free the memory allocated
		// for the pipe tree structure
		free_command(parsed_pipe);

		break;
	}
	}
}
