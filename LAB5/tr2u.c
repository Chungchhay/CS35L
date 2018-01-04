#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, const char * argv[]) {
  // Check if argv has 3 arguments
  if (argc != 3)
    {
      printf("Argument is invalid");
      exit(1);
    }
  
  const char *from = argv[1];
  const char *to = argv[2];
  
  // Check if they both have the same length
  size_t length1 = strlen(argv[1]);
  size_t length2 = strlen(argv[2]);
  if (length1 != length2)
    {
      printf("Arguments have to be the same length");
      exit(1);
    }
  
  // Check if it has a duplicate character
  size_t i;
  size_t j;
  for (i = 0; i < strlen(from); i++)
    {
      for (j = i + 1; j < strlen(from); j++)
	{
	  if (from[i] == from[j])
	    {
	      printf("Characters must not be duplicated");
	      exit(1);
	    }
	}
    }
  
  // Replaced character
  char curr[1];
  size_t temp = read(0, curr, 1);
  
  while (temp > 0)
    {
      size_t i;
      for (i = 0; i < strlen(from); i++)
	{
	  if (curr[0] == from[i])
	    break;
	}
    
      if (from[i] == curr[0])
	{
	  curr[0] = to[i];
	  write(1, curr, 1);
	}
      else
	write(1, curr, 1);
    
      temp = read(0, curr, 1);
    }
  return 0;
}
