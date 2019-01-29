#ifndef DB_H
#define DB_H

#define TRUE 1
#define FALSE 0

typedef struct attendance_rec {
  int sno;
  char name[80];
  int present;
} attend_rec;

/* reset the database for testing */
int clear_db();

/*
  Story 1: As a teacher, I want a user interface for my database.
*/
char get_command(); // prompts for char; returns command
int execute_command(char c); // calls function for command; FALSE if 'x'

/*
  Story 2: As a teacher, I want to add a student to my database, so I
  can track his or her record.
*/
void printf_attend(attend_rec r); // Print a student record
void scanf_attend(attend_rec *r); // Read a student record
int add(attend_rec r); // add a record to the database

/*
  Story 3: As a teacher, I want to be see all of the students in my
  class, so I can see who is present and absent.
 */
void list();

/* 
   Story 4: As a programmer, I want to add a set of records to my
   database for testing purposes.
*/
void initialize_db();

/*
  Story5: As a teacher, I want to be find a student's record so I can
  see whether a student was present or absent for a class.
*/
int get_sno(); // gets a serial number;
attend_rec find(int sno); // Attendence record for student sno

/*
  Story 6: As a teacher, I want to modify a student's record, so I can
  correct mistakes.
*/
int modify(attend_rec r); // Change record; return FALSE if not found

/*
  Story 7: As a teacher, I want to delete a student's record, so I can
  drop a student from the class.
*/
int find_index(int sno); // index of attendence record for student sno
int delete(int sno); // Delete record sno; return FALSE if not found

extern attend_rec not_found;

#endif
