#include <stdio.h>
#include <string.h>
#include "db.h"

int main()
{
  attend_rec a;

  clear_db();

  printf("************* Database Test ******************\n");
  printf("Testing find(sno):\n");
  a = find(11);
  if (-1 == a.present) {
    printf("Student 11 not found\n");
  } else {
    printf ("Found student 11!!!\n");
  }
  printf("\n");
  
  printf("Testing add(rec):\n");
  a.sno = 11;
  strcpy(a.name, "Alice");
  a.present = TRUE;
  add(a);
  a = find(11);
  printf_attend(a);

  printf("Testing list():\n");
  list();

  printf("Testing initialize_db():\n");
  initialize_db();
  list();

  printf("Testing modify(rec):\n");
  strcpy(a.name, "Alice TestUser");
  a.present = FALSE;
  modify(a);
  a = find(11);
  printf_attend(a);
  list();
  

  printf("Testing delete(sno):\n");
  delete(11);
  a = find(11);
  printf_attend(a);
  list();

  return 0;
}
