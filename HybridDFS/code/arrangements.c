//  Mining frequent arrangements of temporal intervals
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

#include "prototypes.h"		    // Contains all the prototypes of the functions used in this project.
#include "check.h"				    // Contains basic input and error checking functions.

#define epsilon 0.5				    // epsilon factor for precision
#define F1_COLS 4				      // Columns of f1_array: array for frequent 1-i-sequences.
#define F2_COLS 17				    // Columns of f2_array: array for frequent 2-i-sequences.

#define T_COLS 2				      // Columns of t_array.
#define I_COLS 4				      // Columns of i_array.
		
#define MAX 50					      // Maximum size of an event type

#define MEET 1
#define MATCH 2
#define OVERLAP 3
#define CONTAIN 4
#define FOLLOW 5
#define CONTAINL 6
#define CONTAINR 7

#define TYPES 7

int **i_array;					      // Used for input storage.
char **t_array;					      // Used to store the types of events.
int *ti_array;

int **cand_f1_array;
int **f1_array;					      // Used to count and store the frequent 1-i-sequences.If an event
								              // is not frequent it is marked with 0, else it is marked with 1. The
								              // array is initialized to 0.

int **cand_f2_array;
int **f2_array;					      // Used to keep track of all 2-i-sequences. If a relation 
								              // is not frequent it is marked with 0, else it is marked with 1. The
								              // array is initialized to 0.
int **short_f2_array;		      // Used to keep track of the frequent 2-i-sequences. If a relation 
								              // is not frequent it is marked with 0, else it is marked with 1. The
								              // array is initialized to 0.

int **f3_array;					      // Used to keep track of frequent 3-i-sequences. If a relation 
								              // is not frequent it is marked with 0, else it is marked with 1. The
								              // array is initialized to 0.

int CID_num;					        // Counts number of CIDs

float min_sup;					      // Defines the minimum support threshold

int min_sup_th;		

char Input_File[50];		      // Name of input file

int **array = NULL;

vector <int> f1_items;

char *relations_1 = "f1.txt";   // Intermediate file for frequent 1-itemsets
char *relations_2 = "f2.txt";   // Intermediate file for frequent 2-itemsets

class Interval;
class Pair_node;
class Interval_Rels;
class Arrangement;
class Arrangement_PTR;
class Single;

typedef Interval *interval;

typedef Pair_node *pair_node;

char directory[20];
                                // Seven types of event intervals
char *int_types[7] = {"MEET","MATCH","OVERLAP","CONTAIN","FOLLOW","CONTAINL","CONTAINR"};

double updateTime = 0.0;
vector<Single *> Singles;


class Pair_node
{
public:
	Pair_node(int a, int b, int start_a, int end_a, int start_b, int end_b);
	~Pair_node();
	int _a;
	int _b;
	int relations[TYPES+1];
	int total_sup[TYPES+1];
	int actual_sup[TYPES+1];
	int total;
	int **cids;
	vector<Interval *> *cid_index;
	vector<Interval *> meet_list;
	vector<Interval *> match_list;
	vector<Interval *> overlap_list;
	vector<Interval *> contain_list;
	vector<Interval *> follow_list;
	vector<Interval *> containL_list;
	vector<Interval *> containR_list;
	vector<Interval *> interval_list;
};

class Single
{
public:
	Single(){};
	~Single(){};
	int label;
	int Start;
	int End;
	int cid;
};


class Interval 
{
public:
	Interval();
	Interval (int type, int cid, int a, int b, int start_a, int end_a, int start_b, int end_b); 
	~Interval(){};
	int _type;						// 1: Meet, 2: Match, 3: Overlap, 4: Contain, 5: Follow, 6: ContainR, 7: COntainL
	int _cid;
	int _a;
	int _b;
	int Start_a;		
	int End_a;
	int Start_b;		
	int End_b;
	int support;

	vector<Single *> singles; 
	Pair_node *father_node;
	Interval *list_next;
};

class Arrangement_PTR
{
public:
	Arrangement_PTR(){};
	~Arrangement_PTR(){};
	vector<Arrangement *> ptr;
};


class Rel_node
{
public:
	Rel_node(){};
	~Rel_node();

	int enum_item_index;
	int enum_node_index;

	int *items;
	int size;
	int rel_cid[TYPES+1];

	Pair_node *father;
	vector<vector<int> > i_relations;
	vector<vector<int> > cid_index;

	vector<vector<Arrangement_PTR *> > agmt_ints;
	vector<Rel_node *> rel_nodes_ptr;
};

class Arrangement
{
public:
	Arrangement(){};
	~Arrangement();

	int _size;
	vector<int> items;
	vector<Interval *> arg_ints;
};

Interval::Interval (int type, int cid, int a, int b, int start_a, int end_a, int start_b, int end_b)
{
	int i, j;

	_type = type;
	_cid = cid;
	_a = a;
	_b = b;
	Start_a = start_a;
	Start_b = start_b;
	End_a = end_a;
	End_b = end_b;

	for (i=0;i<Singles.size();i++)
	{
		if (Singles[i]->label == a && Singles[i]->Start == start_a && Singles[i]->End == end_a)
			singles.push_back(Singles[i]);
		
		if (Singles[i]->label == b && Singles[i]->Start == start_b && Singles[i]->End == end_b)
			singles.push_back(Singles[i]);
	}
}

Pair_node::Pair_node(int a, int b, int start_a, int end_a, int start_b, int end_b)
{
	int i, j;

	_a = a;
	_b = b;

	total = 0;

	cids = new int *[CID_num+1];

	for (i=0;i<CID_num+1;i++)
		cids[i] = new int[TYPES+1];

	for (i=0;i<CID_num+1;i++)
		for (j=0;j<TYPES+1;j++)
			cids[i][j] = 0;

	for (i=0;i<TYPES+1;i++)
	{
		total_sup[i] = 0;
		actual_sup[i] = 0;
		relations[i]= 0;
	}

	cid_index = new vector<Interval *>[CID_num+1];
}

Pair_node::~Pair_node()
{
	int i, j;

	for (i=0;i<CID_num+1;i++)
		delete[] cids[i];

	delete[] cids;

	for (i=0;i<CID_num+1;i++)
		for (j=0;j<cid_index[i].size();j++)
			delete cid_index[i][j];

	meet_list.clear();
	match_list.clear();
	overlap_list.clear();
    contain_list.clear();
	follow_list.clear();
	containL_list.clear();
	containR_list.clear();
}

Rel_node::~Rel_node()
{
	delete this->items;
}

vector<Pair_node *> Pair_nodes;				// Global pointer for Pair_nodes

vector<vector<Rel_node *> > Rel_nodes;		// Global pointer for Rel_nodes

vector<Rel_node *> Enum_nodes;


void delete_i_array(int N)
{
	for (int i=0;i<N;i++)
		delete[] i_array[i];

	delete[] i_array;
}

void delete_f1_array(int N)
{
	for (int i=0;i<N;i++)
		delete[] f1_array[i];

	delete[] f1_array;
}

void delete_f2_array(int N)
{
	for (int i=0;i<N;i++)
		delete[] f2_array[i];

	delete[] f2_array;
}

void delete_cand_f2_array(int N)
{
	for (int i=0;i<N;i++)
		delete[] cand_f2_array[i];

	delete[] cand_f2_array;
}

void delete_cand_f1_array(int N)
{
	int i;

	for (i=0;i<N;i++)
		delete[] cand_f1_array[i];

	delete[] cand_f1_array;
}

void destroy_Pair_node(void)
{
	int i;

	for (i=0;i<Pair_nodes.size();i++)
		delete Pair_nodes[i];

	Pair_nodes.clear();
}


