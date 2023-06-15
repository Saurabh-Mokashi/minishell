#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int	getwords(char *s, char c)
{
	int	i;
	int	word;
	int	flag;

	i = 0;
	word = 0;
	flag = 0;
	while (s[i])
	{
		while (s[i] == c)
			i++;
		if (!s[i])
			flag = 1;
		while (s[i] != c && s[i])
		{
			i++;
		}
		if (flag == 1)
			break ;
		word++;
	}
	return (word);
}

char	*memalloc(char *s, char c)
{
	int		i;
	char	*ptr;

	i = 0;
	while (s[i] && s[i] != c)
		i++;
	ptr = malloc (sizeof(char) * (i + 1));
	if (!ptr)
		return (NULL);
	i = 0;
	while (s[i] && s[i] != c)
	{
		ptr[i] = s[i];
		i++;
	}
	ptr[i] = '\0';
	return (ptr);
}

void	fn(char *s, char c, char **str)
{
	int	i;

	i = 0;
	while (*s)
	{
		while (*s && *s == c)
			s++;
		while (*s && *s != c)
		{
			str[i] = memalloc(s, c);
			i++;
			while (*s && *s != c)
				s++;
		}
	}
	str[i] = NULL;
}

char	**ft_split(char const *p, char c)
{
	char	**str;
	int		i;
	int		word;
	char	*s;

	i = 0;
	s = (char *)p;
	if (!s)
		return (NULL);
	word = getwords(s, c);
	str = (char **)malloc (sizeof(char *) * (word + 1));
	if (!str)
		return (NULL);
	fn(s, c, str);
	return (str);
}

int carryitforward(char **terms,char **envp)
{
	int fd[2];//for pipes;
	if (pipe(fd) == -1)
	{
		printf("cant open pipe\n");
		return 9;
	}
	int id = fork();
	if(id == 0)
	{
		close(fd[0]);
		char *cmd = terms[0];
		dup2(fd[1],1);
		execve("/bin/ls", ft_split(cmd,' '), envp);
		close(fd[1]);
	}
	else
	{
		wait(NULL);
		close(fd[1]);
		//read from fd, and display
		read(fd[0]);
		execve();
		write();
		close(fd[0]);
		char *cmd=terms[1];
	}
	return 0;
}

int main(int ac, char **agv, char **envp)
{
	int i = 0;
	char **terms;
	char s[50] = "ls -la | wc -l";
	char sep[2] = "|";
	int fd[2];
	char *token;
	// strcpy(terms[i],strtok(s,sep));
	// token = strtok(s,sep);
	// // i++;
	// while (token!=NULL)
	// {
	// 	strcpy(terms[i],token);
	// 	token = strtok(NULL,sep);
	// 	i++;
	// }
	terms = ft_split(s,'|');
	printf("%s",terms[1]);//successfuly got the commands seperated
	if(carryitforward(terms,envp)==0)
		printf("success!!\n");

	return (0);
}