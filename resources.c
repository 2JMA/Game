#include "resources.h"

typedef struct _Resource{
	/*This doesn't implies that max>min, the resources can have negative effects*/
	int type;
	int max;
	int min;
	int value;
}resource;

resource* initResource(int type, int max, int min, int value){
	if(type=-1) return NULL;
	resource *r;
	r = (resource*)malloc(sizeof(resource));
	if(r==NULL) return NULL;
	r->type = type;
	r->max = max;
	r->min = min;
	r->value = value;
	return r;
}

Status freeResource(resource *r){
	if(r==NULL) return ERROR;
	free(r);
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

	if(value < min)
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

int getType(resource *r){
	if(r==NULL) return -1;

	return r->type;
}

int getMin(resource *r){
	//TODO Set an error number
	if(r==NULL) return;

	return r->min;
}

int getMax(resource *r){
	//TODO Set an error number
	if(r==NULL) return;

	return r->max;
}

int getValue(resource *r){
	//TODO Set an error number
	if(r==NULL) return;

	return r->value;
}