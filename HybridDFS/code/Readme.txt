to commpile: 
	make

to run program:
	intervals <input database> <input events types> <minimum support> <directory of results>

	1. input database: should be of the following format:
	   SID ITEM START END
	   SID: the e-sequence id
	   ITEM: the code (integer) of the event label
	   START: the start time of the event interval
	   END: the end time of the event interval	
	2. input event types: 
	   file that contains the mapping of the event labels and the codes (integer representations) used
	   for the event labels in the database. 
	3. minimun support"
  	   the minimum support threshold in %
	4. directory of results:
	   the name of the directory where the frequent arrangements will be stored.

understanding the results:

	The output directory will contain a file for each set of k-arrangements. Each file is called: "krelations.txt"
	where k = 2,3,4, ...
	
	here is an example of an arrangement:
	
	1 2 3 
	( 1 5 5 ) 3    50.00 % 

	the first line contains set E of the event labels: in this case {1, 2, 3}
	and the second line contains set R as defined in the paper.

	The types of relations are:
	MEET 1  
	MATCH 2  
	OVERLAP 3  
	CONTAIN 4  
	FOLLOW 5  
	CONTAINL 6  
	CONTAINR 7
