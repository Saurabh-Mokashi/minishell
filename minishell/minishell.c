#include <stdio.h>
#include <stdlib.h>

typedef struct env
{
	char *cmd;
	char *val;
	struct env *ptr;
} t_env;

unsigned int	ft_strlen(const char *str)
{
	unsigned int	i;
	//printf("hello world\n");

	i = 0;
	if(!str)
	{
	//	printf("return ning NULL\n");
		return (0);
	}
	while (str[i] != '\0')
	{
		i++;
	}
	//printf("\nval of i is %d",i);
	return (i);
}

int ft_checkeq(char *s, int sz)
{
	int i = 0;
	while(i < sz)
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

t_env* firstentryll(char *s,int sz)
{
	// int sz = ft_strlen(s);
	int equalpos = ft_checkeq(s,sz);
	char *s1 = malloc(sizeof(char)*(equalpos+1));
	char *s2 = malloc(sizeof(char)*(sz-equalpos));
	populate(s1,s2,s);
	t_env *first;
	first->ptr=NULL;
	first = malloc(sizeof(t_env));
	first->cmd=s1;
	first->val=s2;
	// printf("\nfirst command is %s\n",first->cmd);
	// printf("\nSec command is %s\n",first->val);
	return first;
}

t_env* laterentryll(char *s, t_env *envvrf)
{
	
}

int main(int ac, char **agv, char **env)
{
	int i = 0;
	t_env *envvrf, *t_envvrs;
	printf("%s\n",env[i]);
	int sz = ft_strlen(env[i]);
	int equalpos = ft_checkeq(env[i],sz);
	int j = 0;
	// printf("sz is %d, and equalpos is %d\n",sz,equalpos);
	// printf("and the last val is %c",env[i][sz+3]);

	while (env[i])
	{
		if(i == 0)
			envvrf = firstentryll(env[i],sz);
		else
			t_envvrs = laterentryll(env[i],envvrf);
		i++;
	// 	printf("%s\n",env[i++]);
	}
}