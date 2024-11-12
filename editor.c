#include<stdio.h>
#include<stdlib.h>
#define Initial_size 4096

char* editor();

int main(){

	char filename[40];

	printf("Enter the filename : ");
	scanf("%s",filename);
	printf("\n");

	char *input_buffer = editor();
	if(input_buffer != NULL){
		printf("%s\n",input_buffer);
		free(input_buffer);
	}


	return 0;
}

char* editor(){
	char *input_buffer = (char*)malloc(Initial_size);
	if(input_buffer == NULL){
		printf("Memory allocation failed");
		return NULL; 
	}

	int size = Initial_size;
	int length = 0;
	char input;
	while((input = getchar()) != EOF){
		if(length > size - 1){
			size *= 2;
			char *temp = realloc(input_buffer,size);
			if(temp == NULL){
				printf("rellocatiton failed!");
				free(input_buffer);
				return NULL;
			}
			input_buffer = temp;
		}
		input_buffer[length++] = input;
		input_buffer[length] = '\0';
	}

	return input_buffer;
}
