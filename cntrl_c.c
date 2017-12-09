2240 :  signal(SIGINT, cntrl_c); // cntrl_c를 실행할 때 y/n를 묻기 위해서 선언


void cntrl_c(int sig){//신호를 받아서 ctrl + C 를 통해 끄려고 할 때 끌지 말지 유무를 물어봄
  char *answer;
  answer=(char *)calloc(100,sizeof(char)); //위의 *answer을 동적할당
  printf("\n프로그램을 종료하시겠습니까? (y/n)");
  scanf("%[^\n]%*c", answer); //enter 전까지 answer에 받겠다
  if(*answer =='y' || *answer == 'Y') //answer가 y 나 Y일때에 exi(1)한다.
    exit(1);
  }
