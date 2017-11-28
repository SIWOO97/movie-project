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

struct multiple{
	char data[20];
	struct multiple *next;
};

typedef struct linked_movie{
  int serial_num;
  char *title;
  char *genre;
  char *director;
  int year;
  int time;
  struct multiple *actor;
  struct linked_movie *next_movie;
}LINK_M;
void input_multiple(LINK_M *movie){
	movie->actor = (struct multiple*)malloc(sizeof(struct multiple));
	struct multiple *tmp;
	tmp = movie->actor;
	char temp;
	tmp=(struct multiple*)malloc(sizeof(struct multiple));
	while(1){
		scanf("%[^,\n]", tmp->data);
		temp = getchar();
		if(temp == '\n') break;
		else{
			temp = getchar();
			tmp -> next = (struct multiple*)malloc(sizeof(struct multiple));
			tmp = tmp->next;
			tmp -> next = NULL;
		}
	}
}

void print_multiple_data_log_to_list(FILE *mv_l, LINK_M *movie){
	struct multiple *tmp;
	tmp = movie->actor;
	while(tmp->next != NULL){
		fprintf(mv_l,"%s\n",tmp->data);
		tmp = tmp->next;
	}
}

void input_movie(LINK_M *movie){
  char input_text;
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
  input_multiple(movie);
  print_multiple_data_log_to_list(mv_l, movie);
  
  fclose(mv_l);
}

int main(){
  LINK_M *movie;
  printf(">> Welcome to My Movie <<\n");//프로그램의 시작을 알림
  printf("File Loading.....\n");
  printf("You can use add, update, delete, search, sort, save, end commands.\n");//사용 가능한 명령어를 보여줌
  
  input_movie(movie);
  return 0;
}
