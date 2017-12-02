//이 함수들은 서로 똑같은거 노드하는 함수인데 아직 컴파일을 못해봤음
//이유는 아직 movie 종류만 만들어놔서 다른 종류를 만들어보고 이걸 컴파일 해야될 것 같음
//원래 올린거랑 다른 점은 LINK_M 구조체에서 director 멤버를 DIR구조체로 다시 선언해 준 부분이
//다른 점이고 그 부분은 앞에 올린 내용에서도 수정해야함(어려운 부분은 아님)
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
void compare_movie_word(LINK_M *movie,LINK_D *director,LINK_A *actor){
  LINK_M *com_movie;
  LINK_A *com actor;
  LINK_D *com_director;
  LINK_W *com_word;
  com_movie = movie;
  com_actor = actor;
  com_director = director;
  while(com_movie -> next_movie != NULL){
    com_movie = com_movie -> next_movie;
    while(com_director -> next_director != NULL){
      com_director = com_director -> next_director;
      if(strcmp(com_movie -> director -> data,com_director -> name)==0){
        //무비의 영화 감독과 감독의 이름이 같을 때
        com_movie -> director -> com_word = com_director;
      }
    }
      while(com_actor -> next_actor != NULL){
        com_actor = com_actor -> next_actor;
        while(com_movie -> actor -> next != NULL){
          if(strcmp(com_actor -> name, com_movie -> actor -> data)==0){
            //무비의 배우 이름과 배우의 이름이 같을 때
            com_movie -> actor -> com_word = com_actor;
          }
          com_movie -> actor = com_movie -> actor -> next;
        }
      }
  }
}
void compare_actor_word(LINK_M *movie,LINK_A *actor){
  LINK_M *com_movie;
  LINK_A *com_actor;
  com_movie = movie;
  com_actor = actor;
  while(com_actor -> next_actor != NULL){
    com_actor = com_actor -> next_actor;
    while(com_movie -> next_movie != NULL){
      com_movie = com_movie -> next_movie;
      while(com_actor -> title -> next != NULL){
        if(strcmp(com_actor -> title -> data,com_movie -> title)==0){
          com_actor -> title -> com_word = com_movie;
        }
        com_actor -> title = com_actor -> title -> next;
      }
    }
  }
}
void compare_director_word(LINK_M *movie,LINK_D *director){
  LINK_M *com_movie;
  LINK_D *com_director;
  com_movie = movie;
  com_director = director;
  while(com_director -> next_actor != NULL){
    com_director = com_director -> next_actor;
    while(com_movie -> next_movie != NULL){
      com_movie = com_movie -> next_movie;
      while(com_director -> title -> next != NULL){
        if(strcmp(com_director -> title -> data,com_movie -> title)==0){
          com_director -> title -> com_word = com_movie;
        }
        com_director -> title = com_director -> title -> next;
      }
    }
  }
}