void store_f2_array(int N, char *filename)
{
	int i, j;

	FILE *fp;

	fp = fopen(filename,"w");

//	puts("\nStoring f2_array...");
	
	fprintf(fp, "Input File: %s   Support: %f\n", Input_File, min_sup);
	fprintf(fp, "size: %d\n",N);
	fprintf(fp, "e1: item 1\ne2: item 2\nMe: Support for Meets\nMa: Support for Matches\nO: Support for Overlaps\nC: Support for Contains\nS: Support for Sequents"); 

	fprintf(fp,"\n e1  e2  Me  F   Ma  F   O   F   C   F  S  F\n");

	for (i=0;i<N;i++)
	{
		for (j=0;j<F2_COLS;j++)
			fprintf(fp, " %d  ",cand_f2_array[i][j]);
		fprintf(fp, "\n");
	}
	
	fclose(fp);
//	puts("done");
}

void store_f1_array(int N)
{
	int i, j;

	FILE *fp;

	fp = fopen(relations_1,"w");

//	puts("\nStoring f1_array...");

	fprintf(fp, "Input File: %s   Support: %f\n\n", Input_File, min_sup);

	for (i=0;i<N;i++)
	{
		f1_items.push_back(f1_array[i][0]);

		for (j=0;j<F1_COLS;j++)
			fprintf(fp, "%d ",f1_array[i][j]);
		fprintf(fp, "\n");
	}

	fclose(fp);
}



void print_cand_f2_array(int N)
{
	int i, j;

	puts("\nprinting f2_array...");
	printf("\ne1: item 1\ne2: item 2\nMe: Support for Meets\nMa: Support for Matches\nO: Support for Overlaps\nMe: Support for Contains\n"); 

	printf("\n e1  e2  Me  F   Ma  F   O   F   C   F  \n");
	for (i=0;i<N;i++)
	{
		for (j=0;j<F2_COLS;j++)
			printf(" %d  ",cand_f2_array[i][j]);
		puts("");
	}

}


void print_f1_array(int N)
{
	int i, j;

	puts("\nprinting f1_array...");

	for (i=0;i<N;i++)
	{
		for (j=0;j<F1_COLS;j++)
			printf("%d ",f1_array[i][j]);
		puts("");
	}

//	puts("DONE");
}


void print_i_array(int N, int M)
{
	int i, j;

	puts("\nprinting i_array...");

	for (i=0;i<N;i++)
	{
		for (j=0;j<M;j++)
			printf("%d ",i_array[i][j]);

		puts("");
	}
}


void print_t_array(int N, int M)
{
	int i;

	puts("\nprinting t_array...");

	for (i=0;i<N;i++)
		printf("%s\n",t_array[i]);
}


void insert_relation(int type, int CID, int a, int b, int s_x, int e_x, int s_y, int e_y)
{
	interval new_int;
	int i, j;
	int index;
	int level = 2;
	vector<int> tmp, tmp2;
	vector<Rel_node *> tmp_rel;

	int flag = 0;		// used for support counting

	pair_node new_pair;
	pair_node cur_pair;

	int found = 0;

	for (index=0;index<Pair_nodes.size();index++)
	{
		if (Pair_nodes[index]->_a == a && Pair_nodes[index]->_b == b)
		{
			found = 1;
			break;
		}
	}

	if (found == 0)
	{
		new_pair = new Pair_node(a, b, s_x, e_x, s_y, e_y);	
		Pair_nodes.push_back(new_pair);
		index = Pair_nodes.size()-1;
	}

	if (!Pair_nodes[index]->cid_index[CID].empty())
	{
		for (i=0;i<Pair_nodes[index]->cid_index[CID].size();i++)
		{
			if (Pair_nodes[index]->cid_index[CID][i]->_type == type)
			{
				flag = 1;		// this type of relation already exists in the CID.
				break;
			}
		}
	}

	new_int = new Interval(type, CID, a, b, s_x, e_x, s_y, e_y);

	Pair_nodes[index]->cid_index[CID].push_back(new_int);

	if (flag == 0)
		Pair_nodes[index]->relations[type]++;

	Pair_nodes[index]->interval_list.push_back(new_int);

	Pair_nodes[index]->actual_sup[0]++;

	if (type == MEET)
	{
		Pair_nodes[index]->actual_sup[MEET]++;

		if (flag == 0)
		{
			Pair_nodes[index]->total_sup[0]++;
			Pair_nodes[index]->total_sup[MEET]++;	
		}

		Pair_nodes[index]->meet_list.push_back(new_int);		
	}
	else if (type == MATCH)
	{
		Pair_nodes[index]->actual_sup[MATCH]++;

		if (flag == 0)
		{
			Pair_nodes[index]->total_sup[0]++;
			Pair_nodes[index]->total_sup[MATCH]++;
		}

		Pair_nodes[index]->match_list.push_back(new_int);	
	}
	else if (type == OVERLAP)
	{
		Pair_nodes[index]->actual_sup[OVERLAP]++;

		if (flag == 0)
		{
			Pair_nodes[index]->total_sup[0]++;	
			Pair_nodes[index]->total_sup[OVERLAP]++;	
		}
	
		Pair_nodes[index]->overlap_list.push_back(new_int);	
	}
	else if (type == CONTAIN)
	{
		Pair_nodes[index]->actual_sup[CONTAIN]++;

		if (flag == 0)
		{
			Pair_nodes[index]->total_sup[0]++;	
			Pair_nodes[index]->total_sup[CONTAIN]++;	
		}
	
		Pair_nodes[index]->contain_list.push_back(new_int);	
	}
	else if (type == FOLLOW)
	{
		Pair_nodes[index]->actual_sup[FOLLOW]++;

		if (flag == 0)
		{
			Pair_nodes[index]->total_sup[0]++;	
			Pair_nodes[index]->total_sup[FOLLOW]++;	
		}
	
		Pair_nodes[index]->follow_list.push_back(new_int);	
	}
	else if (type == CONTAINL)
	{
		Pair_nodes[index]->actual_sup[CONTAINL]++;

		if (flag == 0)
		{
			Pair_nodes[index]->total_sup[0]++;	
			Pair_nodes[index]->total_sup[CONTAINL]++;	
		}
	
		Pair_nodes[index]->containL_list.push_back(new_int);	
	}
	else if (type == CONTAINR)
	{
		Pair_nodes[index]->actual_sup[CONTAINR]++;

		if (flag == 0)
		{
			Pair_nodes[index]->total_sup[0]++;	
			Pair_nodes[index]->total_sup[CONTAINR]++;	
		}
	
		Pair_nodes[index]->containR_list.push_back(new_int);	
	}

	if (flag == 0)
	{
		Pair_nodes[index]->total++;
		Pair_nodes[index]->relations[0]++;
		Pair_nodes[index]->cids[CID][type] = 1;
	}
}


