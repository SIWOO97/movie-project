#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#define len 80

int main()
{
  FILE *f1, *f2;
  char line[len];
  char c;
  struct tm *t;
  time_t now;
  now = time(NULL);
  t = localtime(&now);
  int y, m1, d, h, m2, s;
  char filename[100];
f1 = fopen("movie_log", "r");
f2 = fopen("movie_list", "w");
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

printf("FILE COPY COMP\n");

fclose(f1);
fclose(f2);
}
