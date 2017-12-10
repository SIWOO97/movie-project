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

typedef struct compare{//MOVIE 구조체 안에 director멤버에 사용하기 위해 선언한 구조체
  char *data;//director멤버의 내용을 저장하는 포인터
  void *com_word;//director 멤버와 director구조체 안에 멤버가 같을 때 노드하기 위해 선언한 구조체
}DIR;

typedef struct linked_movie{//MOVIE에 해당하는 구조체를 선언함
  int serial_num;//시리얼 넘버를 저장함
  char *title;//영화의 이름을 저장하는 포인터
  char *genre;//영화의 장르를 저장하는 포인터
  DIR *director;//영화의 감독을 저장하는 구조체(같을 때에 노드하기 위해 다른 형을 사용함)
  int year;//영화 개봉 날짜를 저장하는 멤버
  int time;//영화의 시간을 저장하는 멤버
  LINK_W *actor;//영화의 배우를 저장하는 구조체이다(여러 명의 사람을 받아야 하기 때문에 LINK_W을 사용하였다)
  struct linked_movie *next_movie;//다음 MOVIE를 노드하기 위해 선언해줌.(자기 참조 구조체)
}LINK_M;

typedef struct linked_director{ //DIRECTOR에 해당하는 구조체를 선언함
  int serial_num;//시리얼 넘버를 저장함
  char *name;//감독의 이름을 저장하는 포인터
  char sex;//감독의 성별을 저장하는 멤버
  int birth;//감독의 생일을 저장하는 멤버
  LINK_W *title;//감독의 여러 영화를 저장하기 위해 사용한 구조체
  struct linked_director *next_director;//다음 DIRECTOR를 노드하기 위해 선언해줌(자기참고 구조체)
}LINK_D;

typedef struct linked_actor{//ACTOR에 해당하는 구조체를 선언함
  int serial_num;//시리얼 넘버를 저장함
  char *name;//배우의 이름을 저장하는 포인터
  char sex;//배우의 성별을 저장하는 멤버
  int birth;//배우의 생일을 저장하는 멤버
  LINK_W *title;//배우의 여러 영화를 저장하기 위해 사용한 구조체
  struct linked_actor *next_actor;//다음 ACTOR를 노드하기 위해 선언해줌(자기참조 구조체)
}LINK_A;

typedef struct add_movie{//자기 참조 구조체(MOVIE 형)의 자료구조형을 선언해줌
  LINK_M *head;//자기 참조 구조체의 첫 번째 빈 공간을 노드하기 위해 만들어줌
  //정보 저장의 목적이 아니라 구조체의 첫 번째 내용부터 포인터하기 위해서 만들어준 것임
  LINK_M *tail;//자기 참조 구조체의 마지막 부분을 노드하기 위해서 만들어준 포인터(정보를 저장하진 않음)
  int size;//자기 참조 구조체 안에 HEAD와 TAIL부분을 제외한 부분에 얼마나 내용이 있는지 저장하기 위해 선언한 함수
}ALL_M;//MOVIE형을 관리하기 위해 사용

typedef struct add_actor{//자기 참조 구조체(ACTOR 형)의 자료구조형을 선언
  LINK_A *head;//MOVIE형의 HEAD와 같은 목적
  LINK_A *tail;//MOVIE형의 TAIL과 같은 목적
  int size;//MOVIE형 의 목적과 같은 목적
}ALL_A;//ACTOR형을 관리하기 위해 사용

typedef struct add_director{//자기 참고 구조체(DIRECTOR 형)의 자료구조형을 선언함
  LINK_D *head;//MOVIE형의 HEAD와 같은 목적
  LINK_D *tail;//MOVIE형의 TAIL과 같은 목적
  int size;//MOVIE형의 size와 같은 목적
}ALL_D;//DIRECTOR형을 관리하기 위해 사용