void create_2Rel_array(int f2_size)
{
	interval new_int;
	int i, j, fi, k, l, m;
	int index;
	int level = 2;
	int count_ids;
	int flag = 0;		// used for support counting

	Arrangement *new_agmt;

	Arrangement_PTR *new_agmt_ptr;

	vector<int> tmp, tmp2;
	
	vector<Rel_node *> tmp_rel;
	vector<Arrangement *> agmt_vec;
	vector<Arrangement_PTR *> agmt_vec_ptr;
	
	vector<Interval *> tmp_ints, tmp_ints2;
	vector<Interval_Rels *> tmp_int_rels;

	pair_node new_pair;
	pair_node cur_pair;

	Rel_node *new_rel;
	Rel_node *empty_rel;

	Interval_Rels *new_int_rels;
	
	int found = 0;

	for (fi=0;fi<f2_size;fi++)
	{
		for (index=0;index<Pair_nodes.size();index++)
		{
			if (Pair_nodes[index]->_a == f2_array[fi][0] && Pair_nodes[index]->_b == f2_array[fi][1])
			{
				new_rel = new Rel_node();

				new_rel->items = new int[level];

				new_rel->items[0] = Pair_nodes[index]->_a;
				new_rel->items[1] = Pair_nodes[index]->_b;
				new_rel->rel_nodes_ptr.push_back(new_rel);

				new_rel->enum_item_index = 0;

				flag = 0;

				for (i=1;i<=TYPES;i++)
				{
					if (Pair_nodes[index]->total_sup[i] >= min_sup_th)
					{
						tmp.clear();
						tmp2.clear();
						tmp_int_rels.clear();
					
						new_rel->rel_cid[i] = new_rel->cid_index.size();

						count_ids = Pair_nodes[index]->total_sup[i];

						flag = 1;

						tmp.push_back(i);

						tmp2.push_back(Pair_nodes[index]->total_sup[i]);		// store support for relation i at cid_index[i][0]

						for (j=1;j<CID_num+1;j++)
							tmp2.push_back(Pair_nodes[index]->cids[j][i]);

						new_rel->i_relations.push_back(tmp);
						new_rel->cid_index.push_back(tmp2);

						for (k=1;k<new_rel->cid_index[new_rel->cid_index.size()-1].size();k++)
						{
							if (new_rel->cid_index[new_rel->cid_index.size()-1][k] > 0)
							{								
								for (m=0;m<Pair_nodes[index]->cid_index[k].size();m++)
								{
									if (Pair_nodes[index]->cid_index[k][m]->_type == new_rel->i_relations[new_rel->cid_index.size()-1][0])
									{
										new_agmt = new Arrangement();

										new_agmt->items.push_back(Pair_nodes[index]->_a);
										new_agmt->items.push_back(Pair_nodes[index]->_b);
										new_agmt->arg_ints.push_back(Pair_nodes[index]->cid_index[k][m]);

										agmt_vec.push_back(new_agmt);
									}
								}

								new_agmt_ptr = new Arrangement_PTR();
								new_agmt_ptr->ptr = agmt_vec;
								
								agmt_vec_ptr.push_back(new_agmt_ptr);

								agmt_vec.clear();

							}
							else 
							{
								new_agmt_ptr = new Arrangement_PTR();
								agmt_vec_ptr.push_back(new_agmt_ptr);
							}

						}

						new_rel->agmt_ints.push_back(agmt_vec_ptr);
						agmt_vec_ptr.clear();

					}
				}

				if (flag == 0)
				{
					delete new_rel;
					continue;
				}

				new_rel->enum_node_index = 0;

				tmp_rel.clear();

				if (Rel_nodes.size() == level)
				{
					empty_rel = new Rel_node();
					tmp_rel.push_back(empty_rel);
					Rel_nodes.push_back(tmp_rel);
				}

				new_rel->father = Pair_nodes[index];

				Rel_nodes[level].push_back(new_rel);	

				tmp.clear();
			}
		}
	}	
}


int Meet(int k, int a, int b, int i_size)
{
	int i, j;
	int current_cid;			// Keeps track of current CID
	int current_pos;			// Keeps track of current CID position in i_array
	int count;					// Counts number of Meets
	int flag;

	current_cid = i_array[0][0];
	current_pos = 0;
	count = 0;
	flag = 0;
	
//	if (a==b)
//		return 0;

	for (i=0;i<i_size;i++)		
	{	
		if (i_array[i][1] == a)				// first event found		
		{
			for (j=current_pos;i_array[j][0] == current_cid;j++)
			{
				if (i_array[j][1] == b)		// second event found
				{
					// In order to have a Meet(a, b):
					// end(a) = start(b) +/-e
			//		printf("a= %-10d     b= %-10d       end(a)= %-10d     start(b)= %-10d\r", a, b, i_array[i][3], i_array[j][2]);

					if ( abs(i_array[i][3] - i_array[j][2]) <= epsilon )
					{
						if (flag == 0)
						{
							count++;
							flag = 1;
						}
						insert_relation(MEET, current_cid, a, b, i_array[i][2], i_array[i][3], i_array[j][2], i_array[j][3]);
						//break;
					}
				}
				if(j == i_size-1)
					break;
			}		
		}

		if (i < i_size-1 && i_array[i+1][0] != current_cid)
		{
			current_cid = i_array[i+1][0];
			current_pos = i+1;
			flag = 0;
		}
	}

	return count;
}

int Match(int k, int a, int b, int i_size)
{
	int i, j;
	int current_cid;			// Keeps track of current CID
	int current_pos;			// Keeps track of current CID position in i_array
	int count;					// Counts number of Matches
	int flag;

	current_cid = i_array[0][0];
	current_pos = 0;
	count = 0;
	
//	if (a == b)
//		return 0;

	for (i=0;i<i_size;i++)		
	{
		if (i_array[i][1] == a)				// first event found		
		{
			for (j=current_pos;i_array[j][0] == current_cid;j++)
			{
				if (i_array[j][1] == b && i!=j)		// second event found
				{
				//	printf("a= %-10d     b= %-10d       end(a)= %-10d     start(b)= %-10d\r", a, b, i_array[i][3], i_array[j][2]);

					if ( abs(i_array[i][2] - i_array[j][2]) <= epsilon && abs(i_array[i][3] - i_array[j][3]) <= epsilon )
					{
						if (flag == 0)
						{
							count++;
							flag = 1;
						}
						insert_relation(MATCH, current_cid, a, b, i_array[i][2], i_array[i][3], i_array[j][2], i_array[j][3]);
						//break;
					}
				}
				if(j == i_size-1)
					break;

			}
		
		}

		if (i < i_size-1 && i_array[i+1][0] != current_cid)
		{
			current_cid = i_array[i+1][0];
			current_pos = i+1;
			flag = 0;
		}
	}

	return count;


}



int Overlap(int k, int a, int b,int i_size)
{
	int i, j;
	int current_cid;			// Keeps track of current CID
	int current_pos;			// Keeps track of current CID position in i_array
	int count;					// Counts number of Overlaps
	int flag;

	current_cid = i_array[0][0];
	current_pos = 0;
	count = 0;
	flag = 0;
	
//	if (a==b)
//		return 0;

	for (i=0;i<i_size;i++)		
	{
		if (i_array[i][1] == a)				// first event found		
		{
			for (j=current_pos;i_array[j][0] == current_cid;j++)
			{
				if (i_array[j][1] == b)		// second event found
				{
				//	printf("a= %-10d     b= %-10d       end(a)= %-10d     start(b)= %-10d\r", a, b, i_array[i][3], i_array[j][2]);

					if ( i_array[j][2] - i_array[i][2] > epsilon && i_array[j][3] - i_array[i][3] > epsilon && i_array[i][3] - i_array[j][2] > epsilon )
					{
						if (flag == 0)
						{
							count++;
							flag = 1;
						}
						insert_relation(OVERLAP, current_cid, a, b, i_array[i][2], i_array[i][3], i_array[j][2], i_array[j][3]);
						//break;
					}
				}
				if(j == i_size-1)
					break;

			}
		
		}

		if (i < i_size-1 && i_array[i+1][0] != current_cid)
		{
			current_cid = i_array[i+1][0];
			current_pos = i+1;
			flag = 0;
		}
	}

	return count;

}



