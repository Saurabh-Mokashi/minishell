#include <string.h>
#include <stdio.h>

int main(int ac, char **agv, char **envp)
{
	int i = 0;
	char terms[10][50];
	char s[50] = "ls -l | wc -l";
	char sep[2] = "|";
	char *token;
	// strcpy(terms[i],strtok(s,sep));
	token=strtok(s,sep);
	// i++;
	while (token!=NULL)
	{
		strcpy(terms[i],token);
		token = strtok(NULL,sep);
		i++;
	}
	printf("%s",terms[2]);
	
	return (0);
}