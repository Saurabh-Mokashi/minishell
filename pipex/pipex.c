/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smokashi <smokashi@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 21:45:16 by smokashi          #+#    #+#             */
/*   Updated: 2023/06/26 11:35:33 by smokashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

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

int	getwords1(char *s, char c)
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

char	*memalloc1(char *s, char c)
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

void	fn1(char *s, char c, char **str)
{
	int	i;

	i = 0;
	while (*s)
	{
		while (*s && *s == c)
			s++;
		while (*s && *s != c)
		{
			str[i] = memalloc1(s, c);
			i++;
			while (*s && *s != c)
				s++;
		}
	}
	str[i] = NULL;
}

char	**ft_split1(char const *p, char c)
{
	char	**str;
	int		i;
	int		word;
	char	*s;

	i = 0;
	s = (char *)p;
	if (!s)
		return (NULL);
	word = getwords1(s, c);
	str = (char **)malloc (sizeof(char *) * (word + 1));
	if (!str)
		return (NULL);
	fn1(s, c, str);
	return (str);
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

int execution(char **envp, char **cmd)
{
	int i = -1;
	char *pathvar = getpath(envp, "PATH");
	char **paths = ft_split1(pathvar,':');
	// printf("%s\n",paths[0]);
	while(paths[++i])
	{
		char *which = ft_strjoin(paths[i],cmd[0]);
		// printf("%s\n",which);
		if(execve(which,cmd,envp)==-1)
			printf("No cmd for %s",which);
	}
	printf("command not found");
	return 0;
}

void print_doubchar(char **s)
{
	int i=0,j=0;
	while(s[i])
	{
		printf("%s\n",s[i]);
		i++;
	}
	printf("thts it\n");
}
int carryitforward(char **terms,char **envp)
{
	int fd[2];//for pipes;
	int status;
	if (pipe(fd) == -1)
	{
		printf("cant open pipe\n");
		return 9;
	}
	int id = fork();
	if(id == 0)
	{
		close(fd[0]);
		// char *cmd = terms[0];
		dup2(fd[1],1);
		// execve("/bin/ls", ft_split(terms[0],' '), envp);
		execution(envp, ft_split(terms[0],' '));
		// printf("%s\n",ft_split(terms[0]));
		// print_doubchar(ft_split(terms[0],' '));
		// printf("%s\n",terms[0]);
		close(fd[1]);
	}
	else
	{
		// waitpid(id, &status, 0);
		wait(NULL);
		close(fd[1]);
		//read from fd, and display
		// char *cmd = terms[1];
		dup2(fd[0],0);
		// execve("/usr/bin/wc", ft_split(terms[1],' '), envp);
		execution(envp, ft_split(terms[1],' '));
		// print_doubchar(ft_split(terms[1],' '));
		// read(fd[0]);
		// execve();
		// write();
		// printf("%s\n",terms[1]);
		close(fd[0]);
		// char *cmd=terms[1];
	}
	return 0;
}

int ft_size(char **s)
{
	int i = -1;
	while(s[++i]) ;
	return i;
}

int main(int ac, char **agv, char **envp)
{
	int i = 0;
	char **terms;
	int sz;
	char s[50] = "ls -la | wc -l";
	char sep[2] = "|";
	int fd[2];
	char *pathfrmenv;
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
	// printf("\n%d\n",sz);
	// printf("%s",terms[0]);//successfuly got the commands seperated
	if(carryitforward(terms,envp)==0)
		printf("success!!\n");
	// int x = execve("/bin/ls", ft_split(terms[0],' '),envp);
	// printf("\n%d\n",x);
	return (0);
}
