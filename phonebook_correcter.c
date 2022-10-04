#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

char* skipwhitespace( char* p ){// WORKS
    while ( isspace(*p) ) {
        p++;
    }
  return p;
}

char* readunquoteddata( char* inp, char* data ){
    int i = 0;
    while ( *inp != ',' && *inp != 0 ) {
        *(data+i) = *(inp++);
        i++;
    }
    *(data+i) = 0;
    return inp;
}

char* readquoteddata(char* inp, char* data) {
    if ( !inp || *inp != '\"' ) abort( );
  else{
      int i;
      inp++;
     for ( i = 0; *(inp) != '\"' && *(inp) != 0; i++ ) {
        *( data+i ) = *(inp++);
    }
    *(data+i)=0;
    if ( *(inp) == 0 ) {
        return inp;
    }
    return ++inp;
  }
}


char* readdata( char* inp, char* data) {
    inp = skipwhitespace(inp);
  if (*(inp) == '\"'){
    inp = skipwhitespace(inp);
    inp = readquoteddata(inp, data);
  }
  else{
    inp = readunquoteddata(inp, data);
  }
  if (*inp == ','){
    inp++;
  }
  return inp;
}

void correctfield(char *fld, char* corrected) {
    fld = skipwhitespace( fld );
    int j = 0;
    for ( int i = 0; *(fld + i) != 0; i++ ) {
        while (*(fld+i+1) == ' ' && *(fld+i) == ' '){
            i++;
          }
        *(corrected+j) = *(fld+i);
        j++;
    }
    corrected[j] = 0;
}

char USAcode( char c ){ //works
    if (c > '`' && c < 'p'){
        return 48 + ( 2 + (c - 97)/3 );
    }
    else if (c>111&&c<116){
        return '7';

    }
   else if(c>115&&c<119)
   {
     return '8';
   }
   else if(c>118 &&c<123)
   {
     return '9';
   }
   return c;
}

void correctPhoneNumber( char* number, char* corrected ) {
    int i = 0, j = 1;
    if (*number == '0' && *(number+1) == '0'){
        i+=2;
      } else if (*number != '+' ){
          i++;
      }
    *corrected = '+';

      while (*(number + (i)) != 0){
        if ( isalpha( *(number + i) )) {
          *(number+i) = tolower(*(number+i));
          *(number + i) = USAcode(*(number + i));
        }
        if (*(number + i) <= '9' && *(number + i) >= '0'){
          *(corrected + (j++)) = *(number + i);
        }
        i++;
      }
  *(corrected+j) = 0;
}

int countlines( char* fname ) { //WORKS
    FILE* book = fopen(fname,"r");
      if (book == NULL){
          return -1;
      }
    char c;
  char p;
  char string[20];
    int counter = 0;
    while(!feof(book)) {
         c = getc(book);
        if (c == '\n'){
      counter++;
      
    }
  }
    fclose(book);
    return counter;
}


int readline( FILE* f, char line[] ){ // WORKS
    fgets(line,500,f);
     if (line[strlen(line)-1] == '\n'){
        line[strlen(line)-1] = 0;
        return '\n';
      }
      return EOF;
}


typedef struct
{
   char firstname[30];
   char secondname[30];
   char address[50];
   char phonenumber[30];

} PhoneEntry;


void initPhoneEntry( PhoneEntry* e )
{
   e -> firstname[0] = 0;
   e -> secondname[0] = 0;
   e -> address[0] = 0;
   e -> phonenumber[0] = 0;
}

void printPhoneEntry( FILE* f, PhoneEntry* e ){
  fprintf(f,"%-20s", e -> firstname);
  fprintf(f,"%-20s", e -> secondname);
  fprintf(f,"%-42s", e -> address);
  fprintf(f,"%-13s\n", e -> phonenumber);
}


void printPhoneEntryCSV( FILE* f, PhoneEntry* e ){
  fprintf(f,"\"%s\", ", e -> firstname);
  fprintf(f,"\"%s\", ", e -> secondname);
  fprintf(f,"\"%s\", ", e -> address);
  fprintf(f,"\"%s\"\n", e -> phonenumber);
}