int Contain(int k, int a, int b, int i_size)
{
	int i, j;
	int current_cid;			// Keeps track of current CID
	int current_pos;			// Keeps track of current CID position in i_array
	int count;					// Counts number of Contains
	int flag;

	current_cid = i_array[0][0];
	current_pos = 0;
	count = 0;
	flag = 0;
	
//	if (a==b)
//		return 0;

	for (i=0;i<i_size;i++)		
	{
		if (i_array[i][1] == a)				// first event found		
		{
			for (j=current_pos;i_array[j][0] == current_cid;j++)
			{
				if (i_array[j][1] == b)		// second event found
				{
				//	printf("a= %-10d     b= %-10d       end(a)= %-10d     start(b)= %-10d\r", a, b, i_array[i][3], i_array[j][2]);

					if ( i_array[j][2] - i_array[i][2] > epsilon && i_array[i][3] - i_array[j][3] > epsilon )
					{
						if (flag == 0)
						{
							count++;
							flag = 1;
						}
						insert_relation(CONTAIN, current_cid, a, b, i_array[i][2], i_array[i][3], i_array[j][2], i_array[j][3]);
						//break;
					}
				}
				if(j == i_size-1)
					break;

			}
		
		}

		if (i < i_size-1 && i_array[i+1][0] != current_cid)
		{
			current_cid = i_array[i+1][0];
			current_pos = i+1;
			flag = 0;
		}
	}

	return count;
}


int Sequent(int k, int a, int b, int i_size)
{
	int i, j;
	int current_cid;			// Keeps track of current CID
	int current_pos;			// Keeps track of current CID position in i_array
	int count;					// Counts number of Contains
	int flag;

	current_cid = i_array[0][0];
	current_pos = 0;
	count = 0;
	flag = 0;

//	if (a==b)
//		return 0;

	for (i=0;i<i_size;i++)		
	{
		if (i_array[i][1] == a)				// first event found		
		{
			for (j=current_pos;i_array[j][0] == current_cid;j++)
			{
				if (i_array[j][1] == b)		// second event found
				{
				//	printf("a= %-10d     b= %-10d       end(a)= %-10d     start(b)= %-10d\r", a, b, i_array[i][3], i_array[j][2]);

					if ( i_array[j][2] - i_array[i][3] > epsilon )
					{
						if (flag == 0)
						{
							count++;
							flag = 1;
						}
						insert_relation(FOLLOW, current_cid, a, b, i_array[i][2], i_array[i][3], i_array[j][2], i_array[j][3]);
					}
				}
				if(j == i_size-1)
					break;

			}
		
		}

		if (i < i_size-1 && i_array[i+1][0] != current_cid)
		{
			current_cid = i_array[i+1][0];
			current_pos = i+1;
			flag = 0;
		}
	}

	return count;
}

int ContainL(int k, int a, int b, int i_size)
{
	int i, j;
	int current_cid;			// Keeps track of current CID
	int current_pos;			// Keeps track of current CID position in i_array
	int count;					// Counts number of Contains
	int flag;

	current_cid = i_array[0][0];
	current_pos = 0;
	count = 0;
	flag = 0;
	
//	if (a==b)
//		return 0;

	for (i=0;i<i_size;i++)		
	{
		if (i_array[i][1] == a)				// first event found		
		{
			for (j=current_pos;i_array[j][0] == current_cid;j++)
			{
				if (i_array[j][1] == b)		// second event found
				{
				//	printf("a= %-10d     b= %-10d       end(a)= %-10d     start(b)= %-10d\r", a, b, i_array[i][3], i_array[j][2]);

					if ( abs(i_array[i][2] - i_array[j][2]) <= epsilon && i_array[i][3] - i_array[j][3] > epsilon )
					{
						if (flag == 0)
						{
							count++;
							flag = 1;
						}
						insert_relation(CONTAINL, current_cid, a, b, i_array[i][2], i_array[i][3], i_array[j][2], i_array[j][3]);
						//break;
					}
				}
				if(j == i_size-1)
					break;

			}
		
		}

		if (i < i_size-1 && i_array[i+1][0] != current_cid)
		{
			current_cid = i_array[i+1][0];
			current_pos = i+1;
			flag = 0;
		}
	}

	return count;
}

int ContainR(int k, int a, int b, int i_size)
{
	int i, j;
	int current_cid;			// Keeps track of current CID
	int current_pos;			// Keeps track of current CID position in i_array
	int count;					// Counts number of Contains
	int flag;

	current_cid = i_array[0][0];
	current_pos = 0;
	count = 0;
	flag = 0;
	
//	if (a==b)
//		return 0;

	for (i=0;i<i_size;i++)		
	{
		if (i_array[i][1] == a)				// first event found		
		{
			for (j=current_pos;i_array[j][0] == current_cid;j++)
			{
				if (i_array[j][1] == b)		// second event found
				{
				//	printf("a= %-10d     b= %-10d       end(a)= %-10d     start(b)= %-10d\r", a, b, i_array[i][3], i_array[j][2]);

					if ( abs(i_array[i][3] - i_array[j][3]) <= epsilon && i_array[j][2] - i_array[i][2] > epsilon )
					{
						if (flag == 0)
						{
							count++;
							flag = 1;
						}
						insert_relation(CONTAINR, current_cid, a, b, i_array[i][2], i_array[i][3], i_array[j][2], i_array[j][3]);
						//break;
					}
				}
				if(j == i_size-1)
					break;

			}
		
		}

		if (i < i_size-1 && i_array[i+1][0] != current_cid)
		{
			current_cid = i_array[i+1][0];
			current_pos = i+1;
			flag = 0;
		}
	}

	return count;
}


int permute(int *N, int rel_size)
{
	int col, i, j, k, l;
	int K=1;
	int times;
	int line;
	int element;
	int prev = 1;

	for (i=0;i<rel_size;i++)
		K = K*N[i];

	times = K;

	prev = 1;

//	for (i=0;i<rel_size;i++)
//		printf("%d ",N[i]);

//	printf("\n%d %d %d\n",K,times,rel_size);

	for (col=0;col<rel_size;col++)
	{		
		line = 0;
		element = 1;

		times = times / N[col];
		for (j=0;j<prev;j++)
		{
			for (k=0;k<N[col];k++)
			{
				for (l=0;l<times;l++)
				{
					array[line++][col] = element;
				}
				element++;
				if (element > N[col])
					element = 1;
			}
		}
		prev = prev * N[col];
	}
/*
	for(i=0;i<K;i++)
	{
		for(j=0;j<rel_size;j++)
		{
			printf("%d ",array[i][j]);
		}
		puts("");
	}

*/
	return K;
}

int min(int a, int b)
{
	if (a > b)
		return b;
	else 
		return a;
}


int check_relation(vector<int> items, vector<Interval *> ints)
{
	int i, j, k;
	int found = 0;

	Single *sing;

	vector<Single *> int_singles;

	for (i=0;i<items.size();i++)
	{
		sing = new Single();
		sing->cid = -1;

		int_singles.push_back(sing);
	}


	for (i=0;i<items.size();i++)
	{
		for (j=0;j<ints.size();j++)
		{
			if (ints[j]->_a == items[i])
			{
				if (int_singles[i]->cid == -1)
					int_singles[i] = ints[j]->singles[0];
				else
				{
					if (int_singles[i] != ints[j]->singles[0])
						return -1;
				}
			}

			if (ints[j]->_b == items[i])
			{
				if (int_singles[i]->cid == -1)
					int_singles[i] = ints[j]->singles[1];
				else
				{
					if (int_singles[i] != ints[j]->singles[1])
						return -1;
				}
			}
		}
	
	}

	return 0;
}


int cand_check(vector<int> items, vector<Rel_node *> rel_nodes, vector<int> i_relations_i, vector<int> cid_index_i)
{
	int i, j, k;
	int index;
	int check;
	int *M;

	vector<Interval *> ints;
	vector<vector<Interval *> > int_cids;


	for (i=0;i<cid_index_i.size();i++)
	{
		if (cid_index_i[i] == 1)
		{
			for (j=0;j<i_relations_i.size();j++)
			{
				for (k=0;k<rel_nodes[j]->father->cid_index[i].size();k++)
				{
					if (rel_nodes[j]->father->cid_index[i][k]->_type == i_relations_i[j])
						ints.push_back(rel_nodes[j]->father->cid_index[i][k]);
				}
				
				int_cids.push_back(ints);
				ints.clear();
				
			}



			int_cids.clear();
		}
	}

	return 0;
}


