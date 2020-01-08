// CreateDate.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>

unsigned int bintobcd(unsigned int a)
{
  unsigned int r= 0;
  if (a>100000000) return 0xffffffff;
  while (a>=10000000) r+= 0x10000000, a-=10000000;
  while (a>=1000000) r+= 0x1000000, a-=1000000;
  while (a>=100000) r+= 0x100000, a-=100000;
  while (a>=10000) r+= 0x10000, a-=10000;
  while (a>=1000) r+= 0x1000, a-=1000;
  while (a>=100) r+= 0x100, a-=100;
  while (a>=10) r+= 0x10, a-=10;
  return r+a;
}

int main(int argc, char* argv[])
{
  if (argc!=3) { printf("error: params are: output_filename #include_filename\r\n"); return -1; } // error
  int size=143-7+(int)strlen(argv[2]);
  if (size>=1000) { printf("#include filename too long\r\n"); return -1; }
  SYSTEMTIME s;
  char old[1000], news[1000];
  HANDLE h= CreateFileA(argv[1], GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
  old[0]= 0;
  if (h>0)
  {
    DWORD r;
    ReadFile(h, old, size, &r, NULL);
    if (r!=size) old[0]= 0; else old[size]= 0; 
    CloseHandle(h);
  }
  
  GetLocalTime(&s);
  unsigned long long int t= bintobcd(s.wMonth*1000000+s.wDay*10000+s.wYear);
  t<<= 32;
  if (s.wMonth>=10) t= (t>>4)+1;
  t+= 0x7000000000000000LL;
  sprintf(news, "#include \"%s\"\r\nunsigned long long const BuildDate\r\n#ifdef _ARM_\r\n#pragma section = \"version\"\r\n@ \"version\"\r\n#endif\r\n= 0x%08x%08x;\r\n", argv[2], (unsigned int)(t>>32), (unsigned int)t);
  if (strcmp(old, news)!=0)
  {
    HANDLE h= CreateFileA(argv[1], GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
    DWORD r;
    WriteFile(h, news, size, &r, NULL);
    CloseHandle(h);
  }
	return 0;
}

