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
  f1 = fopen("movie_list", "r"); //앞에서 선언한 f1을 movie_list이름의 파일로 읽기 위해서 열어준다.
  f2 = fopen("movie_list", "a"); //앞에서 선언한 f2를 movie_list의 이름의 파일로 참조하기 위해서 열어준다.
  y = t -> tm_year+1900; // 구조체 tm안에 이미 저장되어 있는 년도를 가져다가 쓰고 y에 년도를 저장하기 위해서 선언
  m1 = t -> tm_mon;
  d = t -> tm_mday;
  h = t -> tm_hour;
  m2 = t -> tm_min;
  s = t -> tm_sec;
  sprintf(filename, "movie_list%d%d%d%d%d%d", t -> tm_year+1900, t -> tm_mon, t -> tm_mday, t -> tm_hour, t -> tm_min, t -> tm_sec); //sprintf로 파일이름을 변수로 저장하기 위해서
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
      FILE *f1, *f2; //FILE로 f1은 "director_list"파일을 읽기 위해서, f2는 "director_list"파일을 참조하기 위해서 선언
      char *line; //fgets사용하기 위해서 선언
      struct tm *t; //<time.h>헤더파일에 있는 시간을 가져다 쓰기 위해서 구조체 tm선언
      time_t now;
      now = time(NULL);
      t = localtime(&now); //앞서 선언한 tm 구조체에서 로컬타임을 거져다가 쓰기 위해서 선언(교재 607쪽 수록)
      int y, m1, d, h, m2, s; //각각 년, 달, 일, 시간, 분, 초를 저장하기 위해서 선언
      char *filename; //director_list201720202120 처럼 파일의 이름을 임의로 저장하기 위해서 파일이름을 변수로 선언
      line = (char *)calloc(1024,sizeof(char)); //위에서 선언한 *line을 동적할당
      filename = (char *)calloc(1024,sizeof(char)); //위의 임의의 파일 이름을 저장하기 위해서 동적할당
    f1 = fopen("director_list", "r"); //앞에서 선언한 f1을 director_list이름의 파일로 읽기 위해서 열어준다.
    f2 = fopen("director_list", "a"); //앞에서 선언한 f2를 director_list의 이름의 파일로 참조하기 위해서 열어준다.
    y = t -> tm_year+1900; // 구조체 tm안에 이미 저장되어 있는 년도를 가져다가 쓰고 y에 년도를 저장하기 위해서 선언
    m1 = t -> tm_mon;
    d = t -> tm_mday;
    h = t -> tm_hour;
    m2 = t -> tm_min;
    s = t -> tm_sec;
    sprintf(filename, "director_list%d%d%d%d%d%d", t -> tm_year+1900, t -> tm_mon, t -> tm_mday, t -> tm_hour, t -> tm_min, t -> tm_sec); //sprintf로 파일이름을 변수로 저장하기 위해서
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
      f1 = fopen("actor_list", "r"); //앞에서 선언한 f1을 actor_list이름의 파일로 읽기 위해서 열어준다.
      f2 = fopen("actor_list", "a"); //앞에서 선언한 f2를 actor_list의 이름의 파일로 참조하기 위해서 열어준다.
      y = t -> tm_year+1900; // 구조체 tm안에 이미 저장되어 있는 년도를 가져다가 쓰고 y에 년도를 저장하기 위해서 선언
      m1 = t -> tm_mon;
      d = t -> tm_mday;
      h = t -> tm_hour;
      m2 = t -> tm_min;
      s = t -> tm_sec;
      sprintf(filename, "actor_list%d%d%d%d%d%d", t -> tm_year+1900, t -> tm_mon, t -> tm_mday, t -> tm_hour, t -> tm_min, t -> tm_sec); //sprintf로 파일이름을 변수로 저장하기 위해서
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