int make_join(int level, int join_item, int join_node)
{
	int i, j, k;
	int f3_size;
	int cand_f3_size;
	int found = 1;
	int tmp_value;
	int count;
	int perm_size;
	int check;

	vector<Rel_node *> tmp_rel;

	Rel_node *empty_rel;

	int **new_node_rels;
	int *N;

	vector<vector<int> > new_node_rels_filt;
	vector<vector<int> > new_node_rels_filt_types;

	vector<int> tmp1;
	vector<int> tmp2;
	vector<int> tmp_items;
	vector<vector<int> > tmp3;

	vector<Rel_node *> new_pairs;

	vector<vector<int> > cand_rels;
	vector<vector<int> > cand_rels_cids;

	if (level == 6)
	{
		Rel_nodes[level-1][join_node]->enum_item_index++;
		return -3;
	}

	Rel_node *new_rel = new Rel_node();

	new_rel->items = new int[level];

	for (i=0;i<level;i++)
		tmp_items.push_back(new_rel->items[i]);

	for (i=0;i<level-1;i++)
		new_rel->items[i] = Rel_nodes[level-1][join_node]->items[i];

	new_rel->items[i] = f1_items[join_item];

	new_rel->enum_item_index = 0;
	new_rel->enum_node_index = join_node;
	
	Rel_nodes[level-1][join_node]->enum_item_index++;

	printf("join_node %d, join item %d, item_index %d\n",join_node, join_item, Rel_nodes[level-1][join_node]->enum_item_index);
	
	for (i=0;i<level;i++)
	{
		printf("%d ",new_rel->items[i]);
	}

	puts("");
	

	new_pairs.clear();

	tmp_rel.clear();
	
	for (i=0;i<level-1;i++)
	{
		for (j=1;j<Rel_nodes[2].size();j++)
		{
			if (new_rel->items[i] == Rel_nodes[2][j]->items[0] && Rel_nodes[2][j]->items[1] == f1_items[join_item])
				new_pairs.push_back(Rel_nodes[2][j]);
		}
	}


	if (new_pairs.size() != level-1)		// did not find all pairs of relations in level 2. Abort!
	{
		new_pairs.clear();
		delete new_rel;
		return -1;
	}


	for (k=0;k<Rel_nodes[level-1][join_node]->rel_nodes_ptr.size();k++)
		new_rel->rel_nodes_ptr.push_back(Rel_nodes[level-1][join_node]->rel_nodes_ptr[k]);

	for (k=0;k<new_pairs.size();k++)
		new_rel->rel_nodes_ptr.push_back(new_pairs[k]);

	printf("%d %d %d\n",new_pairs.size(), Rel_nodes[level-1][join_node]->rel_nodes_ptr.size(), new_rel->rel_nodes_ptr.size());


	new_node_rels = new int *[level-1];

	for (i=0;i<level-1;i++)
		new_node_rels[i] = new int[TYPES+1];

	for (i=0;i<level-1;i++)
		for (j=0;j<=TYPES;j++)
			new_node_rels[i][j] = 0;

	for (i=0;i<level-1;i++)
		for (j=0;j<new_pairs[i]->i_relations.size();j++)
			new_node_rels[i][new_pairs[i]->i_relations[j][0]] = 1;


	for (i=0;i<level-1;i++)
	{
		found = 0;
		for (j=1;j<=TYPES;j++)
		{
			if (new_node_rels[i][j] == 1)
				found = 1;
		}

		if  (found == 0)
		{
		//	error("SHOULD NOT BE HERE 2");
			for (i=0;i<level-1;i++)
				delete[] new_node_rels[i];

			delete[] new_node_rels;
			
			new_pairs.clear();
			return -2;
		}
	}


	for (i=0;i<level-1;i++)
	{
		for (j=1;j<=TYPES;j++)
		{
			if (new_node_rels[i][j] == 1)
				tmp2.push_back(j);
		}
		
		new_node_rels_filt_types.push_back(tmp2);

		tmp2.clear();
	}

	for (i=0;i<level-1;i++)
		delete[] new_node_rels[i];

	delete[] new_node_rels;

	N = new int[level-1];

	for (i=0;i<level-1;i++)
		N[i] = new_node_rels_filt_types[i].size();

	k = 1;

	for (i=0;i<level-1;i++)
		k = k*N[i];

	array = new int *[k];

	for (i=0;i<k;i++)
		array[i] = new int[level-1];

	permute(N, level-1);

	perm_size = k;

	tmp1.clear();
	
//	printf("level %d     filt  %d \n", level-1,new_node_rels_filt_types.size());

	for (i=0;i<perm_size;i++)
	{
		for (j=0;j<level-1;j++)
			tmp1.push_back(new_node_rels_filt_types[j][array[i][j]-1]);

		cand_rels.push_back(tmp1);

		tmp1.clear();
	}

	count = 0;

	for (i=0;i<cand_rels.size();i++)
	{
		tmp1.clear();
		tmp1.push_back(0);

		for (k=1;k<CID_num+1;k++)
		{
			for (j=0;j<cand_rels[i].size();j++)
			{
				if (j == 0)
					tmp1.push_back(new_pairs[j]->cid_index[new_pairs[j]->rel_cid[cand_rels[i][j]]][k]);
				else
					tmp1[k] += new_pairs[j]->cid_index[new_pairs[j]->rel_cid[cand_rels[i][j]]][k];

				if (new_pairs[j]->cid_index[new_pairs[j]->rel_cid[cand_rels[i][j]]][k] > 1 )
					error("error");
			}
		}

		count = 0;

		tmp2.clear();

		for (j=0;j<CID_num+1;j++)
		{
			if (tmp1[j] == level-1)
			{
				tmp2.push_back(1);
				count++;
			}
			else
				tmp2.push_back(0);
		}

		cand_rels_cids.push_back(tmp2);

	}

	tmp1.clear();
	tmp2.clear();

//	printf("rel size  %d\n",Rel_nodes[level-1][join_node]->i_relations.size());
	for (i=0;i<Rel_nodes[level-1][join_node]->i_relations.size();i++)
	{
		for (j=0;j<cand_rels.size();j++)
		{
			count = 0;
			for (k=1;k<CID_num+1;k++)
			{
				if ( (Rel_nodes[level-1][join_node]->cid_index[i][k] == 1 && cand_rels_cids[j][k]) == 1)
					count++;
			}

			if (count >= min_sup_th)
			{
				for (k=0;k<CID_num+1;k++)
				{
					if (k == 0)
						tmp2.push_back (count);
					else
					{
						if ( (Rel_nodes[level-1][join_node]->cid_index[i][k] == 1 && cand_rels_cids[j][k]) == 1)
							tmp2.push_back(1);
						else
							tmp2.push_back(0);
					}
				}

				tmp1.clear();
			
				for (k=0;k<Rel_nodes[level-1][join_node]->i_relations[i].size();k++)
					tmp1.push_back(Rel_nodes[level-1][join_node]->i_relations[i][k]);

				for (k=0;k<cand_rels[j].size();k++)
					tmp1.push_back(cand_rels[j][k]);


				// last check

				check = cand_check(tmp_items, new_rel->rel_nodes_ptr, tmp1, tmp2);

				

				new_rel->cid_index.push_back(tmp2);

				tmp2.clear();

				new_rel->i_relations.push_back(tmp1);

				tmp1.clear();

			}
		}
	}


	if (new_rel->i_relations.size() > 0)
	{
		if (Rel_nodes.size() < level+1)
		{
			empty_rel = new Rel_node();
			tmp_rel.push_back(empty_rel);
			Rel_nodes.push_back(tmp_rel);

		}

		Rel_nodes[level].push_back(new_rel);
	}
	else
		return -1;

	for (i=0;i<perm_size;i++)
		delete[] array[i];

	delete[] array;

	delete[] N;

	new_pairs.clear();
	tmp1.clear();
	tmp2.clear();
	tmp3.clear();

	new_node_rels_filt.clear();
	new_node_rels_filt_types.clear();

	new_pairs.clear();

	cand_rels.clear();
	cand_rels_cids.clear();

	return 1;
}


