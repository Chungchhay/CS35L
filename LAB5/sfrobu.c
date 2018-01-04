#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <sys/stat.h>


int fOption = 0;


int frobcmp(char const *a, char const *b)
{
  if(fOption == 0) {
    for(;; a++, b++) {
      if(*a == ' ' && *b == ' ') { return 0; }
      else if (*a == ' ' || ((*a^42) < (*b^42))) { return -1; }
      else if (*b == ' ' || ((*a^42) > (*b^42))) { return 1; }
    }
  } else {
    for(;; a++, b++) {
      if(*a <= UCHAR_MAX && *b <= UCHAR_MAX) {
	if(*a == ' ' && *b == ' ') { return 0; }
	else if(*a == ' ' || (toupper(*a^42) < toupper(*b^42))) { return -1; }
	else if(*b == ' ' || (toupper(*a^42) > toupper(*b^42))) { return 1; }
      }

      if(*a <= UCHAR_MAX && *b > UCHAR_MAX) {
	if(*a == ' ' && *b == ' ') { return 0; }
	else if(*a == ' ' || (toupper(*a^42) < (*b^42))) { return -1; }
	else if(*b == ' ' || (toupper(*a^42) > (*b^42))) { return 1; }
      }

      if(*a > UCHAR_MAX && *b <= UCHAR_MAX) {
	if(*a == ' ' && *b == ' ') { return 0; }
	else if(*a == ' ' || ((*a^42) < toupper(*b^42))) { return -1; }
	else if(*b == ' ' || ((*a^42) > toupper(*b^42))) { return 1; }
      }
    }
  }
  
}

int cmp(const void* in1, const void* in2)
{
  const char* a = *(const char**)in1;
  const char* b = *(const char**)in2;
  return frobcmp(a,b);
}

void readError(ssize_t state)
{
  if(state < 0)
    {
      fprintf(stderr, "Error while reading file!");
      exit(1);
    }
}

int main(int argc, char* argv[]) {

  //Check if it passed the -f 
  if(argc >= 2 && strcmp(argv[1], "-f") == 0) {
    fOption = 1;
  }

  
  struct stat fileStat;
  if(fstat(0,&fileStat) < 0)
    {
      fprintf(stderr, "Error with file stat!");
      exit(1);
    }
  
  char **words; //hold an array of string
  char *arrayFile; // hold name of file
  size_t wordsIterator = 0;
  
  if(S_ISREG(fileStat.st_mode))
    {
      arrayFile = (char*)malloc(fileStat.st_size * sizeof(char));
    
      int counter = 0;
    
      ssize_t s = read(0, arrayFile, fileStat.st_size);
    
      int count = 1;
      if(s > 0)
	{
	  size_t i;
	  for(i = 0; i < fileStat.st_size; i+=count)
	    {
	      count = 1;
	      if(i == fileStat.st_size - 1)
		{
		  arrayFile[i] = ' ';
		}
	      if(arrayFile[i] == ' ')
		{
		  size_t j;
		  for(j = i; arrayFile[j] != ' '; j++)
		    {
		      count++;
		    }
		  counter++;
		}
	    }
	}

      words = (char**)malloc(counter * sizeof(char*));
      int flag = 0;
      size_t i;
      for(i = 0; i < fileStat.st_size; i++)
	{
	  if(flag == 0 && arrayFile[i] != ' ')
	    {
	      words[wordsIterator] = &arrayFile[i];
	      wordsIterator++;
	      flag = 1;
	    }
	  else if(flag == 1 && arrayFile[i] == ' ')
	    {
	      flag = 0;
	    }
	}
    }
  else
    {
      words = (char**)malloc(sizeof(char*));
    }


  char* word; //hold a string
  word = (char*)malloc(sizeof(char));
  
  char curr[1];
  ssize_t currS = read(0, curr, 1);
  readError(currS);
  char next[1];
  ssize_t nextS = read(0, next, 1);
  readError(nextS);
  int letterIterator = 0;

  while(currS > 0) {
    word[letterIterator] = curr[0];
    char* temp = realloc(word, (letterIterator+2)*sizeof(char));
    if(temp != NULL)
      {
	word = temp;
      }
    else
      {
	free(word);
	fprintf(stderr, "Error Allocation Memory!");
	exit(1);
      }

    if(curr[0] == ' ')
      {
	words[wordsIterator] = word;
	char** anotherOne = realloc(words, (wordsIterator+2)*sizeof(char*));
	if(anotherOne != NULL)
	  {
	    words = anotherOne;
	    wordsIterator++;
	    word = NULL;
	    word = (char*)malloc(sizeof(char));
	    letterIterator = -1;
	  }
	else
	  {
	    free(words);
	    fprintf(stderr, "Error Allocation Memory!");
	    exit(1);
	  }
      }
    if(nextS == 0 && curr[0] == ' ')
      {
	break;
      }
    else if (curr[0] == ' ' && next[0] == ' ')
      {
	while(curr[0] == ' ') {
	  currS = read(0,curr,1);
	  readError(currS);
	}
	nextS = read(0,next,1);
	readError(nextS);
	letterIterator++;
	continue;
      }
    else if(nextS == 0)
      {
	curr[0] = ' ';
	letterIterator++;
	continue;
      }
    
    curr[0] = next[0];
    nextS = read(0, next, 1);
    readError(nextS);
    letterIterator++;
  }
  //Sort words
  qsort(words, wordsIterator, sizeof(char*), cmp);
  
  //free memory
  size_t i, j;
  for(i = 0; i < wordsIterator; i++) {
    long wordSize = 0;
    for(j = 0; ;j++) {
      wordSize++;
      if(words[i][j] == ' ') {
	break;
      }
    }
    if(write(1,words[i], wordSize) == 0) {
      fprintf(stderr, "Error while writing!");
      exit(1);
    }
  }
  free(words);
  exit(0);
}
