#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "InvertedIdx.h"

char *normalise(char *str);
int findindex(char* ch);
char **array;
int num = 0;

Tree readData2(){
	Tree t = newTree();

	char buffer[1000];
	char buffer2[1000];
	char *b;
	int i;
	int flag;
	int flag2;

	char urlNumber[1000]; //store e.g: url11
	char urlTxt[1000]; //store e.g: url11.txt

	//int num = 0;
	int len;

	FILE *fp;
	FILE *fp2;

	fp = fopen("./collection.txt", "r");
	fscanf(fp, "\n");
	while (!feof(fp)){
		fscanf(fp, "%s", buffer);
		//printf("%s\n",buffer);
		len = strlen(buffer);
		b = malloc(len*sizeof(char));
		array = realloc(array, (num+1)*sizeof(char*));
		array[num] = b;
		memcpy(array[num], buffer, (len+1)*sizeof(char));
		num ++;
	}
	num -= 1;
	fclose(fp);

	for (i = 0; i < num; i++){	
		sprintf(urlNumber, "%s", array[i]);
		sprintf(urlTxt, "./%s.txt", array[i]);

		//open current file and scan
		fp2 = fopen(urlTxt, "r");
		flag = 0;
		while (!feof(fp2)){
			flag2 = fscanf(fp2, "%s ", buffer2);

			if (flag2 < 0){
				continue;
			}
			if (strcmp(buffer2, "Section-2") == 0){
				flag = 1;
				
			}
			//finish section-2
			if (strcmp(buffer2, "#end") == 0 && flag == 1){
				break;		
			}
			//in section-2
			if (flag == 1 && strcmp(buffer2, "#end") != 0){
				normalise(buffer2);
				//avoid the load of word:"Section-2"
				if (strcmp(buffer2, "section2") != 0){
					t = TreeInsert(t, buffer2, i);
				}
			}
			
		}
		fclose(fp2);
	}
	return t;
}

int findindex(char* ch){
	int id = 0;
	for (id = 0; id < num; id++){
		if (strcmp(array[id], ch) == 0){
			break;
		}
	}
	return id;
}

char* normalise(char *str){
	int count, i;
	char vector[100];
	i = 0;
	count = 0;
	//upper to lower
	for(i = 0; str[i] != '\0';i++){
		if (str[i] >= 'A' && str[i] <= 'Z'){
			str[i] += 32;
		}
	}
	
	i = 0;
	count = 0;
	//remove the punctuation marks at the end part
	while (str[i] != '\0'){
		if (isalnum(str[i]) != 0){
			vector[count] = str[i];
			count ++;
		}
		i ++;
	}
	vector[count] = '\0';
	strcpy(str, vector);
	return str;
}


void showTreeNode(Tree t){
	FILE *fp3 = fopen("invertedIndex.txt", "a");
	int idx = 0;
	fprintf(fp3,"%s", t->data);
	for (idx = 0;idx < t->count;idx++){
		fprintf(fp3," %s", array[t->url[idx]]);
	}
	fprintf(fp3,"\n");
	fclose(fp3);
}


//print in infix order
void TreeInfix(Tree t){
	if (t == NULL){return;}
	TreeInfix(t->left);
	showTreeNode(t);
	TreeInfix(t->right);
}


int main(){
	Tree t = readData2();
	//printf("%d, %s\n", t->count, t->data);
	TreeInfix(t);
	return 0;
}
