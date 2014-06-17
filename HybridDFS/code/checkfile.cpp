
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char *argv[])
{
	FILE *fp;
	
	int i;
	long int a,b,c,d;	
	int N;

	fp = fopen(argv[1],"r");

	fscanf(fp,"%d",&N);

	for (i=0;i<N;i++)
	{
		fscanf(fp,"%ld %ld %ld %ld",&a,&b,&c,&d);
		printf("%ld %ld %ld %ld\n",a,b,c,d);
	}

	fclose(fp);
	
}
