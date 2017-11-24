#include <string.h>
#include "objects.h"


struct _Object{
	int type;
	Bool picked;
	Bool pickable;
	char* name;
	char* image;
	int lat;
	int lng;
	int num_res;
	int* resources;
};

object *iniObject(char* name, int type, Bool picked, Bool pickable, char* image, int lat, int lng, int num_res, int* resources){

	object *obj;
	int i;

	if (type==-1 || image==NULL || resources==NULL) return NULL;

	obj= (object*) malloc (sizeof(object));
	if (obj==NULL) return NULL;
	obj->type = type;
	obj->picked = picked;
	obj->pickable = pickable;
	obj->name = strdup(name);
	if (obj->name == NULL){
		free(obj);
		return NULL;
	}
	obj->num_res=num_res;
	obj->resources=(int*)malloc(sizeof(int)*num_res);
	if(obj->resources == NULL){
		free(obj->name);
		free(obj);
		return NULL;
	}
	for(i=0;i<num_res;i++){
		obj->resources[i]=resources[i];
	}
	obj->image=strdup(image);
	if(obj->image==NULL){
		free(obj->name);
		free(obj->resources);
		free(obj);
		return NULL;
	}
	obj->lat = lat;
	obj->lng = lng;

	return obj;
}

void freeObject (object *obj){

	if(obj==NULL) return;

	if(obj->name != NULL) free(obj->name);
	if(obj->image != NULL) free(obj->image);
	if(obj->resources != NULL)free(obj->resources); 
	free(obj);
	
	return;

}

Status moveObject(object *obj, int lat, int lng){

	if(obj==NULL) return ERROR ;

	obj->lat=lat;
	obj->lng=lng;

	return OK;

}


Status pickObject (object *obj){

	if(obj==NULL) return ERROR;

	if (obj->picked == FALSE) obj->picked = TRUE;
	else if (obj->picked == TRUE) obj->picked = FALSE;
	

	return OK; 
}

Status objectSetType(object* obj, int type){
	if(obj==NULL) return ERROR;
	
	obj->type=type;
	
	return OK;
}

Status objectSetName(object* obj, char* name){
	if((obj==NULL)||(obj->type==-1)) return ERROR;
	
	if(obj->name) free(obj->name);
	obj->name=strdup(name);
	if(obj->name==NULL) return ERROR;
	
	return OK;
}

Status objectSetPicked(object* obj, Bool picked){
	if((obj==NULL)||(obj->type==-1)) return ERROR;
	
	obj->picked=picked;
	
	return OK;
}

Status objectSetPickable(object* obj, Bool pickable){
	if((obj==NULL)||(obj->type==-1)) return ERROR;
	
	obj->pickable=pickable;
	
	return OK;
}

Status objectSetImage(object* obj, char* image){
	if((obj==NULL)||(obj->type==-1)) return ERROR;

	if(obj->image) free(obj->image);
	obj->image=strdup(image);
	if(obj->image==NULL) return ERROR;
	
	return OK;
}

Status objectSetLocation(object* obj, int lat, int lng){
	if((obj==NULL)||(obj->type==-1)) return ERROR;
	obj->lat=lat;
	obj->lng=lng;
	return OK;
}

Status objectSetResources(object* obj, int* res, int num_res){
	if((obj==NULL)||(obj->type==-1)) return ERROR;

	int i;
	if(obj->resources) free(obj->resources);
	obj->resources=(int*)malloc(sizeof(int)*num_res);
	for(i=0;i<num_res;i++){
		obj->resources[i]=res[i];
	}
	obj->num_res=num_res;
	
	return OK;
}

int objectGetType(object* obj){
	if((obj==NULL)||(obj->type==-1)) return-1;
	
	return obj->type;
}

char* objectGetName(object* obj){
	if((obj==NULL)||(obj->type==-1)) return NULL;
	
	return strdup(obj->name);
}