void Rel_nodes_init(void)
{
	vector<Rel_node *> q;
	Rel_node *p = new Rel_node();

	q.push_back(p);

	Rel_nodes.push_back(q);
	Rel_nodes.push_back(q);
}

int mine_arrangements()
{
	int i, j, k;
	int level, item_i, node_i;
	int status;
	int found = 0;

	level = 3;
	node_i = 1;
	item_i = 0;

	clock_t ts = clock();

	while(1)
	{	
		puts("performing join...");
		printf("level: %d    node: %d     item: %d\n",level, node_i, f1_items[item_i]);

		status = make_join(level, item_i, node_i);

		if (status == 1)
			found++;

		item_i = item_i + 1;

		if (item_i == f1_items.size())
		{
			node_i = node_i+1;
			item_i = 0;
		}

		if (node_i == Rel_nodes[level-1].size())
		{
			node_i = 1;
			level = level + 1;

			if (found == 0)
				break;
			
			found = 0;
		}
	}

	clock_t te = clock();
	updateTime += static_cast<double>(te - ts);

	return 0;

}



int find_2relations(int i_size, int t_size, int f1_size, char *filename)
{
	int a, b;			// used to scan the attributes in the f2_array
	int i, j, k;
	int cand_f2_size;
	int f2_size = 0;
	int f1_line = 0;
	int f2_line = 0;
	int count = 0;

	pair_node new_p;

	cand_f2_size = f1_size*f1_size;

	cand_f2_array = new int *[cand_f2_size];

	for (i=0;i<cand_f2_size;i++)
		cand_f2_array[i] = new int[F2_COLS];	

	for (i=0;i<cand_f2_size;i++)
		for (j=0;j<F2_COLS;j++)
			cand_f2_array[i][j] = 0;

	f2_line = 0;

	for (i=0;i<f1_size;i++)
	{
		for (j=0;j<f1_size;j++)
		{
			cand_f2_array[f2_line][0] = f1_array[i][0];
			cand_f2_array[f2_line][1] = f1_array[j][0];
			f2_line++;
		}
	}
		
	//print_cand_f2_array(cand_f2_size);

	f2_size = 0;

	// Scan the database for all possible Meets

//	printf("Finding meets...");

	clock_t ts = clock();


	for (i=0;i<cand_f2_size;i++)
		cand_f2_array[i][2] = Meet(0, cand_f2_array[i][0], cand_f2_array[i][1], i_size);

	clock_t te = clock();
	updateTime += static_cast<double>(te - ts);		

//	printf("...done!\n");

	// Count support and detect frequent sequences on array.  Each frequent sequence i should have f2_array[i][4] = 1


	for (i=0;i<cand_f2_size;i++)
	{
		if ((float)cand_f2_array[i][2]/(float)CID_num >= min_sup)
		{
			cand_f2_array[i][3] = 1;
			cand_f2_array[i][F2_COLS-1] = 1;
		}
	}

//	print_f2_array(f2_size);

	// Scan the database for all possible Matches

//	printf("Finding matches...");

	ts = clock();

	for (i=0;i<cand_f2_size;i++)
		cand_f2_array[i][4] = Match(0, cand_f2_array[i][0], cand_f2_array[i][1], i_size);
	
	te = clock();
	updateTime += static_cast<double>(te - ts);	

//	printf("...done!\n");


	// Count support and detect frequent sequences on array.  Each frequent sequence i should have f2_array[i][4] = 1


	for (i=0;i<cand_f2_size;i++)
	{
		if ((float)cand_f2_array[i][4]/(float)CID_num >= min_sup)
		{
			cand_f2_array[i][5] = 1;
			cand_f2_array[i][F2_COLS-1] = 1;
		}
	}

//	print_f2_array(f2_size);



	// Scan the database for all possible Overlaps

//	printf("Finding overlaps...");

	ts = clock();
	
	for (i=0;i<cand_f2_size;i++)
		cand_f2_array[i][6] = Overlap(0, cand_f2_array[i][0], cand_f2_array[i][1], i_size);
	
	 te = clock();
	updateTime += static_cast<double>(te - ts);		
//	printf("...done!\n");


	// Count support and detect frequent sequences on array.  Each frequent sequence i should have f2_array[i][4] = 1


	for (i=0;i<cand_f2_size;i++)
	{
		if ((float)cand_f2_array[i][6]/(float)CID_num >= min_sup)
		{
			cand_f2_array[i][7] = 1;
			cand_f2_array[i][F2_COLS-1] = 1;
		}
	}

//	print_f2_array(f2_size);


	// Scan the database for all possible Contains

//	printf("Finding contains...");

	
	ts = clock();
	
	for (i=0;i<cand_f2_size;i++)
		cand_f2_array[i][8] = Contain(0, cand_f2_array[i][0], cand_f2_array[i][1], i_size);

	
	te = clock();
	updateTime += static_cast<double>(te - ts);		

//	printf("...done!\n");

	// Count support and detect frequent sequences on array.  Each frequent sequence i should have f2_array[i][4] = 1


	for (i=0;i<cand_f2_size;i++)
	{
		if ((float)cand_f2_array[i][8]/(float)CID_num >= min_sup)
		{
			cand_f2_array[i][9] = 1;
			cand_f2_array[i][F2_COLS-1] = 1;
		}
	}

	//	print_f2_array(f2_size);


	// Scan the database for all possible Sequents

//	printf("Finding sequents...");

	ts = clock();

	for (i=0;i<cand_f2_size;i++)
		cand_f2_array[i][10] = Sequent(0, cand_f2_array[i][0], cand_f2_array[i][1], i_size);

	te = clock();
	updateTime += static_cast<double>(te - ts);	

//	printf("...done!\n");


	// Count support and detect frequent sequences on array.  Each frequent sequence i should have f2_array[i][4] = 1


	for (i=0;i<cand_f2_size;i++)
	{
		if ((float)cand_f2_array[i][10]/(float)CID_num >= min_sup)
		{
			cand_f2_array[i][11] = 1;
			cand_f2_array[i][F2_COLS-1] = 1;
		}
	}

//	print_f2_array(f2_size);


	
	// Scan the database for all possible ContainLs

//	printf("Finding contains...");

	
	ts = clock();
	
	for (i=0;i<cand_f2_size;i++)
		cand_f2_array[i][12] = ContainL(0, cand_f2_array[i][0], cand_f2_array[i][1], i_size);

	
	te = clock();
	updateTime += static_cast<double>(te - ts);		

//	printf("...done!\n");

	// Count support and detect frequent sequences on array.  Each frequent sequence i should have f2_array[i][4] = 1


	for (i=0;i<cand_f2_size;i++)
	{
		if ((float)cand_f2_array[i][12]/(float)CID_num >= min_sup)
		{
			cand_f2_array[i][13] = 1;
			cand_f2_array[i][F2_COLS-1] = 1;
		}
	}

	
	// Scan the database for all possible ContainRs

//	printf("Finding contains...");

	
	ts = clock();
	
	for (i=0;i<cand_f2_size;i++)
		cand_f2_array[i][14] = ContainR(0, cand_f2_array[i][0], cand_f2_array[i][1], i_size);

	
	te = clock();
	updateTime += static_cast<double>(te - ts);		

//	printf("...done!\n");

	// Count support and detect frequent sequences on array.  Each frequent sequence i should have f2_array[i][4] = 1


	for (i=0;i<cand_f2_size;i++)
	{
		if ((float)cand_f2_array[i][14]/(float)CID_num >= min_sup)
		{
			cand_f2_array[i][15] = 1;
			cand_f2_array[i][F2_COLS-1] = 1;
		}
	}

	f2_size = 0;

	puts("f2 array");

	for (i=0;i<cand_f2_size;i++)
	{
		if (cand_f2_array[i][F2_COLS-1] == 1)
			f2_size++;
		for (j=0;j<F2_COLS;j++)
			printf("%d ",cand_f2_array[i][j]);
		puts("");
	}

	printf("size %d\n",f2_size);

//	drop_infrequent_pairs();

	store_f2_array(cand_f2_size, filename);

	// Create f2_array

	f2_array = new int *[f2_size];

	for (i=0;i<f2_size;i++)
		f2_array[i] = new int[F2_COLS];
	
	f2_line = 0;

	for (i=0;i<cand_f2_size;i++)
	{
		if (cand_f2_array[i][F2_COLS-1] == 1)
		{
			for (j=0;j<F2_COLS;j++)
				f2_array[f2_line][j] = cand_f2_array[i][j];
			f2_line++;
		}
	}

	delete_cand_f2_array(cand_f2_size);

//	puts("DONE with f2");

	return f2_size;
}

