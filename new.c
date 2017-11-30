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

typedef struct linked_movie{//MOVIE에 해당하는 구조체를 선언함
  int serial_num;
  char *title;
  char *genre;
  char *director;
  int year;
  int time;
  LINK_W *actor;//actor는 1명이 아니기 때문에 또 다른 구조체를 사용해서 여려 개를 저장하려함
  struct linked_movie *next_movie;//다음 MOVIE를 노드하기 위해 선언해줌.
  //근데 이 구조체로 되지 않을 수도 있기 때문에 새로운 구조체를 선언해야 할 것 같음
}LINK_M;

typedef struct linked_director{ //director구조체 형의 내용
  int serial_num;
  char *name;
  char sex;
  int birth;
  LINK_W *title;
  struct linked_director *next_director;
}LINK_D;

typedef struct linked_actor{//ACTOR에 해당하는 구조체를 선언함
  int serial_num;
  char *name;
  char sex;
  int birth;
  LINK_W *title;
  struct linked_actor *next_actor;
}LINK_A;

typedef struct all_movie{
  LINK_M *head;
  LINK_M *tail;
  int size;
}MOVIE_ALL;

typedef struct all_actor{
  LINK_A *head;
  LINK_A *tail;
  int size;
}ACTOR_ALL;

typedef struct all_director{
  LINK_D *head;
  LINK_D *tail;
  int size;
}DIRECTOR_ALL;
void make_movie_list(LINK_M **movie){
  *movie=(LINK_M *)malloc(sizeof(LINK_M));
  (*movie) -> next_movie = (LINK_M *)malloc(sizeof(LINK_M));
  (*movie) -> next_movie = NULL;
}
void make_actor_list(LINK_A **actor){
  *actor = (LINK_A *)malloc(sizeof(LINK_A));
  (*actor) -> next_actor = (LINK_A *)malloc(sizeof(LINK_A));
  (*actor) -> next_actor = NULL;
}
void make_director_list(LINK_D **director){
  *director = (LINK_D *)malloc(sizeof(LINK_D));
  (*director) -> next_director = (LINK_D *)malloc(sizeof(LINK_D));
  (*director) -> next_director = NULL;
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
void add_movie(LINK_M *movie){
  FILE *mv_l,*mv_s;
  LINK_W *save_word,*last_word;
  LINK_M *save_movie,*new_movie;
  char *input_data, input_text,read_text;
  int input_num,i,serial = 1;
  save_movie = (LINK_M *)malloc(sizeof(LINK_M));
  new_movie = (LINK_M *)malloc(sizeof(LINK_M));
  mv_l=fopen("movie_log","a");
  input_data=(char *)malloc(sizeof(char)*30);
 mv_s = fopen("movie_log","r");
 fscanf(mv_s,"%c",&read_text);
 if(read_text == 'a')
   serial++;
 while(read_text!=EOF){
     fscanf(mv_s,"%c",&read_text);
     if(read_text == '\n'){
       fscanf(mv_s,"%c",&read_text);
         if(read_text == 'a')
           serial++;
         else if(read_text != 'u' || read_text != 'd')
           break;
         }
 }
 fclose(mv_s);
  save_movie -> serial_num = serial;
  fprintf(mv_l,"add:%d:",save_movie -> serial_num);
  save_movie -> actor = (LINK_W *)malloc(sizeof(LINK_W));
  printf("title >");
  scanf("%[^\n]%*c",input_data);
  save_movie -> title = (char *)malloc(strlen(input_data)+1);
  strcpy(save_movie -> title, input_data);
  fprintf(mv_l,"%s:",save_movie ->title);
  printf("genre >");
  scanf("%[^\n]%*c",input_data);
  save_movie -> genre = (char *)malloc(strlen(input_data)+1);
  strcpy(save_movie -> genre, input_data);
  fprintf(mv_l,"%s:",save_movie ->genre);
  printf("director >");
  scanf("%[^\n]%*c",input_data);
  save_movie -> director = (char *)malloc(strlen(input_data)+1);
  strcpy(save_movie -> director, input_data);
  fprintf(mv_l,"%s:",save_movie -> director);
  printf("year >");
  scanf("%d",&input_num);
  save_movie -> year = input_num;
  fprintf(mv_l,"%4d:",save_movie -> year);
  printf("time >");
  scanf("%d%*c",&input_num);
  save_movie -> time = input_num;
  fprintf(mv_l,"%3d:",save_movie -> time);
  new_movie = movie;
  printf("actor >");
  input_multiple(movie);
  print_multiple_data_log_to_list(mv_l,movie);
  while(new_movie -> next_movie != NULL)
  new_movie = new_movie -> next_movie;
  new_movie -> next_movie = save_movie;
  save_movie -> next_movie = NULL;
  free(input_data);
  fclose(mv_l);
}
void update_movie(LINK_M *movie,char *option,int serial){
  FILE *mv_l;
  char *input_data;
  int input_num,i;
  LINK_M *update_movie,*link_movie;
  mv_l = fopen("movie_log","a");
  update_movie = (LINK_M *)malloc(sizeof(LINK_M));
  input_data = (char *)calloc(100,sizeof(char));
  link_movie = movie;
  while(link_movie -> next_movie != NULL){
      link_movie = link_movie -> next_movie;
      if(link_movie -> serial_num == serial){
        update_movie = link_movie;
        while(*(option+i) != ' '){
        if(*(option+i)=='t'){
          printf("title >");
          scanf("%s",input_data);
          update_movie -> title = (char *)malloc(strlen(input_data)+1);
          update_movie -> title = input_data;
          i++;
        }
        else if(*(option+i) == 'g'){
          printf("genre >");
          scanf("%s",input_data);
          update_movie -> genre = (char *)malloc(strlen(input_data)+1);
          update_movie -> genre = input_data;
          i++;
        }
        else if(*(option+i) == 'd'){
          printf("title >");
          scanf("%[^\n]s",input_data);
          update_movie -> director = (char *)malloc(strlen(input_data)+1);
          update_movie -> director = input_data;
          i++;
        }
        else if(*(option+i) == 'y'){
          printf("year >");
          scanf("%d",&input_num);
          update_movie -> year = input_num;
          i++;
        }
        else if(*(option+i) == 'r'){
          printf("running time >");
          scanf("%d",&input_num);
          update_movie -> time = input_num;
          i++;
        }
        else if(*(option+i) == 'a'){

        }
      }
      break;
      }
      fprintf(mv_l,"update:%d:",movie -> serial_num);
      if(strlen(update_movie -> title) == 0){
        fprintf(mv_l,"=:");
      }
      else if(strlen(update_movie -> title) != 0){
        link_movie -> title = (char *)malloc(strlen(update_movie -> title));
        strcpy(link_movie -> title , update_movie -> title);
        fprintf(mv_l,"%s:");
      }
      else if(strlen(update_movie -> genre) == 0){
        fprintf(mv_l,"=:");
      }
      else if(strlen(update_movie -> genre) != 0){
        link_movie -> genre = (char *)malloc(strlen(update_movie -> genre));
        strcpy(link_movie -> genre , update_movie -> genre);
        fprintf(mv_l,"%s:");
      }
      else if(strlen(update_movie -> director) == 0){
        fprintf(mv_l,"=:");
      }
      else if(strlen(update_movie -> director) != 0){
        link_movie -> director = (char *)malloc(strlen(update_movie -> director));
        strcpy(link_movie -> director , update_movie -> director);
        fprintf(mv_l,"%s:");
      }
      else if(update_movie -> year == 0)
      fprintf(mv_l,"=:");
      else if(update_movie -> year != 0){
        link_movie -> year = update_movie -> year;
        fprintf(mv_l,"%d:",link_movie -> year);
      }
      else if(update_movie -> time == 0)
      fprintf(mv_l,"=:");
      else if(update_movie -> time != 0){
        link_movie -> time = update_movie -> time;
        fprintf(mv_l,"%d\n",link_movie -> time);
      }
    }
}
int delete_movie(LINK_M *movie,int serial){
  LINK_M **del_movie,**pre_movie;
  *pre_movie = (LINK_M *)malloc(sizeof(LINK_M));
  *del_movie = (LINK_M *)malloc(sizeof(LINK_M));
  *pre_movie = movie;
  while((*pre_movie) -> next_movie !=NULL){
    (*pre_movie) = (*pre_movie) -> next_movie;
    if((*pre_movie) -> next_movie -> serial_num == serial){
        *del_movie = (*pre_movie) -> next_movie;
      (*pre_movie) -> next_movie = (*pre_movie) -> next_movie -> next_movie;
      free((*del_movie) -> title);
      free((*del_movie) -> genre);
      free((*del_movie) -> director);
      free((*del_movie) -> actor);
      free(*del_movie);
      break;
    }
  }
}
int add_movie_list(LINK_M *movie,FILE *mv_l){
  LINK_M *new_movie,*link_movie;
  LINK_W *tmp;
  char *read_data,read_text;
  int read_num;
  link_movie = (LINK_M *)malloc(sizeof(LINK_M));
  new_movie = (LINK_M *)malloc(sizeof(LINK_M));
  read_data = (char *)malloc(sizeof(char)*1000);
  new_movie -> actor = (LINK_W *)malloc(sizeof(LINK_W));
  tmp = new_movie ->actor;
  fscanf(mv_l,"%*c%d",&read_num);
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
void delete_movie_list(LINK_M *movie,int serial){

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

void commands(LINK_D *director,LINK_A *actor,LINK_M *movie){
  //명령어를 입력받아서 그에 해당하는 함수를 출력하기 위해 선언한 변수
  //serach print add update delete sort save end
  char *command,*input_data;//문자열을 입력받을 char형 포인터 배열을 선언하였음
  char input_text;//옵션을 받을 char형 변수를 선언함
  int input_num,i;
  while(1){
    printf("(movie)");
  command =(char *)malloc(sizeof(char)*10);//command포인터에 동적 메모리를 할당하였음
  scanf("%s", command);//명령어를 입력받음
  if(strcmp(command, "add") == 0){//파일에 새로운 내용을 더함
    scanf("%*c%c%*c",&input_text);//'\n'을 무시하고 명령어를 입력받음
    if(input_text=='d'){//director에 내용을 추가함
    printf("@@done\n");
  }
    else if(input_text=='m'){//movie에 내용을 추가함
      add_movie(movie);
    printf("@@done\n");
  }
    else if(input_text=='a'){//actor에 내용을 추가함
    printf("@@done\n");
  }
    }
  else if(strcmp(command,"update")==0){//파일을 업데이트함
    scanf("%*c%c",&input_text);
    if(input_text=='m'){
      scanf("%*c%s%d",&input_data,&input_num);//input_num을 시리얼 넘버로 선언해 주어야 함
      update_movie(movie,input_data,input_num);
    }
    else if(input_text=='a'){
      scanf("%*c%s%d",&input_data,&input_num);//input_num을 시리얼 넘버로 선언해 주어야 함
    }
    else if(input_text='d'){
      scanf("%*c%s%d",&input_data,&input_num);//input_num을 시리얼 넘버로 선언해 주어야 함
    }
  }
  else if(strcmp(command,"delete")==0){//파일에서 내용을 지워줌
    scanf("%*c%c",&input_text);
    if(input_text == 'm'){
      scanf("%d",&input_num);
      i=delete_movie(movie,input_num);
      if(i!=1)
      printf("No such record");
    }
  }
  else if(strcmp(command,"sort")==0){//파일의 내용을 정렬하여서 출력함

  }
  else if(strcmp(command,"print")==0){//파일의 내용을 출력함
    scanf("%*c%c",&input_text);
    if(input_text=='m'){
      scanf("%d",&input_num);
      i=print_movie_list(movie,input_num);
      if(i!=1){
        printf("No such record\n");
      }
    }
    else if(input_text=='d'){
      scanf("%d",&input_num);
    }
    else if(input_text=='a'){
      scanf("%d",&input_num);
    }
  }
  else if(strcmp(command,"search")==0){//키워드 검색으로 리스트 상에서 지정된 문자열을 찾아서 출력함

  }
  else if(strcmp(command,"save")==0){//LIST파일에 내용을 저장함

  }
  else if(strcmp(command,"end")==0){//프로그램의 실행을 멈춘다
    //파일의 내용을 저장할 필요가 있으면 저장도 해주면 좋다
    break;
  }
  }
}
int main(){
  LINK_M *movie;
  LINK_D *director;
  LINK_A *actor;
  make_movie_list(&movie);
  scan_movie_log(movie);
  commands(director,actor,movie);
  return 0;
}
