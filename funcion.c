
char **create_map(char *file,int *size){
	if (file==NULL) return 0;
	FILE *f;
	f=fopen(file,"r");
	if (f==NULL){
		printf("No existe el archivo\n");
		return 0;
	}
	char buf[1000];
	int rows = atoi(fgets(buf, 1000, f));
	int cols = atoi(strchr(buf, ' ')+1);

	char **map=(char**)malloc(sizeof(char*)*rows);
	for(int r=0;r<rows;r++){
		map[r]=(char*)malloc(cols+1);
		fgets(buf, 1000, f);
		int k=0;
		if (buf[strlen(buf)] == '\n')
			buf[strlen(buf)] = 0;
		for (;k<strlen(buf) && k<cols; k++)
			map[r][k] = buf[k];
		for (;k<cols; k++)
			map[r][k] = ' ';
		map[r][k] = 0;
	}

	fclose(f);
	size[0]=rows;
	size[1]=cols;
	return map;
}