void cntrl_c(int sig);
void make_movie_list(LINK_M **movie);
void make_actor_list(LINK_A **actor);
void make_director_list(LINK_D **director);
void input_multiple(LINK_M *movie);
void add_movie(ALL_M *movie);
int copy_movie_list();
int copy_actor_list();
int copy_director_list();
void compare_movie_word(ALL_M *movie,ALL_D *director,ALL_A *actor);
void compare_actor_word(ALL_M *movie,ALL_A *actor);
void compare_director_word(ALL_M *movie,ALL_D *director);
int compare(const void *, const void *);
int compare_num(const void *, const void *);
void compare_movie(ALL_M *movie,ALL_A *actor);
char *change(char *s, const char *a, const char *b);
char *change(char *s, const char *a, const char *b) { //*a 의 문자열을  *b의 문자열로 바꾸는 함수
  char *result, *sr;
  size_t i, count = 0;
  size_t a_length = strlen(a);
	if (a_length < 1) return s;
  size_t b_length = strlen(b);
  if (b_length != a_length) {
    for (i = 0; *(s+i) != '\0';) {
      if (memcmp((s+i), a, a_length) == 0) count++, i += a_length;
      else i++;
    }
  } else i = strlen(s);
  result = (char *) malloc(i + 1 + count * (b_length - a_length)); //result를 동적할당
  if (result == NULL) return NULL; //에러가 있다면 NULL을 리턴하는 구문
  sr = result;
  while (*s) {
    if (memcmp(s, a, a_length) == 0) {
      memcpy(sr, b, b_length);
      sr += b_length;
      s  += a_length;
    } else *sr++ = *s++;
  }
  *sr = '\0';
  return result; //성공적으로 문자열을 변경하였다면 result의 값을 리턴한다.
}
int compare(const void *a, const void *b){
  return(strcmp(*(char**)a,*(char**)b));
}//문자열을 비교하기 위해 사용함
int compare_num(const void *a, const void *b){
  if(*(int *)a < *(int *)b)
  return -1;
  else if(*(int *)a == *(int *)b)
  return 0;
  else
  return 1;
}//숫자를 비교하기 위해 사용함
int compare_w(const void *a, const void *b){
  return strcmp((char*)a,(char*)b);
}//문자를 비교하기 위해 사용함
void make_movie_admin(ALL_M **movie){//MOVIE형의 자료구조의 메모리를 할당하기 위해 사용
  *movie = (ALL_M *)malloc(sizeof(ALL_M));//ALL_M형 포인터의 메모리를 할당함
  (*movie) -> head = (LINK_M *)calloc(1,sizeof(LINK_M));//헤드에 해당하는 부분에 메모리를 할당함
  (*movie) -> tail = (LINK_M *)calloc(1,sizeof(LINK_M));//테일에 해당하는 부분에 메모리를 할당함
  (*movie) -> head -> next_movie = (*movie) -> tail;//헤드의 넥스트가 무비의 테일 부분을 노드함
  (*movie) -> tail -> next_movie = (*movie) -> tail;//테일의 넥스트가 무비의 테일 부분을 노드함
  (*movie) -> size = 0;//사이즈를 0으로 선언
}
void make_actor_admin(ALL_A **actor){//ACTOR형의 자료구조의 메모리를 할당하기 위해 사용
  *actor = (ALL_A *)malloc(sizeof(ALL_A));//ALL_A 형 포인터의 메모리를 할당함
  (*actor) -> head = (LINK_A *)calloc(1,sizeof(LINK_A));//헤드에 해당하는 부분에 메모리를 할당함
  (*actor) -> tail = (LINK_A *)calloc(1,sizeof(LINK_A));//테일에 해당하는 부분에 메모리를 할당함
  (*actor) -> head -> next_actor = (*actor) -> tail;//헤드의 넥스트가 무비의 테일 부분을 노드함
  (*actor) -> tail -> next_actor = (*actor) -> tail;//테일의 넥스트 무비의 테일 부분을 노드함
  (*actor) -> size = 0;//사이즈를 0으로 선언
}
void make_director_admin(ALL_D **director){//DIRECTOR형의 자료구조의 메모리를 할당하기 위해 사용
  *director = (ALL_D *)malloc(sizeof(ALL_D));//ALL_D 형 포인터의 메모리를 할당함
  (*director) -> head = (LINK_D *)calloc(1,sizeof(LINK_D));//헤드에 해당하는 부분에 메모리를 할당함
  (*director) -> tail = (LINK_D *)calloc(1,sizeof(LINK_D));//테일에 해당하는 부분에 메모리를 할당함
  (*director) -> head -> next_director = (*director) -> tail;//헤드의 넥스트가 무비의 테일 부분을 노드함
  (*director) -> tail -> next_director = (*director) -> tail;//테일의 넥스트가 무비의 테일 부분을 노드함
  (*director) -> size = 0;//사이즈를 0으로 선언
}
void save_file(ALL_M *movie,ALL_A *actor,ALL_D *director,char tag,char *option1){
  ////////////////////////////////////////////////////////////////////////////
  //save 명령어를 받았을 때 옵션과 파일의 이름을 받으면 사용하기 위해 이 함수를 사용함
char *check_option,*filename;//커맨드에서 받은 옵션을 확인하기 위한 포인터와 파일의 이름을 저장하기 위해
//선언한 포인터
int check = strlen(option1);//옵션의 길이를 저장해줌
int i=0;//for문에 사용할 int형 변수를 선언함
FILE *file;//입력 받은 파일의 이름으로 파일을 열기 위해 선언
filename = (char *)calloc(100,sizeof(char));//char형 포인터에 동적 할당을 받음
check_option = (char *)calloc(100,sizeof(char));//옵션을 확인하기 위해 동적 할당을 받음
LINK_M *save_movie;//movie의 헤드 부분을 포인트하기 위해 선언
LINK_A *save_actor;//actor의 헤드 부분을 포인트하기 위해 선언
LINK_D *save_director;//director의 헤드 부분을 포인트하기 위해 선언
LINK_W *save_word;//여러개의 단어를 받기 사용함
save_movie = movie -> head;//head로 포인트시킴
save_actor = actor -> head;//head로 포인트시킴
save_director = director -> head;//head로 포인트시킴
scanf("%s%*c",filename);//파일의 이름을 받음
file = fopen(filename,"w");//받은 이름대로 파일을 열음
  if(tag == 'm'){//앞에서 받은 태그가 영화면
      for(int i=0; i<check; i++){//받은 옵션이 어떤 것이 있는지 확인해줌
         if(*(option1+i) == 't'){//TITEL
        *(check_option+0) = 't';
      }
      if(*(option1+i) == 'g'){//GENRE
        *(check_option+1) = 'g';
      }
      if(*(option1+i) == 'd'){//DIRECTOR
          *(check_option+2) = 'd';
      }
      if(*(option1+i) == 'y'){//YEAR
        *(check_option+3) = 'y';
      }
      if(*(option1+i) == 'r'){//RUNNING TIME
        *(check_option+4) = 'r';
      }
               if(*(option1+i) == 'a'){//ACTOR
         *(check_option+5) = 'a';
       }
     }
  while(save_movie -> next_movie != movie -> tail){
  save_movie = save_movie -> next_movie;//HEAD의 다음부분 부터 출력
  if(*(check_option+0) == 't'){//TITLE
  fprintf(file,"Title :%s\n",save_movie -> title);
  }
  if(*(check_option+1) == 'g'){//GENRE
  fprintf(file,"Genre :%s\n",save_movie -> genre);
  }
  if(*(check_option+2) == 'd'){//DIRECTOR
  fprintf(file,"Director :%s\n",save_movie -> director -> data);
  }
  if(*(check_option+3) == 'y'){//YEAR
  fprintf(file,"Year :%d\n",save_movie -> year);
  }
  if(*(check_option+4) == 'r'){//RUNNING TIME
  fprintf(file,"Running Time :%d\n",save_movie -> time);
  }
  if(*(check_option+5) == 'a'){//ACTOR
  save_word = save_movie -> actor;
  i=1;
  while(save_word -> next != NULL){
  fprintf(file,"Actor %d : %s\n",i,save_word -> data);
  save_word = save_word -> next;
  i++;
  }
  fprintf(file,"Actor %d : %s\n",i,save_word -> data);
  }
}
fclose(file);//파일을 닫음
free(option1);//동적 할당한 내용을 버림
free(check_option);//동적 할당한 내용을 버림
  }
  else if(tag == 'd'){//DIRECTOR인 경우
    for(int i=0; i<check; i++){//옵션을 확인한다
       if(*(option1+i) == 'n'){//NAME
      *(check_option+0) = 'n';
    }
    if(*(option1+i) == 's'){//SEX
      *(check_option+1) = 's';
    }
    if(*(option1+i) == 'b'){//BIRTH
        *(check_option+2) = 'b';
    }
    if(*(option1+i) == 'm'){//MOVIES
      *(check_option+3) = 'm';
    }
   }
   while(save_director -> next_director != director -> tail){//TAIL의 전까지
     save_director = save_director -> next_director;//HEAD의 다음 부분부터 출력
     if(*(check_option+0) == 'n'){
     fprintf(file,"Name :%s\n",save_director -> name);//NAME
     }
     if(*(check_option+1) == 's'){
     fprintf(file,"Sex :%c\n",save_director -> sex);//SEX
     }
     if(*(check_option+2) == 'b'){
     fprintf(file,"Birth :%d\n",save_director -> birth);//BIRTH
     }
     if(*(check_option+3) == 'm'){//MOVIES
       save_word = save_director -> title;
       i=1;
       while(save_word -> next != NULL){
     fprintf(file,"Movie %d :%s\n",i,save_word -> data);
     save_word = save_word -> next;
     i++;
   }
   fprintf(file,"Movie %d :%s\n",i,save_word -> data);
     }
   }
	 free(option1);//동적 할당을 버림
	 free(check_option);//동적 할당을 버림
   fclose(file);//파일을 닫음
  }
  else if(tag == 'a'){//ACTOR
    for(int i=0; i<check; i++){//옵션을 확인함
       if(*(option1+i) == 'n'){//NAME
      *(check_option+0) = 'n';
    }
    if(*(option1+i) == 's'){//SEX
      *(check_option+1) = 's';
    }
    if(*(option1+i) == 'b'){//BIRTH
        *(check_option+2) = 'b';
    }
    if(*(option1+i) == 'm'){//MOVIES
      *(check_option+3) = 'm';
    }
   }
   while(save_actor -> next_actor != actor -> tail){//TAIL의 전까지
     save_actor = save_actor -> next_actor;//HEAD의 다음 부터
     if(*(check_option+0) == 'n'){//NAME
     fprintf(file,"Name :%s\n",save_actor -> name);
     }
     if(*(check_option+1) == 's'){//SEX
     fprintf(file,"Sex :%c\n",save_actor -> sex);
     }
     if(*(check_option+2) == 'b'){//BIRTH
     fprintf(file,"Birth :%d\n",save_actor -> birth);
     }
     if(*(check_option+3) == 'm'){//MOVIES
       save_word = save_actor -> title;
       i=1;
       while(save_word -> next != NULL){
     fprintf(file,"Movie %d :%s\n",i,save_word -> data);
     save_word = save_word -> next;
     i++;
   }
   fprintf(file,"Movie %d :%s\n",i,save_word -> data);
     }
   }
	 free(option1);//동적 할당한 메모리를 버림
	 free(check_option);//동적 할당한 메모미를 버림
   fclose(file);//파일을 닫음
  }
}
void save(ALL_M *movie,ALL_A *actor, ALL_D *director,char tag){
  /////////////////////////////////////////////////////////
  //SAVE를 받은 상황에서 옵션을 받지 않고 태그만 받았을 때에 사용하는 함수
  FILE *file;//파일을 열기 위해 선언
  LINK_W *print_word;//여러개를 저장하기 위해 사용
  LINK_M *save_movie;//HEAD를 포인트하기 위해 사용
  LINK_A *save_actor;//HEAD를 포인트하기 위해 사용
  LINK_D *save_director;//HEAD를 포인트하기 위해 사용
  save_actor = actor -> head;//HEAD를 포인트
  save_director = director -> head;//HEAD를 포인트
  save_movie = movie -> head;//HEAD를 포인트
  print_word = (LINK_W *)malloc(sizeof(LINK_W));//여러개를 저장하기 위해 동적할당을 받아줌
  if(tag == 'm'){//영화 일 때
    file = fopen("movie_list.txt","w");//리스트파일에 씀
  while(save_movie -> next_movie != movie -> tail){//TAIL의 전까지
    save_movie = save_movie -> next_movie;//HEAD의 다음부분 부터
    //정보를 저장
    fprintf(file,"%d:%s:%s:%s:%d:%d:",save_movie -> serial_num,save_movie -> title,save_movie -> genre,save_movie -> director -> data,save_movie -> year,save_movie -> time);
    print_word = save_movie -> actor;//
    while(print_word -> next != NULL){
      fprintf(file,"%s,",print_word->data);
      print_word = print_word->next;
    }
    fprintf(file,"%s\n", print_word->data);
  }
  fclose(file);
  //파일을 닫아줌
  free(print_word);
  //동적할당을 버려줌
}
else if(tag == 'a'){//배우 일 때
  file = fopen("actor_list.txt","w");//리스트 파일을 열어줌
  while(save_actor -> next_actor != actor -> tail){//TAIL의 전까지
    save_actor = save_actor -> next_actor;//HEAD의 다음 부분 부터
    //정보를 저장
    fprintf(file,"%d:%s:%c:%d",save_actor -> serial_num,save_actor -> name,save_actor ->sex,save_actor -> birth);
    print_word = save_actor -> title;
    while(print_word -> next != NULL){
      fprintf(file,"%s,",print_word -> data);
      print_word = print_word -> next;
    }
    fprintf(file,"%s\n",print_word -> data);
  }
  fclose(file);//파일을 닫아줌
  free(print_word);//동적할당을 버려줌
}
else if(tag == 'd'){//감독 일 때
  file = fopen("director_list.txt","w");//리스트 파일을 열어줌
  while(save_director -> next_director != director -> tail){//TAIL의 전까지
    save_director = save_director -> next_director;//HEAD의 다음 부분 부터
    fprintf(file,"%d:%s:%c:%d",save_director -> serial_num,save_director -> name,save_director ->sex,save_director -> birth);
    print_word = save_director -> title;
    while(print_word -> next != NULL){
      fprintf(file,"%s,",print_word -> data);
      print_word = print_word -> next;
    }
    fprintf(file,"%s\n",print_word -> data);
  }
  free(print_word);//동적할당을 버려줌
  fclose(file);//파일을 닫아줌
}
}
void input_multiple(LINK_M *movie){ //linked list를 이용하여 movie에 여러명의 배우이름을 넣는 함수
	movie -> actor = (LINK_W *)malloc(sizeof(LINK_W));
	LINK_W *tmp;
	tmp = movie -> actor;
	char *temp; // temp에 전체 배우이름들을 , 과 함께 1차 저장
	temp=(char*)calloc(1024, sizeof(char));
	scanf("%[^\n]", temp);
	getchar();
	tmp->data = strtok(temp, ","); // , 를 기준으로 데이터를 분리
	while(tmp->data != NULL){ // 입력된 데이터들을 , 를 기준으로 분리하여 linked list에 저장
		char *check;
		check=(char*)malloc(sizeof(char)*30);
		check = strtok(NULL, ",");
		if(check == NULL){ // 분리가 끝나면 break;
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
void print_multiple_data_to_log_mv(FILE *mv_l, LINK_M *movie){ // movie_log파일에 linked list형태로 저장된 배우이름들을  출력하는 함수
	LINK_W *tmp;
	char *checking_colon;
	tmp = movie -> actor;
	while(tmp->next != NULL){
		checking_colon = change(tmp -> data,":","??;"); // 항목 내 ; 처리
		fprintf(mv_l,"%s,",checking_colon);
		tmp = tmp->next;
	}
	checking_colon = change(tmp -> data,":","??;");
	fprintf(mv_l, "%s\n", checking_colon);
}
void add_movie(ALL_M *movie){//무비형 구조체의 내용을 추가하기 위해 선언함
  FILE *mv_l,*mv_s;//파일을 열기 위해 선언
  LINK_W *save_word,*check_word;//내용을 저장하기 위해 사용
  LINK_M *save_movie,*new_movie,*check;//내용을 저장하기 위한 변수와 저장된 정보의 다음 부분을 포인트 하기 위한 포인터
  char *input_data, input_text,read_text,checking,*checking_colon;//colon으로 끊은 내용,저장한 내용
  int input_num,i,serial = 1;
  save_movie = (LINK_M *)malloc(sizeof(LINK_M));
  check = movie -> head;
  input_data=(char *)malloc(sizeof(char)*30);
 mv_s = fopen("movie_log.txt","r");
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
  save_movie -> actor = (LINK_W *)malloc(sizeof(LINK_W));
  printf("title >");
  scanf("%[^\n]%*c",input_data);
  save_movie -> title = (char *)malloc(strlen(input_data)+1);
  strcpy(save_movie -> title, input_data);
  printf("genre >");
  scanf("%[^\n]%*c",input_data);
  save_movie -> genre = (char *)malloc(strlen(input_data)+1);
  strcpy(save_movie -> genre, input_data);
  printf("director >");
  save_movie -> director = (DIR *)malloc(sizeof(DIR));
  scanf("%[^\n]%*c",input_data);
  save_movie -> director -> data = (char *)malloc(strlen(input_data)+1);
  strcpy(save_movie -> director -> data, input_data);
  printf("year >");
  scanf("%d",&input_num);
  save_movie -> year = input_num;
  printf("time >");
  scanf("%d%*c",&input_num);
  save_movie -> time = input_num;
  new_movie = movie -> head;
  printf("actor >");
  input_multiple(save_movie);
  while(check -> next_movie != movie -> tail){
    check = check -> next_movie;
    if(strcmp(check -> title,save_movie -> title)==0){
      printf("@@ You have the same record in movie list\n");
      printf("%d:%s:%s:%s:",check -> serial_num, check -> title,check -> genre,check -> director -> data);
      printf("%d:%d:",check -> year,check -> time);
      check_word = check -> actor;
      while(check_word -> next != NULL){
        printf("%s,",check_word -> data);
        check_word = check_word -> next;
      }
      printf("%s\n\n",check_word -> data);
      printf("Do You want to add any way?(Y/N)");
      scanf("%c%*c",&checking);
      break;
    }
  }
  if(checking == 'N'||checking == 'n'){
    free(input_data);
		free(save_movie);
  }
  else{
    mv_l=fopen("movie_log.txt","a");
  fprintf(mv_l,"add:%d:",save_movie -> serial_num);
	checking_colon = change(save_movie -> title,":","??;");
  fprintf(mv_l,"%s:",checking_colon);
	checking_colon = change(save_movie -> genre,":","??;");
  fprintf(mv_l,"%s:",checking_colon);
	checking_colon = change(save_movie -> director -> data,":","??;");
  fprintf(mv_l,"%s:",checking_colon);
  fprintf(mv_l,"%4d:",save_movie -> year);
  fprintf(mv_l,"%3d:",save_movie -> time);
  print_multiple_data_to_log_mv(mv_l,save_movie);
  while(new_movie -> next_movie != movie -> tail){
  new_movie = new_movie -> next_movie;
}
  new_movie -> next_movie = save_movie;
  save_movie -> next_movie = movie -> tail;
  free(input_data);
  movie -> size++;
  fclose(mv_l);
}
}
void change_num_to_string(char* str, int num){ // 숫자를 문자열로 변환하는 함수
	int cnt=1; // 자릿수를 세는 변수
        int check = num;
        while(check != check%10){ // 자릿수 세기
                cnt++;
                check = check/10;
        }
        for(int i=0; i<cnt; i++){ // 10으로 나눈 나머지가 각 자리의 숫자이므로 그 결과를 res에 저장
                int div=1;
                for(int j=0; j<cnt-i-1; j++)
                        div *= 10;
                int res = num/div;
                char input = res+48; // 문자열로 변환해서 저장
                *(str+i) = input;
                num -= res*div;
        }
}
int update_movie(ALL_M *movie, char *option, int serial){ // movie에 관한 내용을 업데이트하여 movie_ log를 작성하는 함수
	FILE *mv_l;
	mv_l = fopen("movie_log","a"); // movie_log파일을 열음
	LINK_M *tmp = movie->head;
	int state; // title 중복 데이터 체크를 위한 변수
	LINK_M *update; // 입력받은 시리얼넘버에 해당하는 구조체
	while(tmp->next_movie != movie->tail){ // 입력받은 시리얼넘버와 같은 시리얼넘버를 가지고 있는 구조체를 찾음
		tmp = tmp->next_movie;
		if(tmp->serial_num == serial){
			update = tmp; // update가 찾은 구조체를 포인트
			break;
		}
	}
	if(tmp->next_movie == movie->tail){ // 입력받은 시리얼넘버를 가진 구조체가 없을 경우(예를 들어,delete로 삭제한 후 update로 다시 접근했을 때)
                printf("There is no data.\n"); 
                printf("You can`t update.\n");
                return 1;
        }

	int check = strlen(option); // 옵션의 개수 저장 
	char *check_option = (char*)calloc(6, sizeof(char)); // check_option 방에 입력받은 옵션을 넣어줌
	char *revise = (char*)malloc(sizeof(char)*2); // 이미 같은 내용의 데이터가 있을 때 수정여부를 물을 변수
	int num;
	char* str = (char*)calloc(30, sizeof(char));
	for(int i=0; i<check; i++){ // 입력받지 않은 옵션의 방에는 \0문자가 있음 (callco()으로 동적할당)
		if(*(option+i) == 't') *(check_option+0) = 't'; // 옵션을 순서대로 입력하지 않아도 tgdyra 순서대로 출력
		if(*(option+i) == 'g') *(check_option+1) = 'g';
		if(*(option+i) == 'd') *(check_option+2) = 'd';
		if(*(option+i) == 'y') *(check_option+3) = 'y';
		if(*(option+i) == 'r') *(check_option+4) = 'r';
		if(*(option+i) == 'a') *(check_option+5) = 'a';
	}
	fprintf(mv_l, "update:%d:", serial); // movie_log에 작석 시작
	if(*(check_option+0) == 't'){ // 옵션 t를 입력 받을 경우
		state = 0; // state=0 이면 중복되는 데이터가 없음
		printf("title > ");
		scanf("%[^\n]", str);
		getchar();
		LINK_M *checking = (LINK_M*)malloc(sizeof(LINK_M)); // linked list 형태로 저장된 모든 데이터를 도는 포인터
		checking = tmp;
		while(checking -> next_movie != movie -> tail){
			checking = checking -> next_movie;
			if((strcmp(str, checking->title))==0){ // 입력받은 문자열과 기존에 저장되어있던 데이터가 같으면 수정여부를 물음
				printf("You have the same record.\n");
				printf("title : %s\n", checking->title);
				printf("Do you want to revise?(Y : yes, N: no)\n");
				scanf("%s", revise);
				getchar();
				if((strcmp(revise, "Y") == 0)) fprintf(mv_l, "%s:", checking->title); // yes이면 movie_log 파일에 문자열을 찍어줌
				else if((strcmp(revise, "N")==0)) fprintf(mv_l, "=:"); // no이면 = 를 찍어줌
				state = 1; // 중복데이터가 있었으므로 state=1
				break;
			}
		}
		if(state != 1) fprintf(mv_l, "%s:", str); // 중복 데이터가 없으면 새로 입력받은 문자열을 찍음
	}
	else if(*(check_option+0) == '\0') fprintf(mv_l, "=:"); // 옵션 t를 입력받지 않았을 경우
	if(*(check_option+1) == 'g'){ // 옵션 g를 입력받았을 경우
		printf("genre > ");
                scanf("%[^\n]", str);
                getchar();
		if((strcmp(str, update->title))==0){ // 입력받은 문자열과 기존에 저장되어있던 데이터가 같으면
			printf("You have the same record.\n");
		 	printf("title : %s\n", update->title);
		 	printf("Do you want to revise?(Y : yes, N: no)\n"); // 수정여부를 물음
		 	scanf("%s", revise);
		 	getchar();
		 	if((strcmp(revise, "Y") == 0)) fprintf(mv_l, "%s:", update->title);
		 	else if((strcmp(revise, "N")==0)) fprintf(mv_l, "=:");
		}
	}
	else if(*(check_option+1) == '\0') fprintf(mv_l, "=:"); // 옵션 g를 입력받지 않았을 경우
	if(*(check_option+2) == 'd'){ // 옵션 d를 입력받았을 경우
		printf("director > ");
                scanf("%[^\n]", str);
                getchar();
                if((strcmp(str, update->director->data))==0){
			printf("You have the same record.\n");
                        printf("director : %s\n", update->director->data);
                        printf("Do you want to revise?(Y : yes, N: no)\n");
                        scanf("%s", revise);
			getchar();
                        if(strcmp(revise, "Y")==0) fprintf(mv_l, "%s:", update->director->data);
			else if((strcmp(revise, "N")==0)) fprintf(mv_l, "=:");
		}
                else fprintf(mv_l, "%s:", str);
	}
	else if(*(check_option+2) == '\0') fprintf(mv_l, "=:"); // 옵션 d를 입력받지 않았을 경우
	if(*(check_option+3) == 'y'){ // 옵션 y를 입력받았을 경우
		printf("year > ");
                scanf("%d", &num);
		char *snum = (char*)calloc(2, sizeof(char)); // 숫자를 문자열로 바꿔 저장할 메모리 할당
		change_num_to_string(snum, num); // 숫자를 문자열로 바꾸는 함수
                if(update->year == num){
			printf("You have the same record.\n");
                        printf("year : %d\n", update->year);
                        printf("Do you want to revise?(Y : yes, N: no)\n");
                        scanf("%s", revise);
			getchar();
                        if(strcmp(revise, "Y")==0) fprintf(mv_l, "%d:", update->year);
			else if((strcmp(revise, "N")==0)) fprintf(mv_l, "=:");
		}
                else fprintf(mv_l, "%d:", num);
	}
	else if(*(check_option+3) == '\0') fprintf(mv_l, "=:"); // 옵션 y를 입력받지 않았을 경우
	if(*(check_option+4) == 'r'){ // 옵션 r를 입력받았을 경우
		printf("running time > ");
                scanf("%d", &num);
		char *snum = (char*)calloc(2, sizeof(char));
		change_num_to_string(snum, num);
                if(update->time == num){
			printf("You have the same record.\n");
                        printf("running time : %d\n", update->time);
                        printf("Do you want to revise?(Y : yes, N: no)\n");
                        scanf("%s", revise);
			getchar();
                        if(strcmp(revise, "Y")==0) fprintf(mv_l, "%d:", update->time);
			else if((strcmp(revise, "N")==0)) fprintf(mv_l, "=:");
		}
                else fprintf(mv_l, "%d:", num);
	}
	else if(*(check_option+4) == '\0') fprintf(mv_l, "=:"); // 옵션 r을 입력받지 않았을 경우
        if(*(check_option+5) == 'a'){ // 옵션 a를 입력받았을 경우
	       printf("actor > ");

	       LINK_M *ttmp = (LINK_M*)malloc(sizeof(LINK_M)); // 입력받은 데이터를 ttmp에 임시로 저장
	       input_multiple(ttmp); // linked list 형태의 데이터를 저장하는 함수

	       int a=1, b=1; // a는 새로 입력받은 데이터의 개수를 세는 변수, b는 기존에 있었던 데이터의 개수를 세는 변수
	       LINK_W *new;
	       new = ttmp->actor;

	       while(new ->next != NULL){
		       a++;
		      new = new->next;
	       }
	       new = ttmp->actor;
	       LINK_W *org;
	       org = update->actor;
	       while(org->next != NULL){
		       b++;
		      org = org->next;
	       }
	       org = update->actor;
	       if(a != b){ // 기존데이터의 개수와 새로운데이터의 개수가 같지 않으면 
		       while(new->next != NULL){
			       fprintf(mv_l, "%s,", new->data); // movie_log에 찍음
			       new = new->next;
		       }
		       fprintf(mv_l, "%s\n", new->data);
	       }
	       else{ // 기존데이터 개수와 새로운데이터 개수가 같으면 내용이 같을 수도 다를 수도 있음
		       char *gar1 = (char*)calloc(200, sizeof(char)); // 새로운 데이터들을 , 와 함께  저장
		       char *gar2 = (char*)calloc(200, sizeof(char));// 기존의 데이터를을 , 와 함께 저장
		       while(new->next != NULL){
			       strcpy(gar1+strlen(gar1), new->data);
			       strcat(gar1, ",");
			       new = new->next;
		       }
		       strcat(gar1, new->data);
		       while(org->next != NULL){
			       strcpy(gar2+strlen(gar2), org->data);
			       strcat(gar2, ",");
			       org = org->next;
		       }
		       strcat(gar2, org->data);
		       new = ttmp->actor;
		       org = update->actor;
		       if(strcmp(gar1, gar2)==0){ // 새로운 데이터와 기존의 데이터의 내용이 모두 같으면
			       printf("You have the same records.\n");
			       printf("actors : %s\n", gar1);
			       printf("Do you want to revise?(Y : yes, N : no)\n"); // 수정여부를 물음
			       scanf("%s", revise);
			       getchar();
			       if(strcmp(revise, "Y")==0){
				       while(org->next != NULL){
					       fprintf(mv_l, "%s,", org->data);
					       org = org->next;
				       }
				       fprintf(mv_l, "%s\n", org->data);
			       }
			       else if(strcmp(revise, "N")==0) fprintf(mv_l, "=\n");
		       }
		       else{ // 새로운 데이터와 기존의 데이터의 내용이 같지 않으면
			       while(new->next != NULL){
				       fprintf(mv_l, "%s,", new->data); // 새로운 데이터들을 movie_log에 찍어줌
				       new = new->next;
			       }
			       fprintf(mv_l, "%s\n", new->data);
		       }
	       }

	}
	else if(*(check_option+5) == '\0') fprintf(mv_l, "=\n"); // 옵션 a를 받지 않았을 경우
	fclose(mv_l); // movie_log파일을 닫아서 작성한 내용을 저장
	return 0;
}
int delete_movie(ALL_M *movie,int serial){
  LINK_M *pre_movie, *del_movie;
  FILE *mv_d;
  pre_movie = (LINK_M *)malloc(sizeof(LINK_M));
  del_movie = (LINK_M *)malloc(sizeof(LINK_M));
  pre_movie = movie -> head;
  while(pre_movie -> next_movie != movie -> tail || pre_movie -> serial_num == serial){
    if(pre_movie -> serial_num == serial){
      del_movie -> next_movie = pre_movie -> next_movie;
      mv_d = fopen("movie_log.txt","a");
      fprintf(mv_d,"delete:%d::::::\n", serial);
      fclose(mv_d);
			free(pre_movie -> actor);
      free(pre_movie);
      movie -> size--;
      return 1;
    }
    del_movie = pre_movie;
    pre_movie = pre_movie -> next_movie;
  }
  return 0;
}
int add_movie_list(ALL_M *movie,FILE *mv_l){
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
  fscanf(mv_l,"%*c%[^:]",read_data);
  new_movie -> title = (char *)malloc(strlen(read_data));
	new_movie -> title = change(read_data,"??;",":");
  fscanf(mv_l,"%*c%[^:]",read_data);
  new_movie -> genre = (char *)malloc(strlen(read_data));
  new_movie -> genre = change(read_data,"??;",":");
  fscanf(mv_l,"%*c%[^:]",read_data);
  new_movie -> director = (DIR *)malloc(sizeof(DIR));
  new_movie -> director -> data = (char *)malloc(strlen(read_data));
  new_movie -> director -> data = change(read_data,"??;",":");
  fscanf(mv_l,"%*c%d",&read_num);
  new_movie -> year = read_num;
  fscanf(mv_l,"%*c%d%*c",&read_num);
  new_movie -> time = read_num;
  fscanf(mv_l,"%[^\n]%*c",temp);
  tmp->data = strtok(temp, ",");
	tmp->data = change(tmp -> data , "??;",":");
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
			tmp->data = change(check,"??;",":");
		}
	}
  link_movie = movie -> head;
  while(link_movie -> next_movie != movie -> tail)
  link_movie = link_movie -> next_movie;
  link_movie -> next_movie = new_movie;
  new_movie -> next_movie = movie -> tail;
  free(read_data);
}
void update_movie_list(ALL_M *movie,FILE *mv_l){ // movie_log를 읽어 실제 저장된 데이터를 업데이트 해주는 함수
	char *read_data = (char*)calloc(30, sizeof(char)); 
	int read_num;
	fscanf(mv_l, "%*c%d", &read_num); // movie_log파일에서 시리얼넘버를 읽어옴

	LINK_M *tmp = (LINK_M*)malloc(sizeof(LINK_M));
	tmp = movie->head;
	LINK_M *update = (LINK_M*)malloc(sizeof(LINK_M));
	while(tmp->next_movie != movie->tail){ // 읽어온 시리얼 넘버와 같은 시리얼 넘버를 가진 구조체를 찾음
		tmp = tmp->next_movie;
		if(tmp->serial_num == read_num){
			update = tmp; // update가 찾은 구조체를 포인트 하게함
			break;
		}
	}
	update->serial_num = read_num;

	fscanf(mv_l, "%*c%[^:]", read_data); // title을 읽어옴
	if(strcmp(read_data, "=")!=0) // 읽어온 데이터가 = 가 아니면 (수정사항이 있으면)
		strcpy(update->title, read_data); // 수정

	fscanf(mv_l, "%*c%[^:]", read_data); // genre을 읽어옴
	if(strcmp(read_data, "=")!=0)
		strcpy(update->genre, read_data);
  
	fscanf(mv_l, "%*c%[^:]", read_data); // director을 읽어옴
	if(strcmp(read_data, "=")!=0)
		strcpy(update->director->data, read_data);

	fscanf(mv_l, "%*c%[^:]", read_data); // year을 읽어옴
	if(strcmp(read_data, "=")!=0){
		read_num = atoi(read_data); // 읽어온 데이터는 문자열이므로 숫자로 변환해줌
		update->year = read_num;
	}

	fscanf(mv_l, "%*c%[^:]%*c", read_data); // time을 읽어옴
	if(strcmp(read_data, "=")!=0){
		read_num = atoi(read_data);
		update->time = read_num;
	}

	char *temp = (char*)calloc(300, sizeof(char));
	fscanf(mv_l, "%[^\n]%*c", temp); // actor을 전체 다 읽어옴(, 포함)
	if(strcmp(temp, "=")!=0){
		LINK_W *h = (LINK_W*)malloc(sizeof(LINK_W)); 
		LINK_W *l = (LINK_W*)malloc(sizeof(LINK_W));
		h = update->actor; // 
		while(h->next != NULL){ //기존에 저장되어있던 데이터를 지워주는 작업
			LINK_W *d = (LINK_W*)malloc(sizeof(LINK_W));
			d = h; // 앞에서부터 하나씩 메모리 반납
			l = d->next;
			free(d->data);
			free(d);
			h = l; // 메모리를 반납함으로써 떨어진 linked list 를 이어줌
		}
		free(l->data); // 마지막 남은 메모리 반납
		free(l);
		update->actor = (LINK_W *)malloc(sizeof(LINK_W)); // 새로운 linked list를 할당받음
		h->data = strtok(temp, ","); // , 를 기준으로 토큰을 나눠서 저장
		while(h->data != NULL){
			char *check = (char*)malloc(sizeof(char)*30);
			check = strtok(NULL, ",");
			if(check == NULL){
				h->next = NULL;
				break;
			}
			else{
				h->next = (LINK_W *)malloc(sizeof(LINK_W));
				h = h->next;
				h->next = NULL;
				h->data = check;
			}
		}
	}
}

int delete_movie_list(ALL_M *movie,FILE *mv_l){
  LINK_M *pre_movie, *del_movie;
  int read_num;
  fscanf(mv_l,":%d%*[^\n]%*c",&read_num);
  pre_movie = (LINK_M *)malloc(sizeof(LINK_M));
  del_movie = (LINK_M *)malloc(sizeof(LINK_M));
  pre_movie = movie -> head;
  while(pre_movie -> next_movie != movie -> tail || pre_movie -> serial_num == read_num){
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
int scan_movie_log(ALL_M *movie){
  LINK_M *save_movie;
  LINK_W *save_word,*last_word;
  FILE *mv_l;
  char *input_data,input_text,read_text,garbage;
  int input_num,serial,i;
  mv_l=fopen("movie_log.txt","r");
  save_movie=(LINK_M *)malloc(sizeof(LINK_M));
  input_data=(char *)malloc(sizeof(char)*30);
  while(1){
    if(mv_l == NULL){
      return 1;
      break;
    }
  fscanf(mv_l,"%[^:]u",input_data);
  if(strcmp(input_data,"add")==0){
    add_movie_list(movie,mv_l);
    movie -> size++;
    read_text=fgetc(mv_l);
    fseek(mv_l,-1,SEEK_CUR);
    if(read_text==EOF)
    break;
  }
  else if(strcmp(input_data,"update")==0){
		update_movie_list(movie,mv_l);
    read_text=fgetc(mv_l);
    fseek(mv_l,-1,SEEK_CUR);
    if(read_text==EOF)
    break;
  }
  else if(strcmp(input_data,"delete")==0){
    delete_movie_list(movie,mv_l);
    movie -> size--;
    read_text=fgetc(mv_l);
    fseek(mv_l,-1,SEEK_CUR);
    if(read_text==EOF)
    break;
  }
  else
  break;
}
FILE *file;
file = fopen("movie_list.txt","w");
LINK_W *print_word;
LINK_M *print_movie;
print_movie = movie -> head;
print_word = (LINK_W *)malloc(sizeof(LINK_W));
    while(print_movie -> next_movie != movie -> tail){
      print_movie = print_movie -> next_movie;
      fprintf(file,"%d:%s:%s:%s:%d:%d:",print_movie -> serial_num,print_movie -> title,print_movie -> genre,print_movie -> director -> data,print_movie -> year,print_movie -> time);
      print_word = print_movie -> actor;
      while(print_word -> next != NULL){
        fprintf(file,"%s,",print_word->data);
        print_word = print_word->next;
      }
      fprintf(file,"%s\n", print_word->data);
    }
      fclose(file);
return 0;
}
int print_movie_list(ALL_M *movie,int serial){
  LINK_M *print_movie;
  LINK_W *print_word;
  LINK_D *print_director;
  LINK_A *print_actor;
  int i=1;
  print_movie=(LINK_M*)malloc(sizeof(LINK_M));
  print_movie=movie -> head;
  print_word = (LINK_W *)malloc(sizeof(LINK_W));
  while(print_movie -> next_movie != movie -> tail){
    print_movie = print_movie -> next_movie;
    if(print_movie -> serial_num == serial){
      print_director = (LINK_D*)print_movie -> director -> com_word;
    printf("  %d, %s, %s\n",print_movie -> serial_num,print_movie -> title,print_movie -> genre);
    if(print_movie -> director -> com_word == NULL)
    printf(" D: %s (  -  )\n",print_movie -> director -> data);
    else{
      printf(" D: %s (%d)\n",print_movie -> director -> data,print_director -> birth);
    }
    print_word = print_movie -> actor;
    while(print_word -> next != NULL){
      print_actor = (LINK_A *)print_word -> com_word;
      if(print_word -> com_word == NULL)
  		printf("  A%d : %s (  -  )\n",i,print_word->data);
      else
      printf("  A%d : %s (%d)\n",i,print_word->data,print_actor -> birth );
  		print_word = print_word->next;
      i++;
  	}
    print_actor = (LINK_A *)print_word -> com_word;
    if(print_word -> com_word == NULL)
    printf("  A%d : %s (  -  )\n",i,print_word->data);
    else
    printf("  A%d : %s (%d)\n",i,print_word->data,print_actor -> birth);
    return 1;
  }
}
}
void input_multiple_act(LINK_A *actor){ // linked list를 이용하여 배우의 best_movie를 저장하는 함수
	actor -> title = (LINK_W *)malloc(sizeof(LINK_W));
	LINK_W *tmp;
	tmp = actor -> title;
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
void print_multiple_data_to_log_act(FILE *act_l, LINK_A *actor){ // actor_log파일에 linked list 형태로 저장된 배우의 best_movie를 출력하는 함수
	LINK_W *tmp;
	char *checking_colon;
	tmp = actor->title;
	while(tmp->next != NULL){
		checking_colon = change(tmp->data,":","??;"); // 항목 내 ; 처리
		fprintf(act_l,"%s,",checking_colon);
		tmp = tmp->next;
	}
	checking_colon = change(tmp->data,":","??;");
	fprintf(act_l, "%s\n",checking_colon);
}

void add_actor(ALL_A *actor){
  FILE *act_l,*act_s;
  LINK_W *save_word,*check_word;
  LINK_A *save_actor,*new_actor,*check;
  char *input_data, input_text,read_text,checking,*checking_colon;
  int input_num,i,serial = 1;
  save_actor = (LINK_A *)malloc(sizeof(LINK_A));
  check = actor -> head;
  input_data=(char *)malloc(sizeof(char)*30);
  act_s = fopen("actor_log.txt","r");
  if(act_s == NULL)
  serial = 1;
  else{
  fscanf(act_s,"%c",&read_text);
  if(read_text == 'a')
   serial++;
  while(read_text!=EOF){
     fscanf(act_s,"%c",&read_text);
     if(read_text == '\n'){
       fscanf(act_s,"%c",&read_text);
         if(read_text == 'a')
           serial++;
         else if(read_text != 'u' && read_text != 'd')
         break;
         }
  }
  fclose(act_s);
  }
  save_actor -> serial_num = serial;
  printf("name >");
  scanf("%[^\n]%*c",input_data);
  save_actor -> name = (char *)malloc(strlen(input_data)+1);
  strcpy(save_actor -> name, input_data);
  printf("sex >");
  scanf("%c%*c",&input_text);
  save_actor -> sex = input_text;
  printf("birth >");
  scanf("%d%*c",&input_num);
  save_actor -> birth = input_num;
  printf("best_movies >");
  input_multiple_act(save_actor);
  while(check -> next_actor != actor -> tail){
    check = check -> next_actor;
    if(strcmp(check -> name,save_actor -> name)==0){
      printf("@@ You have the same record in actor list\n");
      printf("%d:%s:%c:%d:",check -> serial_num,check -> name,check -> sex, check -> birth);
      check_word = check -> title;
      while(check_word -> next != NULL){
        printf("%s,",check_word -> data);
        check_word = check_word -> next;
      }
      printf("%s\n",check_word -> data);
      printf("Do You want to add any way?(Y/N)");
      scanf("%c%*c",&checking);
      break;
    }
  }
  if(checking == 'N'||checking == 'n'){
    free(input_data);
  }
  else{
    act_l=fopen("actor_log.txt","a");
  fprintf(act_l,"add:%d:",save_actor -> serial_num);
	checking_colon = change(save_actor -> name, ":","??;");
  fprintf(act_l,"%s:",checking_colon);
  fprintf(act_l,"%c:",save_actor ->sex);
  fprintf(act_l,"%8d:",save_actor -> birth);
  print_multiple_data_to_log_act(act_l,save_actor);
  new_actor = actor -> head;
  while(new_actor -> next_actor != actor -> tail)
  new_actor = new_actor -> next_actor;
  new_actor -> next_actor = save_actor;
  save_actor -> next_actor = actor -> tail;
  free(input_data);
  actor -> size ++;
  fclose(act_l);
  }
}
int update_actor(ALL_A *actor, char *option, int serial){
	FILE *act_l = fopen("actor_log","a");
	LINK_A *temp = actor->head;
	LINK_A *update = (LINK_A*)malloc(sizeof(LINK_A));
	int state;
	while(temp->next_actor != actor->tail){
		temp = temp->next_actor;
		if(temp->serial_num == serial){
			update = temp;
			break;
		}
	}
	if(temp->next_actor == actor->tail){
                printf("There is no data.\n");
                printf("You can`t update.\n");
                return 1;
        }

	int check = strlen(option);
	char *check_option = (char*)calloc(4, sizeof(char));
	char *revise = (char*)malloc(sizeof(char)*2);
	char *str = (char*)calloc(30, sizeof(char));
	char c;
	int num;
	for(int i=0; i<check; i++){
		if(*(option+i) == 'n') *(check_option+0) = 'n';
		if(*(option+i) == 's') *(check_option+1) = 's';
		if(*(option+i) == 'b') *(check_option+2) = 'b';
		if(*(option+i) == 'm') *(check_option+3) = 'm';
	}
	fprintf(act_l, "update:%d:", serial);
	if(*(check_option+0) == 'n'){
		state=0;
	       printf("name > ");
	       scanf("%[^\n]", str);
	       getchar();
	       LINK_A *checking = (LINK_A*)malloc(sizeof(LINK_A));
	       checking = actor->head;
	       while(checking->next_actor != actor->tail){
		       checking = checking->next_actor;
	       if((strcmp(str, checking->name))==0){
		       printf("You have the same record.\n");
		       printf("name : %s\n", checking->name);
		       printf("Do you want to revise?(Y : yes, N: no)\n");
		       scanf("%s", revise);
		       getchar();
		       if((strcmp(revise, "Y") == 0)) fprintf(act_l, "%s:", checking->name);
		       else if((strcmp(revise, "N")==0)) fprintf(act_l, "=:");
		       state=1;
		       break;
	       }
	       }
	       if(state != 1)fprintf(act_l, "%s:", str);
	}
	else if(*(check_option+0) == '\0') fprintf(act_l, "=:");
	if(*(check_option+1) == 's'){
		printf("sex > ");
                scanf("%c", &c);
                getchar();
                if(update->sex == c){
			printf("You have the same record.\n");
                        printf("sex : %c\n", update->sex);
                        printf("Do you want to revise?(Y : yes, N: no)\n");
                        scanf("%s", revise);
			getchar();
                        if(strcmp(revise, "Y")==0) fprintf(act_l, "%c:", update->sex);
			else if((strcmp(revise, "N")==0)) fprintf(act_l, "=:");
		}
                else fprintf(act_l, "%c:", c);
	}
	else if(*(check_option+1) == '\0') fprintf(act_l, "=:");
	if(*(check_option+2) == 'b'){
		printf("birth > ");
                scanf("%d", &num);
		char *snum = (char*)calloc(2, sizeof(char));
		change_num_to_string(snum, num);
                if(update->birth == num){
			printf("You have the same record.\n");
                        printf("birth : %d\n", update->birth);
                        printf("Do you want to revise?(Y : yes, N: no)\n");
                        scanf("%s", revise);
			getchar();
                        if(strcmp(revise, "Y")==0) fprintf(act_l, "%d:", update->birth);
			else if((strcmp(revise, "N")==0)) fprintf(act_l, "=:");
		}
                else fprintf(act_l, "%d:", num);
	}
	else if(*(check_option+2) == '\0') fprintf(act_l, "=:");
        if(*(check_option+3) == 'm'){
	       printf("movies > ");

	       LINK_A *ttmp = (LINK_A*)malloc(sizeof(LINK_A));
	       input_multiple_act(ttmp);

	       int a=1, b=1;
	       LINK_W *new;
	       new = ttmp->title;

	       while(new ->next != NULL){
		       a++;
		      new = new->next;
	       }
	       new = ttmp->title;
	       LINK_W *org;
	       org = update->title;
	       while(org->next != NULL){
		       b++;
		      org = org->next;
	       }
	       org = update->title;
	       if(a != b){
		       while(new->next != NULL){
			       fprintf(act_l, "%s,", new->data);
			       new = new->next;
		       }
		       fprintf(act_l, "%s\n", new->data);
	       }
	       else{
		       char *gar1 = (char*)calloc(200, sizeof(char));
		       char *gar2 = (char*)calloc(200, sizeof(char));
		       while(new->next != NULL){
			       strcpy(gar1+strlen(gar1), new->data);
			       strcat(gar1, ",");
			       new = new->next;
		       }
		       strcat(gar1, new->data);
		       while(org->next != NULL){
			       strcpy(gar2+strlen(gar2), org->data);
			       strcat(gar2, ",");
			       org = org->next;
		       }
		       strcat(gar2, org->data);
		       new = ttmp->title;
		       org = update->title;
		       if(strcmp(gar1, gar2)==0){
			       printf("You have the same records.\n");
			       printf("movies : %s\n", gar1);
			       printf("Do you want to revise?(Y : yes, N : no)\n");
			       scanf("%s", revise);
			       getchar();
			       if(strcmp(revise, "Y")==0){
				       while(org->next != NULL){
					       fprintf(act_l, "%s,", org->data);
					       org = org->next;
				       }
				       fprintf(act_l, "%s\n", org->data);
			       }
			       else if(strcmp(revise, "N")==0) fprintf(act_l, "=\n");
		       }
		       else{
			       while(new->next != NULL){
				       fprintf(act_l, "%s,", new->data);
				       new = new->next;
			       }
			       fprintf(act_l, "%s\n", new->data);
		       }
	       }

	}
	else if(*(check_option+3)== '\0') fprintf(act_l, "=\n");
	fclose(act_l);
	return 0;
}


int delete_actor(ALL_A *actor,int serial){
  LINK_A *pre_actor, *del_actor;
  FILE *act_d;
  pre_actor = (LINK_A *)malloc(sizeof(LINK_A));
  del_actor = (LINK_A *)malloc(sizeof(LINK_A));
  pre_actor = actor -> head;
  while(pre_actor -> next_actor != actor -> tail || pre_actor -> serial_num == serial){
    if(pre_actor -> serial_num == serial){
      del_actor -> next_actor = pre_actor -> next_actor;
      act_d = fopen("actor_log.txt","a");
      fprintf(act_d,"delete:%d::::::\n", serial);
      fclose(act_d);
      free(pre_actor);
      actor -> size --;
      return 1;
    }
    del_actor = pre_actor;
    pre_actor = pre_actor -> next_actor;
  }
  return 0;
}
int add_actor_list(ALL_A *actor,FILE *act_l){
  LINK_A *new_actor,*link_actor;
  LINK_W *tmp;
  char *read_data,read_text,*temp;
  int read_num;
  link_actor = (LINK_A *)malloc(sizeof(LINK_A));
  new_actor = (LINK_A *)malloc(sizeof(LINK_A));
  read_data = (char *)malloc(sizeof(char)*1000);
  new_actor -> title = (LINK_W *)malloc(sizeof(LINK_W));
  tmp = new_actor ->title;
  temp=(char*)calloc(1024, sizeof(char));
  fscanf(act_l,"%*c%d",&read_num);
  new_actor -> serial_num = read_num;
  fscanf(act_l,"%*c%[^:]",read_data);
  new_actor -> name = (char *)malloc(strlen(read_data));
	new_actor -> name = change(read_data,"??;",":");
  fscanf(act_l,"%*c%c",&read_text);
  new_actor -> sex = read_text;
  fscanf(act_l,"%*c%d:",&read_num);
  new_actor -> birth = read_num;
  fscanf(act_l,"%[^\n]%*c",temp);
  tmp->data = strtok(temp, ",");
	tmp -> data = change(tmp -> data,"??;",":");
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
			tmp->data = change(check,"??;",":");
		}
	}
  link_actor = actor -> head;
  while(link_actor -> next_actor != actor -> tail)
  link_actor = link_actor -> next_actor;
  link_actor -> next_actor = new_actor;
  new_actor -> next_actor = actor -> tail;
  free(read_data);
  if(read_text=='\n')
  return 1;
}
void update_actor_list(ALL_A *actor,FILE *act_l){
	char *read_data = (char*)calloc(30, sizeof(char));
	int read_num;
	char read_one;
	fscanf(act_l, "%*c%d", &read_num);

	LINK_A *tmp = (LINK_A*)malloc(sizeof(LINK_A));
	tmp = actor->head;
	LINK_A *update = (LINK_A*)malloc(sizeof(LINK_A));
	while(tmp->next_actor != actor->tail){
		tmp = tmp->next_actor;
		if(tmp->serial_num == read_num){
			update = tmp;
			break;
		}
	}
	update->serial_num = read_num;

	fscanf(act_l, "%*c%[^:]", read_data);
	if(strcmp(read_data, "=")!=0)
		strcpy(update->name, read_data);

	fscanf(act_l, "%*c%c", &read_one);
	if(read_one != '=')
		update->sex = read_one;

	fscanf(act_l, "%*c%[^:]", read_data);
	if(strcmp(read_data, "=")!=0){
		read_num = atoi(read_data);
		update->birth = read_num;
	}

	char *temp = (char*)calloc(300, sizeof(char));
	fscanf(act_l, "%[^\n]%*c", temp);
	if(strcmp(temp, "=")!=0){
		LINK_W *h = (LINK_W*)malloc(sizeof(LINK_W));
		LINK_W *l = (LINK_W*)malloc(sizeof(LINK_W));
		h = update->title;
		while(h->next != NULL){
			LINK_W *d = (LINK_W*)malloc(sizeof(LINK_W));
			d = h;
			l = d->next;
			free(d->data);
			free(d);
			h = l;
		}
		free(l->data);
		free(l);
		update->title = (LINK_W *)malloc(sizeof(LINK_W));
		h->data = strtok(temp, ",");
		while(h->data != NULL){
			char *check = (char*)malloc(sizeof(char)*30);
			check = strtok(NULL, ",");
			if(check == NULL){
				h->next = NULL;
				break;
			}
			else{
				h->next = (LINK_W *)malloc(sizeof(LINK_W));
				h = h->next;
				h->next = NULL;
				h->data = check;
			}
		}
	}
}


int delete_actor_list(ALL_A *actor,FILE *act_l){
  LINK_A *pre_actor, *del_actor;
  int read_num;
  fscanf(act_l,":%d%*[^\n]%*c",&read_num);
  pre_actor = (LINK_A *)malloc(sizeof(LINK_A));
  del_actor = (LINK_A *)malloc(sizeof(LINK_A));
  pre_actor = actor -> head;
  while(pre_actor -> next_actor != actor -> tail || pre_actor -> serial_num == read_num){
    if(pre_actor -> serial_num == read_num){
      del_actor -> next_actor = pre_actor -> next_actor;
      free(pre_actor);
      return 1;
    }
    del_actor = pre_actor;
    pre_actor = pre_actor -> next_actor;
  }
  return 0;
}
int scan_actor_log(ALL_A *actor){
  LINK_A *save_actor;
  LINK_W *save_word,*last_word;
  FILE *act_l;
  char *input_data,input_text,read_text,garbage;
  int input_num,serial,i=1;
  act_l=fopen("actor_log.txt","r");
  save_actor=(LINK_A *)malloc(sizeof(LINK_A));
  input_data=(char *)malloc(sizeof(char)*30);
  while(1){
    if(act_l == NULL){
      return 1;
      break;
    }
  fscanf(act_l,"%[^:]",input_data);
  if(strcmp(input_data,"add")==0){
    add_actor_list(actor,act_l);
    actor -> size ++;
    i++;
    read_text=fgetc(act_l);
    fseek(act_l,-1,SEEK_CUR);
    if(read_text==EOF)
    break;
  }
  else if(strcmp(input_data,"update")==0){
		update_actor_list(actor,act_l);
    read_text=fgetc(act_l);
    fseek(act_l,-1,SEEK_CUR);
    if(read_text==EOF)
    break;
  }
  else if(strcmp(input_data,"delete")==0){
    delete_actor_list(actor,act_l);
    actor -> size --;
    read_text=fgetc(act_l);
    fseek(act_l,-1,SEEK_CUR);
    if(read_text==EOF)
    break;
  }
  else
  break;
}
FILE *file;
file = fopen("actor_list.txt","w");
LINK_W *print_word;
LINK_A *print_actor;
print_actor = actor -> head;
print_word = (LINK_W *)malloc(sizeof(LINK_W));
    while(print_actor -> next_actor != actor -> tail){
      print_actor = print_actor -> next_actor;
      fprintf(file,"%d:%s:%c:%d:",print_actor -> serial_num,print_actor -> name,print_actor -> sex,print_actor -> birth);
      print_word = print_actor -> title;
      while(print_word -> next != NULL){
        fprintf(file,"%s,",print_word->data);
        print_word = print_word->next;
      }
      fprintf(file,"%s\n", print_word->data);
    }
      fclose(file);
return 0;
}
int print_actor_list(ALL_A *actor,int serial){
  LINK_A *print_actor;
  LINK_W *print_word;
  LINK_M *print_movie;
  int i=1;
  print_actor=(LINK_A*)malloc(sizeof(LINK_A));
  print_actor=actor -> head;
  print_word = (LINK_W *)malloc(sizeof(LINK_W));
  while(print_actor -> next_actor != actor -> tail){
    print_actor = print_actor -> next_actor;
    if(print_actor -> serial_num == serial){
    printf("  %d, %s, %c,  %8d\n",print_actor -> serial_num,print_actor -> name,print_actor -> sex,print_actor -> birth);
    print_word = print_actor -> title;
    while(print_word -> next != NULL){
      print_movie = (LINK_M *)print_word -> com_word;
      if(print_movie == NULL)
  		printf("  M%d : %s(  -  )\n",i,print_word->data);
      else
      printf("  M%d : %s(%d,%d)\n",i,print_word->data,print_movie -> year,print_movie -> time);
  		print_word = print_word->next;
      i++;
  	}
    print_movie = (LINK_M *)print_word -> com_word;
    if(print_movie == NULL)
    printf("  M%d : %s(  -  )\n",i,print_word->data);
    else
    printf("  M%d : %s(%d,%d)\n",i,print_word->data,print_movie -> year,print_movie -> time);
    return 1;
  }
}
return 0;
}
void input_multiple_dir(LINK_D *director){ //linked list를 이용하여 감독의 best_movie들을 저장하는 함수
	director -> title = (LINK_W *)malloc(sizeof(LINK_W));
	LINK_W *tmp;
	tmp = director -> title;
	char *temp;
	temp=(char*)calloc(1024, sizeof(char));
	scanf("%[^\n]%*c", temp);
	tmp->data = strtok(temp, ","); // , 을 기준으로 토큰을 분리
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
void print_multiple_data_to_log_dir(FILE *dir_l, LINK_D *director){ // director_log파일에 linked list 형태로 저장된 감독의 best_movie를 출력하는 함수
	LINK_W *tmp;
	char *checking_colon;
	tmp = director->title;
	while(tmp->next != NULL){
		checking_colon = change(tmp->data,":","??;"); // 항목 내 ; 처리
		fprintf(dir_l,"%s,",checking_colon);
		tmp = tmp->next;
	}
	checking_colon = change(tmp->data,":","??;");
	fprintf(dir_l, "%s\n",checking_colon);
}

void add_director(ALL_D *director){
  FILE *dir_l,*dir_s;
  LINK_W *save_word,*check_word;
  LINK_D *save_director,*new_director,*check;
  char *input_data, input_text,read_text,checking,*checking_colon;
  int input_num,i,serial = 1;
  save_director = (LINK_D *)malloc(sizeof(LINK_D));
  check = director -> head;
  input_data=(char *)malloc(sizeof(char)*30);
  dir_s = fopen("director_log.txt","r");
  if(dir_s == NULL)
  serial = 1;
  else{
  fscanf(dir_s,"%c",&read_text);
  if(read_text == 'a')
   serial++;
  while(read_text!=EOF){
     fscanf(dir_s,"%c",&read_text);
     if(read_text == '\n'){
       fscanf(dir_s,"%c",&read_text);
         if(read_text == 'a')
           serial++;
         else if(read_text != 'u' && read_text != 'd')
         break;
         }
  }
  fclose(dir_s);
  }
  save_director -> serial_num = serial;
  printf("name >");
  scanf("%[^\n]%*c",input_data);
  save_director -> name = (char *)malloc(strlen(input_data)+1);
  strcpy(save_director -> name, input_data);
  printf("sex >");
  scanf("%c%*c",&input_text);
  save_director -> sex = input_text;
  printf("birth >");
  scanf("%d%*c",&input_num);
  save_director -> birth = input_num;
  printf("best_movies >");
  input_multiple_dir(save_director);
  while(check -> next_director != director -> tail){
    check = check -> next_director;
    if(strcmp(check -> name,save_director -> name)==0){
      printf("@@ You have the same record in director list\n");
      printf("%d:%s:%c:%d:",check -> serial_num,check -> name,check -> sex, check -> birth);
      check_word = check -> title;
      while(check_word -> next != NULL){
        printf("%s,",check_word -> data);
        check_word = check_word -> next;
      }
      printf("%s\n",check_word -> data);
      printf("Do You want to add any way?(Y/N)");
      scanf("%c%*c",&checking);
      break;
    }
  }
  if(checking == 'N'||checking == 'n'){
    free(input_data);
  }
  else{
    dir_l=fopen("director_log.txt","a");
  fprintf(dir_l,"add:%d:",save_director -> serial_num);
	checking_colon = change(save_director -> name, ":","??;");
  fprintf(dir_l,"%s:",checking_colon);
  fprintf(dir_l,"%c:",save_director ->sex);
  fprintf(dir_l,"%8d:",save_director -> birth);
  print_multiple_data_to_log_dir(dir_l,save_director);
  new_director = director -> head;
  while(new_director -> next_director != director -> tail)
  new_director = new_director -> next_director;
  new_director -> next_director = save_director;
  save_director -> next_director = director -> tail;
  free(input_data);
  director -> size ++;
  fclose(dir_l);
  }
}
int update_director(ALL_D *director, char *option, int serial){
	FILE *dir_l;
	dir_l = fopen("director_log","a");
	LINK_D *temp = director->head;
	int state;
	LINK_D *update = (LINK_D*)malloc(sizeof(LINK_D));
	while(temp->next_director != director->tail){
		temp = temp->next_director;
		if(temp->serial_num == serial){
			update = temp;
			break;
		}
	}
	if(temp->next_director == director->tail){
                printf("There is no data.\n");
                printf("You can`t update.\n");
                return 1;
        }

	int check = strlen(option);
	char *check_option = (char*)calloc(4, sizeof(char));
	char *revise = (char*)malloc(sizeof(char)*2);
	char *str = (char*)calloc(30, sizeof(char));
	char c;
	int num;
	for(int i=0; i<check; i++){
		if(*(option+i) == 'n') *(check_option+0) = 'n';
		if(*(option+i) == 's') *(check_option+1) = 's';
		if(*(option+i) == 'b') *(check_option+2) = 'b';
		if(*(option+i) == 'm') *(check_option+3) = 'm';
	}
	fprintf(dir_l, "update:%d:", serial);
	if(*(check_option+0) == 'n'){
		printf("name > ");
		scanf("%[^\n]", str);
	        getchar();
		LINK_D *checking = (LINK_D*)malloc(sizeof(LINK_D));
		checking = director->head;
		while(checking->next_director != director->tail){
			checking = checking->next_director;
	       if((strcmp(str, checking->name))==0){
		       printf("You have the same record.\n");
		       printf("name : %s\n", checking->name);
		       printf("Do you want to revise?(Y : yes, N: no)\n");
		       scanf("%s", revise);
		       getchar();
		       if((strcmp(revise, "Y") == 0)) fprintf(dir_l, "%s:", checking->name);
		       else if((strcmp(revise, "N")==0)) fprintf(dir_l, "=:");
		       state=1;
		       break;
	       }
	       }
	       if(state != 1) fprintf(dir_l, "%s:", str);
	}
	else if(*(check_option+0) == '\0') fprintf(dir_l, "=:");
	if(*(check_option+1) == 's'){
		printf("sex > ");
                scanf("%c", &c);
                getchar();
                if(c == update->sex){
			printf("You have the same record.\n");
                        printf("sex : %c\n", update->sex);
                        printf("Do you want to revise?(Y : yes, N: no)\n");
                        scanf("%s", revise);
			getchar();
                        if(strcmp(revise, "Y")==0) fprintf(dir_l, "%c:", update->sex);
			else if((strcmp(revise, "N")==0)) fprintf(dir_l, "=:");
		}
                else fprintf(dir_l, "%c:", c);
	}
	else if(*(check_option+1) == '\0') fprintf(dir_l, "=:");
	if(*(check_option+2) == 'b'){
		printf("birth > ");
                scanf("%d", &num);
		char *snum = (char*)calloc(2, sizeof(char));
		change_num_to_string(snum, num);
                if(update->birth == num){
			printf("You have the same record.\n");
                        printf("birth : %d\n", update->birth);
                        printf("Do you want to revise?(Y : yes, N: no)\n");
                        scanf("%s", revise);
			getchar();
                        if(strcmp(revise, "Y")==0) fprintf(dir_l, "%d:", update->birth);
			else if((strcmp(revise, "N")==0)) fprintf(dir_l, "=:");
		}
                else fprintf(dir_l, "%d:", num);
	}
	else if(*(check_option+2) == '\0') fprintf(dir_l, "=:");
        if(*(check_option+3) == 'm'){
	       printf("movies > ");

	       LINK_D *ttmp = (LINK_D*)malloc(sizeof(LINK_D));
	       input_multiple_dir(ttmp);

	       int a=1, b=1;
	       LINK_W *new;
	       new = ttmp->title;

	       while(new ->next != NULL){
		       a++;
		      new = new->next;
	       }
	       new = ttmp->title;
	       LINK_W *org;
	       org = update->title;
	       while(org->next != NULL){
		       b++;
		      org = org->next;
	       }
	       org = update->title;
	       if(a != b){
		       while(new->next != NULL){
			       fprintf(dir_l, "%s,", new->data);
			       new = new->next;
		       }
		       fprintf(dir_l, "%s\n", new->data);
	       }
	       else{
		       char *gar1 = (char*)calloc(200, sizeof(char));
		       char *gar2 = (char*)calloc(200, sizeof(char));
		       while(new->next != NULL){
			       strcpy(gar1+strlen(gar1), new->data);
			       strcat(gar1, ",");
			       new = new->next;
		       }
		       strcat(gar1, new->data);
		       while(org->next != NULL){
			       strcpy(gar2+strlen(gar2), org->data);
			       strcat(gar2, ",");
			       org = org->next;
		       }
		       strcat(gar2, org->data);
		       new = ttmp->title;
		       org = update->title;
		       if(strcmp(gar1, gar2)==0){
			       printf("You have the same records.\n");
			       printf("movies : %s\n", gar1);
			       printf("Do you want to revise?(Y : yes, N : no)\n");
			       scanf("%s", revise);
			       getchar();
			       if(strcmp(revise, "Y")==0){
				       while(org->next != NULL){
					       fprintf(dir_l, "%s,", org->data);
					       org = org->next;
				       }
				       fprintf(dir_l, "%s\n", org->data);
			       }
			       else if(strcmp(revise, "N")==0) fprintf(dir_l, "=\n");
		       }
		       else{
			       while(new->next != NULL){
				       fprintf(dir_l, "%s,", new->data);
				       new = new->next;
			       }
			       fprintf(dir_l, "%s\n", new->data);
		       }
	       }

	}
	else if(*(check_option+3) == '\0') fprintf(dir_l, "=\n");
	fclose(dir_l);
	return 0;
}

int delete_director(ALL_D *director,int serial){
  LINK_D *pre_director, *del_director;
  FILE *dir_d;
  pre_director = (LINK_D *)malloc(sizeof(LINK_D));
  del_director = (LINK_D *)malloc(sizeof(LINK_D));
  pre_director = director -> head;
  while(pre_director -> next_director != director -> tail || pre_director -> serial_num == serial){
    if(pre_director -> serial_num == serial){
      del_director -> next_director = pre_director -> next_director;
      dir_d = fopen("director_log.txt","a");
      fprintf(dir_d,"delete:%d::::::\n", serial);
      fclose(dir_d);
			free(pre_director -> title);
      free(pre_director);
      return 1;
      director -> size--;
    }
    del_director = pre_director;
    pre_director = pre_director -> next_director;
  }
  return 0;
}
int add_director_list(ALL_D *director,FILE *dir_l){
  LINK_D *new_director,*link_director;
  LINK_W *tmp;
  char *read_data,read_text,*temp;
  int read_num;
  link_director = (LINK_D *)malloc(sizeof(LINK_D));
  new_director = (LINK_D *)malloc(sizeof(LINK_D));
  read_data = (char *)malloc(sizeof(char)*1000);
  new_director -> title = (LINK_W *)malloc(sizeof(LINK_W));
  tmp = new_director ->title;
  temp=(char*)calloc(1024, sizeof(char));
  fscanf(dir_l,"%*c%d",&read_num);
  new_director -> serial_num = read_num;
  fscanf(dir_l,"%*c%[^:]s",read_data);
  new_director -> name = (char *)malloc(strlen(read_data));
	new_director -> name = change(read_data,"??;",":");
  fscanf(dir_l,"%*c%c",&read_text);
  new_director -> sex = read_text;
  fscanf(dir_l,"%*c%d:",&read_num);
  new_director -> birth = read_num;
  fscanf(dir_l,"%[^\n]%*c",temp);
  tmp->data = strtok(temp, ",");
	tmp -> data = change(tmp -> data,"??;",":");
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
			tmp->data = change(check,"??;",":");
		}
	}
  link_director = director -> head;
  while(link_director -> next_director != director -> tail)
  link_director = link_director -> next_director;
  link_director -> next_director = new_director;
  new_director -> next_director = director -> tail;
  free(read_data);
  if(read_text=='\n')
  return 1;
}
void update_director_list(ALL_D *director,FILE *dir_l){
	char *read_data = (char*)calloc(30, sizeof(char));
	int read_num;
	char read_one;
	fscanf(dir_l, "%*c%d", &read_num);

	LINK_D *tmp = (LINK_D*)malloc(sizeof(LINK_D));
	tmp = director->head;
	LINK_D *update = (LINK_D*)malloc(sizeof(LINK_D));
	while(tmp->next_director != director->tail){
		tmp = tmp->next_director;
		if(tmp->serial_num == read_num){
			update = tmp;
			break;
		}
	}
	update->serial_num = read_num;

	fscanf(dir_l, "%*c%[^:]", read_data);
	if(strcmp(read_data, "=")!=0)
		strcpy(update->name, read_data);

	fscanf(dir_l, "%*c%c", &read_one);
	if(read_one != '=')
		update->sex = read_one;

	fscanf(dir_l, "%*c%[^:]", read_data);
	if(strcmp(read_data, "=")!=0){
		read_num = atoi(read_data);
		update->birth = read_num;
	}

	char *temp = (char*)calloc(300, sizeof(char));
	fscanf(dir_l, "%[^\n]%*c", temp);
	if(strcmp(temp, "=")==0){
		LINK_W *h = (LINK_W*)malloc(sizeof(LINK_W));
		LINK_W *l = (LINK_W*)malloc(sizeof(LINK_W));
		h = update->title;
		while(h->next != NULL){
			LINK_W *d = (LINK_W*)malloc(sizeof(LINK_W));
			d = h;
			l = d->next;
			free(d->data);
			free(d);
			h = l;
		}
		free(l->data);
		free(l);
		update->title = (LINK_W *)malloc(sizeof(LINK_W));
		h = update->title;
		h->data = strtok(temp, ",");
		while(h->data != NULL){
			char *check = (char*)malloc(sizeof(char)*30);
			check = strtok(NULL, ",");
			if(check == NULL){
				h->next = NULL;
				break;
			}
			else{
				h->next = (LINK_W *)malloc(sizeof(LINK_W));
				h = h->next;
				h->next = NULL;
				h->data = check;
			}
		}
	}
}

int delete_director_list(ALL_D *director,FILE *dir_l){
  LINK_D *pre_director, *del_director;
  int read_num;
  fscanf(dir_l,":%d%*[^\n]%*c",&read_num);
  pre_director = (LINK_D *)malloc(sizeof(LINK_D));
  del_director = (LINK_D *)malloc(sizeof(LINK_D));
  pre_director = director -> head;
  while(pre_director -> next_director != director -> tail || pre_director -> serial_num == read_num){
    if(pre_director -> serial_num == read_num){
      del_director -> next_director = pre_director -> next_director;
      free(pre_director);
      return 1;
    }
    del_director = pre_director;
    pre_director = pre_director -> next_director;
  }
  return 0;
}
int scan_director_log(ALL_D *director){
  LINK_D *save_director;
  LINK_W *save_word,*last_word;
  FILE *dir_l;
  char *input_data,input_text,read_text,garbage;
  int input_num,serial,i=1;
  dir_l=fopen("director_log.txt","r");
  save_director=(LINK_D *)malloc(sizeof(LINK_D));
  input_data=(char *)malloc(sizeof(char)*30);
  while(1){
    if(dir_l == NULL){
      return 1;
      break;
    }
  fscanf(dir_l,"%[^:]",input_data);
  if(strcmp(input_data,"add")==0){
    add_director_list(director,dir_l);
      director -> size ++;
    read_text=fgetc(dir_l);
    fseek(dir_l,-1,SEEK_CUR);
    if(read_text==EOF)
    break;
  }
  else if(strcmp(input_data,"update")==0){
		update_director_list(director,dir_l);
    read_text=fgetc(dir_l);
    fseek(dir_l,-1,SEEK_CUR);
    if(read_text==EOF)
    break;
  }
  else if(strcmp(input_data,"delete")==0){
    delete_director_list(director,dir_l);
      director -> size --;
    read_text=fgetc(dir_l);
    fseek(dir_l,-1,SEEK_CUR);
    if(read_text==EOF)
    break;
  }
  else
  break;
}
FILE *file;
file = fopen("director_list.txt","w");
LINK_W *print_word;
LINK_D *print_director;
print_director = director -> head;
print_word = (LINK_W *)malloc(sizeof(LINK_W));
    while(print_director -> next_director != director -> tail){
      print_director = print_director -> next_director;
      fprintf(file,"%d:%s:%c:%d:",print_director -> serial_num,print_director -> name,print_director -> sex,print_director -> birth);
      print_word = print_director -> title;
      while(print_word -> next != NULL){
        fprintf(file,"%s,",print_word->data);
        print_word = print_word->next;
      }
      fprintf(file,"%s\n", print_word->data);
    }
      fclose(file);
return 0;
}
int print_director_list(ALL_D *director,int serial){
  LINK_D *print_director;
  LINK_W *print_word;
  LINK_M *print_movie;
  int i=1;
  print_director=(LINK_D*)malloc(sizeof(LINK_D));
  print_director=director -> head;
  print_word = (LINK_W *)malloc(sizeof(LINK_W));
  while(print_director -> next_director != director -> tail){
    print_director = print_director -> next_director;
    if(print_director -> serial_num == serial){
    printf("  %d, %s, %c,  %8d\n",print_director -> serial_num,print_director -> name,print_director -> sex,print_director -> birth);
    print_word = print_director -> title;
    while(print_word -> next != NULL){
      print_movie = (LINK_M *)print_word -> com_word;
      if(print_movie  == NULL)
  		printf("  M%d : %s(  -  )\n",i,print_word->data);
      else
      printf("  M%d : %s(%d,%d)\n",i,print_word->data,print_movie -> year,print_movie -> time);
  		print_word = print_word->next;
      i++;
  	}
    print_movie = (LINK_M *)print_word -> com_word;
    if(print_movie  == NULL)
    printf("  M%d : %s(  -  )\n",i,print_word->data);
    else
    printf("  M%d : %s(%d,%d)\n",i,print_word->data,print_movie -> year,print_movie -> time);
    return 1;
  }
}
}
void sorting(ALL_M *movie, ALL_A *actor,ALL_D *director,char tag,char option){
  char **copy,**copy_dir,checking,*check_sex;
  int copy_num,*num;
  int i,check,k,ch;
  LINK_M *save_movie,*print_movie;
  LINK_A *save_actor,*print_actor;
  LINK_D *save_director,*print_director;
  LINK_W *print_word,*saving;
  save_movie = movie -> head;
  save_actor = actor -> head;
  save_director = director -> head;
  copy_dir = (char **)calloc(100,sizeof(char *));
  copy = (char **)calloc(100,sizeof(char *));
  check_sex = (char *)calloc(1000,sizeof(char));
  num = (int *)calloc(1000,sizeof(int));
  if(tag == 'm'){
    if(option == 't'){
      while(save_movie -> next_movie != movie -> tail){
        save_movie = save_movie -> next_movie;
        *(copy+i) = (char *)calloc(1024,sizeof(char));
        *(copy+i) = save_movie -> title;
        i++;
      }
      qsort((void *)copy,movie -> size,sizeof(char *), compare);
      save_movie = movie -> head;
      for(check=0;check<movie -> size;check++){
      while(save_movie -> next_movie != movie -> tail){
        if(strcmp(*(copy+check),save_movie -> next_movie -> title)==0){
          print_movie = save_movie -> next_movie;
          saving = print_movie -> actor;
          printf("%s:%s:%s:%d:%d:",print_movie -> title,print_movie -> genre,print_movie -> director -> data,print_movie -> year,print_movie -> time);
          while(print_movie -> actor -> next != NULL){
            printf("%s:",print_movie -> actor -> data);
            print_movie -> actor = print_movie -> actor -> next;
          }
          printf("%s\n",print_movie -> actor -> data);
          print_movie -> actor = saving;
          save_movie -> next_movie = save_movie -> next_movie -> next_movie;
        save_movie = movie -> head;
        break;
      }
      save_movie = save_movie -> next_movie;
    }
    }
  }

  else if(option == 'g'){
    while(save_movie -> next_movie != movie -> tail){
      save_movie = save_movie -> next_movie;
      *(copy+i) = (char *)calloc(1024,sizeof(char));
      *(copy+i) = save_movie -> genre;
      i++;
    }
    qsort((void *)copy,movie -> size,sizeof(char *), compare);
    save_movie = movie -> head;
    for(check=0;check<movie -> size;check++){
    while(save_movie -> next_movie != movie -> tail){
      if(strcmp(*(copy+check),save_movie -> next_movie -> genre)==0){
        print_movie = save_movie -> next_movie;
        saving = print_movie -> actor;
        printf("%s:%s:%s:%d:%d:",print_movie -> title,print_movie -> genre,print_movie -> director -> data,print_movie -> year,print_movie -> time);
        while(print_movie -> actor -> next != NULL){
          printf("%s:",print_movie -> actor -> data);
          print_movie -> actor = print_movie -> actor -> next;
        }
        printf("%s\n",print_movie -> actor -> data);
        print_movie -> actor = saving;
        save_movie -> next_movie = save_movie -> next_movie -> next_movie;
      save_movie = movie -> head;
      break;
    }
    save_movie = save_movie -> next_movie;
  }
  }
  }
  else if(option == 'd'){
    while(save_movie -> next_movie != movie -> tail){
      save_movie = save_movie -> next_movie;
      *(copy+i) = (char *)calloc(1024,sizeof(char));
      *(copy+i) = save_movie -> director -> data;
      i++;
    }
    qsort((void *)copy,movie -> size,sizeof(char *), compare);
    save_movie = movie -> head;
    for(check=0;check<movie -> size;check++){
    while(save_movie -> next_movie != movie -> tail){
      if(strcmp(*(copy+check),save_movie -> next_movie -> director -> data)==0){
        print_movie = save_movie -> next_movie;
        saving = print_movie -> actor;
        printf("%s:%s:%s:%d:%d:",print_movie -> title,print_movie -> genre,print_movie -> director -> data,print_movie -> year,print_movie -> time);
        while(print_movie -> actor -> next != NULL){
          printf("%s:",print_movie -> actor -> data);
          print_movie -> actor = print_movie -> actor -> next;
        }
        printf("%s\n",print_movie -> actor -> data);
        print_movie -> actor = saving;
        save_movie -> next_movie = save_movie -> next_movie -> next_movie;
      save_movie = movie -> head;
      break;
    }
    save_movie = save_movie -> next_movie;
  }
  }
  }
  else if(option == 'a'){
    while(save_movie -> next_movie != movie -> tail){
      save_movie = save_movie -> next_movie;
      *(copy+i) = (char *)calloc(1024,sizeof(char));
      *(copy+i) = save_movie -> actor -> data;
      i++;
    }
    qsort((void *)copy,movie -> size,sizeof(char *), compare);
    save_movie = movie -> head;
    for(check=0;check<movie -> size;check++){
    while(save_movie -> next_movie != movie -> tail){
      if(strcmp(*(copy+check),save_movie -> next_movie -> actor -> data)==0){
        print_movie = save_movie -> next_movie;
        saving = print_movie -> actor;
        printf("%s:%s:%s:%d:%d:",print_movie -> title,print_movie -> genre,print_movie -> director -> data,print_movie -> year,print_movie -> time);
        while(print_movie -> actor -> next != NULL){
          printf("%s:",print_movie -> actor -> data);
          print_movie -> actor = print_movie -> actor -> next;
        }
        printf("%s\n",print_movie -> actor -> data);
        print_movie -> actor = saving;
        save_movie -> next_movie = save_movie -> next_movie -> next_movie;
      save_movie = movie -> head;
      break;
    }
    save_movie = save_movie -> next_movie;
  }
  }
  }
  else if(option == 'y'){
    while(save_movie -> next_movie != movie -> tail){
      save_movie = save_movie -> next_movie;
      *(num+i) = save_movie -> year;
      i++;
    }
    qsort(num,movie -> size,sizeof(int), compare_num);
    save_movie = movie -> head;
    for(check=0;check<movie -> size;check++){
    while(save_movie -> next_movie != movie -> tail){
      if(*(num+check) == save_movie -> next_movie -> year){
        print_movie = save_movie -> next_movie;
        saving = print_movie -> actor;
        printf("%s:%s:%s:%d:%d:",print_movie -> title,print_movie -> genre,print_movie -> director -> data,print_movie -> year,print_movie -> time);
        while(print_movie -> actor -> next != NULL){
          printf("%s:",print_movie -> actor -> data);
          print_movie -> actor = print_movie -> actor -> next;
        }
        printf("%s\n",print_movie -> actor -> data);
        print_movie -> actor = saving;
        save_movie -> next_movie = save_movie -> next_movie -> next_movie;
      save_movie = movie -> head;
      break;
    }
    save_movie = save_movie -> next_movie;
  }
  }
  }
  else if(option == 'r'){
    while(save_movie -> next_movie != movie -> tail){
      save_movie = save_movie -> next_movie;
      *(num+i) = save_movie -> time;
      i++;
    }
    qsort(num,movie -> size,sizeof(int), compare_num);
    save_movie = movie -> head;
    for(check=0;check<movie -> size;check++){
    while(save_movie -> next_movie != movie -> tail){
      if(*(num+check) == save_movie -> next_movie -> time){
        print_movie = save_movie -> next_movie;
        saving = print_movie -> actor;
        printf("%s:%s:%s:%d:%d:",print_movie -> title,print_movie -> genre,print_movie -> director -> data,print_movie -> year,print_movie -> time);
        while(print_movie -> actor -> next != NULL){
          printf("%s:",print_movie -> actor -> data);
          print_movie -> actor = print_movie -> actor -> next;
        }
        printf("%s\n",print_movie -> actor -> data);
        print_movie -> actor = saving;
        save_movie -> next_movie = save_movie -> next_movie -> next_movie;
      save_movie = movie -> head;
      break;
    }
    save_movie = save_movie -> next_movie;
  }
  }
  }
}//마무리
else if(tag == 'd'){
  if(option == 'n'){
    while(save_director -> next_director != director -> tail){
    	save_director = save_director -> next_director;
    	*(copy+i) = (char *)calloc(1024,sizeof(char));
    	*(copy+i) = save_director -> name;
    	i++;
    }
    qsort((void *)copy,director -> size,sizeof(char *), compare);
    save_director = director -> head;
    for(check=0;check<director -> size;check++){
    while(save_director -> next_director != director -> tail){
    	if(strcmp(*(copy+check),save_director -> next_director -> name)==0){
    		print_director = save_director -> next_director;
    		saving = print_director -> title;
    		printf("%s:%c:%d:",print_director -> name,print_director -> sex,print_director -> birth);
        while(print_director -> title -> next != NULL){
          printf("%s,",print_director -> title -> data);
    			print_director -> title = print_director -> title -> next;
    		}
    		printf("%s\n",print_director -> title -> data);
    		print_director -> title = saving;
    		save_director -> next_director = save_director -> next_director -> next_director;
    	save_director = director -> head;
    	break;
    }
    save_director = save_director -> next_director;
    }
    }
  }
else if(option == 's'){
  while(save_director -> next_director != director -> tail){
    save_director = save_director -> next_director;
    *(check_sex+i) = save_director -> sex;
    i++;
  }
qsort((void *)check_sex,director -> size,sizeof(char ), compare_w);
  save_director = director -> head;
  for(check=0;check<director -> size;check++){
  while(save_director -> next_director != director -> tail){
    if(*(check_sex+check)==save_director -> next_director -> sex){
      print_director = save_director -> next_director;
      saving = print_director -> title;
      printf("%s:%c:%d:",print_director -> name,print_director -> sex,print_director -> birth);
      while(print_director -> title -> next != NULL){
        printf("%s,",print_director -> title -> data);
        print_director -> title = print_director -> title -> next;
      }
      printf("%s\n",print_director -> title -> data);
      print_director -> title = saving;
      save_director -> next_director = save_director -> next_director -> next_director;
    save_director = director -> head;
    break;
  }
  save_director = save_director -> next_director;
  }
  }
}
else if(option == 'b'){
  while(save_director -> next_director != director -> tail){
  	save_director = save_director -> next_director;
  	*(num+i) = save_director -> birth;
  	i++;
  }
  qsort(num,director -> size,sizeof(int), compare_num);
  save_director = director -> head;
  for(check=0;check<director -> size;check++){
  while(save_director -> next_director != director -> tail){
  	if(*(num+check) == save_director -> next_director -> birth){
      print_director = save_director -> next_director;
      saving = print_director -> title;
      printf("%s:%c:%d:",print_director -> name,print_director -> sex,print_director -> birth);
      while(print_director -> title -> next != NULL){
        printf("%s,",print_director -> title -> data);
        print_director -> title = print_director -> title -> next;
      }
      printf("%s\n",print_director -> title -> data);
      print_director -> title = saving;
      save_director -> next_director = save_director -> next_director -> next_director;
    save_director = director -> head;
    break;
  }
  save_director = save_director -> next_director;
  }
}
}
else if(option == 'm'){
  while(save_director -> next_director != director -> tail){
    save_director = save_director -> next_director;
    *(copy+i) = (char *)calloc(1024,sizeof(char));
    *(copy+i) = save_director -> title -> data;
    i++;
  }
  qsort((void *)copy,director -> size,sizeof(char *), compare);
  save_director = director -> head;
  for(check=0;check<director -> size;check++){
  while(save_director -> next_director != director -> tail){
    if(strcmp(*(copy+check),save_director -> next_director -> title -> data)==0){
      print_director = save_director -> next_director;
      saving = print_director -> title;
      printf("%s:%c:%d:",print_director -> name,print_director -> sex,print_director -> birth);
      while(print_director -> title -> next != NULL){
        printf("%s,",print_director -> title -> data);
        print_director -> title = print_director -> title -> next;
      }
      printf("%s\n",print_director -> title -> data);
      print_director -> title = saving;
      save_director -> next_director = save_director -> next_director -> next_director;
    save_director = director -> head;
    break;
  }
  save_director = save_director -> next_director;
  }
  }
}
}
else if(tag == 'a'){
  if(option == 'n'){
    while(save_actor -> next_actor != actor -> tail){
      save_actor = save_actor -> next_actor;
      *(copy+i) = (char *)calloc(1024,sizeof(char));
      *(copy+i) = save_actor -> name;
      i++;
    }
    qsort((void *)copy,actor -> size,sizeof(char *), compare);
    save_actor = actor -> head;
    for(check=0;check<actor -> size;check++){
    while(save_actor -> next_actor != actor -> tail){
      if(strcmp(*(copy+check),save_actor -> next_actor -> name)==0){
        print_actor = save_actor -> next_actor;
        saving = print_actor -> title;
        printf("%s:%c:%d:",print_actor -> name,print_actor -> sex,print_actor -> birth);
        while(print_actor -> title -> next != NULL){
          printf("%s,",print_actor -> title -> data);
          print_actor -> title = print_actor -> title -> next;
        }
        printf("%s\n",print_actor -> title -> data);
        print_actor -> title = saving;
        save_actor -> next_actor = save_actor -> next_actor -> next_actor;
      save_actor = actor -> head;
      break;
    }
    save_actor = save_actor -> next_actor;
    }
    }
  }
else if(option == 's'){
  while(save_actor -> next_actor != actor -> tail){
    save_actor = save_actor -> next_actor;
    *(check_sex+i) = save_actor -> sex;
    i++;
  }
qsort((void *)check_sex,actor -> size,sizeof(char ), compare_w);
  save_actor = actor -> head;
  for(check=0;check<actor -> size;check++){
  while(save_actor -> next_actor != actor -> tail){
    if(*(check_sex+check)==save_actor -> next_actor -> sex){
      print_actor = save_actor -> next_actor;
      saving = print_actor -> title;
      printf("%s:%c:%d:",print_actor -> name,print_actor -> sex,print_actor -> birth);
      while(print_actor -> title -> next != NULL){
        printf("%s,",print_actor -> title -> data);
        print_actor -> title = print_actor -> title -> next;
      }
      printf("%s\n",print_actor -> title -> data);
      print_actor -> title = saving;
      save_actor -> next_actor = save_actor -> next_actor -> next_actor;
    save_actor = actor -> head;
    break;
  }
  save_actor = save_actor -> next_actor;
  }
  }
}
else if(option == 'b'){
  while(save_actor -> next_actor != actor -> tail){
  	save_actor = save_actor -> next_actor;
  	*(num+i) = save_actor -> birth;
  	i++;
  }
  qsort(num,actor -> size,sizeof(int), compare_num);
  save_actor = actor -> head;
  for(check=0;check<actor -> size;check++){
  while(save_actor -> next_actor != actor -> tail){
  	if(*(num+check) == save_actor -> next_actor -> birth){
      print_actor = save_actor -> next_actor;
      saving = print_actor -> title;
      printf("%s:%c:%d:",print_actor -> name,print_actor -> sex,print_actor -> birth);
      while(print_actor -> title -> next != NULL){
        printf("%s,",print_actor -> title -> data);
        print_actor -> title = print_actor -> title -> next;
      }
      printf("%s\n",print_actor -> title -> data);
      print_actor -> title = saving;
      save_actor -> next_actor = save_actor -> next_actor -> next_actor;
    save_actor = actor -> head;
    break;
  }
  save_actor = save_actor -> next_actor;
  }
}
}
else if(option == 'm'){
  while(save_actor -> next_actor != actor -> tail){
    save_actor = save_actor -> next_actor;
    *(copy+i) = (char *)calloc(1024,sizeof(char));
    *(copy+i) = save_actor -> title -> data;
    i++;
  }
  qsort((void *)copy,actor -> size,sizeof(char *), compare);
  save_actor = actor -> head;
  for(check=0;check<actor -> size;check++){
  while(save_actor -> next_actor != actor -> tail){
    if(strcmp(*(copy+check),save_actor -> next_actor -> title -> data)==0){
      print_actor = save_actor -> next_actor;
      saving = print_actor -> title;
      printf("%s:%c:%d:",print_actor -> name,print_actor -> sex,print_actor -> birth);
      while(print_actor -> title -> next != NULL){
        printf("%s,",print_actor -> title -> data);
        print_actor -> title = print_actor -> title -> next;
      }
      printf("%s\n",print_actor -> title -> data);
      print_actor -> title = saving;
      save_actor -> next_actor = save_actor -> next_actor -> next_actor;
    save_actor = actor -> head;
    break;
  }
  save_actor = save_actor -> next_actor;
  }
  }
}
}
}
void sorting_file(ALL_M *movie, ALL_A *actor,ALL_D *director,char tag,char option){
	char *filename;
	FILE *file;
  filename = (char *)calloc(1000,sizeof(char));
	scanf("%[^\n]%*c",filename);
	file = fopen(filename,"w");
	char **copy,**copy_word,checking,*check_sex;
  int copy_num,*num;
  int i,check,k,ch;
  LINK_M *save_movie,*print_movie;
  LINK_A *save_actor,*print_actor;
  LINK_D *save_director,*print_director;
  LINK_W *print_word,*saving;
  save_movie = movie -> head;
  save_actor = actor -> head;
  save_director = director -> head;
  copy = (char **)calloc(1,sizeof(char *));
  copy_word = (char **)calloc(1,sizeof(char *));
  num = (int *)calloc(1000,sizeof(int));
  check_sex = (char *)calloc(1000,sizeof(char));
  if(tag == 'm'){
    if(option == 't'){
      while(save_movie -> next_movie != movie -> tail){
      	save_movie = save_movie -> next_movie;
      	*(copy+i) = (char *)calloc(1024,sizeof(char));
      	*(copy+i) = save_movie -> title;
      	i++;
      }
      qsort((void *)copy,movie -> size,sizeof(char *), compare);
      save_movie = movie -> head;
      for(check=0;check<movie -> size;check++){
      while(save_movie -> next_movie != movie -> tail){
      	if(strcmp(*(copy+check),save_movie -> next_movie -> title)==0){
      		print_movie = save_movie -> next_movie;
      		saving = print_movie -> actor;
      		fprintf(file,"%s:%s:%s:%d:%d:",print_movie -> title,print_movie -> genre,print_movie -> director -> data,print_movie -> year,print_movie -> time);
      		while(print_movie -> actor -> next != NULL){
      			fprintf(file,"%s:",print_movie -> actor -> data);
      			print_movie -> actor = print_movie -> actor -> next;
      		}
      		fprintf(file,"%s\n",print_movie -> actor -> data);
      		print_movie -> actor = saving;
      		save_movie -> next_movie = save_movie -> next_movie -> next_movie;
      	save_movie = movie -> head;
      	break;
      }
      save_movie = save_movie -> next_movie;
      }
      }
    fclose(file);
  }

  else if(option == 'g'){
    while(save_movie -> next_movie != movie -> tail){
      save_movie = save_movie -> next_movie;
      *(copy+i) = (char *)calloc(1024,sizeof(char));
      *(copy+i) = save_movie -> genre;
      i++;
    }
    qsort((void *)copy,movie -> size,sizeof(char *), compare);
    save_movie = movie -> head;
    for(check=0;check<movie -> size;check++){
    while(save_movie -> next_movie != movie -> tail){
      if(strcmp(*(copy+check),save_movie -> next_movie -> genre)==0){
        print_movie = save_movie -> next_movie;
        saving = print_movie -> actor;
        fprintf(file,"%s:%s:%s:%d:%d:",print_movie -> title,print_movie -> genre,print_movie -> director -> data,print_movie -> year,print_movie -> time);
        while(print_movie -> actor -> next != NULL){
          fprintf(file,"%s:",print_movie -> actor -> data);
          print_movie -> actor = print_movie -> actor -> next;
        }
        fprintf(file,"%s\n",print_movie -> actor -> data);
        print_movie -> actor = saving;
        save_movie -> next_movie = save_movie -> next_movie -> next_movie;
      save_movie = movie -> head;
      break;
    }
    save_movie = save_movie -> next_movie;
    }
    }
  fclose(file);
  }
  else if(option == 'd'){
    while(save_movie -> next_movie != movie -> tail){
      save_movie = save_movie -> next_movie;
      *(copy+i) = (char *)calloc(1024,sizeof(char));
      *(copy+i) = save_movie -> director -> data;
      i++;
    }
    qsort((void *)copy,movie -> size,sizeof(char *), compare);
    save_movie = movie -> head;
    for(check=0;check<movie -> size;check++){
    while(save_movie -> next_movie != movie -> tail){
      if(strcmp(*(copy+check),save_movie -> next_movie -> director -> data)==0){
        print_movie = save_movie -> next_movie;
        saving = print_movie -> actor;
        fprintf(file,"%s:%s:%s:%d:%d:",print_movie -> title,print_movie -> genre,print_movie -> director -> data,print_movie -> year,print_movie -> time);
        while(print_movie -> actor -> next != NULL){
          fprintf(file,"%s:",print_movie -> actor -> data);
          print_movie -> actor = print_movie -> actor -> next;
        }
        fprintf(file,"%s\n",print_movie -> actor -> data);
        print_movie -> actor = saving;
        save_movie -> next_movie = save_movie -> next_movie -> next_movie;
      save_movie = movie -> head;
      break;
    }
    save_movie = save_movie -> next_movie;
    }
    }
  fclose(file);
  }
  else if(option == 'a'){
    while(save_movie -> next_movie != movie -> tail){
      save_movie = save_movie -> next_movie;
      *(copy+i) = (char *)calloc(1024,sizeof(char));
      *(copy+i) = save_movie -> actor -> data;
      i++;
    }
    qsort((void *)copy,movie -> size,sizeof(char *), compare);
    save_movie = movie -> head;
    for(check=0;check<movie -> size;check++){
    while(save_movie -> next_movie != movie -> tail){
      if(strcmp(*(copy+check),save_movie -> next_movie -> actor -> data)==0){
        print_movie = save_movie -> next_movie;
        saving = print_movie -> actor;
        fprintf(file,"%s:%s:%s:%d:%d:",print_movie -> title,print_movie -> genre,print_movie -> director -> data,print_movie -> year,print_movie -> time);
        while(print_movie -> actor -> next != NULL){
          fprintf(file,"%s:",print_movie -> actor -> data);
          print_movie -> actor = print_movie -> actor -> next;
        }
        fprintf(file,"%s\n",print_movie -> actor -> data);
        print_movie -> actor = saving;
        save_movie -> next_movie = save_movie -> next_movie -> next_movie;
      save_movie = movie -> head;
      break;
    }
    save_movie = save_movie -> next_movie;
    }
    }
  fclose(file);
  }
  else if(option == 'y'){
    while(save_movie -> next_movie != movie -> tail){
    	save_movie = save_movie -> next_movie;
    	*(num+i) = save_movie -> year;
    	i++;
    }
    qsort(num,movie -> size,sizeof(int), compare_num);
    save_movie = movie -> head;
    for(check=0;check<movie -> size;check++){
    while(save_movie -> next_movie != movie -> tail){
    	if(*(num+check) == save_movie -> next_movie -> year){
    		print_movie = save_movie -> next_movie;
    		saving = print_movie -> actor;
    		fprintf(file,"%s:%s:%s:%d:%d:",print_movie -> title,print_movie -> genre,print_movie -> director -> data,print_movie -> year,print_movie -> time);
    		while(print_movie -> actor -> next != NULL){
    			fprintf(file,"%s:",print_movie -> actor -> data);
    			print_movie -> actor = print_movie -> actor -> next;
    		}
    		fprintf(file,"%s\n",print_movie -> actor -> data);
    		print_movie -> actor = saving;
    		save_movie -> next_movie = save_movie -> next_movie -> next_movie;
    	save_movie = movie -> head;
    	break;
    }
    save_movie = save_movie -> next_movie;
    }
    }
    fclose(file);
  }
  else if(option == 'r'){
    while(save_movie -> next_movie != movie -> tail){
    	save_movie = save_movie -> next_movie;
    	*(num+i) = save_movie -> time;
    	i++;
    }
    qsort(num,movie -> size,sizeof(int), compare_num);
    save_movie = movie -> head;
    for(check=0;check<movie -> size;check++){
    while(save_movie -> next_movie != movie -> tail){
    	if(*(num+check) == save_movie -> next_movie -> time){
    		print_movie = save_movie -> next_movie;
    		saving = print_movie -> actor;
    		fprintf(file,"%s:%s:%s:%d:%d:",print_movie -> title,print_movie -> genre,print_movie -> director -> data,print_movie -> year,print_movie -> time);
    		while(print_movie -> actor -> next != NULL){
    			fprintf(file,"%s:",print_movie -> actor -> data);
    			print_movie -> actor = print_movie -> actor -> next;
    		}
    		fprintf(file,"%s\n",print_movie -> actor -> data);
    		print_movie -> actor = saving;
    		save_movie -> next_movie = save_movie -> next_movie -> next_movie;
    	save_movie = movie -> head;
    	break;
    }
    save_movie = save_movie -> next_movie;
    }
    }
    fclose(file);
  }
}
else if(tag == 'd'){
  if(option == 'n'){
    while(save_director -> next_director != director -> tail){
    	save_director = save_director -> next_director;
    	*(copy+i) = (char *)calloc(1024,sizeof(char));
    	*(copy+i) = save_director -> name;
    	i++;
    }
    qsort((void *)copy,director -> size,sizeof(char *), compare);
    save_director = director -> head;
    for(check=0;check<director -> size;check++){
    while(save_director -> next_director != director -> tail){
    	if(strcmp(*(copy+check),save_director -> next_director -> name)==0){
    		print_director = save_director -> next_director;
    		saving = print_director -> title;
    		fprintf(file,"%s:%c:%d:",print_director -> name,print_director -> sex,print_director -> birth);
        while(print_director -> title -> next != NULL){
          fprintf(file,"%s,",print_director -> title -> data);
    			print_director -> title = print_director -> title -> next;
    		}
    		fprintf(file,"%s\n",print_director -> title -> data);
    		print_director -> title = saving;
    		save_director -> next_director = save_director -> next_director -> next_director;
    	save_director = director -> head;
    	break;
    }
    save_director = save_director -> next_director;
    }
    }
    fclose(file);
  }
else if(option == 's'){
  while(save_director -> next_director != director -> tail){
    save_director = save_director -> next_director;
    *(check_sex+i) = save_director -> sex;
    i++;
  }
qsort((void *)check_sex,director -> size,sizeof(char ), compare_w);
  save_director = director -> head;
  for(check=0;check<director -> size;check++){
  while(save_director -> next_director != director -> tail){
    if(*(check_sex+check)==save_director -> next_director -> sex){
      print_director = save_director -> next_director;
      saving = print_director -> title;
      fprintf(file,"%s:%c:%d:",print_director -> name,print_director -> sex,print_director -> birth);
      while(print_director -> title -> next != NULL){
        fprintf(file,"%s,",print_director -> title -> data);
        print_director -> title = print_director -> title -> next;
      }
      fprintf(file,"%s\n",print_director -> title -> data);
      print_director -> title = saving;
      save_director -> next_director = save_director -> next_director -> next_director;
    save_director = director -> head;
    break;
  }
  save_director = save_director -> next_director;
  }
  }
  fclose(file);
}
else if(option == 'b'){
  while(save_director -> next_director != director -> tail){
  	save_director = save_director -> next_director;
  	*(num+i) = save_director -> birth;
  	i++;
  }
  qsort(num,director -> size,sizeof(int), compare_num);
  save_director = director -> head;
  for(check=0;check<director -> size;check++){
  while(save_director -> next_director != director -> tail){
  	if(*(num+check) == save_director -> next_director -> birth){
      print_director = save_director -> next_director;
      saving = print_director -> title;
      fprintf(file,"%s:%c:%d:",print_director -> name,print_director -> sex,print_director -> birth);
      while(print_director -> title -> next != NULL){
        fprintf(file,"%s,",print_director -> title -> data);
        print_director -> title = print_director -> title -> next;
      }
      fprintf(file,"%s\n",print_director -> title -> data);
      print_director -> title = saving;
      save_director -> next_director = save_director -> next_director -> next_director;
    save_director = director -> head;
    break;
  }
  save_director = save_director -> next_director;
  }
}
fclose(file);
}
else if(option == 'm'){
  while(save_director -> next_director != director -> tail){
    save_director = save_director -> next_director;
    *(copy+i) = (char *)calloc(1024,sizeof(char));
    *(copy+i) = save_director -> title -> data;
    i++;
  }
  qsort((void *)copy,director -> size,sizeof(char *), compare);
  save_director = director -> head;
  for(check=0;check<director -> size;check++){
  while(save_director -> next_director != director -> tail){
    if(strcmp(*(copy+check),save_director -> next_director -> title -> data)==0){
      print_director = save_director -> next_director;
      saving = print_director -> title;
      fprintf(file,"%s:%c:%d:",print_director -> name,print_director -> sex,print_director -> birth);
      while(print_director -> title -> next != NULL){
        fprintf(file,"%s,",print_director -> title -> data);
        print_director -> title = print_director -> title -> next;
      }
      fprintf(file,"%s\n",print_director -> title -> data);
      print_director -> title = saving;
      save_director -> next_director = save_director -> next_director -> next_director;
    save_director = director -> head;
    break;
  }
  save_director = save_director -> next_director;
  }
  }
  fclose(file);
}
}
else if(tag == 'a'){
  if(option == 'n'){
		while(save_actor -> next_actor != actor -> tail){
      save_actor = save_actor -> next_actor;
      *(copy+i) = (char *)calloc(1024,sizeof(char));
      *(copy+i) = save_actor -> name;
      i++;
    }
    qsort((void *)copy,actor -> size,sizeof(char *), compare);
    save_actor = actor -> head;
    for(check=0;check<actor -> size;check++){
    while(save_actor -> next_actor != actor -> tail){
      if(strcmp(*(copy+check),save_actor -> next_actor -> name)==0){
        print_actor = save_actor -> next_actor;
        saving = print_actor -> title;
        fprintf(file,"%s:%c:%d:",print_actor -> name,print_actor -> sex,print_actor -> birth);
        while(print_actor -> title -> next != NULL){
          fprintf(file,"%s,",print_actor -> title -> data);
          print_actor -> title = print_actor -> title -> next;
        }
        fprintf(file,"%s\n",print_actor -> title -> data);
        print_actor -> title = saving;
        save_actor -> next_actor = save_actor -> next_actor -> next_actor;
      save_actor = actor -> head;
      break;
    }
    save_actor = save_actor -> next_actor;
    }
    }
		fclose(file);
  }
else if(option == 's'){
  while(save_actor -> next_actor != actor -> tail){
    save_actor = save_actor -> next_actor;
    *(check_sex+i) = save_actor -> sex;
    i++;
  }
qsort((void *)check_sex,actor -> size,sizeof(char ), compare_w);
  save_actor = actor -> head;
  for(check=0;check<actor -> size;check++){
  while(save_actor -> next_actor != actor -> tail){
    if(*(check_sex+check)==save_actor -> next_actor -> sex){
      print_actor = save_actor -> next_actor;
      saving = print_actor -> title;
      fprintf(file,"%s:%c:%d:",print_actor -> name,print_actor -> sex,print_actor -> birth);
      while(print_actor -> title -> next != NULL){
        fprintf(file,"%s,",print_actor -> title -> data);
        print_actor -> title = print_actor -> title -> next;
      }
      fprintf(file,"%s\n",print_actor -> title -> data);
      print_actor -> title = saving;
      save_actor -> next_actor = save_actor -> next_actor -> next_actor;
    save_actor = actor -> head;
    break;
  }
  save_actor = save_actor -> next_actor;
  }
  }
	fclose(file);
}
else if(option == 'b'){
  while(save_actor -> next_actor != actor -> tail){
  	save_actor = save_actor -> next_actor;
  	*(num+i) = save_actor -> birth;
  	i++;
  }
  qsort(num,actor -> size,sizeof(int), compare_num);
  save_actor = actor -> head;
  for(check=0;check<actor -> size;check++){
  while(save_actor -> next_actor != actor -> tail){
  	if(*(num+check) == save_actor -> next_actor -> birth){
      print_actor = save_actor -> next_actor;
      saving = print_actor -> title;
      fprintf(file,"%s:%c:%d:",print_actor -> name,print_actor -> sex,print_actor -> birth);
      while(print_actor -> title -> next != NULL){
        fprintf(file,"%s,",print_actor -> title -> data);
        print_actor -> title = print_actor -> title -> next;
      }
      fprintf(file,"%s\n",print_actor -> title -> data);
      print_actor -> title = saving;
      save_actor -> next_actor = save_actor -> next_actor -> next_actor;
    save_actor = actor -> head;
    break;
  }
  save_actor = save_actor -> next_actor;
  }
}
fclose(file);
}
else if(option == 'm'){
  while(save_actor -> next_actor != actor -> tail){
    save_actor = save_actor -> next_actor;
    *(copy+i) = (char *)calloc(1024,sizeof(char));
    *(copy+i) = save_actor -> title -> data;
    i++;
  }
  qsort((void *)copy,actor -> size,sizeof(char *), compare);
  save_actor = actor -> head;
  for(check=0;check<actor -> size;check++){
  while(save_actor -> next_actor != actor -> tail){
    if(strcmp(*(copy+check),save_actor -> next_actor -> title -> data)==0){
      print_actor = save_actor -> next_actor;
      saving = print_actor -> title;
      fprintf(file,"%s:%c:%d:",print_actor -> name,print_actor -> sex,print_actor -> birth);
      while(print_actor -> title -> next != NULL){
        fprintf(file,"%s,",print_actor -> title -> data);
        print_actor -> title = print_actor -> title -> next;
      }
      fprintf(file,"%s\n",print_actor -> title -> data);
      print_actor -> title = saving;
      save_actor -> next_actor = save_actor -> next_actor -> next_actor;
    save_actor = actor -> head;
    break;
  }
  save_actor = save_actor -> next_actor;
  }
  }
	fclose(file);
}
}
}
void search_print_m(LINK_M *search_movie){  //지정된 문자열을 찾았을 때 해당 레코드 전체를 출력하는 함수
  LINK_W *actor;  //actor가 포인트하는 곳을 초기화 시키기 위한 변수
  actor = search_movie -> actor;  //actor의 위치를 첫 번째 acotr의 위치로 지정해 준다.
  printf("%d:%s:%s:%s:%d:%d:",search_movie -> serial_num, search_movie -> title, search_movie -> genre, search_movie -> director -> data, search_movie -> year, search_movie -> time);  //해당 레코드 정보 출력
  while(search_movie -> actor ->  next != NULL){  //해당 레코드의 actor 정보 출력
    printf("%s,", search_movie -> actor -> data);
    search_movie -> actor = search_movie -> actor -> next;
    }
  printf("%s\n", search_movie -> actor -> data);  //해당 레코드의 마지막 actor부분 출력
  search_movie -> actor = actor;  //마지막으로 간 actor의 노드를 초기화 시켜줌
}
void search_m(ALL_M *movie, char *string){  //movie링크드리스트에서 지정된 문자열 찾는 함수
  LINK_W *actor;  //actor가 포인트하는 곳을 초기화 시키기 위한 변수
  char *string_y; //int형인 year을 char형으로 바꿔서 저장할 곳
  char *string_t; //int형인 time을 char형으로 바꿔서 저장할 곳
  LINK_M *search_movie = movie -> head; //search_movie가 노드하는 곳을 지정함
  string_y = (char *)calloc(sizeof(char), 4); //string_y 동적할당
  string_t = (char *)calloc(sizeof(char), 4); //string_t 동적할당
  int word_l = 0; //string의 문자열 길이를 넣는 변수
  int word_noq = 0; //string에서 ?를 제외한 문자열 길이를 넣는 변수
  int word_samechar = 0;  //string과 비교할 문자열에서 같은 문자의 수를 넣는 변수
  int word_star = 0;  //string 문자열에서 *의 변수
  int word_qnum = 0;  //string 문자열에서 ?의 변수
  int data_len = 0; //비교할 문자열의 길이를 저장하는 변수
  int gap_star = 0; //비교할 때 원하는 배열 포인터로 가기위한 연산을 하기 위해 필요한 변수
  word_l = strlen(string);  //word_l에 string길이 저장
  for(int i = 0; i < word_l; i++){  //word_star에 string의 *수 저장
    if(*(string + i) == '*')
      word_star++;
  }

  for(int i = 0; i < word_l; i++){  //word_noq에 string의 ?가 아닌 문자의 수 저장
    if(*(string + i) != '?')
      word_noq++;
  }

  for(int i = 0; i < word_l; i++){  //word_qnum에 string의 ?의 수 저장
    if(*(string + i) == '?')
      word_qnum++;
  }

  if(word_star != 0){ //stirng에 *이 있는 경우
    if(*(string + word_l - 1) == '*'){  //string에 *이 마지막에 나오는 경우
      while(search_movie -> next_movie != movie -> tail){ //search_movie -> next_movie가 노드하는 곳을 마지막까지 검사
        search_movie = search_movie -> next_movie;  //search_movie가 다음을 노드하도록 함
        change_num_to_string(string_y, search_movie -> year); //year를 문자열로 바꿈
        change_num_to_string(string_t, search_movie -> time); //time을 문자열로 바꿈

        if(word_qnum != 0){ //?가 있는 경우, title과 비교
          word_samechar = 0;  //word_samechar를 0으로 초기화 해줌
          for(int i = 0; i <(word_l - 1); i++){ //*의 수를 뺀 string 길이 만큼 비교할 문자열과 비교
            if(*(string + i) == *(search_movie -> title +i))  //stirng과 비교한 문자가 같으면 word_samechar 증가
              word_samechar++;
          }
          if(word_samechar == word_noq - word_star){  //같은 문자 수와 문자열에서 ?와 *이 없는 문자의 수와 같으면 해당 레이블 정보 출력
            search_print_m(search_movie);
            continue;
          }
        }
        else{ //?가 없는 경우, title과 비교
          word_samechar = 0;  //word_samehar를 0으로 초기화 해줌
          for(int i = 0; i <(word_l - 1); i++){ //*의 수를 뺀 string 길이 만큼 비교할 문자열과 비교
            if(*(string + i) == *(search_movie -> title + i)) //stirng과 비교한 문자가 같으면 word_samechar 증가
              word_samechar++;
          }
          if(word_samechar == (word_l - word_star)){   //같은 문자 수와 문자열에서 *이 없는 문자의 수와 같으면 해당 레이블 정보 출력
            search_print_m(search_movie);
            continue;
          }
        }
        if(word_qnum != 0){ //?가 있는 경우, genre와 비교
          word_samechar = 0;  //word_samechar를 0으로 초기화 해줌
          for(int i = 0; i <(word_l - 1); i++){ //*의 수를 뺀 string 길이 만큼 비교할 문자열과 비교
            if(*(string + i) == *(search_movie -> genre +i))  //stirng과 비교한 문자가 같으면 word_samechar 증가
              word_samechar++;
          }
          if(word_samechar == word_noq - word_star){  //같은 문자 수와 문자열에서 ?와 *이 없는 문자의 수와 같으면 해당 레이블 정보 출력
            search_print_m(search_movie);
            continue;
          }
        }
        else{ //?가 없는 경우, genre와 비교
          word_samechar = 0;  //word_samehar를 0으로 초기화 해줌
          for(int i = 0; i <(word_l - 1); i++){ //*의 수를 뺀 string 길이 만큼 비교할 문자열과 비교
            if(*(string + i) == *(search_movie -> genre + i)) //stirng과 비교한 문자가 같으면 word_samechar 증가
              word_samechar++;
          }
          if(word_samechar == (word_l - word_star)){  //같은 문자 수와 문자열에서 *이 없는 문자의 수와 같으면 해당 레이블 정보 출력
            search_print_m(search_movie);
            continue;
          }
        }
        if(word_qnum != 0){ //?가 있는 경우, director와 비교
          word_samechar = 0;  //word_samechar를 0으로 초기화 해줌
          for(int i = 0; i <(word_l - 1); i++){ //*의 수를 뺀 string 길이 만큼 비교할 문자열과 비교
            if(*(string + i) == *(search_movie -> director -> data +i)) //stirng과 비교한 문자가 같으면 word_samechar 증가
              word_samechar++;
          }
          if(word_samechar == word_noq - word_star){  //같은 문자 수와 문자열에서 ?와 *이 없는 문자의 수와 같으면 해당 레이블 정보 출력
            search_print_m(search_movie);
            continue;
          }
        }
        else{  //?가 없는 경우, director와 비교
          word_samechar = 0;  //word_samechar를 0으로 초기화 해줌
          for(int i = 0; i <(word_l - 1); i++){ //*의 수를 뺀 string 길이 만큼 비교할 문자열과 비교
            if(*(string + i) == *(search_movie -> director -> data + i))  //stirng과 비교한 문자가 같으면 word_samechar 증가
              word_samechar++;
          }
          if(word_samechar == (word_l - word_star)){  //같은 문자 수와 문자열에서 *이 없는 문자의 수와 같으면 해당 레이블 정보 출력
            search_print_m(search_movie);
            continue;
          }
        }
        if(word_qnum != 0){ //? 있는 경우, year와 비교
          word_samechar = 0; //word_samechar를 0으로 초기화 해줌
          for(int i = 0; i <(word_l - 1); i++){ //*의 수를 뺀 string 길이 만큼 비교할 문자열과 비교
            if(*(string + i) == *(string_y +i)) //stirng과 비교한 문자가 같으면 word_samechar 증가
              word_samechar++;
          }
          if(word_samechar == word_noq - word_star){  //같은 문자 수와 문자열에서 ?와 *이 없는 문자의 수와 같으면 해당 레이블 정보 출력
            search_print_m(search_movie);
            continue;
          }
        }
        else{ //? 없는 경우, year와 비교
          word_samechar = 0;  //word_samechar를 0으로 초기화 해줌
          for(int i = 0; i <(word_l - 1); i++){ //*의 수를 뺀 string 길이 만큼 비교할 문자열과 비교
            if(*(string + i) == *(string_y + i))  //stirng과 비교한 문자가 같으면 word_samechar 증가
              word_samechar++;
          }
          if(word_samechar == (word_l - word_star)){  //같은 문자 수와 문자열에서 *이 없는 문자의 수와 같으면 해당 레이블 정보 출력
            search_print_m(search_movie);
            continue;
          }
        }
        if(word_qnum != 0){ //? 있는 경우, time과 비교
          word_samechar = 0;  //word_samechar를 0으로 초기화 해줌
          for(int i = 0; i <(word_l - 1); i++){ //*의 수를 뺀 string 길이 만큼 비교할 문자열과 비교
            if(*(string + i) == *(string_t +i)) //stirng과 비교한 문자가 같으면 word_samechar 증가
              word_samechar++;
          }
          if(word_samechar == word_noq - word_star){  //같은 문자 수와 문자열에서 ?와 *이 없는 문자의 수와 같으면 해당 레이블 정보 출력
            search_print_m(search_movie);
            continue;
          }
        }
        else{ //? 없는 경우, time과 비교
          word_samechar = 0;  //word_samechar를 0으로 초기화 해줌
          for(int i = 0; i <(word_l - 1); i++){ //*의 수를 뺀 string 길이 만큼 비교할 문자열과 비교
            if(*(string + i) == *(string_t + i)) //stirng과 비교한 문자가 같으면 word_samechar 증가
              word_samechar++;
          }
          if(word_samechar == (word_l - word_star)){  //같은 문자 수와 문자열에서 ?와 *이 없는 문자의 수와 같으면 해당 레이블 정보 출력
            search_print_m(search_movie);
            continue;
          }
        }
        actor = search_movie -> actor;  //search_movie -> actor가 노드 하는 곳을 초기화 시켜주기 위해 actor에 노드 할 곳 저장
        if(word_qnum != 0){ //? 있는 경우, actor와 비교
          word_samechar = 0;  //word_samechar를 0으로 초기화 해줌
          for(int i = 0; i < (word_l - 1); i++){  //*의 수를 뺀 string 길이 만큼 비교할 문자열과 비교
            if(*(string + i) == *(search_movie -> actor -> data + i)){  //stirng과 비교한 문자가 같으면 word_samechar 증가
              word_samechar++;
            }
          }
          if(word_samechar == word_noq - word_star){  //같은 문자 수와 문자열에서 ?와 *이 없는 문자의 수와 같으면 해당 레이블 정보 출력
            search_print_m(search_movie);
            continue;
          }
        }
        else{ //? 없는 경우, actor와 비교
          word_samechar = 0;  //word_samechar를 0으로 초기화 해줌
          for(int i = 0; i < (word_l - 1); i++){  //*의 수를 뺀 string 길이 만큼 비교할 문자열과 비교
            if(*(string + i) == *(search_movie -> actor -> data + i)){  //stirng과 비교한 문자가 같으면 word_samechar 증가
              word_samechar++;
            }
          }
          if(word_samechar == word_l - word_star){  //같은 문자 수와 문자열에서 *이 없는 문자의 수와 같으면 해당 레이블 정보 출력
            search_print_m(search_movie);
            continue;
          }
        }

        while(search_movie -> actor -> next != NULL){ //search_movie -> actor가 노드하는 곳이 NULL까지 while문 실행
          word_samechar = 0;  //word_samechar를 0으로 초기화 해줌
          search_movie -> actor = search_movie -> actor -> next;  //search_movie -> actor가 노드 하는 곳을 다음 actor로 바꺼줌
          if(word_qnum != 0){ //? 있는 경우, actor와 비교
            word_samechar = 0;  //word_samechar를 0으로 초기화 해줌
            for(int i = 0; i < (word_l - 1); i++){  //*의 수를 뺀 string 길이 만큼 비교할 문자열과 비교
              if(*(string + i) == *(search_movie -> actor -> data + i)){  //stirng과 비교한 문자가 같으면 word_samechar 증가
                word_samechar++;
              }
            }
            if(word_samechar == word_noq - word_star){  //같은 문자 수와 문자열에서 ?와 *이 없는 문자의 수와 같으면 해당 레이블 정보 출력
              search_movie -> actor = actor;  //출력하기전 search_movie -> actor의 위치를 초기화
              search_print_m(search_movie);
              break;
            }
          }
          else{
            word_samechar = 0;  //word_samechar를 0으로 초기화 해줌
            for(int i = 0; i < (word_l - 1); i++){  //*의 수를 뺀 string 길이 만큼 비교할 문자열과 비교
              if(*(string + i) == *(search_movie -> actor -> data + i)){  //stirng과 비교한 문자가 같으면 word_samechar 증가
                word_samechar++;
              }
            }
            if(word_samechar == word_l - word_star){  //같은 문자 수와 문자열에서 *이 없는 문자의 수와 같으면 해당 레이블 정보 출력
              search_movie -> actor = actor;  //출력하기전 search_movie -> actor의 위치를 초기화
              search_print_m(search_movie);
              break;
            }
          }
        }
        search_movie -> actor = actor;  //출력이 없었을 때 search_movie -> actor의 위치를 초기화
      }// while
    } //if(*(string + word_l - 1) == '*')


    else if(*string == '*'){  //string에 처음에 *이 나오는 경우
      while(search_movie -> next_movie != movie -> tail){ //search_movie -> next_movie가 노드하는 곳을 마지막까지 검사
        search_movie = search_movie -> next_movie;  //search_movie가 노드 하는 곳을 다음 search_movie -> next_movie로 옮김
        change_num_to_string(string_y, search_movie -> year); //year를 문자열로 바꿈
        change_num_to_string(string_t, search_movie -> time); //time를 문자열로 바꿈

        if(word_qnum != 0){ //? 있는 경우, title과 비교
          data_len = strlen(search_movie -> title); //search_movie -> title의 문자열 길이를 구함
          gap_star = data_len - (word_l - word_star); //비교할 위치를 맞추기 위해 gap_star를 구함
          word_samechar = 0;  //word_samechar를 0으로 초기화 해줌
          for(int i = 0; i<(word_l - 1); i++){  //*의 수를 뺀 string 길이 만큼 비교할 문자열과 비교
            if(*(string + i + 1) == *(search_movie -> title + gap_star + i))  //string과 비교할 문자열을 뒤에서 부터 비교하여 같으면 word_samechar증가
              word_samechar++;
          }
          if(word_samechar == word_noq - word_star){  //같은 문자 수와 문자열에서 ?와 *이 없는 문자의 수와 같으면 해당 레이블 정보 출력
            search_print_m(search_movie);
            continue;
          }
        }
        else{ //? 없는 경우, title과 비교
          data_len = strlen(search_movie -> title); //search_movie -> title의 문자열 길이를 구함
          gap_star = data_len - (word_l - word_star);  //비교할 위치를 맞추기 위해 gap_star를 구함
          word_samechar = 0;  //word_samechar를 0으로 초기화 해줌
          for(int i = 0; i<(word_l - 1); i++){  //*의 수를 뺀 string 길이 만큼 비교할 문자열과 비교
            if(*(string + i + 1) == *(search_movie -> title + gap_star + i))  //string과 비교할 문자열을 뒤에서 부터 비교하여 같으면 word_samechar증가
              word_samechar++;
          }
          if(word_samechar == word_l - word_star){  //같은 문자 수와 문자열에서 *이 없는 문자의 수와 같으면 해당 레이블 정보 출력
            search_print_m(search_movie);
            continue;
          }
        }
        if(word_qnum != 0){ //? 있는 경우, genre와 비교
          data_len = strlen(search_movie -> genre); //search_movie -> genre의 문자열 길이를 구함
          gap_star = data_len - (word_l - word_star); //비교할 위치를 맞추기 위해 gap_star를 구함
          word_samechar = 0;  //비교할 위치를 맞추기 위해 gap_star를 구함
          for(int i = 0; i<(word_l - 1); i++){  //*의 수를 뺀 string 길이 만큼 비교할 문자열과 비교
            if(*(string + i + 1) == *(search_movie -> genre + gap_star + i))  //string과 비교할 문자열을 뒤에서 부터 비교하여 같으면 word_samechar증가
              word_samechar++;
          }
          if(word_samechar == word_noq - word_star){  //같은 문자 수와 문자열에서 ?와 *이 없는 문자의 수와 같으면 해당 레이블 정보 출력
            search_print_m(search_movie);
            continue;
          }
        }
        else{ //? 없는 경우, title과 비교
          data_len = strlen(search_movie -> genre); //search_movie -> genre의 문자열 길이를 구함
          gap_star = data_len - (word_l - word_star); //비교할 위치를 맞추기 위해 gap_star를 구함
          word_samechar = 0;   //word_samechar를 0으로 초기화 해줌
          for(int i = 0; i<(word_l - 1); i++){  //*의 수를 뺀 string 길이 만큼 비교할 문자열과 비교
            if(*(string + i + 1) == *(search_movie -> genre + gap_star + i))  //string과 비교할 문자열을 뒤에서 부터 비교하여 같으면 word_samechar증가
              word_samechar++;
          }
          if(word_samechar == word_l - word_star){  //같은 문자 수와 문자열에서 *이 없는 문자의 수와 같으면 해당 레이블 정보 출력
            search_print_m(search_movie);
            continue;
          }
        }
        if(word_qnum != 0){ //? 있는 경우, directro와 비교
          data_len = strlen(search_movie -> director -> data);  //search_movie -> director -> data의 문자열 길이를 구함
          gap_star = data_len - (word_l - word_star); //비교할 위치를 맞추기 위해 gap_star를 구함
          word_samechar = 0;  //word_samechar를 0으로 초기화 해줌
          for(int i = 0; i<(word_l - 1); i++){  //*의 수를 뺀 string 길이 만큼 비교할 문자열과 비교
            if(*(string + i + 1) == *(search_movie -> director -> data + gap_star + i)) //string과 비교할 문자열을 뒤에서 부터 비교하여 같으면 word_samechar증가
              word_samechar++;
          }
          if(word_samechar == word_noq - word_star){  //같은 문자 수와 문자열에서 ?와 *이 없는 문자의 수와 같으면 해당 레이블 정보 출력
            search_print_m(search_movie);
            continue;
          }
        }
        else{ //? 없는 경우, director 비교
          data_len = strlen(search_movie -> director -> data);  //search_movie -> director -> data의 문자열 길이를 구함
          gap_star = data_len - (word_l - word_star); //비교할 위치를 맞추기 위해 gap_star를 구함
          word_samechar = 0;  //word_samechar를 0으로 초기화 해줌
          for(int i = 0; i<(word_l - 1); i++){  //*의 수를 뺀 string 길이 만큼 비교할 문자열과 비교
            if(*(string + i + 1) == *(search_movie -> director -> data + gap_star + i)) //string과 비교할 문자열을 뒤에서 부터 비교하여 같으면 word_samechar증가
              word_samechar++;
          }
          if(word_samechar == word_l - word_star){  //같은 문자 수와 문자열에서 *이 없는 문자의 수와 같으면 해당 레이블 정보 출력
            search_print_m(search_movie);
            continue;
          }
        }
        if(word_qnum != 0){ //? 있는 경우, birth와 비교
          data_len = strlen(string_y);   //string_y의 문자열 길이를 구함
          gap_star = data_len - (word_l - word_star); //비교할 위치를 맞추기 위해 gap_star를 구함
          word_samechar = 0;  //word_samechar를 0으로 초기화 해줌
          for(int i = 0; i<(word_l - 1); i++){  //*의 수를 뺀 string 길이 만큼 비교할 문자열과 비교
            if(*(string + i + 1) == *(string_y + gap_star + i)) //string과 비교할 문자열을 뒤에서 부터 비교하여 같으면 word_samechar증가
              word_samechar++;
          }
          if(word_samechar == word_noq - word_star){  //같은 문자 수와 문자열에서 ?와 *이 없는 문자의 수와 같으면 해당 레이블 정보 출력
            search_print_m(search_movie);
            continue;
          }
        }
        else{ //? 없는 경우, year와 비교
          data_len = strlen(string_y);  //string_y의 문자열 길이를 구함
          gap_star = data_len - (word_l - word_star); //비교할 위치를 맞추기 위해 gap_star를 구함
          word_samechar = 0;  //word_samechar를 0으로 초기화 해줌
          for(int i = 0; i<(word_l - 1); i++){  //*의 수를 뺀 string 길이 만큼 비교할 문자열과 비교
            if(*(string + i + 1) == *(string_y + i))  //string과 비교할 문자열을 뒤에서 부터 비교하여 같으면 word_samechar증가
              word_samechar++;
          }
          if(word_samechar == word_l - word_star){  //같은 문자 수와 문자열에서 *이 없는 문자의 수와 같으면 해당 레이블 정보 출력
            search_print_m(search_movie);
            continue;
          }
        }
        if(word_qnum != 0){ //? 있는 경우, time과 비교
          data_len = strlen(string_t);  //string_t의 문자열 길이를 구함
          gap_star = data_len - (word_l - word_star); //비교할 위치를 맞추기 위해 gap_star를 구함
          word_samechar = 0;  //word_samechar를 0으로 초기화 해줌
          for(int i = 0; i<(word_l - 1); i++){   //*의 수를 뺀 string 길이 만큼 비교할 문자열과 비교
            if(*(string + i + 1) == *(string_t + gap_star + i)) //string과 비교할 문자열을 뒤에서 부터 비교하여 같으면 word_samechar증가
              word_samechar++;
          }
          if(word_samechar == word_noq - word_star){  //같은 문자 수와 문자열에서 ?와 *이 없는 문자의 수와 같으면 해당 레이블 정보 출력
            search_print_m(search_movie);
            continue;
          }
        }
        else{ //? 없는 경우, time와 비교
          data_len = strlen(string_t);  //string_t의 문자열 길이를 구함
          gap_star = data_len - (word_l - word_star); //비교할 위치를 맞추기 위해 gap_star를 구함
          word_samechar = 0;  //word_samechar를 0으로 초기화 해줌
          for(int i = 0; i<(word_l - 1); i++){  //*의 수를 뺀 string 길이 만큼 비교할 문자열과 비교
            if(*(string + i + 1) == *(string_t + i))  //string과 비교할 문자열을 뒤에서 부터 비교하여 같으면 word_samechar증가
              word_samechar++;
          }
          if(word_samechar == word_l - word_star){  //같은 문자 수와 문자열에서 *이 없는 문자의 수와 같으면 해당 레이블 정보 출력
            search_print_m(search_movie);
            continue;
          }
        }
        actor = search_movie -> actor;  //search_movie -> actor가 노드 하는 곳을 초기화 시켜주기 위해 actor에 노드 할 곳 저장
        if(word_qnum != 0){  //? 있는 경우, actor와 비교
          data_len = strlen(search_movie -> actor -> data); //search_movie -> actor -> data의 문자열 길이를 구함
          gap_star = data_len - (word_l - word_star); //비교할 위치를 맞추기 위해 gap_star를 구함
          word_samechar = 0;  //word_samechar를 0으로 초기화 해줌
          for(int i = 0; i<(word_l - 1); i++){  //*의 수를 뺀 string 길이 만큼 비교할 문자열과 비교
            if(*(string + i + 1) == *(search_movie -> actor -> data + gap_star + i))  //string과 비교할 문자열을 뒤에서 부터 비교하여 같으면 word_samechar증가
              word_samechar++;
          }
          if(word_samechar == word_noq - word_star){  //같은 문자 수와 문자열에서 ?와 *이 없는 문자의 수와 같으면 해당 레이블 정보 출력
            search_print_m(search_movie);
            continue;
          }
        }
        else{ //? 없는 경우, actor와 비교, actor비교
          data_len = strlen(search_movie -> actor -> data); //search_movie -> actor -> data의 문자열 길이를 구함
          gap_star = data_len - (word_l - word_star); //비교할 위치를 맞추기 위해 gap_star를 구함
          word_samechar = 0;  //word_samechar를 0으로 초기화 해줌
          for(int i = 0; i<(word_l - 1); i++){  //*의 수를 뺀 string 길이 만큼 비교할 문자열과 비교
            if(*(string + i + 1) == *(search_movie -> actor -> data + gap_star + i))  //string과 비교할 문자열을 뒤에서 부터 비교하여 같으면 word_samechar증가
              word_samechar++;
          }
          if(word_samechar == word_l - word_star){  //같은 문자 수와 문자열에서 *이 없는 문자의 수와 같으면 해당 레이블 정보 출력
            search_print_m(search_movie);
            continue;
          }
        }

        while(search_movie -> actor -> next != NULL){ //search_movie -> actor가 노드하는 곳이 NULL까지 while문 실행
          search_movie -> actor = search_movie -> actor -> next;  //search_movie -> actor가 노드 하는 곳을 다음 actor로 바꺼줌
          if(word_qnum != 0){ //? 있는 경우, actor와 비교
            data_len = strlen(search_movie -> actor -> data);  //search_movie -> actor -> data의 문자열 길이를 구함
            gap_star = data_len - (word_l - word_star); //비교할 위치를 맞추기 위해 gap_star를 구함
            word_samechar = 0;  //word_samechar를 0으로 초기화 해줌
            for(int i = 0; i<(word_l - 1); i++){  //word_samechar를 0으로 초기화 해줌
              if(*(string + i + 1) == *(search_movie -> actor -> data + gap_star + i))  //string과 비교할 문자열을 뒤에서 부터 비교하여 같으면 word_samechar증가
                word_samechar++;
            }
            if(word_samechar == word_noq - word_star){  //같은 문자 수와 문자열에서 ?와 *이 없는 문자의 수와 같으면 해당 레이블 정보 출력
              search_movie -> actor = actor;
              search_print_m(search_movie);
              break;
            }
          }
          else{
            data_len = strlen(search_movie -> actor -> data);
            gap_star = data_len - (word_l - word_star);
            word_samechar = 0;
            for(int i = 0; i<(word_l - 1); i++){
              if(*(string + i + 1) == *(search_movie -> actor -> data + gap_star + i))
                word_samechar++;
            }
            if(word_samechar == word_l - word_star){
              search_movie -> actor = actor;
              search_print_m(search_movie);
              break;
            }
          }
        }
        search_movie -> actor = actor;

      } //while
    } //else if(*string == '*')
  } //if word_star

  else if(word_l == word_noq){
    while(search_movie -> next_movie != movie -> tail){
      search_movie = search_movie -> next_movie;
      if(strcmp(string, search_movie->title) == 0){
        search_print_m(search_movie);
        continue;
      }
      if(strcmp(string, search_movie -> genre) == 0){
        search_print_m(search_movie);
        continue;
      }
      if(strcmp(string, search_movie -> director -> data) == 0){
        search_print_m(search_movie);
        continue;
      }
      if(atoi(string) == search_movie -> year){
        search_print_m(search_movie);
        continue;
      }
      if(atoi(string) == search_movie -> time){
        search_print_m(search_movie);
        continue;
      }
      else{
        actor = search_movie -> actor;
        while(search_movie -> actor -> next != NULL || strcmp(search_movie -> actor -> data, string) == 0){
        if(strcmp(search_movie -> actor -> data, string) == 0){
          search_movie -> actor = actor;
          search_print_m(search_movie);
          break;
          }
          search_movie -> actor = search_movie -> actor -> next;
        }
        search_movie -> actor = actor;
      }
    }
  }

  else if(word_l != word_noq){
    while(search_movie -> next_movie != movie -> tail){
      search_movie = search_movie -> next_movie;
      change_num_to_string(string_y, search_movie -> year);
      change_num_to_string(string_t, search_movie -> time);

      if(strlen(search_movie -> title) == word_l){
        word_samechar = 0;
        for(int i = 0; i < word_l; i++){
          if(*(string + i) == *(search_movie -> title + i))
            word_samechar++;
          }
        if(word_samechar == word_noq){
          search_print_m(search_movie);
          continue;
        }
      }
      if(strlen(search_movie -> genre) == word_l){
        word_samechar = 0;
        for(int i = 0; i < word_l; i++){
          if(*(string + i) == *(search_movie -> genre + i))
            word_samechar++;
          }
        if(word_samechar == word_noq){
          search_print_m(search_movie);
          continue;
        }
      }

      if(strlen(search_movie -> director -> data) == word_l){
        word_samechar = 0;
        for(int i = 0; i < word_l; i++){
          if(*(string + i) == *(search_movie -> director -> data + i))
            word_samechar++;
          }
        if(word_samechar == word_noq){
          search_print_m(search_movie);
          continue;
        }
      }

      if(strlen(string_y) == word_l){
        word_samechar = 0;
        for(int i = 0; i < word_l; i++){
          if(*(string + i) == *(string_y + i))
            word_samechar++;
          }
        if(word_samechar == word_noq){
          search_print_m(search_movie);
          continue;
        }
      }
      if(strlen(string_t) == word_l){
        word_samechar = 0;
        for(int i = 0; i < word_l; i++){
          if(*(string + i) == *(string_t + i))
            word_samechar++;
          }
        if(word_samechar == word_noq){
          search_print_m(search_movie);
          continue;
        }
      }

      actor = search_movie -> actor;
      if(strlen(search_movie -> actor -> data) == word_l){
        word_samechar = 0;
        for(int i = 0; i < word_l; i++){
          if(*(string + i) == *(search_movie -> actor -> data +i))
            word_samechar++;
        }
      if(word_samechar == word_noq){
        search_print_m(search_movie);
        continue;
        }
      }

      while(search_movie -> actor -> next != NULL){
        word_samechar = 0;
        search_movie -> actor = search_movie -> actor -> next;
        if(strlen(search_movie -> actor -> data) == word_l){
          for(int i = 0; i < word_l; i++){
            if(*(string + i) == *(search_movie -> actor -> data + i))
              word_samechar++;
          }
            if(word_samechar == word_noq){
              search_movie -> actor = actor;
              search_print_m(search_movie);
              break;
            }
        }
      }

      search_movie -> actor = actor;
    } //while
  } //else if

} // function
void search_print_d(LINK_D *search_director){
  LINK_W *title;
  title = search_director -> title;
  printf("%d:%s:%c:%d:", search_director -> serial_num, search_director -> name, search_director ->sex, search_director -> birth );
  while(search_director -> title -> next != NULL){
    printf("%s,", search_director -> title -> data);
    search_director -> title = search_director -> title -> next;
  }
  printf("%s\n", search_director -> title -> data);
  search_director -> title = title;
}

void search_d(ALL_D *director, char *string){
  LINK_W *title;
  char *string_y;
  LINK_D *search_director = director -> head;
  string_y = (char *)calloc(sizeof(char), 8);
  int word_l = 0;
  int word_noq = 0;
  int word_samechar =0;
  int word_star = 0;
  int word_qnum = 0;
  int data_len = 0;
  int gap_star = 0;
  word_l =strlen(string);

  for(int i = 0; i < word_l; i++){
    if(*(string +i) == '*')
    word_star++;
  }

  for(int i = 0; i < word_l; i++){
    if(*(string + i) != '?')
    word_noq++;
  }

  for(int i = 0; i < word_l; i++){
    if(*(string + i) == '?')
    word_qnum++;
  }

  if(word_star != 0){
    if(*(string + word_l - 1) == '*'){
      while(search_director -> next_director != director -> tail){
        search_director = search_director -> next_director;
        change_num_to_string(string_y, search_director -> birth);

        if(word_qnum != 0){
          word_samechar = 0;
          for(int i = 0; i < (word_l -1); i++){
            if(*(string + i ) == *(search_director -> name + i))
            word_samechar++;
          }
          if(word_samechar == word_noq - word_star){
            search_print_d(search_director);
            continue;
          }
        }
        else{
          word_samechar =0;
          for(int i = 0; i < (word_l - 1); i++){
            if(*(string + i ) == *(search_director -> name + i))
              word_samechar++;
          }
          if(word_samechar == (word_l - word_star)){
            search_print_d(search_director);
            continue;
            }
          }
          //Name
          if(word_qnum != 0){
            word_samechar = 0;
            for(int i = 0; i <(word_l - 1); i++){
              if(*(string + i) == *(string_y +i))
                word_samechar++;
            }
            if(word_samechar == word_noq - word_star){
              search_print_d(search_director);
              continue;
            }
          }
          else{
            word_samechar = 0;
            for(int i = 0; i <(word_l - 1); i++){
              if(*(string + i) == *(string_y + i))
                word_samechar++;
            }
            if(word_samechar == (word_l - word_star)){
              search_print_d(search_director);
              continue;
            }
          }
          //Birth
          title = search_director -> title;
          if(word_qnum != 0){
            word_samechar = 0;
            for(int i = 0; i < (word_l - 1); i++){
              if(*(string + i) == *(search_director -> title -> data + i)){
                word_samechar++;
              }
            }
            if(word_samechar == word_noq - word_star){
              search_print_d(search_director);
              continue;
            }
          }
          else{
            word_samechar = 0;
            for(int i = 0; i < (word_l - 1); i++){
              if(*(string + i) == *(search_director -> title -> data + i)){
                word_samechar++;
              }
            }
            if(word_samechar == word_l - word_star){
              search_print_d(search_director);
              continue;
            }
          }

          while(search_director -> title -> next != NULL){
            word_samechar = 0;
            search_director -> title = search_director -> title -> next;
            if(word_qnum != 0){
              word_samechar = 0;
              for(int i = 0; i < (word_l - 1); i++){
                if(*(string + i) == *(search_director -> title -> data + i)){
                  word_samechar++;
                }
              }
              if(word_samechar == word_noq - word_star){
                search_director -> title = title;
                search_print_d(search_director);
                break;
              }
            }
            else{
              word_samechar = 0;
              for(int i = 0; i < (word_l - 1); i++){
                if(*(string + i) == *(search_director -> title -> data + i)){
                  word_samechar++;
                }
              }
              if(word_samechar == word_l - word_star){
                search_director -> title = title;
                search_print_d(search_director);
                break;
              }
            }
          }
          search_director -> title = title;
      } //while
    } //if(*(string + word_l - 1) == '*')

    else if(*string == '*'){
      while(search_director -> next_director != director -> tail){
        search_director = search_director -> next_director;
        change_num_to_string(string_y, search_director -> birth);

        if(word_qnum != 0){
          data_len = strlen(search_director -> name);
          gap_star = data_len - (word_l - word_star);
          word_samechar = 0;
          for(int i = 0; i < (word_l - 1); i++){
            if(*(string + i + 1) == *(search_director -> name + gap_star + i))
            word_samechar++;
          }
          if(word_samechar == word_noq - word_star){
            search_print_d(search_director);
            continue;
          }
        }
        else{
          data_len = strlen(search_director -> name);
          gap_star = data_len - (word_l - word_star);
          word_samechar = 0;
          for(int i = 0; i<(word_l -1); i++){
            if(*(string + i + 1) == *(search_director -> name + gap_star + i))
            word_samechar++;
          }
          if(word_samechar == word_l - word_star){
            search_print_d(search_director);
            continue;
          }
        }
        if(word_qnum != 0){
          data_len = strlen(string_y);
          gap_star = data_len - (word_l - word_star);
          word_samechar = 0;
          for(int i = 0; i < (word_l - 1); i++){
            if(*(string + i + 1) == *(string_y + gap_star + i))
            word_samechar++;
          }
          if(word_samechar == word_noq - word_star){
            search_print_d(search_director);
            continue;
          }
        }
        else{
          data_len = strlen(string_y);
          gap_star = data_len - (word_l - word_star);
          word_samechar = 0;
          for(int i = 0; i < (word_l -1); i++){
            if(*(string + i + 1) == *(string_y + i))
            word_samechar++;
          }
          if(word_samechar == word_l - word_star){
            search_print_d(search_director);
            continue;
          }
        }

        title = search_director -> title;
        if(word_qnum != 0){
          data_len = strlen(search_director -> title -> data);
          gap_star = data_len - (word_l - word_star);
          word_samechar = 0;
          for(int i = 0; i<(word_l - 1); i++){
            if(*(string + i + 1) == *(search_director -> title -> data + gap_star + i))
              word_samechar++;
          }
          if(word_samechar == word_noq - word_star){
            search_print_d(search_director);
            continue;
          }
        }
        else{
          data_len = strlen(search_director -> title -> data);
          gap_star = data_len - (word_l - word_star);
          word_samechar = 0;
          for(int i = 0; i<(word_l - 1); i++){
            if(*(string + i + 1) == *(search_director -> title -> data + gap_star + i))
              word_samechar++;
          }
          if(word_samechar == word_l - word_star){
            search_print_d(search_director);
            continue;
          }
        }

        while(search_director -> title -> next != NULL){
          search_director -> title = search_director -> title -> next;
          if(word_qnum != 0){
            data_len = strlen(search_director -> title -> data);
            gap_star = data_len - (word_l - word_star);
            word_samechar = 0;
            for(int i = 0; i<(word_l - 1); i++){
              if(*(string + i + 1) == *(search_director -> title -> data + gap_star + i))
                word_samechar++;
            }
            if(word_samechar == word_noq - word_star){
              search_director -> title = title;
              search_print_d(search_director);
              break;
            }
          }

          else{
            data_len = strlen(search_director -> title -> data);
            gap_star = data_len - (word_l - word_star);
            word_samechar = 0;
            for(int i = 0; i<(word_l - 1); i++){
              if(*(string + i + 1) == *(search_director -> title -> data + gap_star + i))
                word_samechar++;
            }
            if(word_samechar == word_l - word_star){
              search_director -> title = title;
              search_print_d(search_director);
              break;
            }
          }
        }
        search_director -> title = title;
      } //while
    } //(*string == '*')
  } //if(word_star != 0)
  else if(word_l == word_noq){
    while(search_director -> next_director != director -> tail){
      search_director = search_director -> next_director;
      if(strcmp(string, search_director -> name) == 0){
        search_print_d(search_director);
        continue;
      }
      if(*string == search_director -> sex){
        search_print_d(search_director);
        continue;
      }
      if(atoi(string) == search_director -> birth){
        search_print_d(search_director);
        continue;
      }
      else{
        title = search_director -> title;
        while(search_director -> title -> next != NULL || strcmp(search_director -> title -> data, string) == 0){
          if(strcmp(search_director -> title -> data, string) == 0){
            search_director -> title = title;
            search_print_d(search_director);
            break;
          }
          search_director -> title = search_director -> title -> next;
        }
        search_director ->title = title;
      }
    }
  }
  else if(word_l != word_noq){
    while(search_director -> next_director != director -> tail){
      search_director = search_director -> next_director;
      change_num_to_string(string_y, search_director -> birth);

      if(strlen(search_director -> name) == word_l){
        word_samechar =0;
        for(int i =0; i < word_l; i++){
          if(*(string + i) == *(search_director -> name +i))
          word_samechar++;
        }
        if(word_samechar == word_noq){
          search_print_d(search_director);
          continue;
        }
      }
      //name
      if(strlen(string_y) == word_l){
        word_samechar = 0;
        for(int i = 0; i < word_l; i++){
          if(*(string + i) == *(string_y + i))
            word_samechar++;
        }
        if(word_samechar == word_noq){
          search_print_d(search_director);
          continue;
        }
      }
      //Birth
      if(word_l == 1){
        if(*string == '?'){
          search_print_d(search_director);
          continue;
        }
      }
      // ?한개만 있을때
      title = search_director -> title;
      if(strlen(search_director -> title -> data) == word_l){
        word_samechar = 0;
        for(int i = 0; i < word_l; i++){
          if(*(string + i) == *(search_director -> title -> data +i))
            word_samechar++;
        }
        if(word_samechar == word_noq){
          search_print_d(search_director);
          continue;
        }
      }
      title = search_director -> title;
      if(strlen(search_director -> title -> data) == word_l){
        word_samechar = 0;
        for(int i = 0; i < word_l; i++){
          if(*(string + i) == *(search_director -> title -> data +i))
            word_samechar++;
        }
      if(word_samechar == word_noq){
        search_print_d(search_director);
        continue;
        }
      }

      while(search_director -> title -> next != NULL){
        word_samechar = 0;
        search_director -> title = search_director -> title -> next;
        if(strlen(search_director -> title -> data) == word_l){
          for(int i = 0; i < word_l; i++){
            if(*(string + i) == *(search_director -> title -> data + i))
              word_samechar++;
          }
            if(word_samechar == word_noq){
              search_director -> title = title;
              search_print_d(search_director);
              break;
            }
        }
      }
      search_director -> title = title;
    } //while
  } //else(word_l != word_noq)

} //function

void search_print_a(LINK_A *search_actor){
  LINK_W *title;
  title = search_actor -> title;
  printf("%d:%s:%c:%d:", search_actor -> serial_num, search_actor -> name, search_actor ->sex, search_actor -> birth );
  while(search_actor -> title -> next != NULL){
    printf("%s,", search_actor -> title -> data);
    search_actor -> title = search_actor -> title -> next;
  }
  printf("%s\n", search_actor -> title -> data);
  search_actor -> title = title;
}

void search_a(ALL_A *actor, char *string){
  LINK_W *title;
  char *string_y;
  LINK_A *search_actor = actor -> head;
  string_y = (char *)calloc(sizeof(char), 8);
  int word_l = 0;
  int word_noq = 0;
  int word_samechar =0;
  int word_star = 0;
  int word_qnum = 0;
  int data_len = 0;
  int gap_star = 0;
  word_l =strlen(string);

  for(int i = 0; i < word_l; i++){
    if(*(string +i) == '*')
    word_star++;
  }

  for(int i = 0; i < word_l; i++){
    if(*(string + i) != '?')
    word_noq++;
  }

  for(int i = 0; i < word_l; i++){
    if(*(string + i) == '?')
    word_qnum++;
  }

  if(word_star != 0){
    if(*(string + word_l - 1) == '*'){  //*마지막
      while(search_actor -> next_actor != actor -> tail){
        search_actor = search_actor -> next_actor;
        change_num_to_string(string_y, search_actor -> birth);

        if(word_qnum != 0){ //*마지막에 ?가 중간
          word_samechar = 0;
          for(int i = 0; i < (word_l -1); i++){
            if(*(string + i ) == *(search_actor -> name + i))
            word_samechar++;
          }
          if(word_samechar == word_noq - word_star){
            search_print_a(search_actor);
            continue;
          }
        }
        else{
          word_samechar =0;
          for(int i = 0; i < (word_l - 1); i++){
            if(*(string + i ) == *(search_actor -> name + i))
              word_samechar++;
          }
          if(word_samechar == (word_l - word_star)){
            search_print_a(search_actor);
            continue;
            }
          }
          //Name
          if(word_qnum != 0){
            word_samechar = 0;
            for(int i = 0; i <(word_l - 1); i++){
              if(*(string + i) == *(string_y +i))
                word_samechar++;
            }
            if(word_samechar == word_noq - word_star){
              search_print_a(search_actor);
              continue;
            }
          }
          else{
            word_samechar = 0;
            for(int i = 0; i <(word_l - 1); i++){
              if(*(string + i) == *(string_y + i))
                word_samechar++;
            }
            if(word_samechar == (word_l - word_star)){
              search_print_a(search_actor);
              continue;
            }
          }
          //Birth
          title = search_actor -> title;
          if(word_qnum != 0){
            word_samechar = 0;
            for(int i = 0; i < (word_l - 1); i++){
              if(*(string + i) == *(search_actor -> title -> data + i)){
                word_samechar++;
              }
            }
            if(word_samechar == word_noq - word_star){
              search_print_a(search_actor);
              continue;
            }
          }
          else{
            word_samechar = 0;
            for(int i = 0; i < (word_l - 1); i++){
              if(*(string + i) == *(search_actor -> title -> data + i)){
                word_samechar++;
              }
            }
            if(word_samechar == word_l - word_star){
              search_print_a(search_actor);
              continue;
            }
          }

          while(search_actor -> title -> next != NULL){
            word_samechar = 0;
            search_actor -> title = search_actor -> title -> next;
            if(word_qnum != 0){
              word_samechar = 0;
              for(int i = 0; i < (word_l - 1); i++){
                if(*(string + i) == *(search_actor -> title -> data + i)){
                  word_samechar++;
                }
              }
              if(word_samechar == word_noq - word_star){
                search_actor -> title = title;
                search_print_a(search_actor);
                break;
              }
            }
            else{
              word_samechar = 0;
              for(int i = 0; i < (word_l - 1); i++){
                if(*(string + i) == *(search_actor -> title -> data + i)){
                  word_samechar++;
                }
              }
              if(word_samechar == word_l - word_star){
                search_actor -> title = title;
                search_print_a(search_actor);
                break;
              }
            }
          }
          search_actor -> title = title;
      } //while
    } //if(*(string + word_l - 1) == '*')

    else if(*string == '*'){  //*가 앞에
      while(search_actor -> next_actor != actor -> tail){
        search_actor = search_actor -> next_actor;
        change_num_to_string(string_y, search_actor -> birth);

        if(word_qnum != 0){
          data_len = strlen(search_actor -> name);
          gap_star = data_len - (word_l - word_star);
          word_samechar = 0;
          for(int i = 0; i < (word_l - 1); i++){
            if(*(string + i + 1) == *(search_actor -> name + gap_star + i))
            word_samechar++;
          }
          if(word_samechar == word_noq - word_star){
            search_print_a(search_actor);
            continue;
          }
        }
        else{
          data_len = strlen(search_actor -> name);
          gap_star = data_len - (word_l - word_star);
          word_samechar = 0;
          for(int i = 0; i<(word_l -1); i++){
            if(*(string + i + 1) == *(search_actor -> name + gap_star + i))
            word_samechar++;
          }
          if(word_samechar == word_l - word_star){
            search_print_a(search_actor);
            continue;
          }
        }
        if(word_qnum != 0){
          data_len = strlen(string_y);
          gap_star = data_len - (word_l - word_star);
          word_samechar = 0;
          for(int i = 0; i < (word_l - 1); i++){
            if(*(string + i + 1) == *(string_y + gap_star + i))
            word_samechar++;
          }
          if(word_samechar == word_noq - word_star){
            search_print_a(search_actor);
            continue;
          }
        }
        else{
          data_len = strlen(string_y);
          gap_star = data_len - (word_l - word_star);
          word_samechar = 0;
          for(int i = 0; i < (word_l -1); i++){
            if(*(string + i + 1) == *(string_y + i))
            word_samechar++;
          }
          if(word_samechar == word_l - word_star){
            search_print_a(search_actor);
            continue;
          }
        }

        title = search_actor -> title;
        if(word_qnum != 0){
          data_len = strlen(search_actor -> title -> data);
          gap_star = data_len - (word_l - word_star);
          word_samechar = 0;
          for(int i = 0; i<(word_l - 1); i++){
            if(*(string + i + 1) == *(search_actor -> title -> data + gap_star + i))
              word_samechar++;
          }
          if(word_samechar == word_noq - word_star){
            search_print_a(search_actor);
            continue;
          }
        }
        else{
          data_len = strlen(search_actor -> title -> data);
          gap_star = data_len - (word_l - word_star);
          word_samechar = 0;
          for(int i = 0; i<(word_l - 1); i++){
            if(*(string + i + 1) == *(search_actor -> title -> data + gap_star + i))
              word_samechar++;
          }
          if(word_samechar == word_l - word_star){
            search_print_a(search_actor);
            continue;
          }
        }

        while(search_actor -> title -> next != NULL){
          search_actor -> title = search_actor -> title -> next;
          if(word_qnum != 0){
            data_len = strlen(search_actor -> title -> data);
            gap_star = data_len - (word_l - word_star);
            word_samechar = 0;
            for(int i = 0; i<(word_l - 1); i++){
              if(*(string + i + 1) == *(search_actor -> title -> data + gap_star + i))
                word_samechar++;
            }
            if(word_samechar == word_noq - word_star){
              search_actor -> title = title;
              search_print_a(search_actor);
              break;
            }
          }

          else{
            data_len = strlen(search_actor -> title -> data);
            gap_star = data_len - (word_l - word_star);
            word_samechar = 0;
            for(int i = 0; i<(word_l - 1); i++){
              if(*(string + i + 1) == *(search_actor -> title -> data + gap_star + i))
                word_samechar++;
            }
            if(word_samechar == word_l - word_star){
              search_actor -> title = title;
              search_print_a(search_actor);
              break;
            }
          }
        }
        search_actor -> title = title;
      } //while
    } //(*string == '*')
  } //if(word_star != 0)
  else if(word_l == word_noq){
    while(search_actor -> next_actor != actor -> tail){
      search_actor = search_actor -> next_actor;
      if(strcmp(string, search_actor -> name) == 0){
        search_print_a(search_actor);
        continue;
      }
      if(*string == search_actor -> sex){
        search_print_a(search_actor);
        continue;
      }
      if(atoi(string) == search_actor -> birth){
        search_print_a(search_actor);
        continue;
      }
      else{
        title = search_actor -> title;
        while(search_actor -> title -> next != NULL || strcmp(search_actor -> title -> data, string) == 0){
          if(strcmp(search_actor -> title -> data, string) == 0){
            search_actor -> title = title;
            search_print_a(search_actor);
            break;
          }
          search_actor -> title = search_actor -> title -> next;
        }
        search_actor ->title = title;
      }
    }
  }
  else if(word_l != word_noq){
    while(search_actor -> next_actor != actor -> tail){
      search_actor = search_actor -> next_actor;
      change_num_to_string(string_y, search_actor -> birth);

      if(strlen(search_actor -> name) == word_l){
        word_samechar =0;
        for(int i =0; i < word_l; i++){
          if(*(string + i) == *(search_actor -> name +i))
          word_samechar++;
        }
        if(word_samechar == word_noq){
          search_print_a(search_actor);
          continue;
        }
      }
      //name
      if(strlen(string_y) == word_l){
        word_samechar = 0;
        for(int i = 0; i < word_l; i++){
          if(*(string + i) == *(string_y + i))
            word_samechar++;
        }
        if(word_samechar == word_noq){
          search_print_a(search_actor);
          continue;
        }
      }
      //Birth
      if(word_l == 1){
        if(*string == '?'){
          search_print_a(search_actor);
          continue;
        }
      }
      // ?한개만 있을때
      title = search_actor -> title;
      if(strlen(search_actor -> title -> data) == word_l){
        word_samechar = 0;
        for(int i = 0; i < word_l; i++){
          if(*(string + i) == *(search_actor -> title -> data +i))
            word_samechar++;
        }
        if(word_samechar == word_noq){
          search_print_a(search_actor);
          continue;
        }
      }
      title = search_actor -> title;
      if(strlen(search_actor -> title -> data) == word_l){
        word_samechar = 0;
        for(int i = 0; i < word_l; i++){
          if(*(string + i) == *(search_actor -> title -> data +i))
            word_samechar++;
        }
      if(word_samechar == word_noq){
        search_print_a(search_actor);
        continue;
        }
      }

      while(search_actor -> title -> next != NULL){
        word_samechar = 0;
        search_actor -> title = search_actor -> title -> next;
        if(strlen(search_actor -> title -> data) == word_l){
          for(int i = 0; i < word_l; i++){
            if(*(string + i) == *(search_actor -> title -> data + i))
              word_samechar++;
          }
            if(word_samechar == word_noq){
              search_actor -> title = title;
              search_print_a(search_actor);
              break;
            }
        }
      }
      search_actor -> title = title;
    } //while
  } //else(word_l != word_noq)

} //function
void commands(ALL_D *director,ALL_A *actor,ALL_M *movie){
  //명령어를 입력받아서 그에 해당하는 함수를 출력하기 위해 선언한 변수
  //serach print add update delete sort save end
  char *command,*input_data,*string, *check_input_data;//문자열을 입력받을 char형 포인터 배열을 선언하였음
  char input_text,option;//옵션을 받을 char형 변수를 선언함
  char checking;
  FILE *check_movie,*check_actor,*check_director;
  check_movie = fopen("movie_list.txt","r");
  check_actor = fopen("actor_list.txt","r");
  check_director = fopen("director_list.txt","r");
  if(check_movie != NULL)
  copy_movie_list();
  if(check_actor != NULL)
  copy_actor_list();
  if(check_director != NULL)
  copy_director_list();
  fclose(check_movie);
  fclose(check_actor);
  fclose(check_director);
  int input_num,i;
  scan_movie_log(movie);
  scan_actor_log(actor);
  scan_director_log(director);
  compare_movie_word(movie,director,actor);
  compare_actor_word(movie,actor);
  compare_director_word(movie,director);
    printf("You can use add, update, delete, search, sort, save, end commands.\n");//사용 가능한 명령어를 보여줌
  while(1){
    printf("(movie)");
    input_data = (char *)calloc(100,sizeof(char));
  command =(char *)malloc(sizeof(char)*10);//command포인터에 동적 메모리를 할당하였음
  check_input_data = (char *)calloc(100,sizeof(char));
  string = (char *)calloc(100,sizeof(char));
  scanf("%s", command);//명령어를 입력받음
  if(strcmp(command, "add") == 0){//파일에 새로운 내용을 더함
    scanf("%*c%c%*c",&input_text);//'\n'을 무시하고 명령어를 입력받음
    if(input_text=='d'){//director에 내용을 추가함
      add_director(director);
      free(movie);
      free(director);
      free(actor);
      make_movie_admin(&movie);
      make_actor_admin(&actor);
      make_director_admin(&director);
      scan_movie_log(movie);
      scan_actor_log(actor);
      scan_director_log(director);
      compare_movie_word(movie,director,actor);
      compare_actor_word(movie,actor);
      compare_director_word(movie,director);
    printf("@@done\n");
  }
    else if(input_text=='m'){//movie에 내용을 추가함
      add_movie(movie);
      free(movie);
      free(director);
      free(actor);
      make_movie_admin(&movie);
      make_actor_admin(&actor);
      make_director_admin(&director);
      scan_movie_log(movie);
      scan_actor_log(actor);
      scan_director_log(director);
      compare_movie_word(movie,director,actor);
      compare_actor_word(movie,actor);
      compare_director_word(movie,director);
    printf("@@done\n");
  }
    else if(input_text=='a'){//actor에 내용을 추가함
      add_actor(actor);
      free(movie);
      free(director);
      free(actor);
      make_movie_admin(&movie);
      make_actor_admin(&actor);
      make_director_admin(&director);
      scan_movie_log(movie);
      scan_actor_log(actor);
      scan_director_log(director);
      compare_movie_word(movie,director,actor);
      compare_actor_word(movie,actor);
      compare_director_word(movie,director);
    printf("@@done\n");
  }
}
  else if(strcmp(command,"update")==0){//파일을 업데이트함
    scanf("%*c%c",&input_text);
    if(input_text=='m'){
      getchar();
			scanf("%s%*c",input_data);
			scanf("%d%*c",&input_num);
			printf("%d\n",input_num);
			update_movie(movie,input_data,input_num);
      free(movie);
      free(director);
      free(actor);
      make_movie_admin(&movie);
      make_actor_admin(&actor);
      make_director_admin(&director);
      scan_movie_log(movie);
      scan_actor_log(actor);
      scan_director_log(director);
      compare_movie_word(movie,director,actor);
      compare_actor_word(movie,actor);
      compare_director_word(movie,director);
	  }
    else if(input_text=='a'){
			getchar();
			scanf("%s%*c",input_data);
			scanf("%d%*c",&input_num);
			printf("%d\n",input_num);
			update_actor(actor,input_data,input_num);
      free(movie);
      free(director);
      free(actor);
      make_movie_admin(&movie);
      make_actor_admin(&actor);
      make_director_admin(&director);
      scan_movie_log(movie);
      scan_actor_log(actor);
      scan_director_log(director);
      compare_movie_word(movie,director,actor);
      compare_actor_word(movie,actor);
      compare_director_word(movie,director);
    }
    else if(input_text='d'){
			getchar();
			scanf("%s%*c",input_data);
			scanf("%d%*c",&input_num);
			printf("%d\n",input_num);
			update_director(director,input_data,input_num);
      free(movie);
      free(director);
      free(actor);
      make_movie_admin(&movie);
      make_actor_admin(&actor);
      make_director_admin(&director);
      scan_movie_log(movie);
      scan_actor_log(actor);
      scan_director_log(director);
      compare_movie_word(movie,director,actor);
      compare_actor_word(movie,actor);
      compare_director_word(movie,director);
    }
  }
  else if(strcmp(command,"delete")==0){//파일에서 내용을 지워줌
    scanf("%*c%c",&input_text);
    if(input_text == 'm'){
      scanf("%d",&input_num);
      i=delete_movie(movie,input_num);
      free(movie);
      free(director);
      free(actor);
      make_movie_admin(&movie);
      make_actor_admin(&actor);
      make_director_admin(&director);
      scan_movie_log(movie);
      scan_actor_log(actor);
      scan_director_log(director);
      compare_movie_word(movie,director,actor);
      compare_actor_word(movie,actor);
      compare_director_word(movie,director);
      if(i!=1)
      printf("No such record\n");
    }
    else if(input_text == 'a'){
      scanf("%d",&input_num);
      i=delete_actor(actor,input_num);
      free(movie);
      free(director);
      free(actor);
      make_movie_admin(&movie);
      make_actor_admin(&actor);
      make_director_admin(&director);
      scan_movie_log(movie);
      scan_actor_log(actor);
      scan_director_log(director);
      compare_movie_word(movie,director,actor);
      compare_actor_word(movie,actor);
      compare_director_word(movie,director);
      if(i!=1)
      printf("No such record\n");
  }
  else if(input_text == 'd'){
    scanf("%d",&input_num);
    i=delete_director(director,input_num);
    free(movie);
    free(director);
    free(actor);
    make_movie_admin(&movie);
    make_actor_admin(&actor);
    make_director_admin(&director);
    scan_movie_log(movie);
    scan_actor_log(actor);
    scan_director_log(director);
    compare_movie_word(movie,director,actor);
    compare_actor_word(movie,actor);
    compare_director_word(movie,director);
    if(i!=1)
    printf("No such record\n");
  }
}
  else if(strcmp(command,"sort")==0){//파일의 내용을 정렬하여서 출력함
    scanf("%*c%c",&input_text);
    scanf("%*c%c",&option);
    char check = getchar();
    if(check == '\n'){
    sorting(movie,actor,director,input_text,option);
    free(movie);
    free(director);
    free(actor);
    make_movie_admin(&movie);
    make_actor_admin(&actor);
    make_director_admin(&director);
    scan_movie_log(movie);
    scan_actor_log(actor);
    scan_director_log(director);
    compare_movie_word(movie,director,actor);
    compare_actor_word(movie,actor);
    compare_director_word(movie,director);
  }
    else if(check ==' '){
      char checking;
      scanf("%*c%c",&checking);
      if(checking == 'f'){
      getchar();
      sorting_file(movie,actor,director,input_text,option);
      make_movie_admin(&movie);
      make_actor_admin(&actor);
      make_director_admin(&director);
      scan_movie_log(movie);
      scan_actor_log(actor);
      scan_director_log(director);
      compare_movie_word(movie,director,actor);
      compare_actor_word(movie,actor);
      compare_director_word(movie,director);
    }
    }
  }
  else if(strcmp(command,"print")==0){//파일의 내용을 출력함
    getchar();
    scanf("%c%*c",&input_text);
    if(input_text=='m'){
      scanf("%d",&input_num);
      i=print_movie_list(movie,input_num);
      if(i!=1){
        printf("No such record\n");
      }
    }
      else if(input_text =='a'){
        scanf("%d",&input_num);
        i=print_actor_list(actor,input_num);
        if(i!=1)
          printf("No such record\n");
      }
      else if(input_text =='d'){
        scanf("%d",&input_num);
        i=print_director_list(director,input_num);
        if(i!=1)
          printf("No such record\n");
      }
    }
  else if(strcmp(command,"search")==0){//키워드 검색으로 리스트 상에서 지정된 문자열을 찾아서 출력함
    getchar();
      checking = getchar();
      if(checking == '-'){
        scanf("%s%*c",input_data);
        scanf("%[^\n]%*c",string);
        i = strlen(input_data);
        for(int j = 0; j < i; j++){
          if(*(input_data + j) == 'm') *(check_input_data + 0) = 'm';
          if(*(input_data + j) == 'd') *(check_input_data + 1) = 'd';
          if(*(input_data + j) == 'a') *(check_input_data + 2) = 'a';
        }
        if(*(check_input_data + 0) == 'm')
          search_m(movie, string);
        if(*(check_input_data + 1) == 'd')
          search_d(director, string);
        if(*(check_input_data + 2) == 'a')
          search_a(actor, string);
      }
      else{
        scanf("%[^\n]%*c",string);
        *input_data = checking;
        strcat(input_data,string);
        search_m(movie, input_data);
        search_d(director, input_data);
        search_a(actor, input_data);
      }
    }
    else if(strcmp(command,"save")==0){//LIST파일에 내용을 저장함
      scanf("%*c%c",&input_text);
      checking = getchar();
      if(checking == '\n'){
        save(movie,actor,director,input_text);
      }
      else if(checking == ' '){
        scanf("%*c%s%*c",input_data);
        checking = getchar();
        if(checking == '-'){
          checking = getchar();
          if(checking == 'f'){
            save_file(movie,actor,director,input_text,input_data);
        }
        }
      }
  }
  else if(strcmp(command,"save")==0){//LIST파일에 내용을 저장함
    scanf("%*c%c",&input_text);
    checking = getchar();
    if(checking == '\n'){
      save(movie,actor,director,input_text);
    }
    else if(checking == ' '){
      scanf("%*c%s%*c",input_data);
      checking = getchar();
      if(checking == '-'){
        checking = getchar();
        if(checking == 'f'){
          save_file(movie,actor,director,input_text,input_data);
      }
      }
    }
  }
  else if(strcmp(command,"end")==0){//프로그램의 실행을 멈춘다
    //파일의 내용을 저장할 필요가 있으면 저장도 해주면 좋다
    copy_movie_list();
    copy_actor_list();
    copy_director_list();
    free(movie);
    free(actor);
    free(director);
    make_movie_admin(&movie);
    make_actor_admin(&actor);
    make_director_admin(&director);
    scan_movie_log(movie);
    scan_actor_log(actor);
    scan_director_log(director);
    exit(1);
  }
  }
}
void cntrl_c(int sig){//신호를 받아서 ctrl + C 를 통해 끄려고 할 때 끌지 말지 유무를 물어봄
  char *answer; //나중에 포인터로 바꾸기
  answer=(char *)calloc(100,sizeof(char));
  printf("\n프로그램을 종료하시겠습니까? (y/n)");
  scanf("%[^\n]%*c", answer);
  if(*answer =='y' || *answer == 'Y') //answer에 'y'나 'Y'가 들어올 경우 (다른 문자가 들어올 경우에는 프로그램을 계속 실행함)
    exit(1); //프로그램 종료
  }
  int copy_movie_list(){
   FILE *f1, *f2; //FILE로 f1은 "movie_list"파일을 읽기 위해서, f2는 "movie_list"파일을 참조하기 위해서 선언
   char *line; //fgets사용하기 위해서 선언
   struct tm *t; //<time.h>헤더파일에 있는 시간을 가져다 쓰기 위해서 구조체 tm선언
   time_t now;
   now = time(NULL);
   t = localtime(&now); //앞서 선언한 tm 구조체에서 로컬타임을 거져다가 쓰기 위해서 선언(교재 607쪽 수록)
   int y, m1, d, h, m2, s; //각각 년, 달, 일, 시간, 분, 초를 저장하기 위해서 선언
   char *filename; //movie_list201720202120 처럼 파일의 이름을 임의로 저장하기 위해서 파일이름을 변수로 선언
   line = (char *)calloc(1024,sizeof(char)); //위에서 선언한 *line을 동적할당
   filename = (char *)calloc(1024,sizeof(char)); //위의 임의의 파일 이름을 저장하기 위해서 동적할당
 f1 = fopen("movie_list.txt", "r"); //앞에서 선언한 f1을 movie_list이름의 파일로 읽기 위해서 열어준다.
 f2 = fopen("movie_list.txt", "a"); //앞에서 선언한 f2를 movie_list의 이름의 파일로 참조하기 위해서 열어준다.
 y = t -> tm_year+1900; // 구조체 tm안에 이미 저장되어 있는 년도를 가져다가 쓰고 y에 년도를 저장하기 위해서 선언
 m1 = t -> tm_mon;
 d = t -> tm_mday;
 h = t -> tm_hour;
 m2 = t -> tm_min;
 s = t -> tm_sec;
 sprintf(filename, "movie_list.%d%d%d%d%d.txt", t -> tm_year+1900, t -> tm_mon+1, t -> tm_hour, t -> tm_min, t -> tm_sec); //sprintf로 파일이름을 변수로 저장하기 위해서
 f2 = fopen(filename, "w"); //위에서 sprintf로 파일이름을 변수로 저장한 후 쓰기 위해서 filename을 열어준다


 while(fgets(line, len -1, f1)) //movie_ 파일을 파일의 끝까지 한줄씩 읽어서 filename 파일에 복사하기 위해서 선언 널을 만나면 line에 저장된다.
 {
   fputs(line, f2); // f2파일에 널로 끝나는 문자열 line을 복사한다.
 }
 fclose(f1); //movie_list파일을 닫아준다
 fclose(f2); //movie_list20170202012파일을 닫아준다.
 free(line); //동적할당해준 line을 free해 준다
 free(filename); //동적할당해준 filname을 free해 준다.
}
int copy_director_list(){
     FILE *f1, *f2; //FILE로 f1은 "director_list.txt"파일을 읽기 위해서, f2는 "director_list.txt"파일을 참조하기 위해서 선언
     char *line; //fgets사용하기 위해서 선언
     struct tm *t; //<time.h>헤더파일에 있는 시간을 가져다 쓰기 위해서 구조체 tm선언
     time_t now;
     now = time(NULL);
     t = localtime(&now); //앞서 선언한 tm 구조체에서 로컬타임을 거져다가 쓰기 위해서 선언(교재 607쪽 수록)
     int y, m1, d, h, m2, s; //각각 년, 달, 일, 시간, 분, 초를 저장하기 위해서 선언
     char *filename; //director_list201720202120 처럼 파일의 이름을 임의로 저장하기 위해서 파일이름을 변수로 선언
     line = (char *)calloc(1024,sizeof(char)); //위에서 선언한 *line을 동적할당
     filename = (char *)calloc(1024,sizeof(char)); //위의 임의의 파일 이름을 저장하기 위해서 동적할당
   f1 = fopen("director_list.txt", "r"); //앞에서 선언한 f1을 director_list이름의 파일로 읽기 위해서 열어준다.
   f2 = fopen("director_list.txt", "a"); //앞에서 선언한 f2를 director_list의 이름의 파일로 참조하기 위해서 열어준다.
   y = t -> tm_year+1900; // 구조체 tm안에 이미 저장되어 있는 년도를 가져다가 쓰고 y에 년도를 저장하기 위해서 선언
   m1 = t -> tm_mon;
   d = t -> tm_mday;
   h = t -> tm_hour;
   m2 = t -> tm_min;
   s = t -> tm_sec;
   sprintf(filename, "director_list%d%d%d%d%d.txt", t -> tm_year+1900, t -> tm_mon+1, t -> tm_hour, t -> tm_min, t -> tm_sec); //sprintf로 파일이름을 변수로 저장하기 위해서
   f2 = fopen(filename, "w"); //위에서 sprintf로 파일이름을 변수로 저장한 후 쓰기 위해서 filename을 열어준다


   while(fgets(line, len -1, f1)) //director_list 파일을 파일의 끝까지 한줄씩 읽어서 filename 파일에 복사하기 위해서 선언 널을 만나면 line에 저장된다.
   {
     fputs(line, f2); // f2파일에 널로 끝나는 문자열 line을 복사한다.
   }
   fclose(f1); //director_list파일을 닫아준다
   fclose(f2); //director_list20170202012파일을 닫아준다.
   free(line); //동적할당해준 line을 free해 준다
   free(filename); //동적할당해준 filname을 free해 준다.
   }

   int copy_actor_list(){
       FILE *f1, *f2; //FILE로 f1은 "actor_list"파일을 읽기 위해서, f2는 "actor_list"파일을 참조하기 위해서 선언
       char *line; //fgets사용하기 위해서 선언
       struct tm *t; //<time.h>헤더파일에 있는 시간을 가져다 쓰기 위해서 구조체 tm선언
       time_t now;
       now = time(NULL);
       t = localtime(&now); //앞서 선언한 tm 구조체에서 로컬타임을 거져다가 쓰기 위해서 선언(교재 607쪽 수록)
       int y, m1, d, h, m2, s; //각각 년, 달, 일, 시간, 분, 초를 저장하기 위해서 선언
       char *filename; //actor_list201720202120 처럼 파일의 이름을 임의로 저장하기 위해서 파일이름을 변수로 선언
       line = (char *)calloc(1024,sizeof(char)); //위에서 선언한 *line을 동적할당
       filename = (char *)calloc(1024,sizeof(char)); //위의 임의의 파일 이름을 저장하기 위해서 동적할당
     f1 = fopen("actor_list.txt", "r"); //앞에서 선언한 f1을 actor_list이름의 파일로 읽기 위해서 열어준다.
     f2 = fopen("actor_list.txt", "a"); //앞에서 선언한 f2를 actor_list의 이름의 파일로 참조하기 위해서 열어준다.
     y = t -> tm_year+1900; // 구조체 tm안에 이미 저장되어 있는 년도를 가져다가 쓰고 y에 년도를 저장하기 위해서 선언
     m1 = t -> tm_mon;
     d = t -> tm_mday;
     h = t -> tm_hour;
     m2 = t -> tm_min;
     s = t -> tm_sec;
     sprintf(filename, "actor_list%d%d%d%d%d.txt", t -> tm_year+1900, t -> tm_mon+1, t -> tm_hour, t -> tm_min, t -> tm_sec); //sprintf로 파일이름을 변수로 저장하기 위해서
     f2 = fopen(filename, "w"); //위에서 sprintf로 파일이름을 변수로 저장한 후 쓰기 위해서 filename을 열어준다


     while(fgets(line, len -1, f1)) //actor_list 파일을 파일의 끝까지 한줄씩 읽어서 filename 파일에 복사하기 위해서 선언 널을 만나면 line에 저장된다.
     {
       fputs(line, f2); // f2파일에 널로 끝나는 문자열 line을 복사한다.
     }
     fclose(f1); //actor_list파일을 닫아준다
     fclose(f2); //actor_list20170202012파일을 닫아준다.
     free(line); //동적할당해준 line을 free해 준다
     free(filename); //동적할당해준 filname을 free해 준다.
     }


    void compare_movie_word(ALL_M *movie,ALL_D *director,ALL_A *actor){
      LINK_M *com_movie,*link_movie;
      LINK_A *com_actor,*link_actor;
      LINK_D *com_director,*link_director;
      LINK_W *com_word,*last_word,*last;
      com_movie = movie -> head;
      link_movie = movie -> head;
      com_actor = actor -> head;
      com_director = director -> head;
      link_director = director -> head;
      link_actor = actor -> head;
      while(com_movie -> next_movie != movie -> tail){//무비 구조체 확인
        com_movie = com_movie -> next_movie;
        while(com_director -> next_director != director -> tail){//디렉터를 끝까지 보내
          com_director = com_director -> next_director;
          if(strcmp(com_movie -> director -> data,com_director -> name)==0){
            //무비의 영화 감독과 감독의 이름이 같을 때
            com_movie -> director -> com_word = com_director;
          }
        }
        com_director = link_director;
      }
      com_movie = movie -> head;
      while(com_movie -> next_movie != movie -> tail){
        com_movie = com_movie -> next_movie;
        com_word = com_movie -> actor;
        while(com_actor -> next_actor != actor -> tail){
          com_actor = com_actor -> next_actor;
          if(strcmp(com_word -> data,com_actor -> name)==0){
            com_word -> com_word = com_actor;
          }
        }
              com_actor = actor -> head;
              while(com_word -> next != NULL){
          last = com_word -> next;
          com_actor = actor -> head;
          while(com_actor -> next_actor != actor -> tail){
            com_actor = com_actor -> next_actor;
            last = com_word -> next;
            if(strcmp(com_word -> data,com_actor -> name)==0){
              com_word -> com_word = com_actor;
            }
            else if(strcmp(last -> data,com_actor -> name)==0){
              last -> com_word = com_actor;
            }
          }
          com_word = com_word -> next;
        }
      }
    }
    void compare_actor_word(ALL_M *movie,ALL_A *actor){
      LINK_M *com_movie;
      LINK_A *com_actor;
      LINK_W *com_word,*last;
      com_movie = movie -> head;
      com_actor = actor -> head;
      while(com_actor -> next_actor != actor -> tail){
        com_actor = com_actor -> next_actor;
        com_word = com_actor -> title;
        while(com_movie -> next_movie != movie -> tail){
          com_movie = com_movie -> next_movie;
          if(strcmp(com_word -> data,com_movie -> title)==0){
            com_word -> com_word = com_movie;
          }
        }
        com_movie = movie -> head;
        while(com_word -> next != NULL){
          last = com_word -> next;
          com_movie = movie -> head;
          while(com_movie -> next_movie != movie -> tail){
            com_movie = com_movie -> next_movie;
            last = com_word -> next;
            if(strcmp(com_word -> data,com_movie -> title)==0){
              com_word -> com_word = com_movie;
            }
            else if(strcmp(last -> data,com_movie -> title)==0){
              last -> com_word = com_movie;
            }
          }
          com_word = com_word -> next;
        }
      }
      }
    void compare_director_word(ALL_M *movie,ALL_D *director){
      LINK_M *com_movie;
      LINK_D *com_director;
      LINK_W *com_word,*last;
      com_movie = movie -> head;
      com_director = director -> head;
      while(com_director -> next_director != director -> tail){
        com_director = com_director -> next_director;
        com_word = com_director -> title;
        while(com_movie -> next_movie != movie -> tail){
          com_movie = com_movie -> next_movie;
          if(strcmp(com_word -> data,com_movie -> title)==0){
            com_word -> com_word = com_movie;
          }
        }
        com_movie = movie -> head;
        while(com_word -> next != NULL){
          last = com_word -> next;
          com_movie = movie -> head;
          while(com_movie -> next_movie != movie -> tail){
            com_movie = com_movie -> next_movie;
						last = com_word -> next;
            if(strcmp(com_word -> data,com_movie -> title)==0){
              com_word -> com_word = com_movie;
            }
            else if(strcmp(last -> data,com_movie -> title)==0){
              last -> com_word = com_movie;
            }
          }
          com_word = com_word -> next;
        }
      }
    }

int main(){
  signal(SIGINT, cntrl_c);
  ALL_M *movie;
  ALL_D *director;
  ALL_A *actor;
  make_movie_admin(&movie);
  make_actor_admin(&actor);
  make_director_admin(&director);
  printf(">> Welcome to My Movie <<\n");//프로그램의 시작을 알림
  printf("File Loading.....\n");
  commands(director,actor,movie);
  return 0;
}
