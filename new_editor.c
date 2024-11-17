#include<ncurses.h>
#include<stdlib.h>

#define INITIAL_LENGTH 4096

void control(char*);
void save(char*,char*);
char* input_data(char*,int*);

int main(){
	printf("Enter file name :");
	char *filename;
	scanf("%s",filename);

	initscr();

	keypad(stdscr, TRUE);
	noecho();
	move(0,0);


	int x = 0;
	int y = LINES - 1;
	move(y,x);
	printw("Keep on typing(CTRL+X on quit)(CTRL+O to save)");
	refresh();
	move(0,0);

	control(filename);

	endwin();

	return 0;
}

void control(char *filename){
	int capacity = INITIAL_LENGTH;
	int x = 0;
	int y = 0;
	int ch;
	int length = 0;
	char *input_buffer = (char*)malloc(INITIAL_LENGTH*sizeof(char));
	if(input_buffer == NULL){
		printw("Memory allocation failed");
		exit(1);
	}

	move(0,0);
	refresh();
	FILE *file = fopen(filename,"r");
	if(file != NULL){
		char line[256];
		while(fgets(line,sizeof(line),file)){
			printw("%s",line);
		}
		fclose(file);
	}

	while((ch = getch()) != 24){
		if(length >= capacity - 1){
			input_buffer = input_data(input_buffer,&capacity);
		}
		switch(ch){
			case KEY_UP:
				if(y > 0) y--;
				break;
			case KEY_DOWN:
				if(y < LINES - 1) y++;
				break;
			case KEY_LEFT:
				if(x > 0) x--;
				break;
			case KEY_RIGHT:
				if(x < COLS - 1) x++;
				break;
			case KEY_BACKSPACE:
				if(length > 0) input_buffer[length--] = '\0';
				if(x > 0) x--;
				mvprintw(y,x," ");
				continue;
			case '\n':
				if(y < LINES - 1){
					y++;
					x = 0;
					mvprintw(y,x,"\n");
				}
				break;
			case 15:
				save(input_buffer,filename);
				break;
			default:
				mvaddch(y,x,ch);
				x++;
				if(x >= COLS){
					x = 0;
					y++;
					if(y >= LINES) y = LINES - 1;
				}
				break;
		}
		move(y,x);
		refresh();
		input_buffer[length++] = (char)ch;
	}
	input_buffer[length++] = '\n';
	input_buffer[length] = '\0';

	free(input_buffer);
}

char* input_data(char *input_buffer,int *capacity){
	*capacity *= 2;
	char *new_buffer = realloc(input_buffer,*capacity);
	if(new_buffer == NULL){
		printw("failed to rellocate");
		free(input_buffer);
		exit(1);
	}
	return new_buffer;
}

void save(char *input_buffer,char *filename){
	FILE *file = fopen(filename,"a");
	if(file == NULL){
		printw("saving failed");
		exit(0);
	}

	fprintf(file,input_buffer);
	fclose(file);
}




