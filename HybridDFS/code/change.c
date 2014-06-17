//  Mining frequent arrangements of temporal intevals
//  Panagiotis Papapetrou
//  Boston University	

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <list>
#include <string>
#include <iostream>
#include <sys/types.h>
#include <sys/times.h>

using namespace std;


int main(int argc, char *argv[])
{
	FILE *fp1, *fp2;

	vector<int> conv;
	int i, j, k;
	int a,b,s,t;
	int lines = 0;
	int current;
	char c;

	fp1 = fopen(argv[1],"r");

	while ( (c = fgetc(fp1)) != EOF)
	{
		if (c == '\n')
			lines++;
	}

	rewind(fp1);

	fp2 = fopen(argv[2],"w");

	for (i=0;i<lines;i++)
	{
		fscanf(fp1,"%d %d %d %d\n",&a,&b,&s,&t);
	
		if (i == 0)
		{
			current = a;
			conv.push_back(a);
		}

		if (current != a)
		{
			conv.push_back(a);
			current = a;
		}

		fprintf(fp2,"%d %d %d %d\n",conv.size(),b,s,t);

	}

	fclose(fp1);
	fclose(fp2);

	return 0;
}
