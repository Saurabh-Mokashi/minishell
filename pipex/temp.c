#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <signal.h>
#include <errno.h>
#include <sys/wait.h>

typedef struct env
{
	char *cmd;
	char *val;
	struct env *next;
} t_env;

typedef struct ptr
{
	struct env* env;
	struct env* export;
} t_ptr;

bool echooption;

void	ft_putstr_fd(char *s, int fd)
{
	int		i;

	i = 0;
	if (!s)
		return ;
	while (s[i])
	{
		write(fd, &s[i], 1);
		i++;
	}
}

// int execution(char **envp, char **cmd,t_ptr *ptr, int *fd)
// {
//     printf("hello from execution\n");
//     int i = -1;
//     char *pathvar = getpath(envp, "PATH");
//     char **paths = ft_splitpath(pathvar,':');
//     // printf("%s\n",paths[0]);
// 	if(checkforbuiltin(cmd,ptr,fd))
// 		return 0;
//     // printf("in execution\n");
//     while(paths[++i])
//     {
//         char *which = ft_strjoin(paths[i],cmd[0]);
//         // printf("%s\n",which);
//         // if(execve(which,cmd,envp)==-1)
// 		// printf("executing\n");
// 		execve(which,cmd,envp);
//             // printf("No cmd for %s\n",which);
//         // printf("no execve comm found\n");
//     }
//     printf("command not found\n");
//     return 0;
// }


int pipex(char *s, char *p, char **envp)
{
    int stdin_copy = dup(STDIN_FILENO);
	int stdout_copy = dup(STDOUT_FILENO);
    int fd[1][2];
    int i = 0;
    while(i<1)
    {
        if(pipe(fd[i])==-1)
            printf("cant open pipe with desired fd\n");
        int id = fork();
        if(id == 0)
        {
            close(fd[i][0]);
            // ft_putstr_fd("hello, just closed fd[i][0], printing s\n",STDOUT_FILENO);
            // ft_putstr_fd(s,STDOUT_FILENO);
            dup2(fd[i][1],1);
			// printf("in id == 0\n");
			close(fd[i][1]);
			// if (execution(envp,ft_split(terms[i],' '),ptr, (int *)fd[i])==0)
			// 	printf("could not execute\n");
            // ft_putstr_fd(s,fd[0][1]);
            // ft_putstr_fd(s,STDOUT_FILENO);
            execve("/bin/pwd",&s,envp);
        }
        else
        {
			// printf("in parent\n");
            waitpid(id,NULL,0);
            close(fd[i][1]);
            dup2(fd[i][0],0);
			// close(fd[i][0]);
        }
        i++; 
    }
    int id1=fork();
	if(id1==0)
	{
        ft_putstr_fd(p,STDOUT_FILENO);
		printf("in id1\n");
		// execution(envp,ft_split(terms[i],' '),ptr, (int *)fd[i-1]);

	}
	else
		waitpid(id1,NULL,0);
    dup2(stdin_copy, STDIN_FILENO);
	dup2(stdout_copy, STDOUT_FILENO);
    return 0;

}
//pipex functions end

// int main(int ac, char **agv, char **env)
// {
// 	char *s="pwd";
// 	int errorcode;
// 	errorcode = errno;
// 	echooption = false;
//     printf("hello wor\n");
// 		// pipex("pwd","wc",env);
//     execve("/bin/pwd",&s,env);
//     printf("hello world\n");
// 		// printf("pipex success\n");
// 		// printf("pipex over");
// 		// free(s);
// 		// printf("free s\n");
// 	// }
// }
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    int pipefd[2];
    pid_t pid;
    
    // Create a pipe
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // Fork the first child (for the first command)
    pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // Child process - first command (e.g., "ls")
        // Close the read end of the pipe
        close(pipefd[0]);

        // Redirect stdout to the pipe
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);

        // Execute the first command (e.g., "ls")
        // char *cmd1[] = { "pwd", NULL };
        // execvp(cmd1[0], cmd1);
        // ft_putstr_fd("hello my name is",pipefd[1]);
        printf("helloo");
        perror("execvp");
        exit(EXIT_FAILURE);
    } else {
        // Parent process
        // Fork the second child (for the second command)
        pid_t pid2 = fork();
        if (pid2 == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (pid2 == 0) {
            // Child process - second command (e.g., "wc")
            // Close the write end of the pipe
            close(pipefd[1]);

            // Redirect stdin to the pipe
            dup2(pipefd[0], STDIN_FILENO);
            close(pipefd[0]);

            // Execute the second command (e.g., "wc")
            char *cmd2[] = { "wc", NULL };
            execvp(cmd2[0], cmd2);
            perror("execvp");
            exit(EXIT_FAILURE);
        } else {
            // Parent process
            // Close both ends of the pipe in the parent
            close(pipefd[0]);
            close(pipefd[1]);

            // Wait for both children to complete
            wait(NULL);
            wait(NULL);
        }
    }

    return 0;
}

