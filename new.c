/*정적외부변수를 사용해서 특정한 함수에서만 내용을 수정하도록 함(중요한 부분임)
그리고 스택에 대한 이해도 필요하다
nm명령어는 컴파일을 하면서
size명령어는 영역의 크기가 얼마나 되는지 보여준다*/
//지금 모든 왠만한 부분에서는 이상한 부분이 없는데 리스트 파일을 만들 때 이상하게
//3번 찍히는게 좀 이상해서 그 부분을 고쳐야 됨ㅠㅠ
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include<ctype.h>
#include<signal.h>

#define len 80

typedef struct linked_words{//여러 개의 문자열을 받기 위해 선언한 구조체
  char *data;//내용을 포인터 배열로 저장함
  void *com_word;//같은 문자가 있으면 비교하기 위해서 만들어 놓은 포인터
  struct linked_words *next;//다음 문자열을 노드하기 위해 선언한 자기참조 구조체
}LINK_W;

typedef struct compare{
  char *data;
  void *com_word;
}DIR;

typedef struct linked_movie{//MOVIE에 해당하는 구조체를 선언함
  int serial_num;
  char *title;
  char *genre;
  DIR *director;
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
void cntrl_c(int sig);
void make_movie_list(LINK_M **movie);
void make_actor_list(LINK_A **actor);
void make_director_list(LINK_D **director);
void input_multiple(LINK_M *movie);
void add_movie(LINK_M *movie);
int copy_movie_list();


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
void make_movie_list_file(LINK_M *save_movie,int serial){//이거 왜 3번도는지 모르겠음.... 짜증남 이유를 모름
  FILE *mv_list;
  mv_list = fopen("movie_list","w");
  LINK_W *print_word;
  int i;
  print_word = (LINK_W *)malloc(sizeof(LINK_W));
    while(i<serial-1){
    save_movie = save_movie -> next_movie;
    fprintf(mv_list,"%d:%s:%s:%s:%d:%d:",save_movie -> serial_num,save_movie -> title,save_movie -> genre,save_movie -> director -> data,save_movie -> year,save_movie -> time);
    print_word = save_movie -> actor;
    while(print_word -> next != NULL){
  		fprintf(mv_list,"%s,",print_word->data);
  		print_word = print_word->next;
  	}
  	fprintf(mv_list,"%s\n", print_word->data);
    i++;
  }
  fclose(mv_list);
  }
void input_multiple(LINK_M *movie){
	movie->actor = (LINK_W *)malloc(sizeof(LINK_W));
	LINK_W *tmp;
	tmp = movie->actor;
	char *temp;
	temp=(char*)calloc(1024, sizeof(char));
	scanf("%[^\n]%*c", temp);
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
 if(mv_s == NULL)
 serial = 1;
 else{
 fscanf(mv_s,"%c",&read_text);
 if(read_text == 'a')
   serial++;
 while(read_text!=EOF){
     fscanf(mv_s,"%c",&read_text);
     if(read_text == '\n'){
       fscanf(mv_s,"%c",&read_text);
         if(read_text == 'a')
           serial++;
         else if(read_text != 'u' && read_text != 'd')
         break;
         }
 }
 fclose(mv_s);
}
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
  save_movie -> director = (DIR *)malloc(sizeof(DIR));
  scanf("%[^\n]%*c",input_data);
  save_movie -> director -> data = (char *)malloc(strlen(input_data)+1);
  strcpy(save_movie -> director -> data, input_data);
  fprintf(mv_l,"%s:",save_movie -> director -> data);
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
  input_multiple(save_movie);
  print_multiple_data_log_to_list(mv_l,save_movie);
  while(new_movie -> next_movie != NULL)
  new_movie = new_movie -> next_movie;
  new_movie -> next_movie = save_movie;
  save_movie -> next_movie = NULL;
  free(input_data);
  fclose(mv_l);
}
int delete_movie(LINK_M *movie,int serial){
  LINK_M *pre_movie, *del_movie;
  FILE *mv_d;
  pre_movie = (LINK_M *)malloc(sizeof(LINK_M));
  del_movie = (LINK_M *)malloc(sizeof(LINK_M));
  pre_movie = movie;
  while(pre_movie -> next_movie != NULL || pre_movie -> serial_num == serial){
    if(pre_movie -> serial_num == serial){
      del_movie -> next_movie = pre_movie -> next_movie;
      mv_d = fopen("movie_log","a");
      fprintf(mv_d,"delete:%d::::::\n", serial);
      fclose(mv_d);
      free(pre_movie);
      return 1;
    }
    del_movie = pre_movie;
    pre_movie = pre_movie -> next_movie;
  }
  return 0;
}
int add_movie_list(LINK_M *movie,FILE *mv_l){
  LINK_M *new_movie,*link_movie;
  LINK_W *tmp;
  char *read_data,read_text,*temp;
  int read_num;
  link_movie = (LINK_M *)malloc(sizeof(LINK_M));
  new_movie = (LINK_M *)malloc(sizeof(LINK_M));
  read_data = (char *)malloc(sizeof(char)*1000);
  new_movie -> actor = (LINK_W *)malloc(sizeof(LINK_W));
  tmp = new_movie ->actor;
  temp=(char*)calloc(1024, sizeof(char));
  fscanf(mv_l,"%*c%d",&read_num);
  new_movie -> serial_num = read_num;
  fscanf(mv_l,"%*c%[^:]s",read_data);
  new_movie -> title = (char *)malloc(strlen(read_data));
  strcpy(new_movie -> title, read_data);
  fscanf(mv_l,"%*c%[^:]s",read_data);
  new_movie -> genre = (char *)malloc(strlen(read_data));
  strcpy(new_movie -> genre, read_data);
  fscanf(mv_l,"%*c%[^:]s",read_data);
  new_movie -> director = (DIR *)malloc(sizeof(DIR));
  new_movie -> director -> data = (char *)malloc(strlen(read_data));
  strcpy(new_movie -> director -> data, read_data);
  fscanf(mv_l,"%*c%d",&read_num);
  new_movie -> year = read_num;
  fscanf(mv_l,"%*c%d:",&read_num);
  new_movie -> time = read_num;
  fscanf(mv_l,"%[^\n]%*c",temp);
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
  link_movie = movie;
  while(link_movie -> next_movie != NULL)
  link_movie = link_movie -> next_movie;
  link_movie -> next_movie = new_movie;
  new_movie -> next_movie = NULL;
  free(read_data);
  if(read_text=='\n')
  return 1;
}
int delete_movie_list(LINK_M *movie,FILE *mv_l){
  LINK_M *pre_movie, *del_movie;
  int read_num;
  fscanf(mv_l,":%d%*[\n]%*c",&read_num);
  pre_movie = (LINK_M *)malloc(sizeof(LINK_M));
  del_movie = (LINK_M *)malloc(sizeof(LINK_M));
  pre_movie = movie;
  while(pre_movie -> next_movie != NULL || pre_movie -> serial_num == read_num){
    if(pre_movie -> serial_num == read_num){
      del_movie -> next_movie = pre_movie -> next_movie;
      free(pre_movie);
      return 1;
    }
    del_movie = pre_movie;
    pre_movie = pre_movie -> next_movie;
  }
  return 0;
}
int scan_movie_log(LINK_M *movie){
  LINK_M *save_movie;
  LINK_W *save_word,*last_word;
  FILE *mv_l;
  char *input_data,input_text,read_text,garbage;
  int input_num,serial,i=1;
  mv_l=fopen("movie_log","r");
  save_movie=(LINK_M *)malloc(sizeof(LINK_M));
  input_data=(char *)malloc(sizeof(char)*30);
  while(1){
    if(mv_l == NULL){
      return 1;
      break;
    }
  fscanf(mv_l,"%[^:]s",input_data);
  if(strcmp(input_data,"add")==0){
    add_movie_list(movie,mv_l);
    i++;
    read_text=fgetc(mv_l);
    fseek(mv_l,-1,SEEK_CUR);
    if(read_text==EOF)
    break;
  }
  else if(strcmp(input_data,"update")==0){
    read_text=fgetc(mv_l);
    fseek(mv_l,-1,SEEK_CUR);
    if(read_text==EOF)
    break;
  }
  else if(strcmp(input_data,"delete")==0){
    delete_movie_list(movie,mv_l);
    read_text=fgetc(mv_l);
    fseek(mv_l,-1,SEEK_CUR);
    if(read_text==EOF)
    break;
  }
  else
  break;
}
return i;
}

int print_movie_list(LINK_M *movie,int serial){
  LINK_M *print_movie;
  LINK_W *print_word;
  int i=1;
  print_movie=(LINK_M*)malloc(sizeof(LINK_M));
  print_movie=movie;
  print_word = (LINK_W *)malloc(sizeof(LINK_W));
  while(print_movie -> next_movie != NULL){
    print_movie = print_movie -> next_movie;
    if(print_movie -> serial_num == serial){
    printf("  %d, %s, %s\n  D: %s\n",print_movie -> serial_num,print_movie -> title,print_movie -> genre,print_movie -> director -> data);
    print_word = print_movie -> actor;
    while(print_word -> next != NULL){
  		printf("  A%d : %s\n",i,print_word->data);
  		print_word = print_word->next;
      i++;
  	}
  	printf("  A%d : %s\n",i,print_word->data);
    return 1;
  }
}
}
void commands(LINK_D *director,LINK_A *actor,LINK_M *movie){
  //명령어를 입력받아서 그에 해당하는 함수를 출력하기 위해 선언한 변수
  //serach print add update delete sort save end
  char *command,*input_data;//문자열을 입력받을 char형 포인터 배열을 선언하였음
  char input_text;//옵션을 받을 char형 변수를 선언함
  int input_num,i;
    if(scan_movie_log(movie)==1){
      printf("log파일이 존재하지 않습니다\n");
  }
    else if(scan_movie_log(movie) != 1){
    scan_movie_log(movie);
    copy_movie_list();
    make_movie_list_file(movie,scan_movie_log(movie));
  }
    printf("You can use add, update, delete, search, sort, save, end commands.\n");//사용 가능한 명령어를 보여줌
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
      scanf("%*c%s%*c%d",&input_data,&input_num);//input_num을 시리얼 넘버로 선언해 주어야 함
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
void cntrl_c(int sig){//신호를 받아서 ctrl + C 를 통해 끄려고 할 때 끌지 말지 유무를 물어봄
  char *answer; //나중에 포인터로 바꾸기
  answer=(char *)calloc(100,sizeof(char));
  printf("\n프로그램을 종료하시겠습니까? (y/n)");
  scanf("%s", answer);
  if(*answer =='y' || *answer == 'Y')
    exit(1);
  }
int copy_movie_list(){
    FILE *f1, *f2;
    char *line;
    char c;
    struct tm *t;
    time_t now;
    now = time(NULL);
    t = localtime(&now);
    int y, m1, d, h, m2, s;
    char *filename;
    line = (char *)calloc(1024,sizeof(char));
    filename = (char *)calloc(1024,sizeof(char));
  f1 = fopen("movie_list", "r");
  f2 = fopen("movie_list", "a");
  y = t -> tm_year+1900;
  m1 = t -> tm_mon;
  d = t -> tm_mday;
  h = t -> tm_hour;
  m2 = t -> tm_min;
  s = t -> tm_sec;
  sprintf(filename, "movie_list%d%d%d%d%d%d", t -> tm_year+1900, t -> tm_mon, t -> tm_mday, t -> tm_hour, t -> tm_min, t -> tm_sec);
  //파일열기
  f2 = fopen(filename, "w");

  //파일 한라인씩 읽고 복사 fgets 교과서에 나와있는데 프로젝트에 사용가능한지 모름
  while(fgets(line, len -1, f1))
  {
    fputs(line, f2);
  }
  fclose(f1);
  fclose(f2);
  }


int main(){
  signal(SIGINT, cntrl_c);
  LINK_M *movie;
  LINK_D *director;
  LINK_A *actor;
  make_movie_list(&movie);
  printf(">> Welcome to My Movie <<\n");//프로그램의 시작을 알림
  printf("File Loading.....\n");
  commands(director,actor,movie);
  return 0;
}