void insert_single(int cid, int item, int start, int end)
{
	int i, j, k;

	Single *new_sing;

	new_sing = new Single();

	new_sing->cid = cid;
	new_sing->label = item;
	new_sing->Start = start;
	new_sing->End = end;

	Singles.push_back(new_sing);
}

int find_1relations(int i_size, int t_size)
{
	int a, b;			// used to scan the attributes in the f_array
	int i, j;	
	int count;			// counts frequent sequences to be returned by function
	int tmp_size;
	int tmp_cols = 4;

	// Initialize f1_array

	cand_f1_array = new int *[t_size];

	for (i=0;i<t_size;i++)
		cand_f1_array[i] = new int[F1_COLS];

	for (i=0;i<t_size;i++)
	{
		cand_f1_array[i][0] = ti_array[i];

		for (j=1;j<F1_COLS;j++)
			cand_f1_array[i][j] = 0;
	}

	// Scan Database for frequent 1-i-sequences

	for (i=0;i<i_size;i++)
	{
		if (cand_f1_array[i_array[i][1]-1][2] != i_array[i][0])
		{
			cand_f1_array[i_array[i][1]-1][1]++;
			cand_f1_array[i_array[i][1]-1][2] = i_array[i][0];
		}
	}

	// Scan f1_array and find frequent 1-sequences according to min_sup

	count = 0;
	for (i=0;i<t_size;i++)
	{
		if ((float)cand_f1_array[i][1]/(float)CID_num >= min_sup )
		{
		//	printf("%d %d %f \n",f1_array[i][1],CID_num, (float)f1_array[i][1]/(float)CID_num);
			count++;
			cand_f1_array[i][3] = 1;
		}
	}		

//	printf("%d\n",count);
	f1_array = new int *[count];

	for (i=0;i<count;i++)
		f1_array[i] = new int[F1_COLS];

	count = 0;

	for (i=0;i<t_size;i++)
	{
		if (cand_f1_array[i][3] == 1 )
		{
			for (j=0;j<F1_COLS;j++)
				f1_array[count][j] = cand_f1_array[i][j];
			count++;
		}
	}		

	for (i=0;i<count;i++)
	{
		for (j=0;j<i_size;j++)
		{
			if (i_array[j][1] == f1_array[i][0])
				insert_single(i_array[j][0], i_array[j][1], i_array[j][2], i_array[j][3]);
		}
	}


	store_f1_array(count);

	delete_cand_f1_array(count);

	return count;
}


int read_support(float support)
{
	if (support > 1 || support < 0)
		return 1;

	min_sup = support;

	return 0;
}

int load_ifile(char *filename)
{
	// Structure of ifile:
	// First line: Number of records
	// Then: 
	// CID  X START END
	//	1	1	 1	  4
	//  1   1    8    14
	//		...
	// CID:		Customer ID (In our case Utterance Number)
	// TID:		Item
	// START:	Start time of Interval
	// END:		End time of Interval

	FILE *fp;

	int N;		// variable used to represent the number of CIDs, i.e. records in the file
	int i;
	int j;
	int current_cid;
	int lines = 0;

	char c;

	if ( (fp = fopen(filename, "r")) == NULL)
		return -1;

	while ( (c = fgetc(fp)) != EOF)
	{
		if (c == '\n')
			lines++;
	}

	rewind(fp);

	N = lines;

	strcpy(Input_File,filename);

	
	// Initialize i_array

	i_array = new int *[N];

	for (i=0;i<N;i++)
		i_array[i] = new int[I_COLS];
	
	// Initialize CID_num to 1, the code below starts counting fromt he second different CID
	CID_num = 1;

	// Read file and update i_array
//	printf("%d\n",N);
	for (i=0;i<N;i++)
	{
		fscanf (fp, "%d %d %d %d",&i_array[i][0], &i_array[i][1], &i_array[i][2], &i_array[i][3]);
		
		if (i==0)
			current_cid = i_array[i][0];

		if (current_cid != i_array[i][0])
		{
			current_cid = i_array[i][0];
			CID_num++;
		}
	}
	CID_num = i_array[N-1][0];
	//printf("CID_num = %d\n",CID_num);		
	
	fclose(fp);


	// Print i_array

//	print_i_array(N, 4);
    
	return N;
}



int load_tfile(char *filename)
{
	// Structure of ifile:
	// First line: Number of records
	// Then: 
	// CODE TYPE
	//	1	 A
	//  2    B
	//	  ...
	// CODE:	Number used to encode type in the input file
	// TYPE:	Actual type

	FILE *fp;

	int N;		// variable used to read file. Represents the number of types, i.e. records in the file
	int i;
	int j;
	int k;
	int lines = 0;

	char c;
	char buf[MAX]; 

	if (  (fp = fopen(filename, "r")) == NULL  )
		return -1;


	while ( (c = fgetc(fp)) != EOF)
	{
		if (c == '\n')
			lines++;
	}

	rewind(fp);

	N = lines;


	ti_array = new int[N];

	t_array = (char **)malloc(N*sizeof(char *));

	for (i=0;i<N;i++)
		t_array[i] = (char *)malloc(MAX*sizeof(char));

	// Read file and update t_array

	for (i=0;i<N;i++)
	{
			k=0;
			fscanf (fp, "%d", &ti_array[i]);
			
			while((c = fgetc(fp))!='\n')
				buf[k++] = c;
			
			buf[k] = '\0';	
			strcpy(t_array[i],buf);
			
	}
	fclose(fp);

	// Print t_array

	//print_t_array(N, 1);
	
	return N;
}


int log(void)
{
	int a, b;					// used to scan the attributes in the f2_array
	int i, j, k, l, m, n, p, n1, n2, fn;
	int K;

	int f3_cols;

	int level;
	int count = 0;

	FILE *fp3;

//	level = 2;

//	puts("Saving....");
	for (level=2;level<Rel_nodes.size();level++)
	{
		for (i=1;i<Rel_nodes[level].size();i++)
		{
			for (j=0;j<Rel_nodes[level][i]->i_relations.size();j++)
			{
				for (k=0;k<Rel_nodes[level][i]->i_relations[j].size();k++)
				{
					count++;
				}
			}
		}
	}

	printf("found %d relations\n",count);

}


