#include <stdio.h>      /* printf, scanf, puts */
#include <stdlib.h>     /* realloc, free, exit, NULL */


int main ()
{
  int input,n;
  int count = 0;
  int* numbers = NULL;
  int* more_numbers = NULL;

     printf ("Enter an integer value : ");
     scanf ("%d", &input);
  do {
     count++;

     more_numbers = (int*) realloc (numbers, count * sizeof(int) + 1024);
	 if ( more_numbers != numbers )
        printf ("input %d count %d more_numbers %p numbers %p\n",input,count,more_numbers ,numbers);
     //scanf ("%d", &input);
     if (more_numbers!=NULL) {
       numbers=more_numbers;
       numbers[count-1]=input;
     }
     else {
       free (numbers);
       puts ("Error (re)allocating memory");
       exit (1);
     }
  } while (input!=count);
#if 0
  printf ("Numbers entered(%d): ",count);
  for (n=0;n<count;n++) printf ("%d ",numbers[n]);
#endif
  free (numbers);
  printf ("numbers  1 %p\n ",numbers);
  free (numbers);
  printf ("numbers  2 %p\n ",numbers);
  return 0;
}
