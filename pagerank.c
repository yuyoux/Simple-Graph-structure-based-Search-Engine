#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "Graph.h" // Adjacency Matrix Representation ... COMP9024

char **collections; //array used to store the index and the related url file
int num = 0; //number of the url files in collections
int *outlinks; //array used to store #outdegree
double *sortedPR;

int findindex(char* ch);
int findindex2(double* arr, double a);
void pageRank(double d, double diffPR, int maxIterations);

//read data
void readData(){
	FILE *fp;
	char buffer[1000];
	char *b;
	int len;
	fp = fopen("./collection.txt", "r");
	while (!feof(fp)){
		fscanf(fp, "%s", buffer);
		//printf("%s\n",buffer);
		len = strlen(buffer);
		b = malloc(len*sizeof(char));
		//assert(b != NULL);	
		collections = realloc(collections, (num+1)*sizeof(char*));
		collections[num] = b;
		memcpy(collections[num], buffer, (len+1)*sizeof(char));
		num++;
	}
	num -= 1;
	fclose(fp);
}


//build graph
Graph GetGraph(){
	int i;
	FILE *fp2;
	Graph g = newGraph(num);
	char ptr[1000];
	int flag = 0;
	char buffer2[1000];
	int outDegree;
	int index;
	outlinks = malloc(num*sizeof(int));

	for (i = 0; i < num; i++){
		sprintf(ptr,"./%s.txt",collections[i]);
		//printf("%s\n",ptr);
		fp2 = fopen(ptr, "r");
		flag = 0;
		//assert(fp2 != NULL);
		outDegree = 0;

		while (!feof(fp2)){
			if (flag == 0){
				flag = fscanf(fp2, "#start Section-1 %s", buffer2);
				//printf("%s\n",buffer2);
				outDegree ++;
			}
			else{
				fscanf(fp2, "%s", buffer2);
				//printf("%s\n",buffer2);	
				outDegree ++;
			}
			if (strcmp(buffer2, "#end") == 0){
				break;		
			}
			//convert url (buffer2) to index (int)
			index = findindex(buffer2);
			Edge e;
			e.v = i;
			e.w = index;
			insertEdge(g, e);
			
		}
		outDegree -= 1;
		outlinks[i] = outDegree;
		//printf("%d\n",outDegree);
		//printf("%d\n",outlinks[i]);
		fclose(fp2);
	}
	return g;
}

int findindex(char* ch){
	int id = 0;
	for (id = 0; id < num; id++){
		if (strcmp(collections[id], ch) == 0){
			break;
		}
	}
	return id;
}

int findindex2(double* arr, double a){
	int idx = 0;
	for (idx = 0;idx<num;idx++){
		if (arr[idx] == a){
			break;	
		}
	}
	return idx;
}

//calculate graph
void pageRank(double d, double diffPR, int maxIterations){
	Graph graph = GetGraph();
	//printf("d = %1f, diffPR = %1f\n", d, diffPR);

	int iteration = 0;
	double diff = diffPR;
	double N = graph ->nV ;
	int total = graph ->nV;
	double sum = 0;
	double swap;//for bubble sort

	//printf("total num = %d\n", total);
	//printf("A8number of vertex = %1f\n", N);

	double oldPR[total]; //array to store the PR in previous iteration
	double newPR[total]; //array to store the PR in new iteration

	int i = 0; 
	int j = 0; 
	int k = 0; 
	int l = 0;

	while (i < total){ //give intial PR to every url
		oldPR[i] = 1/N;
		newPR[i] = 0;
		i++;
	}

	while (iteration < maxIterations && diff >= diffPR){
		iteration ++;
		for (i = 0; i < total; i++){
			sum = 0;
			for (j = 0; j <total;j++){
				if (graph ->edges[j][i]){
					sum += oldPR[j]/outlinks[j];
				}
			}
			newPR[i] = (1.0 - d)/N + d*sum;		
		}
		for (i = 0; i < total;i++){
			diff += fabs(newPR[i] - oldPR[i]);
		}
		l = 0;
		while (l<total){
			oldPR[l] = newPR[l]; //store the PR value
			newPR[l] = 0;
			l ++;
		}
	}

	//bubble sort..
	sortedPR = malloc(num*sizeof(double));
	k = 0;
	while (k < total){
		sortedPR[k] = oldPR[k];
		k ++;
	}
	for (i = 0; i<num; i++){
		for (j = 0; j<num; j++){
			if (sortedPR[j]	< sortedPR[j+1]){
				swap = sortedPR[j];
				sortedPR[j] = sortedPR[j+1];
				sortedPR[j+1] = swap;
			}
		}
	}
	//for (i = 0; i < total; i++){
	//	printf("%f\n", sortedPR[i]);
	//}

	//store the number of links
	int Noflinks[total];
	i = 0;
	k = 0;
	int counter = 0;
	while (k < total){
		while (i < total){
			if (graph ->edges[k][i] == 1){
				counter = counter + 1;
			}
			i ++;		
		}
		Noflinks[k] = counter;
		i = 0;
		counter = 0;
		k ++;
	}

	//writing to the txt file
	FILE *fp3;
	fp3 = fopen("./pagerankList.txt","w");
	k = 0;
	while (k < num){
		fprintf(fp3,"%s, %d, %.7lf\n", collections[findindex2(oldPR,sortedPR[k])], Noflinks[findindex2(oldPR,sortedPR[k])],sortedPR[k]);
		k ++;
	}
}



int main(int argc, char* argv[]){
	readData();
	//Graph g = GetGraph();
	//showGraph(g);

	double d = atof(argv[1]);
	double diffPR = atof(argv[2]);
	int maxIterations = atoi(argv[3]);
	pageRank(d, diffPR, maxIterations);

	//int i,j;
	//for (i = 0; i<num;i++){
	//	for (j = 0; j<num;j++){
	//		printf("%s\n", collections[i]);
	//	}
	//}
	return 0;
}








