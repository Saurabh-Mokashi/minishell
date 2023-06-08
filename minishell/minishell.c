#include <stdio.h>

typedef struct env
{
	char *cmd;
	char *val;
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
int main(int ac, char **agv, char **env)
{
	int i = 0;
	t_env *envvr;
	printf("%s\n",env[i]);
	int sz = ft_strlen(env[i]);
	int equalpos = ft_checkeq(env[i],sz);
	printf("sz is %d, and equalpos is %d",sz,equalpos);

	// while (env[i])
	// {
	// 	int sz = ft_strlen(env[i]);
	// 	int equalpos = ft_checkeq(env[i],sz);
	
	// // 	printf("%s\n",env[i++]);
	// }

}