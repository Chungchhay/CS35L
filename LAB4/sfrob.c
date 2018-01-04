#include <stdio.h>
#include <stdlib.h>

int frobcmp(char const *a, char const *b);
int compare(const void* read1, const void* read2);
void readError();

int main() {
  char **arr = (char**)malloc(sizeof(char*));
  //Hold strings of array
  char *word = (char*)malloc(sizeof(char));
  //Hold a string
  char currChar1 = getchar();
  readError();
  int countChar = 0;
  int countWord = 0;
  
  while(currChar1 != EOF && !ferror(stdin)) //Read file until EOF
  {
    char currChar2 = getchar();
    word[countChar] = currChar1; //add a char to word
    char *temp = realloc(word, (countChar + 2) * sizeof(char));
    //Reallocate the size of the word
    
    if (temp == NULL)
    {
      free(word);
      fprintf(stderr, "Error Allocation Memory!");
      exit(1);
    }
    
    word = temp;
    
    if (currChar1 == ' ')
    {
      arr[countWord] = word;
      char **temp1 = realloc(arr, (countWord + 2) * sizeof(char*));
      
      if (arr == NULL)
      {
        free(arr);
        fprintf(stderr, "Error Allocation Memory!");
        exit(1);
      }
      arr = temp1;
      countWord++;
      
      word = NULL;
      word = (char*)malloc(sizeof(char));
      countChar = -1;
    }
    
    if (currChar2 == EOF)
    {
      if (currChar1 == ' ')
      {
        //if reach end of the file and have a space I exit
        break;
      }
      
      //If reach end of the file and have no space I add a space at the end
      currChar1 = ' ';
      countChar++;
      continue;
    }
    else if (currChar1 == ' ' && currChar2 == ' ')
    {
      while (currChar1 == ' ')
      {
        currChar1 = getchar();
        readError();
      }
      
      currChar2 = getchar();
      readError();
      countChar++;
      continue;
    }
    else if (currChar2 == '\1' || currChar2 == '\2' || currChar2 == '\3'
             || currChar2 == '\4' || currChar2 == '\5' || currChar2 == '\6'
             || currChar2 == '\7')
    {
      countChar++;
      word[countChar] = 'e';
      char *temp = realloc(word, (countChar + 3) * sizeof(char));
      word = temp;
      countChar++;
      word[countChar] = 'o';
      countChar++;
      word[countChar] = 't';
      arr[countWord] = word;
      currChar2 = getchar();
      
      if (currChar2 == EOF)
      {
        if (currChar1 == ' ')
          break;
        
        currChar1 = ' ';
        countChar++;
        continue;
      }
    }
    
    currChar1 = currChar2;
    countChar++;
  }
  
  //printf("The size of countword %d\n", countWord);
  qsort(arr, countWord, sizeof(char*), compare);
  size_t i, j;
  
  for(i= 0; i < countWord; i++)
  {
    for(j = 0; ;j++)
    {
      //check if I read the end of the file
      if(putchar(arr[i][j]) == EOF)
      {
        fprintf(stderr, "Error while writing character!");
        exit(1);
      }
      
      if(arr[i][j] == ' ')
      {
        break;
      }
    }
  }
  //Free the memory locations
  for(i = 0; i < countWord; i++)
  {
    free(arr[i]);
  }
  free(arr);
  
  return 0;
}

void readError()
{
  if(ferror(stdin))
  {
    fprintf(stderr, "Error reading the file!");
    exit(1);
  }
}

int compare(const void* read1, const void* read2)
{
  char* a = *(char **)read1;
  char* b = *(char **)read2;
  return frobcmp(a, b);
}

int frobcmp(char const *a, char const *b)
{
  for(;; a++, b++)
  {
    if(*a == ' ' && *b == ' ') { return 0; }
    else if (*a == ' ' || ((*a^42) < (*b^42))) { return -1; }
    else if (*b == ' ' || ((*a^42) > (*b^42))) { return 1; }
  }
}