/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex-multi.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smokashi <smokashi@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 21:37:28 by smokashi          #+#    #+#             */
/*   Updated: 2023/06/21 21:37:28 by smokashi         ###   ########.fr       */
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

// int carryitforward(char **terms,char **envp)
// {
// 	int fd[2];//for pipes;
// 	int status;
// 	if (pipe(fd) == -1)
// 	{
// 		printf("cant open pipe\n");
// 		return 9;
// 	}
// 	int id = fork();
// 	if(id == 0)
// 	{
// 		close(fd[0]);
// 		dup2(fd[1],1);
// 		execution(envp, ft_split(terms[0],' '));
// 	}
// 	else
// 	{
// 		wait(NULL);
// 		close(fd[1]);
// 		dup2(fd[0],0);
// 		execution(envp, ft_split(terms[1],' '));
// 		close(fd[0]);
// 	}
// 	return 0;
// }

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

int execution(char **envp, char **cmd)
{
    // printf("hello from execution\n");
    int i = -1;
    char *pathvar = getpath(envp, "PATH");
    char **paths = ft_splitpath(pathvar,':');
    // printf("%s\n",paths[0]);
    // printf("in execution\n");
    while(paths[++i])
    {
        char *which = ft_strjoin(paths[i],cmd[0]);
        // printf("%s\n",which);
        if(execve(which,cmd,envp)==-1)
            printf("No cmd for %s",which);
        // printf("no execve comm found\n");
    }
    printf("command not found");
    return 0;
}

// void fnpercmd(char *cmd, char **envp, int i, int **fd,int sz)
// {
//     int id = fork();
//     if(id==0)
//     {
//         if (i==0)
//         {
//             printf("b4\n");
//             // if(close(fd[1][1]) == -1)
//             //     printf("couldnt close fd[1][1]");
//             int x = close(fd[1][1]);
//             printf("x val is %d\n",x);
//             printf("after\n");
//             if(close(fd[1][0])== -1)
//                 printf("couldnt close fd[1][0]");
//             if(close(fd[0][0])==-1)
//                 printf("couldnt close fd[0][0]");
//             printf("closed fd[1][1], fd[1][0], fd[0][0]\n");
//             if(dup2(fd[i][1],1)==-1)
//                 printf("error\n");
//             printf("before exec\n");
//             execution(envp,ft_split(cmd,' '));
//             printf("after exec\n");
//             close(fd[0][1]);//close(fd[i][1]);
//         }
//         else if(i == sz-1)
//         {
//             close(fd[0][1]);
//             close(fd[0][0]);
//             close(fd[1][1]);
//             printf("closed fd[0][1], fd[0][0], fd[1][1]\n");
//             if(dup2(fd[i-1][0],0)==-1)
//                 printf("error\n");
//             execution(envp,ft_split(cmd,' '));
//             printf("after exec\n");
//             close(fd[1][0]);
//         }
//         else
//         {
//             close(fd[0][1]);
//             close(fd[1][0]);
//             printf("closed fd[0][1] and fd[1][0]\n");
//             if(dup2(fd[i-1][0],0)==-1)
//                 printf("error\n");
//             if(dup2(fd[i][1],1)==-1)
//                 printf("error\n");
//             execution(envp,ft_split(cmd,' '));
//             printf("after exec\n");
//             close(fd[0][0]);//close(fd[i-1][0]);
//             close(fd[1][1]);//close(fd[i][1]);
//         }
//     }
//     else
//     {
//         wait(NULL);
//     }
// }

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

int main(int ac, char **agv, char **envp)
{
    int i = 0;
	char **terms;
	int sz,pipecnt;
	char s[50] = "ps -ef|grep root|wc -l";
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
	printf("%d is the number of terms\n",sz);
	printf("%d is the number of pipes\n",pipecnt);
    int fd[pipecnt][2];
    while(i<pipecnt)
    {
        if(pipe(fd[i])==-1)
            printf("cant open pipe with desired fd\n");
        i++; 
    }
    printf("pipes ready\n");
    i=0;
    int id1 = fork();
    if(id1 == 0)
    {
        close(fd[1][1]);
        close(fd[1][0]);
        close(fd[0][0]);
        dup2(fd[0][1],1);
        close(fd[0][1]);
        execution(envp,ft_split(terms[i],' '));
    }
    int id2 = fork();
    if(id2==0)
    {
        close(fd[0][1]);
        close(fd[1][0]);
        printf("closed fd[0][1] and fd[1][0]\n");
        dup2(fd[0][0],0);
        dup2(fd[1][1],1);
        close(fd[0][0]);//close(fd[i-1][0]);
        close(fd[1][1]);//close(fd[i][1]);
        execution(envp,ft_split(terms[1],' '));
        printf("after exec\n");
    }
    //parent process
    close(fd[0][1]);
    close(fd[0][0]);
    close(fd[1][1]);
    printf("closed fd[0][1], fd[0][0], fd[1][1]\n");
    dup2(fd[1][0],0);
    close(fd[1][0]);
    execution(envp,ft_split(terms[2],' '));
    printf("after exec\n");
    // waitpid(id1,NULL,0);
    // waitpid(id2,NULL,0);
    // while (i<sz)
    // {
    //     printf("hi\n");
    //     // fnpercmd(terms[i],envp,i,(int **)fd,sz);
    //     //below is copied from fnpercmd function
    // int id = fork();
    // if(id==0)
    // {
    //     if (i == 0)
    //     {
    //         printf("b4\n");
    //         // if(close(fd[1][1]) == -1)
    //         //     printf("couldnt close fd[1][1]");
    //         int x = close(fd[1][1]);
    //         printf("x val is %d\n",x);
    //         printf("after\n");
    //         if(close(fd[1][0])== -1)
    //             printf("couldnt close fd[1][0]");
    //         if(close(fd[0][0])==-1)
    //             printf("couldnt close fd[0][0]");
    //         printf("closed fd[1][1], fd[1][0], fd[0][0]\n");
    //         dup2(fd[0][1],1);
    //         printf("before exec\n");
    //         execution(envp,ft_split(terms[i],' '));
    //         printf("after exec\n");
    //         close(fd[0][1]);//close(fd[i][1]);
    //     }
    //     else if (i == 1)
    //     {
    //         close(fd[0][1]);
    //         close(fd[1][0]);
    //         printf("closed fd[0][1] and fd[1][0]\n");
    //         dup2(fd[0][0],0);
    //         dup2(fd[1][1],1);
    //         execution(envp,ft_split(terms[i],' '));
    //         printf("after exec\n");
    //         close(fd[0][0]);//close(fd[i-1][0]);
    //         close(fd[1][1]);//close(fd[i][1]);
    //     }
    //     else
    //     {
    //         close(fd[0][1]);
    //         close(fd[0][0]);
    //         close(fd[1][1]);
    //         printf("closed fd[0][1], fd[0][0], fd[1][1]\n");
    //         dup2(fd[1][0],0);
    //         execution(envp,ft_split(terms[i],' '));
    //         printf("after exec\n");
    //         close(fd[1][0]);
    //     }
    // }
    // else
    // {
    //     waitpid(id, NULL, 0);
    // }
    //     //above is copied from fnpercmd function
    //     i++;
    // }
	// printf("%s",terms[0]);//successfuly got the commands seperated
	// if(carryitforward(terms,envp)==0)
	// 	printf("success!!\n");
	// int x = execve("/bin/ls", ft_split(terms[0],' '),envp);
	// printf("\n%d\n",x);
	return (0);
}
