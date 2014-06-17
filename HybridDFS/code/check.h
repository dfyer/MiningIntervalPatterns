
int error(char *string)
{
	fprintf(stderr,"%s\n",string);
	exit(1);
}

int check_arguments(int argc)
{
	if (argc != 5)
	{
		printf("Invalid number of arguments\nSyntax: <a.out> <input database> <input events types> <minimum support> <directory of results>\n");	
		return 1;
	}
	else 
		return 0;
}
