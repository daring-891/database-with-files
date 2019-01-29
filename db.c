
#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include "db.h"

#define TRUE 1
#define FALSE 0
#define DB_FILE "test.bin"

const size_t rec_size = sizeof(attend_rec);

/*
  Story 1: As a teacher, I want a user interface for my database.
*/

int is_whitespace(char c) {
  return (' ' == c || '\t' == c || '\n' == c);
}

char get_first_non_white(char *s) {
  for (int i = 0; '\0' != s[i] && i < 80; i++) {
    if (!is_whitespace(s[i])) {
      return(s[i]);
    }
  }
  return('\0');
}

char get_command()
{
  char buffer[80];

  printf("Enter operator> ");
  scanf("%80s", buffer);
  return get_first_non_white(buffer);
}

// Returns FALSE when 'x' enter; TRUE otherwise
int execute_command(char c)
{
  int sno;
  attend_rec ar;

  switch (c) {
  case 'a':
    scanf_attend(&ar);
    add(ar);
    printf_attend(ar);
    return TRUE;
  case 'l':
    list();
    return TRUE;
  case 'i':
    initialize_db();
    return TRUE;
  case 'c':
    clear_db();
    return TRUE;
  case 'f':
    sno = get_sno();
    ar = find(sno);
    if (ar.sno == -1) {
      printf("Student %d not found\n", sno);
    } else {
      printf_attend(ar);
    }
    return TRUE;
  case 'm':
    scanf_attend(&ar);
    if (!modify(ar)) {
      printf("Modify failed on: ");
      printf_attend(ar);
    }
    return TRUE;
  case 'd':
    sno = get_sno();
    if (!delete(sno)) {
      printf("Delete failed on: ");
      printf_attend(ar);
    }
    return TRUE;
  case 'x':
    return FALSE;
  default:
    printf("Invalid command\n");
    return TRUE;
  }
}

/*
  Story 2: As a teacher, I want to add a student to my database, so I
  can track his or her record.
*/

void scanf_attend(attend_rec *r)
{
	int i=0;
	printf("Enter serial number>");
	scanf("%d",&r->sno);
	printf("Enter name>");
	scanf("%s",r->name);
	printf("Enter attendance>");
	scanf("%d",&r->present);
	while(r->present!=1&& r->present!=0&&i<10)
	{
		printf("wrong input try again");
		scanf("%d",&r->present);
	}
}

void printf_attend(attend_rec r)
{
	printf("%s is %s \n",r.name,(r.present==1?"present" : "absent"));
}

int add(attend_rec ar)
{
	FILE *fp;
	fp=fopen("DB_FILE","ab");
	if(fp==NULL)
	{
		fprintf(stderr,"%s :file not open",DB_FILE);
		return FALSE;
	}
	fwrite(&ar,rec_size,1,fp);
	if((fclose(fp))==EOF)
	{
		fprintf(stderr,"%s: file not closed",DB_FILE);
	}
return TRUE;
}

/*
  Story 3: As a teacher, I want to be see all of the students in my
  class, so I can see who is present and absent.
 */
void list()
{
	FILE *fp;
	fp=fopen("DB_FILE","rb");
	attend_rec r;
	if(fp==NULL)
	{
		fprintf(stderr,"%s : file not open",DB_FILE);
		return;
	}
	printf("SNO\tNAME\tattendance\n");
		fread(&r,rec_size,1,fp);
	while(!feof(fp))
	{
		printf("%d\t%s\t%s\n",r.sno,r.name,(r.present==1?"present" : "absent"));
		fread(&r,rec_size,1,fp);
	}
	if((fclose(fp))==EOF)
	{
		fprintf(stderr,"%s : file not close",DB_FILE);
	}
}

/*
   Story 4: As a programmer, I want to add a set of records to my
   database for testing purposes.
*/

void initialize_db()
{
	FILE *fp;
	fp=fopen("DB_FILE","ab");
	attend_rec r;
	if(fp==NULL)
	{
		fprintf(stderr,"%s : file not open",DB_FILE);
		return;
	}

	for(int i=0;i<10;i++)
	{
		r.sno=i;
		strcpy(r.name,"Student");
		r.name[7]='0'+i;
		r.name[8]='\0';
		r.present=rand()%2;
		fwrite(&r,rec_size,1,fp);
	}
	if((fclose(fp))==EOF)
	{
		fprintf(stderr,"%s : file not closed",DB_FILE);
	}

}

/*
   Story 5: As a programmer, I want to delete records the from the
   database, so I can test my program against an known state.
*/

int clear_db()
{
	FILE *fp;
	fp=fopen("DB_FILE","wb");
	if(fp==NULL)
	{
		fprintf(stderr,"%s ; file not open",DB_FILE);
		return FALSE;
}
	if((fclose(fp))==EOF)
	{
		fprintf(stderr,"%s : file not open",DB_FILE);
		return FALSE;
	}

  return TRUE;
}

/*
  Story 6: As a teacher, I want to be find a student's record so I can
  see whether a student was present or absent for a class.
*/

// We need a special record to return when the one we are looking for
// is not found
attend_rec not_found = {.sno = -1,
			.name = "none",
			.present = -1};