void printPhoneBook( FILE* f, PhoneEntry* start, int size ) //WORKS
{
    for (int i = 0; i < 45; i++)
        fprintf(f," ");
    fprintf( f, "Phonebook\n" );
    for (int i = 0; i < (20+20+50+14); i++)
        fprintf(f,"=");
    fprintf(f,"\n");
    fprintf(f,"Firstname           Surname             Address                                   Phone number \n");
    for (int i = 0; i < (20+20+50+14); i++)
        fprintf(f,"-");
    fprintf(f,"\n");
    for (int i = 0; i < size; i++){
      printPhoneEntry(f,start+i);
    }
}
void printPhoneBookCSV( FILE* f, PhoneEntry* start, int size ){
    for (int i = 0; i < size; i++){
        printPhoneEntryCSV( f, start+i);
    }
}

char* readwithcutoff(char* inp, char* fld, int flen ){
      char data[500];
      inp = readdata(inp,data);
      inp = skipwhitespace( inp );
    data[flen]=0;
    strcpy(fld, data);
    return inp;
}


void parseEntry( PhoneEntry* e, char* inp )
{
  initPhoneEntry(e);
  inp = readwithcutoff(inp, e->firstname, 30);
  inp = readwithcutoff(inp, e->secondname, 30);
  inp = readwithcutoff(inp, e->address, 50);
  inp = readwithcutoff(inp, e->phonenumber, 30);
}

void checkPhoneEntry( PhoneEntry* e, int linenr )
{
   char inp[500];
   PhoneEntry corrected;
   int accepted = 0;
   while( !accepted )
   {  accepted = 1;
      correctfield(e -> firstname, corrected.firstname);
      correctfield(e -> secondname, corrected.secondname);
      correctfield(e -> address, corrected.address);
      correctPhoneNumber(e -> phonenumber, corrected.phonenumber);
      if (strlen(corrected.firstname) == 0){
        printf("Firstname isn't set in %i line. Please, set one: ", linenr+1);
        scanf("%s", inp);
        readwithcutoff(inp, e -> firstname, 30);
        accepted = 0;
      }
      if (strlen(corrected.secondname) == 0){
        printf("Secondname isn't set in %i line. Please, set one: ",linenr+1);
        scanf("%s", inp);
        readwithcutoff(inp, e -> secondname, 30);
        accepted = 0;
      }
      if (strlen(corrected.address) == 0){
        printf("Address isn't set in %i line. Please, set one: ",linenr+1);
        scanf("%s", inp);
        readwithcutoff(inp, e -> address, 50);
        accepted = 0;
      }
      if (strlen(corrected.phonenumber) != 13){
        printf("Phone number isn't set in line %i. Please, set one: ",linenr+1);
        scanf("%s", inp);
        readwithcutoff(inp, e -> phonenumber, 50);
        accepted = 0;
      }
   }
   *e = corrected;
}


int main( ){
   
   char bookname[] = "phoneobook.csv";

   int lines = countlines( bookname );
   printf( "number of lines = %d\n", lines );

   if( lines < 0 )
   {
      printf( "could not open the phonebook %s\n", bookname );
      return -1;
   }


   PhoneEntry* thebook  = malloc( ( lines + 1 ) * sizeof( PhoneEntry ));
   int thebooksize = 0;
   if( !thebook )
   {
      printf( "could not reserve memory\n" );
      return 0;
   }

   FILE* f = fopen( bookname, "r" );
   if( !f )
   {
      printf( "could not open the phonebook %s\n", bookname );
      free( thebook );
      return -1;
   }

   int linenr = 1;

   char line[500];
   char c = readline( f, line );
   while( c != EOF )
   {
      char* p = line;
      p = skipwhitespace(p);

      if( *p )
      {
         parseEntry( thebook + thebooksize, p );
         ++ thebooksize;
      }
      c = readline( f, line );
      ++ linenr;
      
   }

   FILE* output = fopen("output.txt","w");
   FILE* outputcsv = fopen("output.csv","w");
   for (int i = 0; i < thebooksize; i++){
    checkPhoneEntry(&thebook[i], i);
   }
   printPhoneBook(output, thebook, thebooksize);
   printPhoneBookCSV(outputcsv, thebook,thebooksize);
   fclose(output);
  
   free( thebook );

   return 0;
}

