/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drm <drm@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/02 02:01:43 by drm               #+#    #+#             */
/*   Updated: 2023/07/17 01:48:36 by drm              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

bool echooption;

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

// size_t	ft_strlcpy(char *destination, const char *source, size_t n)
// {
// 	size_t			i;
// 	unsigned int	len;

// 	len = ft_strlen(source);
// 	if (!n)
// 		return (len);
// 	i = 0;
// 	while (source[i] && i < n - 1)
// 	{
// 		destination[i] = source[i];
// 		i++;
// 	}
// 	destination[i] = '\0';
// 	return (len);
// }

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

void populateforexport(char *s1,char *s2,char *s)
{
	// char *cpy;
	// cpy = (char *)malloc(11);
	ft_memcpy(s1,"declare -x ",11);
	int i,j;

	i = 0;
	j = 11;
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
//j=1 for env, j = 0 for export
t_env* firstentryll(char *s,int j)
{
	char *s1;
	char *s2;
	// int sz = ft_strlen(s);
	int equalpos = ft_checkeq(s);
	if(j == 0)
		s1 = malloc(sizeof(char)*(equalpos+1+11));
	else
		s1 = malloc(sizeof(char)*(equalpos+1));
	s2 = malloc(sizeof(char)*(ft_strlen(s)-equalpos));
	if(j==0)
		populateforexport(s1,s2,s);
	else
		populate(s1,s2,s);
	t_env *first;
	first = malloc(sizeof(t_env));
	first->next=NULL;
	first->cmd=s1;
	first->val=s2;
	// printf("\nfirst command is %s\n",first->cmd);
	// printf("\nSec command is %s\n",first->val);
	return first;
}

t_env* laterentryll(char *s, t_env *envvrf,int j)
{
	char *s1;
	char *s2;
	int equalpos = ft_checkeq(s);
	if(j==0)
		s1 = malloc(sizeof(char)*(equalpos+1+11));
	else
		s1 = malloc(sizeof(char)*(equalpos+1));
	s2 = malloc(sizeof(char)*(ft_strlen(s)-equalpos));
	if(j == 0)
		populateforexport(s1,s2,s);
	else
		populate(s1,s2,s);
	t_env *later;
	later = malloc(sizeof(t_env));
	later->cmd=s1;
	later->val=s2;
	envvrf->next = later;
	later->next = NULL;
	return later;
}

t_env *converttoll(char **agv, char**env,int j)
{
	int i = 0;
	t_env *envvrf, *t_envvrs, *first;
	// printf("%s\n",env[i]);
	int sz = ft_strlen(env[i]);
	int equalpos = ft_checkeq(env[i]);
	// printf("sz is %d, and equalpos is %d\n",sz,equalpos);
	// printf("and the last val is %c",env[i][sz+3]);

	while (env[i])
	{
		if (i == 0)
		{
			// printf("hi\n");
			envvrf = firstentryll(env[i],j);
			first = envvrf;
			// printf("cmd is %s\n",first->cmd);
		}
		else
		{
			t_envvrs = laterentryll(env[i],envvrf,j);
			envvrf = t_envvrs;
			// printf("cmd is %s\n",envvrf->cmd);
		}
		i++;
	// 	printf("%s\n",env[i++]);
	}
	envvrf->next=first;
	// printf("cmd is %s, and its value is %s\n",first->cmd,first->val);
	// printf("cmd is %s, and its value is %s\n",envvrf->cmd,envvrf->val);
	// printf("and after making it to point to next it is \n");
	// printf("cmd is %s, and its value is %s\n",envvrf->next->cmd,envvrf->next->val);
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

int ft_exit()//DOES NOT WORK
{
	// printf("welcm to exit secn");
	exit (0);
	// return 1;
}

// int flagcheckforexport(char *s)
// {
// 	int i = 0;
// 	if(s[i]=='-')
// 		return 1;
// 	return 0;
// }

char *parseforpurecmd(char *s)
{
	printf("welcomen and string received to be made to substr is %s\n",s);
	int equalpos = ft_checkeq(s);
	if(equalpos==-1)
		return s;
	printf("got string is %s\n",ft_substr(s,0,equalpos));
	return ft_substr(s,0,equalpos);
}

void update_export(t_env *first, char *s)
{
	free(first->val);
	int equalpos=ft_checkeq(s);
	first->val = ft_substr(s,equalpos+1,ft_strlen(s));
}

int checkforrepeat(char *s,t_env *first)
{
	printf("check for repeat\n");
	t_env *sec;

	sec = first->next;
	// printf("comparing %s and %s as same\n",parseforpurecmd(s),first->cmd);
	if(ft_strncmp(parseforpurecmd(s),first->cmd,(ft_strlen(s)>ft_strlen(first->cmd))?ft_strlen(s):ft_strlen(first->cmd)) == 0)
		// printf("found repeat\n");
	{
		// update
		printf("return 1\n");
		update_export(first,s);
		return 1;
	}
	while(sec!=first)
	{
		// printf("comparing %s and %s as same\n",s,parseforpurecmd(sec->cmd));
		if(ft_strncmp(parseforpurecmd(s),sec->cmd,(ft_strlen(s)>ft_strlen(sec->cmd))?ft_strlen(s):ft_strlen(sec->cmd)) == 0)
		{
			printf("return 1\n");
			update_export(sec,s);
			return 1;
		}
		sec=sec->next;
	}
	return 0;
}


// void update_export(t_env *llforexport, char *s)
// {
// 	int i = 1;
// 	if(s[0]=='-')
// 		return ;
// 	t_env *first=llforexport->next;
// 	//add a check for the first ele i.e.llforexport as well*****
// 	// if(ft_strncmp(llforexport->cmd,ft_strjoin("declare -x ",s),ft_strlen(llforexport->cmd)) == 0)
// 	// ft_strncmp(parseforpurecmd(s),sec->cmd
// 	if(ft_strncmp(parseforpurecmd(s),first->cmd,ft_strlen(llforexport->cmd)) == 0)
// 	{
// 		free(llforexport->val);
// 		int equalpos=ft_checkeq(s);
// 		if(equalpos == -1)
// 			return ;
// 		char *temp = ft_substr(s,equalpos,ft_strlen(s));
// 		llforexport->val = temp;
// 		printf("using new substr, we get %s\n\n",temp);
		
// 	}
// 	// printf("\n\n%s\n\n",first->cmd);

// 	t_env *sec;

// 	sec = llforexport->next;
// 	printf("comparing %s and %s as same\n",parseforpurecmd(s),llforexport->cmd);
// 	if(ft_strncmp(parseforpurecmd(s),llforexport->cmd,(ft_strlen(s)>ft_strlen(llforexport->cmd))?ft_strlen(s):ft_strlen(llforexport->cmd)) == 0)
// 		// printf("found repeat\n");
// 	{
// 		// update
// 		free(llforexport->val);
// 		int equalpos=ft_checkeq(s);
// 		if(equalpos == -1)
// 			return ;
// 		char *temp = ft_substr(s,equalpos,ft_strlen(s));
// 		llforexport->val = temp;
// 		printf("using new substr, we get %s\n\n",temp);

// 	}
// 	while(sec!=first)
// 	{
// 		printf("comparing %s and %s as same\n",s,parseforpurecmd(sec->cmd));
// 		if(ft_strncmp(parseforpurecmd(s),sec->cmd,(ft_strlen(s)>ft_strlen(sec->cmd))?ft_strlen(s):ft_strlen(sec->cmd)) == 0)
// 		{
// 		free(sec->val);
// 		int equalpos=ft_checkeq(s);
// 		if(equalpos == -1)
// 			return ;
// 		char *temp = ft_substr(s,equalpos,ft_strlen(s));
// 		sec->val = temp;
// 		printf("using new substr, we get %s\n\n",temp);
// 		}
// 		sec=sec->next;
// 	}
// }

void addtoexport(t_env *llforexport, char **cmd)
{
	//also have to add to env, if = present
	int i = 1;
	t_env *first=llforexport;
	while(llforexport->next!=first)
		llforexport=llforexport->next;
	printf("cmd is %s and val is %s",llforexport->cmd,llforexport->val);
	while(i<ft_size(cmd))
	{
		//possibly use later entry ll??
		if(checkforrepeat(ft_strjoin("declare -x ",cmd[i]),first) || cmd[i][0] == '-')
		{
			printf("repeat or flag\n");
			update_export(llforexport,cmd[i]);
			i++;
			continue;
		}
		t_env *new = malloc(sizeof(t_env));
		int equalpos=ft_checkeq(cmd[i]);
		// new->cmd = malloc((ft_strlen(cmd[i]) + 11) * sizeof(char));
		if(equalpos==-1)
			new->cmd = ft_strjoin("declare -x ",cmd[i]);
		else
		{
			new->cmd = malloc(sizeof(char)*(equalpos+1+11));
			new->val = malloc(sizeof(char)*(ft_strlen(cmd[i])-equalpos));
			populateforexport(new->cmd,new->val,cmd[i]);
		}
		printf("cmd is %s\n",new->cmd);
		// new->val=(char*)' ';
		new->next=first;
		// llforexport->next=new;
		// ft_memcpy(new->cmd,cmd[i],ft_strlen(cmd[i]));
		llforexport->next=new;
		llforexport=llforexport->next;//or llforexport=new;
		i++;
	}
	// // laterentryll();
	// if(flagcheckforexport(cmd))
	// {
		
	// }
	// t_env->cmd = malloc(sizeof())
}

int ft_export(t_env *llforexport,char **cmd)
{
	t_env *temp;
	printf("%d is the size\n",ft_size(cmd));
	if(ft_size(cmd) == 1)
	{
		temp = llforexport;
		llforexport=llforexport->next;
		if(temp->val)
			printf("%s=\"%s\"\n",temp->cmd,temp->val);
		else
			printf("%s\n",temp->cmd);
		while(llforexport!=temp)
		{
			if(llforexport->val)
				printf("%s=\"%s\"\n",llforexport->cmd,llforexport->val);
			else
				printf("%s\n",llforexport->cmd);
			llforexport = llforexport->next;
		}
	}
	else
	{
		addtoexport(llforexport,cmd);
	}
	return 1;
}

int ft_env(t_env *llforenv)
{
	t_env *s;

	s=llforenv->next;
	printf("%s=%s\n",llforenv->cmd,llforenv->val);
	while (s!=llforenv)
	{
		printf("%s=%s\n",s->cmd,s->val);
		s = s->next;
	}
	return 1;
}

int ft_unset(t_ptr *ptr,char **cmd)
{
	//remove from export and env if there
	//compare after joining with "declare -x"
	t_env *first, *sec, *end;
	int i = 1;
	while(i<ft_size(cmd))
	{
		printf("gonna try\n");
		first = ptr->export;
		end = first;
		while(end->next!=ptr->export)
			end = end->next;
		sec = ptr->export->next;//or first->next
		// printf("first cmd is %s and val is %s",first->cmd,first->val);
		// printf("sec cmd is %s and val is %s",sec->cmd,sec->val);
		// printf("end cmd is %s and val is %s",end->cmd,end->val);
		// printf("after end cmd is %s and val is %s",end->next->cmd,end->next->val);
		printf("comparing %s and %s\n",first->cmd,ft_strjoin("declare -x ",cmd[i]));
		if(ft_strncmp(first->cmd,ft_strjoin("declare -x ",cmd[i]),(ft_strlen(ft_strjoin("declare -x ",cmd[i]))>ft_strlen(first->cmd))?ft_strlen(ft_strjoin("declare -x ",cmd[i])):ft_strlen(first->cmd)) == 0)
		{
			printf("found\n");
			end->next=sec;
			free(first);
			i++;
			ptr->export = sec;
			break;// continue ;
			// return 1;
		}
		// ft_strncmp(s,first->cmd,(ft_strlen(s)>ft_strlen(first->cmd))?ft_strlen(s):ft_strlen(first->cmd)) == 
		while(sec != ptr->export)
		{
			printf("hello and comparing %s and %s\n",sec->cmd,ft_strjoin("declare -x ",cmd[i]));
			
			if(ft_strncmp(sec->cmd,ft_strjoin("declare -x ",cmd[i]),(ft_strlen(ft_strjoin("declare -x ",cmd[i]))>ft_strlen(sec->cmd))?ft_strlen(ft_strjoin("declare -x ",cmd[i])):ft_strlen(sec->cmd)) == 0)
			{
				printf("found\n");
				first = sec->next;
				free(sec);
				i++;
				break;// continue;
				// return 1;
			}
		first = sec;
		sec = sec->next;
		}
		i++;
	}
	printf("ending\n");
	return 1;
}

int checkforbuiltin(char **cmd,t_ptr *ptr)
{
	int i = 0;
	// printf("in builtin checker\n");
	if(ft_strncmp(cmd[0],"pwd",ft_strlen(cmd[0]))==0)
		i = ft_getpwd();
	else if(ft_strncmp(cmd[0],"echo",ft_strlen(cmd[0])) == 0)
		i = ft_echo(cmd);
	else if(ft_strncmp(cmd[0],"exit",ft_strlen(cmd[0])) == 0)
		i = ft_exit();
	else if(ft_strncmp(cmd[0],"env",ft_strlen(cmd[0])) == 0)
		i = ft_env(ptr->env);
	else if(ft_strncmp(cmd[0],"export",ft_strlen(cmd[0])) == 0)
		i = ft_export(ptr->export,cmd);
	else if(ft_strncmp(cmd[0],"unset",ft_strlen(cmd[0])) == 0)
		i = ft_unset(ptr,cmd);
	return i;
}

int execution(char **envp, char **cmd,t_ptr *ptr)
{
    // printf("hello from execution\n");
    int i = -1;
    char *pathvar = getpath(envp, "PATH");
    char **paths = ft_splitpath(pathvar,':');
    // printf("%s\n",paths[0]);
	if(checkforbuiltin(cmd,ptr))
		return 1;
    // printf("in execution\n");
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

int pipex(char *s, char **envp, t_ptr *ptr)
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
			close(fd[i][1]);
			if (execution(envp,ft_split(terms[i],' '),ptr)==0)
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
		execution(envp,ft_split(terms[i],' '),ptr);
	else
		waitpid(id1,NULL,0);
	// printf("seg1\n");
	return 0;
}
//pipex functions end

void sig_handler(int code, siginfo_t *info, void *ucontext)
{
	if(code == 2)
	{
		//SIGINT-new prompt on newline
		rl_replace_line("", 0);
		// ft_putstr_fd("\n", 2);
		rl_on_new_line();
		rl_redisplay();
	}
	else if(code == 3)
		return ;//ctrl-\ - do nothing
}

void signals()
{
	struct sigaction sa;
	sa.sa_sigaction = sig_handler;
	// sa.sa_flags = SA_RESTART | SA_SIGINFO;
	sigaction(SIGINT,&sa,NULL);
	sigaction(SIGQUIT,&sa,NULL);
	
}

void handle_sigint(int sigcode)
{
	printf("code is %d",sigcode);
}

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
	// signals();//DOES NOT WORK
	// signal(SIGINT, handle_sigint);
	// signal(SIGQUIT, handle_sigint);
	// signal(SIGSTOP, handle_sigint);
	
	
	t_env *llforenv = converttoll(agv,env,1);
	t_env *llforexport = converttoll(agv,env,0);
	t_ptr *ptr = (t_ptr *)malloc(sizeof(t_ptr));
	ptr->env = llforenv;
	ptr->export = llforexport;
	// while(llforenv->next!=ptr->env)
	// {
	// 	printf("%s=%s\n",llforenv->cmd,llforenv->val);
	// 	llforenv=llforenv->next;
	// }
	// printf("some random strings are %s and its val is %s\n",llforexport->next->next->cmd,llforexport->next->next->val);
	// printf("some random strings are %s and its val is %s\n",llforenv->next->next->cmd,llforenv->next->next->val);
	while(1)
	{
		echooption = false;
		
		// printf("No new line? lets check:");
		// printf("ll conversion success\n");
		// rl_on_new_line();
		s = readline("myshell> ");
		rl_redisplay();
		// printf("readline success\n");
		if(s)
			add_history(s);
		// printf("history added\n");
		pipex(s,env,ptr);
		// printf("pipex success\n");
		// printf("pipex over");
		free(s);
		// printf("free s\n");
	}
}
