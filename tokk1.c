#include <stdio.h>
#include <string.h>
#include <stdlib.h>

 char *token;   //명령어 저장하는 토큰
 char *sector; //토큰 구분의 기준 sector
 char *menu;   //명령 담는 포인터
 char *factor;   //m, d, a 저장
 char *option;   //옵션 저장
 char *option1; //옵션의 -를 빼주기 위해 선언
 char *check_option; //옵션 구분 위해서 선언
 char *file_name; //파일이름 저장 포인

int menu_func(char *input) {
  sector = (char *)calloc(sizeof(char), 50);
  menu = (char *)calloc(sizeof(char), 50);
  factor = (char *)calloc(sizeof(char), 50);
  option = (char *)calloc(sizeof(char), 50);
  option1 = (char *)calloc(sizeof(char), 50);
  check_option = (char *)calloc(sizeof(char), 50);
  file_name = (char *)calloc(sizeof(char), 50);

  sector = " ";

   token = strtok(input, sector);
   strcpy(menu, token);
   printf("menu : %s\n", menu);

   if (strcmp(menu, "save") == 0) {
      token = strtok(NULL, sector);
      strcpy(factor, token);
      printf("factor : %s\n", factor);   //factor 확인


      if ((token = strtok(NULL, sector)) != NULL) {
         if (strcmp(token, "-f") == 0) {
            token = strtok(NULL, sector);
            strcpy(file_name, token);
            printf("file_name : %s\n", file_name);   //file_name 확인


         }
         else {
            strcpy(option, token);
            printf("option : %s\n", option); //옵션 확인
            option1 = (option+1);
            printf("option1 : %s\n", option1);   //option 확인

            if ((token = strtok(NULL, sector)) != NULL) {
               token = strtok(NULL, sector);
               strcpy(file_name, token);
            }
            if(*(factor) == 'm'){
            for(int i=0; i<check; i++){
               		if(*(option1+i) == 't'){
                    *(check_option+0) = 't';
                  }
                  if(*(option1+i) == 'g'){
                    *(check_option+1) = 'g';
                  }
                  if(*(option1+i) == 'd'){
                      *(check_option+2) = 'd';
                  }
                  if(*(option1+i) == 'y'){
                    *(check_option+3) = 'y';
                  }
                  if(*(option1+i) == 'r'){
                    *(check_option+4) = 'r';
                  }
               		if(*(option1+i) == 'a'){
                     *(check_option+5) = 'a';
                   }
                 }
  fprintf(    //파일 넣어주기     );
	int res=-1;
	int k=0;
	for(int i=0; i<check; i++);
  	if(res != -1)
    	k = res+1;

		for(int j=k; j<6; j++)
    if(*(check_option+j) != '\0'){
      res = j;
      break;
    }
    if(res == 0){
      if(*(check_option+0) == 't'){
//여기다 넣음



      }
      else{

//여기다 삽입
}
else if(res == 1){
  if(*(check_option+1) == 'g');{



  }
}
else{



}

else if(res == 2){
  if(*(check_option+2) == 'd');{



  }
else{



}
else
}

else if(res == 3){
  if(*(check_option+3) == 'y');{



  }
  else{



  }
  else
}

else if(res == 4){
  if(*(check_option+4) == 'y');{



  }
  else{



  }
  else
}

else if(res == 5){
  if(*(check_option+5) == 'r'){



  }
  else{



  }
  else
}

else if(res == 6){
  if(*(check_option+6) == 'a'){




  }
  else{



  }
  else
}

}
}

else if(*(factor) == 'd'){
  for(int i=0; i<check; i++){
    if(*(option1+i) == 'n'){
      *(check_option+0) = 'n';
}
if(*(option1+i) == 's'){
  *(check_option+1) = 's';
}
if(*option1+i) == 'b'){
  *(check_option+2) = 'b';
}
if(*(option1+i) == 'm'){
  *(checl_option+3) = 'm';
}
}
fprintf(    //파일 넣어주기     );
int res=-1;
int k=0;
for(int i=0; i<check; i++);
  if(res != -1)
    k = res+1;

  for(int j=k; j<6; j++)
  if(*(check_option+j) != '\0'){
    res = j;
    break;
  }
  if(res == 0){
    if(*(check_option+0) == 'n'){
//여기다 넣음



    }
    else{

//여기다 삽입
}
else if(res == 1){
  if(*check_option+1) == 's'){



  }
  else{



  }
}

else if(res == 2){
  if(*check_option+2) == 'b'){



  }
  else{



  }
}

else if(res == 3){
  if(*check_option+3) == 'm'){




  }
  else{




  }
}
}

else if(*factor) == 'a'){
    for(int i=0; i<check; i++){
      if(*(option1+i) == 'n'){
        *(check_option+0) = 'n';
  }
  if(*(option1+i) == 's'){
    *(check_option+1) = 's';
  }
  if(*option1+i) == 'b'){
    *(check_option+2) = 'b';
  }
  if(*(option1+i) == 'm'){
    *(checl_option+3) = 'm';
  }
}

    fprintf(    //파일 넣어주기     );
    int res=-1;
    int k=0;
    for(int i=0; i<check; i++);
      if(res != -1)
        k = res+1;

      for(int j=k; j<6; j++)
      if(*(check_option+j) != '\0'){
        res = j;
        break;
      }
      if(res == 0){
        if(*(check_option+0) == 'n'){
    //여기다 넣음



        }
        else{

    //여기다 삽입
    }
  }
    else if(res == 1){
      if(*check_option+1) == 's'){



      }
      else{



      }
    }

    else if(res == 2){
      if(*check_option+2) == 'b'){



      }
      else{



      }
    }

    else if(res == 3){
      if(*check_option+3) == 'm'){




      }
      else{




      }
    }
  }













         }
      }
   }
}

int main(void) {
   char *input;
   input = (char *)malloc(sizeof(char) * 100);

   while (input != NULL) {
      printf("(movie) ");
      gets(input);
      menu_func(input);


   }

   return 0;
 }
