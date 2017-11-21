//시리얼 넘버를 어떻게 주고 실행할 때 마다 어떠한 방식으로 해야할 지 결정해야함
//:를 항목에서  입력할 때에 ??;로 바꾸고 다시 출력할 때에 :로 바꿔서 출력하는 걸 생각해야함
//list를 먼저 실행한 후 log를 실행해야 함.그리고 list파일이 존재하면 그 내용을 director_list.201711180021와 같은 내용으로 만들어줘야함
//시그널 받은 후에 yes또는 no를 입력받아서 프로그램이 작동하도록 해야함
//포인터들은 링크드 리스트로 메모리에 저장해야한다.
//director,director,actor포인터의 내용이 겹치면 서로 상호적으로 노드해야함
//명령어를 지원받아야 하는데 옵션도 생각해야 한다
//*와?를 사용할 수 있어야함(우리가 흔히 사용하는 특수문자와 똑같은 의미를 가짐)
//sort를 동작시킬 때에는 qsort를 사용해야함
//헤더파일을 추가해야 하는데 그 방법을 모르겠음
//구조체 선언 방식이나 아니면 내용을 읽어오는 방식 자체를 고쳐야 프로그램이 제대로 동작할 듯함
//movie,director,actor에 해당하는 내용을 관리할 구조체가 추가적으로 필요할 것 같음
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>

typedef struct linked_words{//여러 개의 문자열을 받기 위해 선언한 구조체
  char *head;//내용을 포인터 배열로 저장함
  void *com_word;//같은 문자가 있으면 비교하기 위해서 만들어 놓은 포인터
  struct linked_words *next_word;//다음 문자열을 노드하기 위해 선언한 자기참조 구조체
}ELEMENT_W;

typedef ELEMENT_W *LINK_W;

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

int make_movie_data(MOVIE_ALL *mv_a){
  mv_a -> head = (LINK_M *)malloc(sizeof(LINK_M));
  mv_a -> tail = (LINK_M *)malloc(sizeof(LINK_M));
  mv_a -> head -> next_movie = mv_a -> tail;
  mv_a -> tail -> next_movie = mv_a -> tail;
  mv_a -> size = 0;
}
int make_director_data(DIRECTOR_ALL *dir_a){
  dir_a -> head = (LINK_D *)malloc(sizeof(LINK_D));
  dir_a -> tail = (LINK_D *)malloc(sizeof(LINK_D));
  dir_a -> head -> next_director = dir_a -> tail;
  dir_a -> tail -> next_director = dir_a -> tail;
  dir_a -> size = 0;
}
int make_actor_data(ACTOR_ALL *act_a){
  act_a -> head = (LINK_A *)malloc(sizeof(LINK_A));
  act_a -> tail = (LINK_A *)malloc(sizeof(LINK_A));
  act_a -> head -> next_actor = act_a -> tail;
  act_a -> tail -> next_actor = act_a -> tail;
  act_a -> size = 0;
}