// Prompt the user for a serial number and read from keyboard
int get_sno()
{
	int sno;
  	printf("enter sno on which you want to perform>");
	scanf("%d",&sno);
	return sno;
}

attend_rec find(int sno)
{
	attend_rec r;
	FILE *fp;
	fp=fopen("DB_FILE","rb");
	int found=-1;
	if(fp==NULL)
	{
		fprintf(stderr,"%s : file not open",DB_FILE);
	}
	fread(&r,rec_size,1,fp);
	while(!feof(fp))
	{
		if(r.sno==sno)
		{
			found=sno;
			break;
		}
		fread(&r,rec_size,1,fp);
	}
	if((fclose(fp))==EOF)
	{
		fprintf(stderr,"%s : file not closed",DB_FILE);
	}
	if(found==-1)
	{
		return not_found;
	}
	else
	{
		return r;
	}
}
/*
  Story 7: As a teacher, I want to modify a student's record, so I can
  correct mistakes.
*/

int modify(attend_rec ar)
{
	FILE *fp,*test;
	int pos,i=0;
	attend_rec r;
	pos=find_index(ar.sno);
	char ch;
	if(pos==-1)
	{
		printf("NOT FOUND!!!!!!!!!\nDO you want to add");
		scanf(" %c",&ch);
		if(ch=='y'||ch=='Y')
		{
			add(ar);		
		}
		return TRUE;
	}
	fp=fopen("DB_FILE","ab+");
	rewind(fp);
	test=fopen("temp.bin","wb+");
	if(fp==NULL||test==NULL)
	{
		fprintf(stderr,"%s : file not closed",DB_FILE);
		return FALSE;
	}
	fread(&r,rec_size,1,fp);
	while(!feof(fp))
	{
		if(i!=pos)
		{
			fwrite(&r,rec_size,1,test);
			fread(&r,rec_size,1,fp);
			i++;
		}
		else
		{
			fwrite(&ar,rec_size,1,test);
			fread(&r,rec_size,1,fp);
			i++;
		}
	}
	if(fclose(fp)==EOF)
	{
		fprintf(stderr,"%s : file not open",DB_FILE);
		return FALSE;
	}
	fp=fopen("DB_FILE","wb");
	if(fp==NULL)
	{
		fprintf(stderr,"%s : file not open",DB_FILE);
	}

	rewind(test);
	fread(&r,rec_size,1,test);
	while(!feof(test))
	{
		fwrite(&r,rec_size,1,fp);
		fread(&r,rec_size,1,test);
	}
	remove("temp.bin");

	if(fclose(fp)==EOF||(fclose(test))==EOF)
	{
		fprintf(stderr,"%s : file not closed",DB_FILE);
		return FALSE;
	}
  return TRUE;
}



/*
  Story 8: As a teacher, I want to delete a student's record, so I can
  drop a student from the class.
*/

int find_index(int sno)
{
	FILE *fp;
	int pos=0,found=-1;
	attend_rec r;
	fp=fopen("DB_FILE","rb");
	if(fp==NULL)
	{
		fprintf(stderr,"%s : file not open",DB_FILE);
		return FALSE;
	}
	fread(&r,rec_size,1,fp);
	while(!feof(fp))
	{
		if(r.sno==sno)
		{
			found=pos;
			break;
		}
		fread(&r,rec_size,1,fp);
		pos++;
	}
	if((fclose(fp))==EOF)
	{
		fprintf(stderr,"%s : file not closed",DB_FILE);
	}
  return found;
}

int delete(int sno)
{
	FILE *fp,*test;
	int pos,i=0;
	attend_rec ar;
	pos=find_index(sno);
	if(pos==-1)
	{
		printf("NOT FOUND!!!!!!!!!\n");
		return TRUE;
	}
	fp=fopen("DB_FILE","ab+");
	rewind(fp);
	test=fopen("temp.bin","wb+");
	if(fp==NULL||test==NULL)
	{
		fprintf(stderr,"%s : file not closed",DB_FILE);
		return FALSE;
	}
	fread(&ar,rec_size,1,fp);
	while(!feof(fp))
	{
		if(i!=pos)
		{
			fwrite(&ar,rec_size,1,test);
			fread(&ar,rec_size,1,fp);
			i++;
		}
		else
		{
			fread(&ar,rec_size,1,fp);
			i++;
		}
	}
	if(fclose(fp)==EOF)
	{
		fprintf(stderr,"%s : file not open",DB_FILE);
		return FALSE;
	}
	fp=fopen("DB_FILE","wb");
	if(fp==NULL)
	{
		fprintf(stderr,"%s : file not open",DB_FILE);
	}

	fseek(test,0,SEEK_SET);
	fread(&ar,rec_size,1,test);
	while(!feof(test))
	{
		fwrite(&ar,rec_size,1,fp);
		fread(&ar,rec_size,1,test);
	}
	remove("temp.bin");

	if(fclose(fp)==EOF||(fclose(test))==EOF)
	{
		fprintf(stderr,"%s : file not closed",DB_FILE);
		return FALSE;
	}
  return TRUE;
}
