/*정적외부변수를 사용해서 특정한 함수에서만 내용을 수정하도록 함(중요한 부분임)
그리고 스택에 대한 이해도 필요하다
nm명령어는 컴파일을 하면서
size명령어는 영역의 크기가 얼마나 되는지 보여준다*/
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include<ctype.h>

typedef struct linked_words{//여러 개의 문자열을 받기 위해 선언한 구조체
  char *data;//내용을 포인터 배열로 저장함
  void *com_word;//같은 문자가 있으면 비교하기 위해서 만들어 놓은 포인터
  struct linked_words *next;//다음 문자열을 노드하기 위해 선언한 자기참조 구조체
}LINK_W;

typedef struct linked_movie{
  int serial_num;
  char *title;
  char *genre;
  char *director;
  int year;
  int time;
  LINK_W *actor;
  struct linked_movie *next_movie;
}LINK_M;

void make_movie_list(LINK_M **movie){
  *movie=(LINK_M *)malloc(sizeof(LINK_M));
  (*movie) -> next_movie = (LINK_M *)malloc(sizeof(LINK_M));
  (*movie) -> next_movie = NULL;
}

void input_multiple(LINK_M *movie){
	movie->actor = (LINK_W *)malloc(sizeof(LINK_W));
	LINK_W *tmp;
	tmp = movie->actor;
	char *temp;
	temp=(char*)calloc(1024, sizeof(char));
	scanf("%[^\n]", temp);
	tmp->data = strtok(temp, ",");
	while(tmp->data != NULL){
		char *check;
		check=(char*)malloc(sizeof(char)*30);
		check = strtok(NULL, ",");
		if(check == NULL){
		       tmp->next=NULL;
		       break;
		}
		else{
			tmp->next = (LINK_W *)malloc(sizeof(LINK_W));
			tmp = tmp->next;
			tmp->next=NULL;
			tmp->data = check;
		}
	}
}
void print_multiple_data_log_to_list(FILE *mv_l, LINK_M *movie){
	LINK_W *tmp;
	tmp = movie->actor;
	while(tmp->next != NULL){
		fprintf(mv_l,"%s,",tmp->data);
		tmp = tmp->next;
	}
	fprintf(mv_l, "%s\n", tmp->data);
}
void update_movie(LINK_M *movie, char *option, int serial){
	FILE *mv_l;
	mv_l = fopen("movie_log","a");
	LINK_M *tmp = movie;
	LINK_M *update;
	while(tmp->next_movie != NULL){
		if(tmp->serial_num == serial){
			update = tmp;
			break;
		}
		tmp = tmp->next_movie;
	}
	int check = strlen(option);
	char *check_option = (char*)calloc(6, sizeof(char));
	char *revise = (char*)malloc(sizeof(char)*2);
	char *str;
	str = (char*)calloc(30, sizeof(char));
	int num;
	

	for(int i=0; i<check; i++){
		if(*(option+i) == 't') *(check_option+0) = 't';
		if(*(option+i) == 'g') *(check_option+1) = 'g';
		if(*(option+i) == 'd') *(check_option+2) = 'd';
		if(*(option+i) == 'y') *(check_option+3) = 'y';
		if(*(option+i) == 'r') *(check_option+4) = 'r';
		if(*(option+i) == 'a') *(check_option+5) = 'a';
	}
	fprintf(mv_l, "update:%d:", movie->serial_num);
	int res=-1;
	int k=0;
	for(int i=0; i<check; i++){
		if(res != -1)
			k = res+1;

		for(int j=k; j<6; j++)
			if(*(check_option+j) != '\0'){
				res = j;
				break;
			}
		if(res == 0){
		       if(*(check_option+0) == 't'){
			       printf("title > ");
			       scanf("%[^\n]", str);
			       getchar();
			       if((strcmp(str, update->title))==0){
				       printf("You have the same record.\n");
				       printf("title : %s\n", update->title);
				       printf("Do you want to revise?(Y : yes, N: no)\n");
				       scanf("%s", revise);
				       if((strcmp(revise, "Y") == 0))
					       fprintf(mv_l, "update:%d:%s:", update->serial_num, update->title);
			       }
			       else{
				       strcpy(update->title, str);
				       fprintf(mv_l, "%s:", update->title);
			       }
		       }
		       else fprintf(mv_l, "=:");
		}
		else if(res == 1){
			if(*(check_option+1) == 'g'){
                               printf("genre > ");
                               scanf("%[^\n]", str);
                               getchar();
                               if((strcmp(str, update->genre))==0){
                                       printf("You have the same record.\n");
                                       printf("genre : %s\n", update->genre);
                                       printf("Do you want to revise?(Y : yes, N: no)\n");
                                       scanf("%s", revise);
                                       if(strcmp(revise, "Y")==0)
                                               fprintf(mv_l, "%s:", update->genre);
                               }
                               else{
                                       strcpy(update->genre, str);
                                       fprintf(mv_l, "%s:", update->genre);
                               }
                       }
                       else fprintf(mv_l, "=:");
                }
		else if(res == 2){
                       if(*(check_option+2) == 'd'){
                               printf("director > ");
                               scanf("%[^\n]", str);
                               getchar();
                               if((strcmp(str, movie->director))==0){
                                       printf("You have the same record.\n");
                                       printf("director : %s\n", movie->director);
                                       printf("Do you want to revise?(Y : yes, N: no)\n");
                                       scanf("%s", revise);
                                       if(strcmp(revise, "Y")==0)
                                               fprintf(mv_l, "%s:", update->director);
                               }
                               else{
                                       strcpy(update->title, str);
                                       fprintf(mv_l, "%s:", update->director);
                               }
                       }
                       else fprintf(mv_l, "=:");
                }
		else if(res == 3){
                       if(*(check_option+3) == 'y'){
                               printf("year > ");
                               scanf("%d", num);
                               if(movie->year == num){
                                       printf("You have the same record.\n");
                                       printf("year : %d\n", movie->year);
                                       printf("Do you want to revise?(Y : yes, N: no)\n");
                                       scanf("%s", revise);
                                       if(strcmp(revise, "Y")==0)
                                               fprintf(mv_l, "%d:", update->year);
                               }
                               else{
                                       update->year = num;
                                       fprintf(mv_l, "%d:", update->year);
                               }
                       }
                       else fprintf(mv_l, "=:");
                }
		else if(res == 4){
                       if(*(check_option+4) == 'r'){
                               printf("running time > ");
                               scanf("%d", num);
                               if(movie->time == num){
                                       printf("You have the same record.\n");
                                       printf("running time : %d\n", movie->time);
                                       printf("Do you want to revise?(Y : yes, N: no)\n");
                                       scanf("%s", revise);
                                       if(strcmp(revise, "Y")==0)
                                               fprintf(mv_l, "%d:", update->time);
                               }
                               else{
                                       update->time = num;
                                       fprintf(mv_l, "%d:", update->time);
                               }
                       }
                       else fprintf(mv_l, "=:");
		}
		else if(res == 5){
                       if(*(check_option+5) == 'a'){
			       printf("actor > ");

			       LINK_M * check;
			       input_multiple(check);

			       int a=1, b=1;
			       LINK_W *new;
			       new = (LINK_W *)malloc(sizeof(LINK_W));
			       new = check->actor;
			       while(new ->next != NULL){
				       a++;
				      new = new->next;
			       }
			       LINK_W *org;
			       org = (LINK_W *)malloc(sizeof(LINK_W));
			       org = update->actor;
			       while(org->next != NULL){
				       b++;
				      org = org->next;
			       }

			       if(a==b){
				       new = check->actor;
				       org = update->actor;
				       /*while(new->next != NULL){
					       if(strcmp(new->data, org->data)==0){
						       new = new->next;
						       org = org->next;
					       }
					       else{
						       free(update->actor);
						       update->actor = (LINK_W*)malloc(sizeof(LINK_W));
						       while(new->next != NULL){
							       strcpy(tmp->data, new->data);
							       tmp = tmp->next;
							       new = new->next;
						       }
						       strcpy(tmp->data, new->data);
						       tmp = update->actor;
						       while(tmp->next != NULL){
							       fprintf(mv_l, "%s, ", tmp->data);
							       tmp = tmp->next;
						       }
						       fprintf(mv_l, "%s\n", tmp->data);
					       }
				       }*/
				       if(strcmp(new->data, org->data)==0){
					       printf("You have the same record.\n");
				               printf("actors : ");
					       while(org->next != NULL){
						       printf("%s, ", org->data);
						       org = org->next;
					       }
					       printf("%s\n", org->data);
					       printf("Do you want to revise?(Y : yes, N: no)\n");
                                               scanf("%s", &revise);
                                               if(strcmp(revise, "Y")){
						       org = update->actor;
						       while(org->next != NULL){
							       fprintf(mv_l, "%s, ", org->data);
							       org = org->next;
						       }
						       fprintf(mv_l, "%s\n", org->data);
					       }
				       }
			       }
			       /*else{
				       free(update->actor);
                                       update->actor = (LINK_W*)malloc(sizeof(LINK_W));
                                       while(new->next != NULL){
					       strcpy(tmp->data, new->data);
                                               tmp = tmp->next;
                                               new = new->next;
				       }
                                       strcpy(tmp->data, new->data);
                                       tmp = update->actor;
                                       while(tmp->next != NULL){
					       fprintf(mv_l, "%s, ", tmp->data);
                                               tmp = tmp->next;
				       }
                                       fprintf(mv_l, "%s\n", tmp->data);
			       }*/
		       }
		       else
			       fprintf(mv_l, "=");
		}
	}
	fclose(mv_l);
}
int add_movie_list(LINK_M *movie,FILE *mv_l){
  LINK_M *new_movie,*link_movie;
  LINK_W *tmp;
  char *read_data,read_text;
  int read_num;
  link_movie = (LINK_M *)malloc(sizeof(LINK_M));
  new_movie = (LINK_M *)malloc(sizeof(LINK_M));
  read_data = (char *)malloc(sizeof(char)*1000);

  new_movie -> serial_num = read_num;
  fscanf(mv_l,"%*c%[^:]s",read_data);
  
  new_movie -> title = (char *)malloc(strlen(read_data));
  strcpy(new_movie -> title, read_data);
  fscanf(mv_l,"%*c%[^:]s",read_data);
  
  new_movie -> genre = (char *)malloc(strlen(read_data));
  strcpy(new_movie -> genre, read_data);
  fscanf(mv_l,"%*c%[^:]s",read_data);
  
  new_movie -> director = (char *)malloc(strlen(read_data));
  strcpy(new_movie -> director, read_data);
  fscanf(mv_l,"%*c%d",&read_num);
  
  new_movie -> year = read_num;
  fscanf(mv_l,"%*c%d\n",&read_num);
  
  new_movie -> time = read_num;
  fscanf(mv_l,"%[^\n]\n",read_data);
  
  new_movie->actor = (LINK_W*)malloc(sizeof(LINK_W));
  tmp = new_movie->actor;
  fscanf(mv_l, 

  tmp->data = strtok(read_data, ",");
	while(tmp->data != NULL){
		char *check;
		check=(char*)malloc(sizeof(char)*30);
		check = strtok(NULL, ",");
		if(check == NULL){
		       tmp->next=NULL;
		       break;
		}
		else{
			tmp->next = (LINK_W *)malloc(sizeof(LINK_W));
			tmp = tmp->next;
			tmp->next=NULL;
			tmp->data = check;
		}
	}
  link_movie = movie;
  while(link_movie -> next_movie != NULL)
  link_movie = link_movie -> next_movie;
  link_movie -> next_movie = new_movie;
  new_movie -> next_movie = NULL;
  free(read_data);
  if(read_text=='\n')
  return 1;
}
void update_movie_list(LINK_M *movie,FILE *mv_l,int serial){
  int read_num;
  char read_text,*read_data;
  read_data = (char *)malloc(sizeof(char)*100);
  fscanf(mv_l,":%d:",&read_num);
  serial = read_num;
  while(movie -> next_movie != NULL){
    movie = movie -> next_movie;
    if(movie -> serial_num == read_num){//title,genre,director,year,time,actor
      fscanf(mv_l,"%s:",read_data);
      if(*read_data != '='){
        movie -> title = malloc(strlen(read_data));
        movie -> title = read_data;
      }
      fscanf(mv_l,"%s:",read_data);
      if(*read_data != '='){
        movie -> genre = malloc(strlen(read_data));
        movie -> genre = read_data;
      }
      fscanf(mv_l,"%s:",read_data);
      if(*read_data != '='){
        movie -> director = malloc(strlen(read_data));
        movie -> director = read_data;
      }
      fscanf(mv_l,"%c:",&read_text);
      if(read_text != '=')
      movie -> year = read_text - 48;
     fscanf(mv_l,"%c\n",&read_text);
     if(read_text != '=')
     movie -> time = read_text - 48;
     free(read_data);
     break;
  }
}
}
void scan_movie_log(LINK_M *movie){
  LINK_M *save_movie;
  LINK_W *save_word,*last_word;
  FILE *mv_l;
  char *input_data,input_text,read_text,garbage;
  int input_num,serial,i;
  mv_l=fopen("movie_log","r");
  save_movie=(LINK_M *)malloc(sizeof(LINK_M));
  input_data=(char *)malloc(sizeof(char)*30);
  while(1){
  fscanf(mv_l,"%[^:]s",input_data);
  if(strcmp(input_data,"add")==0){
    add_movie_list(movie,mv_l);
    read_text=fgetc(mv_l);
    fseek(mv_l,-1,SEEK_CUR);
    if(read_text==EOF)
    break;
  }
  else if(strcmp(input_data,"update")==0){
    update_movie_list(movie,mv_l,serial);
    read_text=fgetc(mv_l);
    fseek(mv_l,-1,SEEK_CUR);
    if(read_text==EOF)
    break;
  }
  else if(strcmp(input_data,"delete")==0){

  }
  else
  break;
}
}

int print_movie_list(LINK_M *movie,int serial){
  LINK_M *print_movie;
  LINK_W *print_word;
  print_movie=(LINK_M*)malloc(sizeof(LINK_M));
  print_movie=movie;
  while(print_movie -> next_movie != NULL){
    print_movie = print_movie -> next_movie;
    if(print_movie -> serial_num == serial){
    printf("serial:%d\ntitle:%s\n:genre:%s\n:director:%s\n:year:%d\n:time:%d\n",print_movie -> serial_num,print_movie -> title,print_movie -> genre,print_movie -> director,print_movie -> year,print_movie -> time);
    print_word = print_movie -> actor;
    while(print_word -> next != NULL){
  		printf("actor:%s,",print_word->data);
  		print_word = print_word->next;
  	}
  	printf("%s\n", print_word->data);
    
    return 1;
    break;
  }
}
}

void commands(LINK_M *movie){
	char *command;
	command = (char*)malloc(sizeof(char)*10);
	char *input_data;
	input_data = (char*)calloc(10, sizeof(char));
	char input_text;
	int input_num,i;
	while(1){
		printf("(movie)");
		scanf("%s", command);
		if(strcmp(command,"update")==0){
			scanf("%*c%c",&input_text);
			if(input_text=='m'){
				scanf("%*c%[^ ]", input_data);
				getchar();
				scanf("%d", &input_num);
				getchar();
				update_movie(movie,input_data,input_num);
			}
		}
	}
}
int main(){
  LINK_M *movie;
  make_movie_list(&movie);
  scan_movie_log(movie);
  commands(movie);
  return 0;
}