void scan_movie_log(MOVIE_ALL *mv_a){
  FILE *mv_l,*mv_list;
  char *read_text;
  mv_a = (MOVIE_ALL *)malloc(sizeof(MOVIE_ALL));
  read_text = (char *)malloc(sizeof(char *)*10);
  mv_l = fopen("movie_log","r");
  mv_list = fopen("movie_list","w");
  fscanf(mv_l,"%[^:]s",&read_text);
  if(strcmp(read_text,"add") == 0){

  }
}
void scan_actor_log(){

}
void scan_director_log(){

}
void input_movie(LINK_M *movie){//MOVIE에 해당하는 구조체에 내용을 받는 함수 안의 내용을 따로 함수로 바꿀지는 고민해봐야 될듯
  //그 업데이트 할꺼면 나중에 옵션 하나하나마다 함수를 짜버리면 귀찮아 질듯함
  char input_text;//어떠한 방식으로 읽고 저장하는지는 input_actor()함수의 방식과 똑같기 때문에 주석을 읽어보면 됨
  int input_num;
  FILE *mv_l;
  mv_l=fopen("movie_log","a");
  fprintf(mv_l,"add:");
  movie=(LINK_M *)malloc(sizeof(LINK_M));
  printf("title >");
  movie->title=malloc(sizeof(char *)*30);
  scanf("%s",movie->title);
  fprintf(mv_l,"%s:",movie->title);
  printf("genre >");
  movie->genre=malloc(sizeof(char *)*30);
  scanf("%s",movie->genre);
  fprintf(mv_l,"%s:",movie->genre);
  printf("director >");
  movie->director=malloc(sizeof(char *)*30);
  scanf("%s",movie->director);
  fprintf(mv_l,"%s:",movie->director);
  printf("year >");
  scanf("%d",&input_num);
  movie->year=input_num;
  fprintf(mv_l,"%4d:",movie->year);
  printf("time >");
  scanf("%d",&input_num);
  movie->time=input_num;
  fprintf(mv_l,"%3d:",movie->time);
  printf("actor >");
  movie->actor=malloc(sizeof(char *)*30);
  scanf("%s",movie->actor);
  fprintf(mv_l,"%s\n",movie->actor);
  fclose(mv_l);
}
void input_actor(LINK_A *actor){//ACTOR에 해당하는 구조체에 파일을 넣는 함수
  //앞에서 말한대로 따로 함수로 만들어 버릴지는 고민중임
  //받는 방식을 토요일에 고민했던 거랑 좀 다르게 바꿨는데 좀 이상하면 다른 방식으로 바꿔야 함
  char input_text;
  int input_num;
  FILE *act_l;
  //시리얼 넘버는 아직 어떻게 할 지 고민중
  act_l=fopen("actor_log","a");//actor_list파일을 첨부하기 위해 선언함
  fprintf(act_l,"add:");//add명령어 일 때에 사용할 것이기 때문에 add:를 파일에 출력함
  actor=(LINK_A *)malloc(sizeof(LINK_A));//변수에 동적 메모리를 할당시킴
  //앞에 변수를 동적 메모리를 할당을 main()함수에서 선언하는 것이 더 나은 방법일 수도 있음
  printf("name >");
  actor->name=malloc(sizeof(char *)*30);//멤버에 메모리를 할당함
  scanf("%s",actor->name);
  fprintf(act_l,"%s:",actor->name);//이름에 내용 저장
  printf("sex >");
  scanf("%*c%c",&input_text);//*를 사용하여 '\n'을 버림
  actor->sex=input_text;
  fprintf(act_l,"%c:",actor->sex);//성별 저장
  printf("birth >");
  scanf("%d",&input_num);//생일 입력받음
  actor->birth=input_num;
  fprintf(act_l,"%8d:",actor->birth);//저장
  printf("title >");
  actor->title=malloc(sizeof(char *)*30);//멤버에 메모리를 할당함
  scanf("%s",actor->title);//작품 입력받음(여러 개는 어떻게 받을지 고민중임)
  //지금은 1개밖에 못받아서 고쳐야 함
  fprintf(act_l,"%s\n",actor->title);//작품이름 저장
  fclose(act_l);//파일에 내용을 다 저장했으므로 파일을 닫아줌
}
void input_director(LINK_D *director){//방식은 input_actor()함수를 보면 똑같음
  FILE *dir_l;
  char input_text;
  int input_num;
  director=(LINK_D *)malloc(sizeof(LINK_D));
  dir_l=fopen("director_log","a");
  fprintf(dir_l,"add:");
  printf("director >");
  director->name=malloc(sizeof(char *)*30);
  scanf("%s",director->name);
  fprintf(dir_l,"%s:",director->name);
  printf("sex >");
  scanf("%*c%c",&input_text);
  director->sex=input_text;
  fprintf(dir_l,"%c:",director->sex);
  printf("birth >");
  scanf("%d",&input_num);
  director->birth=input_num;
  fprintf(dir_l,"%8d:",director->birth);
  printf("title >");
  director->title=malloc(sizeof(char *)*30);
  scanf("%*c%s",director->title);
  fprintf(dir_l,"%s\n",director->title);
  fclose(dir_l);
  }
void commands(LINK_D *director,LINK_A *actor,LINK_M *movie){
  //명령어를 입력받아서 그에 해당하는 함수를 출력하기 위해 선언한 변수
  //serach print add update delete sort save end
  char *command,*input_data;//문자열을 입력받을 char형 포인터 배열을 선언하였음
  char input_text;//옵션을 받을 char형 변수를 선언함
  int input_num;
  while(1){
    printf("(movie)");
  command =(char *)malloc(sizeof(char)*10);//command포인터에 동적 메모리를 할당하였음
  scanf("%s", command);//명령어를 입력받음
  if(strcmp(command, "add") == 0){//파일에 새로운 내용을 더함
    scanf("%*c%c",&input_text);//'\n'을 무시하고 명령어를 입력받음
    if(input_text=='d'){//director에 내용을 추가함
    input_director(director);
    printf("@@done\n");
  }
    else if(input_text=='m'){//movie에 내용을 추가함
    input_movie(movie);
    printf("@@done\n");
  }
    else if(input_text=='a'){//actor에 내용을 추가함
    input_actor(actor);
    printf("@@done\n");
  }
    }
  else if(strcmp(command,"update")==0){//파일을 업데이트함
    scanf("%*c%c",&input_text);
    if(input_text=='m'){
      scanf("%*c%s%d",&input_data,&input_num);//input_num을 시리얼 넘버로 선언해 주어야 함
    }
    else if(input_text=='a'){

    }
    else if(input_text='d'){

    }
  }
  else if(strcmp(command,"delete")==0){//파일에서 내용을 지워줌

  }
  else if(strcmp(command,"sort")==0){//파일의 내용을 정렬하여서 출력함

  }
  else if(strcmp(command,"print")==0){//파일의 내용을 출력함
    scanf("%*c%c",&input_text);
    if(input_text=='m'){
      scanf("%d",&input_num);
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
  else{
    continue;
  }
  }
}
int main(){
  LINK_D *director;
  LINK_A *actor;
  LINK_M *movie;
  printf(">> Welcome to My Movie <<\n");//프로그램의 시작을 알림
  printf("File Loading.....\n");
  printf("You can use add, update, delete, search, sort, save, end commands.\n");//사용 가능한 명령어를 보여줌
  commands(director,actor,movie);
  return 0;
}
