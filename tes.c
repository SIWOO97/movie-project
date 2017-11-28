#include<stdio.h>
#include<stdlib.h>
#include <time.h>
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
  int y, m, d, h, m, s, time;
f1 = fopen("input.txt", "r");
f2 = fopen("output.txt", "w");
y = t -> tm_year+1900;
m = t -> tm_mon; 
d = t -> tm_mday;
h = t -> tm_hour;
m = t -> tm_min;
s = t -> tm_sec;
time = y*10000000000 + m*100000000 + d*1000000 + h*10000 + m*100 +s;

//파일열기
f2 = fopen("output.txt", "w");

//파일 한라인씩 읽고 복사 fgets 교과서에 나와있는데 프로젝트에 사용가능한지 모름
while(fgets(line, len -1, f1))
{
  fputs(line, f2);
}

printf("FILE COPY COMP\n");

fclose(f1);
fclose(f2);
}