int store_nrelations(void)
{
	int a, b;					// used to scan the attributes in the f2_array
	int i, j, k, l, m, n, p, n1, n2, fn;
	int K;

	int f3_cols;

	int item1, item2;
	int count = 0;
	int v_count = 0;
	int f_count = 0;
	int count2 = 0;
	int counter = 0;
	int v1, v2, v3;
	int rel_cols = 3;
	int t1, t2, t3;
	int found = 0;
	int cand_count = 0;
	int entries = 0;
	int sup_count = 0;
	int new_support = 0;
	static int cand_size = 1;
	char str[15] = "relations.txt";
	char relations[15];
	char relationsb[15];
	int unique = 0;
	int level = 3;
	int sup;

	FILE *fp3;

//	level = 2;

//	puts("Saving....");
	for (level=2;level<Rel_nodes.size();level++)
	{
		sprintf(relations,"%d",level);
		strcat(relations,str);

		//printf("file  %s\n",relations);
		fp3 = fopen(relations,"w");

		for (i=1;i<Rel_nodes[level].size();i++)
		{
			for (j=0;j<level;j++)
			{
				fprintf(fp3, "%d ",Rel_nodes[level][i]->items[j]);
			}

			fprintf(fp3,"\n");

		//	printf("rel size of %d %d \r",i,Rel_nodes[level][i]->i_relations.size());

			for (j=0;j<Rel_nodes[level][i]->i_relations.size();j++)
			{
				sup = 0;
				for (k=1;k<CID_num+1;k++)
					sup = sup + Rel_nodes[level][i]->cid_index[j][k];

				if (sup < min_sup_th)
					continue;

				fprintf(fp3,"( ");
				for (k=0;k<Rel_nodes[level][i]->i_relations[j].size();k++)
				{
					fprintf(fp3,"%d ",Rel_nodes[level][i]->i_relations[j][k]);
				}
				fprintf(fp3,") ");

				fprintf(fp3,"%.2f %% ",(float)sup*100/(float)CID_num);

				count++;
		
				fprintf(fp3,"\n");
			}
		}
		fclose(fp3);
	}

	return count;

}

int main(int argc, char *argv[])
{
	int t_size;		// size of t_array
	int i_size;		// size of i_array

	int f1_size;

	int f2_size;

	int K,i,j, k, l, m, n;

	int fn_size = 1;

	int item_i;

	int node_i;

	int rel_size = 3;

	int level = 3;

	char command[20] = "mkdir ";

	if(check_arguments(argc))
		return 1;

	if ( read_support( atof(argv[3]) ) )
		error("Bad support number! Should be between 0 and 1");

	if ( (t_size = load_tfile(argv[2])) < 0)
		error("Type File Error");

	if ( (i_size = load_ifile(argv[1])) < 0)
		error("Input File Error");

	if ( strcpy(directory, argv[4]) == 0) 
		error("Output directory Error");

	min_sup_th = (int) (min_sup*CID_num);

	printf("cids: %d      min_sup: %f    \n",CID_num,min_sup);
	
	// Create directory of results

	strcat(command, directory);
	system(command);

	chdir(directory);	

	if ( (f1_size = find_1relations(i_size, t_size)) < 0)
		error("Error in finding 1-i-sequences");

	Rel_nodes_init();

	if ( f1_size != 0 )
	{
		if ( (f2_size = find_2relations(i_size, t_size, f1_size, relations_2)) < 0)
			error("Error in finding 2-e-sequences");
	}


	puts("Printing Pair_nodes ...");

	for (i=0;i<Pair_nodes.size();i++)
	{
		printf("a = %d   b = %d\n",Pair_nodes[i]->_a,Pair_nodes[i]->_b);

		printf("actual supports: ");
		for (j=0;j<TYPES+1;j++)
			printf("%d ",Pair_nodes[i]->actual_sup[j]);

		puts("");

		printf("total supports: ");
		for (j=0;j<TYPES+1;j++)
			printf("%d ",Pair_nodes[i]->total_sup[j]);

		printf("\ntotal: %d     relations[0]: %d\n",Pair_nodes[i]->total, Pair_nodes[i]->relations[0]); 

		puts("");

		for (j=1;j<CID_num+1;j++)
		{
			for (k=0;k<Pair_nodes[i]->cid_index[j].size();k++)
			{
				printf("->%d %d %d %d\n",Pair_nodes[i]->cid_index[j][k]->_a, Pair_nodes[i]->cid_index[j][k]->_b, Pair_nodes[i]->cid_index[j][k]->_type, Pair_nodes[i]->cid_index[j][k]->_cid);
			}
			puts("new cid:");
		}
		puts("");
	}


	if (f2_size > 0)
		create_2Rel_array(f2_size);
	else
	{
		return -1;
	}

	puts("\nREL_NODES:");

	for (i=0;i<Rel_nodes.size();i++)
	{
		for (j=1;j<Rel_nodes[i].size();j++)
		{
			printf("items: %d %d\n",Rel_nodes[i][j]->items[0], Rel_nodes[i][j]->items[1]);

			printf("\nREL_CID: ");

			for (k=0;k<TYPES+1;k++)
				printf("%d ",Rel_nodes[i][j]->rel_cid[k]);
			puts("");

			printf("father: %d\n",Rel_nodes[i][j]->father->relations[0]);

			printf("\ni_relations:");

			for (k=0;k<Rel_nodes[i][j]->i_relations.size();k++)
				printf("%d ", Rel_nodes[i][j]->i_relations[k][0]);
			puts("");

			puts("cid_index:");

			for (k=0;k<Rel_nodes[i][j]->cid_index.size();k++)
			{
				for (l=0;l<Rel_nodes[i][j]->cid_index[k].size();l++)
					printf("%d ",Rel_nodes[i][j]->cid_index[k][l]);
				puts("");
			}
			puts("");

			puts("agmt_ints");

			for (k=0;k<Rel_nodes[i][j]->agmt_ints.size();k++)
			{
				for (l=0;l<Rel_nodes[i][j]->agmt_ints[k].size();l++)
				{
					for (m=0;m<Rel_nodes[i][j]->agmt_ints[k][l]->ptr.size();m++)
					{
						for (n=0;n<Rel_nodes[i][j]->agmt_ints[k][l]->ptr[m]->arg_ints.size();n++)
							printf("%d (%d %d) %d (%d %d) type: %d  cid: %d\n",Rel_nodes[i][j]->agmt_ints[k][l]->ptr[m]->arg_ints[n]->_a, Rel_nodes[i][j]->agmt_ints[k][l]->ptr[m]->arg_ints[n]->Start_a, Rel_nodes[i][j]->agmt_ints[k][l]->ptr[m]->arg_ints[n]->End_a, Rel_nodes[i][j]->agmt_ints[k][l]->ptr[m]->arg_ints[n]->_b, Rel_nodes[i][j]->agmt_ints[k][l]->ptr[m]->arg_ints[n]->Start_b, Rel_nodes[i][j]->agmt_ints[k][l]->ptr[m]->arg_ints[n]->End_b, Rel_nodes[i][j]->agmt_ints[k][l]->ptr[m]->arg_ints[n]->_type, Rel_nodes[i][j]->agmt_ints[k][l]->ptr[m]->arg_ints[n]->_cid ) ;
						puts("");
					}
				}
			}
		}
	}

	if ( f2_size != 0 )
	{
		if ( (fn_size = mine_arrangements()) < 0)
			error("Error in finding n-e-sequences");
	}


	printf("Found %d relations.\n",store_nrelations());


	delete_i_array(i_size);
	delete_f1_array(f1_size);
	delete_f2_array(f2_size);

	destroy_Pair_node();

	std::cout << "Seconds: " << updateTime / 
	static_cast<double>(CLOCKS_PER_SEC) << std::endl;
	
	return 0;
}
