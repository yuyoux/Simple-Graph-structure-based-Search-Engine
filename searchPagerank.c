#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "InvertedIdx.h"

#define INIT_NUM -1

int num = 0; //#url in pagerankList
//int num2 = 0; //#node in tree for invertedIndex
int *countofwords;

typedef struct PRnode{ //e.g: url131,0.2623549
   char* url;
   double PR;
} PRnode;

PRnode *array; //collections to store the related PR info

int findindex(char* ch);
bool checkUrl(char* ch);

void getA(){
	FILE *fp;
	char buffer1[1000];//e.g: url131
	char buffer2[1000];//e.g: 3
	char buffer3[1000];//e.g: 0.2623549
	char *b;
	char *ptr;
	int len;

	fp = fopen("./pagerankList.txt", "r");
	while (!feof(fp)){
		fscanf(fp, "%s", buffer1);
		fscanf(fp, "%s", buffer2);
		fscanf(fp, "%s", buffer3);
		
		len = strlen(buffer1);
		b = malloc(len*sizeof(char));

		array = realloc(array, (num+1)*sizeof(PRnode));

		array[num].url = b;
		memcpy(array[num].url, buffer1, len*sizeof(char));
		
		array[num].url[len-1] = '\0';
		array[num].PR = strtod(buffer3, &ptr);
		num++;
		//printf("666 %s %f\n", array[num-1].url, array[num-1].PR);
	}
	num -= 1;
	fclose(fp);
}

void getcountofwords(){ //an array to store the count of url with initial value zero
	int i;
	countofwords = malloc(num*sizeof(int));
	for (i = 0; i < num; i++){
		countofwords[i] = 0;
	}
}

Tree getB(){
	Tree t = newTree();
	FILE *fp2;
	char buffer4[1000];
	char *ptr;
	char *delim = " ";
	char temp[1000];//store the word to help create tree
	
	fp2 = fopen("./invertedIndex.txt", "r");
	
	while (!feof(fp2)){
		int num2 = 0;
		fgets(buffer4, sizeof(buffer4) - 1,fp2);
		//get the first ptr
		char *s = strdup(buffer4);
		ptr = strsep(&s, delim);
		strcpy(temp, ptr);
		//printf("%s\n",temp);
		
		//continue, create the search tree of these words
		while ((ptr = strsep(&s, delim))!= NULL){
			//printf("%s\n",ptr);
			if (checkUrl(ptr)){
				//printf("%d----%s\n",findindex(ptr),ptr);
				t = TreeInsert(t, temp, findindex(ptr));
			}
		}
		num2 ++;
	}

	//showTree(t);
	fclose(fp2);
	return t;
}

bool checkUrl(char* ch){
	if (strstr(ch, "url")){
		return true;
	}else{
		return false;
	}
}

int findindex(char* ch){
	int id = 0;
	int len;
	len = strlen(ch);
	while (id < num ){
		if (strchr(ch, '\n')){
			ch[len - 1] = '\0';
		}
		if (strcmp(ch, array[id].url) == 0){
			return id;
		}
		id ++;
	}
	return 0;
}

void RemoveBufferRepNum(int buffer[]){
    int i,j;
    for(i=0;i<num;i++){
       for(j = i+1;j<num;j++){
           if(buffer[i] == buffer[j]){
              buffer[i] = INIT_NUM;
              break;
           }
       }
    }  
    for(i=0,j=0;i<num;i++){
       if(buffer[i] == INIT_NUM)
           continue;
       buffer[j++] = buffer[i];
    } 
    while(j < num)
        buffer[j++] = INIT_NUM; 
}


int main(int argc, char *argv[]){
	int id;
	int i;
	int j;
	int f;
	int swap;//for bubble sort
	getA();
	getcountofwords();
	Tree t = getB();
	int sort_set[num];
	//int sort_set2[num];
	//int length;
	//length = num;

	if (argc > 1){
		int counter;
		for (counter = 1; counter < argc; counter++){ //use tree search to find the word, calculate the counts of url-using
			if (TreeSearch(t, argv[counter]) != NULL){
				//showcurrent(TreeSearch(t, argv[counter]));
				for (id = 0; id < TreeSearch(t, argv[counter])->count ; id++){
					countofwords[TreeSearch(t, argv[counter])->url[id]] ++;
				}
			}
		}
	}

	//sort the counts of url first
	memcpy(sort_set, countofwords, num*sizeof(int));
	
	for (i = 0; i < num-1; i++){ //bubble sort
		for (j = 0; j < num-1; j++){
			if (sort_set[j]	< sort_set[j+1]){
				swap = sort_set[j];
				sort_set[j] = sort_set[j+1];
				sort_set[j+1] = swap;
			}
		}
	}

	RemoveBufferRepNum(sort_set); //sorting - remove duplicated value

	//for test of countofwords
	/*for (f = 0; f<num;f++){
			//printf("%d\n", countofwords[i]);
			printf("p");
			printf("%d\n", sort_set[f]);
	}*/

	//output the results
	for (f = 0; f < 30 && array[f].PR > 0; f ++){
		if(sort_set[f] != -1 && sort_set[f] != 0){
			for (j = 0;j<num;j++){
				if (sort_set[f] == countofwords[j]){
					printf("%s\n", array[j].url);
				}
			}
		}
		
	}

	return 0;
}
