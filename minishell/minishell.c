/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drm <drm@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/02 02:01:43 by drm               #+#    #+#             */
/*   Updated: 2023/07/05 01:57:47 by drm              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <errno.h>
#include <sys/wait.h>

bool echooption;

typedef struct env
{
	char *cmd;
	char *val;
	struct env *ptr;
} t_env;

int ft_checkeq(char *s)
{
	int i = 0;
	// while(i < sz)
	while(s[i])
	{
		if(s[i] == '=')
			return i;
		i++;
	}
	return -1;
}

void populate(char *s1, char *s2, char *s)
{
	int i,j;

	i = 0;
	j = 0;
	while(s[i]!='=')
	{
		s1[j]=s[i];
		j++;
		i++;
	}
	s1[j]='\0';
	i++;
	j=0;
	while(s[i])
	{
		s2[j]=s[i];
		i++;
		j++;
	}
	s2[j]='\0';
}

size_t	ft_strlen(const char *str)
{
	size_t	i;

	i = 0;
	while (str[i] != '\0')
	{
		i++;
	}
	return (i);
}

t_env* firstentryll(char *s)
{
	// int sz = ft_strlen(s);
	int equalpos = ft_checkeq(s);
	char *s1 = malloc(sizeof(char)*(equalpos+1));
	char *s2 = malloc(sizeof(char)*(ft_strlen(s)-equalpos));
	populate(s1,s2,s);
	t_env *first;
	first = malloc(sizeof(t_env));
	first->ptr=NULL;
	first->cmd=s1;
	first->val=s2;
	// printf("\nfirst command is %s\n",first->cmd);
	// printf("\nSec command is %s\n",first->val);
	return first;
}

t_env* laterentryll(char *s, t_env *envvrf)
{
	int equalpos = ft_checkeq(s);
	char *s1 = malloc(sizeof(char)*(equalpos+1));
	char *s2 = malloc(sizeof(char)*(ft_strlen(s)-equalpos));
	populate(s1,s2,s);
	t_env *later;
	later = malloc(sizeof(t_env));
	later->cmd=s1;
	later->val=s2;
	envvrf->ptr = later;
	later->ptr=NULL;
	return later;
}

t_env *converttoll(char **agv, char**env)
{
	int i = 0;
	t_env *envvrf, *t_envvrs, *first;
	// printf("%s\n",env[i]);
	int sz = ft_strlen(env[i]);
	int equalpos = ft_checkeq(env[i]);
	int j = 0;
	// printf("sz is %d, and equalpos is %d\n",sz,equalpos);
	// printf("and the last val is %c",env[i][sz+3]);

	while (env[i])
	{
		if (i == 0)
		{
			// printf("hi\n");
			envvrf = firstentryll(env[i]);
			first=envvrf;
			// printf("cmd is %s\n",first->cmd);
		}
		else
		{
			t_envvrs = laterentryll(env[i],envvrf);
			envvrf = t_envvrs;
			// printf("cmd is %s\n",envvrf->cmd);
		}
		i++;
	// 	printf("%s\n",env[i++]);
	}
	// printf("cmd is %s, and its value is %s\n",first->ptr->cmd,first->ptr->val);
	// printf("and the ori val is %s\n",env[1]);
	return first;
}
//pipex functions start

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*str;
	int		i;
	int		l;
	int		r;
	int		len;

	l = 0;
	r = 0;
	i = 0;
	if (!s1 || !s2)
		return (NULL);
	len = ft_strlen(s1) + ft_strlen(s2);
	str = malloc(len + 1);
	if (!str)
		return (NULL);
	while (s1[l] != '\0')
		str[i++] = s1[l++];
	while (s2[r] != '\0')
		str[i++] = s2[r++];
	str[i] = '\0';
	return (str);
}

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	char	*d;
	char	*s;

	d = (char *)dest;
	s = (char *)src;
	if (!s && !d)
		return (dest);
	while (n > 0)
	{
		*d = *s;
		d++;
		s++;
		n--;
	}
	return ((void *)dest);
}

char	*ft_strdup(const char *s)
{
	char	*s1;
	int		i;

	i = ft_strlen(s);
	s1 = malloc(i + 1);
	if (!s1)
		return (NULL);
	ft_memcpy(s1, s, i);
	s1[i] = '\0';
	return (s1);
}

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
	ptr = malloc (sizeof(char) * (i + 1));//
	if (!ptr)
		return (NULL);
	i = 0;
	while (s[i] && s[i] != c)
	{
		ptr[i] = s[i];
		i++;
	}
	// ptr[i] = '/';
	// ptr[i+1] = '\0';
	ptr[i]='\0';
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

int ft_size(char **s)
{
	int i = -1;
	while(s[++i]) ;
	return i;
}

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	int				i;
	unsigned char	p1;
	unsigned char	p2;

	i = 0;
	while (n > 0)
	{
		p1 = (unsigned char)(s1[i]);
		p2 = (unsigned char)(s2[i]);
		if (!s1[i] && !s2[i])
			break ;
		if (p1 != p2)
		{
			return (p1 - p2);
		}
		i++;
		n--;
	}
	return (0);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*str;
	size_t	i;

	if (!s)
		return (NULL);
	if (start >= ft_strlen(s))
	{
		str = ft_strdup ("");
		return (str);
	}
	if (len > ft_strlen(s) - start)
		len = ft_strlen(s) - start;
	str = malloc(len + 1);
	if (!str)
		return (NULL);
	i = 0;
	while (i < len)
	{
		str[i] = s[start + i];
		i++;
	}
	str[i] = '\0';
	return (str);
}

char *getpath(char **envp, char *s)
{
	int i = -1;
	char *pathname;
	while(envp[++i])
	{
		if (ft_strncmp(envp[i],s,4) == 0)
		{
			// char *pathname = (char *)malloc((ft_strlen(envp[i])-4)*sizeof(char));
			pathname = ft_substr(envp[i],5,ft_strlen(envp[i])-5);
		}
	}
	return pathname;
}

int	getwordspath(char *s, char c)
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

char	*memallocpath(char *s, char c)
{
	int		i;
	char	*ptr;

	i = 0;
	while (s[i] && s[i] != c)
		i++;
	ptr = malloc (sizeof(char) * (i + 2));//
	if (!ptr)
		return (NULL);
	i = 0;
	while (s[i] && s[i] != c)
	{
		ptr[i] = s[i];
		i++;
	}
	ptr[i] = '/';
	ptr[i+1] = '\0';
	// ptr[i]='\0';
	return (ptr);
}

void	fnpath(char *s, char c, char **str)
{
	int	i;

	i = 0;
	while (*s)
	{
		while (*s && *s == c)
			s++;
		while (*s && *s != c)
		{
			str[i] = memallocpath(s, c);
			i++;
			while (*s && *s != c)
				s++;
		}
	}
	str[i] = NULL;
}

char	**ft_splitpath(char const *p, char c)
{
	char	**str;
	int		i;
	int		word;
	char	*s;

	i = 0;
	s = (char *)p;
	if (!s)
		return (NULL);
	word = getwordspath(s, c);
	str = (char **)malloc (sizeof(char *) * (word + 1));
	if (!str)
		return (NULL);
	fnpath(s, c, str);
	return (str);
}

int iposition(char **cmd)
{
	int sz = ft_size(cmd);
	int i = 1;
	while(i<sz)
	{
		if(ft_strncmp(cmd[i],"-n",ft_strlen(cmd[i])) == 0)
			i++;
		else
			break;
	}
	return i;
}

int optioncheck(char **cmd)
{
	int sz;
	sz = ft_size(cmd);
	if(sz > 1)
	// printf("welcome to option check for echo command\n");
	{
		if(ft_strncmp(cmd[1],"-n",ft_strlen(cmd[1])) == 0)
		{
			echooption = true;
			return 1;
		}
	}
	return 0;//if not an option
}

int ft_echo(char **cmd)
{
	int i =1, sz;
	int npresence=0;

	sz = ft_size(cmd);
	if(optioncheck(cmd))
		// printf("-n option enabled\n");
		// while(ft_strncmp(cmd[i],"-n",ft_strlen(cmd[i])) == 0 && i < sz)
		// 	i++;
		i = iposition(cmd);
		// printf("i val is %d\n",i);
		// i = 2;
	// printf("welcm to echo!! nad -n is %d\n",i);
	while (cmd[i])
	{
		printf("%s",cmd[i]);
		if (cmd[i+1])
			printf(" ");
		i++;
	}
	if(!echooption)
		printf("\n");
	return 1;	
}

int ft_getpwd()
{
	// printf("getting pwd\n");
	char *pwd = getcwd(NULL,0);
	printf("is the path %s ?\n",pwd);
	free(pwd);
	// printf("exiting pwd\n");
	return 1;
}

int ft_exit()
{
	exit (0);
	return 1;
}

int checkforbuiltin(char **cmd,char **envp)
{
	int i = 0;
	// printf("in builtin checker\n");
	if(ft_strncmp(cmd[0],"pwd",ft_strlen(cmd[0]))==0)
		i = ft_getpwd();
	else if(ft_strncmp(cmd[0],"echo",ft_strlen(cmd[0])) == 0)
		i = ft_echo(cmd);
	else if(ft_strncmp(cmd[0],"exit",ft_strlen(cmd[0])) == 0)
		i = ft_exit();
	return i;
}

int execution(char **envp, char **cmd)
{
    // printf("hello from execution\n");
    int i = -1;
    char *pathvar = getpath(envp, "PATH");
    char **paths = ft_splitpath(pathvar,':');
    // printf("%s\n",paths[0]);
    // printf("in execution\n");
	if(checkforbuiltin(cmd,envp))
		return 0;
    while(paths[++i])
    {
        char *which = ft_strjoin(paths[i],cmd[0]);
        // printf("%s\n",which);
        // if(execve(which,cmd,envp)==-1)
		// printf("executing\n");
		execve(which,cmd,envp);
            // printf("No cmd for %s\n",which);
        // printf("no execve comm found\n");
    }
    printf("command not found\n");
    return 0;
}

int ft_pipecnt(char *s)
{
    int i=0;
    int cnt=0;
    while(s[i])
    {
        if(s[i]=='|')
            cnt++;
        i++;
    }
    return cnt;
}

int pipex(char *s, char **envp)
{
	int i = 0;
	char **terms;
	int sz,pipecnt;
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
	sz = ft_size(terms);
    pipecnt = ft_pipecnt(s);
	// printf("%d is the number of terms\n",sz);
	// printf("%d is the number of pipes\n",pipecnt);
    int fd[pipecnt][2];
    while(i<pipecnt)
    {
        if(pipe(fd[i])==-1)
            printf("cant open pipe with desired fd\n");
        int id = fork();
        if(id == 0)
        {
            close(fd[i][0]);
            dup2(fd[i][1],1);
					// close(fd[i][1]);
			if (execution(envp,ft_split(terms[i],' '))==0)
				printf("could not execute\n");
        }
        else
        {
            wait(NULL);
            close(fd[i][1]);
            dup2(fd[i][0],0);
			// close(fd[i][0]);
        }
        i++; 
    }
	// printf("seg2\n");
	int id1=fork();
	if(id1==0)
		execution(envp,ft_split(terms[i],' '));
	else
		waitpid(id1,NULL,0);
	// printf("seg1\n");
	return 0;
}
//pipex functions end
int main(int ac, char **agv, char **env)
{
	char *s;
	int errorcode;
	errorcode = errno;
	// printf("\n\n%d\n\n",errorcode);
	// int i = 0;
	// t_env *envvrf, *t_envvrs, *first;
	// // printf("%s\n",env[i]);
	// int sz = ft_strlen(env[i]);
	// int equalpos = ft_checkeq(env[i]);
	// int j = 0;
	// // printf("sz is %d, and equalpos is %d\n",sz,equalpos);
	// // printf("and the last val is %c",env[i][sz+3]);

	// while (env[i])
	// {
	// 	if (i == 0)
	// 	{
	// 		// printf("hi\n");
	// 		envvrf = firstentryll(env[i]);
	// 		first=envvrf;
	// 		// printf("cmd is %s\n",first->cmd);
	// 	}
	// 	else
	// 	{
	// 		t_envvrs = laterentryll(env[i],envvrf);
	// 		envvrf = t_envvrs;
	// 		// printf("cmd is %s\n",envvrf->cmd);
	// 	}
	// 	i++;
	// // 	printf("%s\n",env[i++]);
	// }
	// printf("cmd is %s, and its value is %s\n",first->ptr->cmd,first->ptr->val);
	// printf("and the ori val is %s\n",env[1]);
	while(1)
	{
		echooption = false;
		t_env *llforenv = converttoll(agv,env);
		// printf("No new line? lets check:");
		// printf("ll conversion success\n");
		rl_on_new_line();
		s = readline("myshell> ");
		// printf("readline success\n");
		add_history(s);
		// printf("history added\n");
		pipex(s,env);
		// printf("pipex success\n");
		free(s);
		// printf("free s\n");
	}
}
