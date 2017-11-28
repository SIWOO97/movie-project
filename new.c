/*정적외부변수를 사용해서 특정한 함수에서만 내용을 수정하도록 함(중요한 부분임)
그리고 스택에 대한 이해도 필요하다
nm명령어는 컴파일을 하면서
size명령어는 영역의 크기가 얼마나 되는지 보여준다*/
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>

typedef struct linked_words{//여러 개의 문자열을 받기 위해 선언한 구조체
  char *head;//내용을 포인터 배열로 저장함
  void *com_word;//같은 문자가 있으면 비교하기 위해서 만들어 놓은 포인터
  struct linked_words *next_word;//다음 문자열을 노드하기 위해 선언한 자기참조 구조체
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
void add_movie(LINK_M *movie){
  FILE *mv_l;
  LINK_W *save_word,*last_word;
  LINK_M *save_movie,*new_movie;
  char *input_data, input_text;
  int input_num,i;
  save_movie = (LINK_M *)malloc(sizeof(LINK_M));
  new_movie = (LINK_M *)malloc(sizeof(LINK_M));
  mv_l=fopen("movie_log","a");
  input_data=(char *)malloc(sizeof(char)*30);
  save_movie -> actor = (LINK_W *)malloc(sizeof(LINK_W));
  printf("title >");
  scanf("%s",input_data);
  save_movie -> title = (char *)malloc(strlen(input_data)+1);
  strcpy(save_movie -> title, input_data);
  fprintf(mv_l,"add:%s:",save_movie ->title);
  printf("genre >");
  scanf("%s",input_data);
  save_movie -> genre = (char *)malloc(strlen(input_data)+1);
  strcpy(save_movie -> genre, input_data);
  fprintf(mv_l,"%s:",save_movie ->genre);
  printf("director >");
  scanf("%s",input_data);
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
  fprintf(mv_l,"%3d\n",save_movie -> time);
  new_movie = movie;
  while(new_movie -> next_movie != NULL)
  new_movie = new_movie -> next_movie;
  new_movie -> next_movie = save_movie;
  save_movie -> next_movie = NULL;
  free(input_data);
}
int add_movie_list(LINK_M *movie,FILE *mv_l){
  LINK_M *new_movie,*link_movie;
  char *read_data,read_text;
  int read_num;
  link_movie = (LINK_M *)malloc(sizeof(LINK_M));
  new_movie = (LINK_M *)malloc(sizeof(LINK_M));
  read_data = (char *)malloc(sizeof(char)*100);
  fscanf(mv_l,"%*c%[^:]s",read_data);
  new_movie -> title = (char *)malloc(strlen(read_data));
  strcpy(new_movie -> title, read_data);
  printf("%s:",read_data);
  fscanf(mv_l,"%*c%[^:]s",read_data);
  new_movie -> genre = (char *)malloc(strlen(read_data));
  strcpy(new_movie -> genre, read_data);
  printf("%s:",read_data);
  fscanf(mv_l,"%*c%[^:]s",read_data);
  new_movie -> director = (char *)malloc(strlen(read_data));
  strcpy(new_movie -> director, read_data);
  printf("%s:",read_data);
  fscanf(mv_l,"%*c%d",&read_num);
  new_movie -> year = read_num;
  printf("%d:",read_num);
  fscanf(mv_l,"%*c%d\n",&read_num);
  printf("%d\n",read_num);
  new_movie -> time = read_num;
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
  fscanf(":%d:",&serial);
}
void delete_movie_list(LINK_M *movie,int serial){

}
void scan_movie_log(LINK_M *movie){
  LINK_M *save_movie;
  LINK_W *save_word,*last_word;
  FILE *mv_l;
  char *input_data,read_text,garbage;
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
    update_movie_list(movie,serial);
  }
  else if(strcmp(input_data,"delete")==0){
    delete_movie_list(movie,serial);
  }
  else
  break;
}
}

void print_movie_list(LINK_M *movie){
  LINK_M *print_movie;
  print_movie=(LINK_M*)malloc(sizeof(LINK_M));
  print_movie=movie;
  printf("링크드 리스트 확인 작업\n");
  while(print_movie -> next_movie != NULL){
    print_movie = print_movie -> next_movie;
    printf("title:%s:genre:%s:director:%s:year:%d:time:%d\n",print_movie -> title,print_movie -> genre,print_movie -> director,print_movie -> year,print_movie -> time);
  }
}
int main(){
  LINK_M *movie;
  make_movie_list(&movie);
  scan_movie_log(movie);
  print_movie_list(movie);
  return 0;
}
