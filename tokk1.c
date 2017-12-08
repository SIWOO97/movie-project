#include <stdio.h>
#include <string.h>
#include <stdlib.h>

 char *token;   //명령어 쪼개서 저장하는 토큰
 //char *temp;   //input받는 임시 변수, input을 바꾸는 사태가 일어나지 않게 해줌
 char *menu;   //명령 부분을 담는 포인터(예: update)
 char *factor;   //m, d, a 저장
 char *option;   //옵션 저장
 char *file_name;
int menu_func(char *input) {
  menu = (char *)calloc(sizeof(char), 50);
  option = (char *)calloc(sizeof(char), 50);
  factor = (char *)calloc(sizeof(char), 50);
//  temp = (char *)calloc(sizeof(char), 50);
  file_name = (char *)calloc(sizeof(char), 50);
  file_name = (char *)calloc(sizeof(char), 50);

//   strcpy(temp, input);   //temp에 input 복사

   token = strtok(input, " ");   //어차피 sort같은 menu는 항상 올테니 위에서부터 쪼개줌
   strcpy(menu, token);
   printf("menu : %s\n", menu);

   if (strcmp(menu, "save") == 0) {   //menu가 sort이면
      token = strtok(NULL, " ");   //한번 더 쪼개서 m, d, a중에 하나인걸 확인
      strcpy(factor, token);
      printf("factor : %s\n", factor);   //factor 확인

      if ((token = strtok(NULL, " ")) != NULL) {   //뒤에 뭐가 더 있는지 확인, 없다면 끝냄
         if (strcmp(token, "-f") == 0) {   //뒤에 있는게 -f이면 옵션없이 파일이름이 있는 것이니
            token = strtok(NULL, " ");   //파일 이름을 받기 위해서 -f 건너뛰어서 파일이름 token에 저장
            strcpy(file_name, token);
            printf("file_name : %s\n", file_name);   //file_name 확인
         }
         else {   //뒤에 있는게 옵션이면
            strcpy(option, token);
            printf("option : %s\n", option);   //option 확인
            if ((token = strtok(NULL, " ")) != NULL) {   //뒤에 뭐가 더 있는지 확인
               token = strtok(NULL, " ");   //있다면 -f일테니 건너뛰기
               strcpy(file_name, token);
               printf("file_name : %s\n", file_name);   //file_name 확인
            }
         }
      }
   }
}

int main(void) {
   char *input;
   input = (char *)malloc(sizeof(char) * 100);

   while (input != NULL) {
      printf("\nex: save m -tdy -f my_list\n");
      printf("(movie) ");
      gets(input);
      menu_func(input);


   }

   return 0;
 }