Bool objectGetPicked(object* obj){
	if((obj==NULL)||(obj->type==-1)) return 0;
	
	return obj->picked;
}

Bool objectGetPickable(object* obj){
	if((obj==NULL)||(obj->type==-1)) return 0;
	
	return obj->pickable;
}

char* objectGetImage(object* obj){
	if((obj==NULL)||(obj->type==-1)) return NULL;
	
	return strdup(obj->image);
}

int* objectGetLocation(object* obj){
	if((obj==NULL)||(obj->type==-1)) return NULL;
	
	int* loc=(int*)malloc(sizeof(int)*2);
	loc[0]=obj->lat;
	loc[1]=obj->lng;
	
	return loc;
}

int* objectGetResources(object* obj){
	if((obj==NULL)||(obj->type==-1)) return NULL;

	int i;
	int* res;
	res=(int*)malloc(sizeof(int)*(obj->num_res));
	for(i=0;i<obj->num_res;i++){
		res[i]=obj->resources[i];
	}
	
	return res;
}

int objectGetnum_res(object* obj){
	if((obj==NULL)||(obj->type==-1)) return -1;
	
	return obj->num_res;
}

object* objectSearch(int type, object** obj){
	return obj[type];
}


object** getObjectsFromFile(char* file){
	if(file==NULL) return NULL;

	object** obj;
	char* line, *name, *image, *token, *line1;
	int type, picked, pickable, lat, lng, num_res, i, j, num_obj;
	int* res;
	FILE* pf;
	line=(char*)malloc(sizeof(char)*200);
	if(line==NULL) return NULL;
	line1=(char*)malloc(sizeof(char)*10);
	if(line1==NULL){
		free(line);
		return NULL;
	}
	pf=fopen(file, "r");
	fgets(line1, 10 , pf);
	num_obj=atoi(line1);
	free(line1);
	obj=(object**)malloc(sizeof(object*)*(num_obj+1));
	if(obj==NULL){
		free(line);
		return NULL;
	}
	for(j=0; j<num_obj; j++){
		fgets(line, 200, pf);
		token=strtok(line,",");
		for(i=0; token; i++){
			switch(i){
				case(0):
					name=strdup(token);
					if(name==NULL){
						free(line);
						for(i=0;i<j;i++){
							freeObject(obj[i]);
						}
						free(obj);
						fclose(pf);
						return NULL;
					}
					break;	
				case(1):
					type=atoi(token);
					break;
				case(2):
					picked=atoi(token);
					break;
				case(3):
					pickable=atoi(token);
					break;
				case(4):
					image=strdup(token);
					if(image==NULL){
						free(name);
						free(line);
						for(i=0;i<j;i++){
						freeObject(obj[i]);
						}
						free(obj);
						fclose(pf);
						return NULL;
					}
					break;
				case(5):
					lat=atoi(token);
					break;
				case(6):
					lng=atoi(token);
					break;
				case(7):
					num_res=atoi(token);
					break;
			}
			token=strtok(NULL,",");
		}
		res=(int*)malloc(sizeof(int)*num_res);
		if(res==NULL){
			free(image);
			free(name);
			free(line);
			for(i=0;i<j;i++){
				freeObject(obj[i]);
			}
			free(obj);
			fclose(pf);
			return NULL;
		}
		fgets(line, 200, pf);
		token=strtok(line,",");
		for(i=0;token;i++){
			res[i]=atoi(token);
			token=strtok(NULL,",");
		}
		obj[j]=iniObject(name, type, picked, pickable, image, lat, lng, num_res, res);
		if(obj[j]==NULL){

			free(res);
			free(name);
			free(image);
			free(line);
			for(i=0;i<j;i++){
				freeObject(obj[i]);
			}
			free(obj);
			fclose(pf);
			return NULL;
		}
		free(res);
		free(name);
		free(image);
	}
	obj[num_obj]=NULL;
	free(line);
	fclose(pf);
	
	return obj;
}



