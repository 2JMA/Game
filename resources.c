#include "resources.h"
#include <string.h>
#include <stdio.h>

struct _Resource{
	/*This doesn't implies that max>min, the resources can have negative effects*/
	char* name;
	int type;
	int max;
	int min;
	int value;
};

resource* iniResource(char* name, int type, int max, int min, int value){
	resource *r;
	if(type==-1) return NULL;
	
	r = (resource*)malloc(sizeof(resource));
	if(r==NULL){
		return NULL;
	} 
	r->name=strdup(name);
	if(r->name==NULL){
		free(r);
		return NULL;
	}
	r->type = type;
	r->max = max;
	r->min = min;
	r->value = value;
	return r;
}

void freeResource(resource *r){
	if(r==NULL) return;

	if(r->name) free(r->name);
	free(r);

	return;
}

Status changeName(resource* r, char* name){
	if(r==NULL) return ERROR;

	if(r->name) free(r->name);
	r->name=strdup(name);
	if(r->name==NULL) return ERROR;

	return OK;
}

Status changeMax(resource* r, int max){
	if(r==NULL) return ERROR;
	r->max = max;

	if(max < r->value)
		r->value = max;

	return OK;
}

Status changeMin(resource* r, int min){
	if(r==NULL) return ERROR;
	r->min = min;

	if(r->value < min)
		r->value = min;
	
	return OK;
}

Status changeValue(resource* r, int value){
	if(r==NULL) return ERROR;
	r->value = value;

	if(value > r->max)
		r->value = r->max;

	if(value < r->min)
		r->value = r->min;

	return OK;
}

Status varyValue(resource* r, int value){
	if(r==NULL) return ERROR;
	r->value += value;

	if(r->value > r->max)
		r->value = r->max;

	if(r->value < r->min)
		r->value = r->min;

	return OK;
}

char* resourceGetName(resource* r){
	if(r==NULL) return NULL;
	return strdup(r->name);
}

int getType(resource *r){
	if(r==NULL) return -1;

	return r->type;
}

int getMin(resource *r){
	/*TODO Set an error number*/
	if(r==NULL) return -1;

	return r->min;
}

int getMax(resource *r){
	/*TODO Set an error number*/
	if(r==NULL) return -1;

	return r->max;
}

int getValue(resource *r){
	/*TODO Set an error number*/
	if(r==NULL) return -1;

	return r->value;
}


resource** getResourcesFromFile(char* file){
	if(file==NULL) return NULL;

	resource** r;
	char* line, *line1, *name, *token;
	int type, max, min, value, i, j, num_r;
	FILE* pf;
	line=(char*)malloc(sizeof(char)*100);
	if(line==NULL) return NULL;
	line1=(char*)malloc(sizeof(char)*10);
	if(line1==NULL){
		free(line);
		return NULL;
	}
	pf=fopen(file, "r");
	fgets(line1, 10 , pf);
	num_r=atoi(line1);
	free(line1);
	r=(resource**)malloc(sizeof(resource*)*(num_r+1));
	if(r==NULL){
		free(line);
		return NULL;
	}
	for(j=0; j<num_r; j++){
		fgets(line, 200, pf);
		token=strtok(line,",");
		for(i=0; token; i++){
			switch(i){
				case(0):
					name=strdup(token);
					if(name==NULL){
						free(line);
						for(i=0;i<j;i++){
							freeResource(r[i]);
						}
						free(r);
						fclose(pf);
						return NULL;
					}
					break;
				case(1):
					type=atoi(token);
					break;
				case(2):
					max=atoi(token);
					break;
				case(3):
					min=atoi(token);
					break;
				case(4):
					value=atoi(token);
					break;
			}
			token=strtok(NULL,",");
		}
		r[j]=iniResource(name, type, max, min, value);
		if(r[j]==NULL){
			free(name);
			free(line);
			for(i=0;i<j;i++){
				freeResource(r[i]);
			}
			free(r);
			fclose(pf);
			return NULL;
		}
		free(name);
	}
	r[num_r]=NULL;
	free(line);
	fclose(pf);
	return r;
}