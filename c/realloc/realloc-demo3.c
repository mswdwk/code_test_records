#include <stdio.h>      /* printf, scanf, puts */
#include <stdlib.h>     /* realloc, free, exit, NULL */

int main ()
{
  int input,n;
  int count = 0;
  int* numbers = NULL;
  //int* more_numbers = NULL;

  do {
     printf ("Enter an integer value (0 to end): ");
     scanf ("%d", &input);
     count++;

     //more_numbers = (int*) realloc (numbers, count * sizeof(int));
	int i = 0;
	for(;i<input;i++)
     numbers = (int*) realloc (numbers, 256* count  );
	printf("count=%d\n",count);
     if (numbers!=NULL) {
       //numbers=more_numbers;
       numbers[count-1]=input;
     }
     else {
       free (numbers);
       puts ("Error (re)allocating memory");
       exit (1);
     }
  } while (input!=0);

  printf ("Numbers entered(%d): ",count);
  for (n=0;n<count;n++) printf ("%d ",numbers[n]);
  free (numbers);

  return 0;
}
