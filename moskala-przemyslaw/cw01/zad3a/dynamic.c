//
// Created by przemek on 15.03.17.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "addressbook.h"
#include <sys/times.h>
#include <time.h>
#include <dlfcn.h>

clock_t startTime;
clock_t endTime;

static struct tms starttms;
static struct tms endtms;


void start_clock(){
    startTime=clock();
    times(&starttms);

}
void end_clock(){
    times(&endtms);
    endTime=clock();
    printf("\nreal:    ");
    printf("%f", (double) (endTime-startTime)/CLOCKS_PER_SEC );
    printf("\nuser:    ");
    printf("%f",
           (double) (endtms.tms_utime-starttms.tms_utime)/CLOCKS_PER_SEC );

    printf("\nsys:    ");
    printf("%f",
           (double) (endtms.tms_stime-starttms.tms_stime)/CLOCKS_PER_SEC ) ;


}








static char *randDigital(char * str, size_t size){
    const char charset[] = "0123456789";
    if (size) {
        --size;
        for (size_t n = 0; n < size; n++) {
            int key = rand() % (int) (sizeof charset - 1);
            str[n] = charset[key];
        }
        str[size] = '\0';
    }
    return str;


}

char* randDigitalloc(size_t size)
{
    char *s = malloc(size + 1);
    if (s) {
        randDigital(s, size);
    }
    return s;
}

static char *randChars(char *str, size_t size)
{
    const char charset[] = "abcdefghijklmnopqrstuvwxy";
    if (size) {
        --size;
        for (size_t n = 0; n < size; n++) {
            int key = rand() % (int) (sizeof charset - 1);
            str[n] = charset[key];
        }
        str[size] = '\0';
    }
    return str;
}
char* rand_string_alloc(size_t size)
{
    char *s = malloc(size + 1);
    if (s) {
        randChars(s, size);
    }
    return s;
}


char * generateMail(){
    return strcat(rand_string_alloc(rand()%1+3), "@gmail.com");
}
char * generateDate(){
    char * date =randDigitalloc(4);
    date=strcat(date, "-");
    date=strcat(date, randDigitalloc(2));
    date=strcat(date, "-");
    date=strcat(date, randDigitalloc(2));
    return date;
}

void start_clock(void);
void end_clock();


void test_list() {
    printf("\n________________________________________");
    printf("\n_____________Oparta na liscie___________");
    printf("\n________________________________________\n");
    void* library = dlopen("./lib/libaddressbook_shared.so", RTLD_LAZY);
    printf("Tworzenie ksiązki (10k kontaktów)");
    start_clock();
    PhoneBook* (*createPB) ( char a);
    createPB =dlsym(library, "createPB");
   if(!createPB){
       printf("errr");
   }

    PhoneBook * p= ((*createPB)('l'));


    void (*add) (PhoneBook * phoneBook,char* name, char* surname, char* email, char *phone, char* date, char* address);
    add = dlsym(library, "addContactToPhoneBook");

    for ( int i=0 ; i< 1000;  i++)
      ((*add) (p,rand_string_alloc(rand()%10+3),rand_string_alloc(rand()%1+3),generateMail(),randDigitalloc(9),generateDate(),rand_string_alloc(rand()%10+3)));
    end_clock();


    printf("\n\nSortowanie ksiazki:");
    start_clock();
    void (*sort) (PhoneBook * phoneBook, char option);
    sort= dlsym(library,"sortPhoneBook");
    (*sort)(p,'n');

    //sortPhoneBook(phoneBook, 'n');
    end_clock();




    printf("\n\nDodanie jednego kontaktu:");
    start_clock();
    (*add) (p,"Jan","Kowalski",generateMail(),randDigitalloc(9),generateDate(),rand_string_alloc(rand()%10+3));
    end_clock();

    printf("\n\nUsuwanie jednego kontaktu (optymistyczne):");
    start_clock();
    void (*delete) (PhoneBook * p, char* name, char * surname);
    delete=dlsym(library,"deleteContact");
    (*delete) (p, "Przemek", "Moskala");
    end_clock();
    (*add) (p,"Jan2","Kowalski2",generateMail(),randDigitalloc(9),generateDate(),rand_string_alloc(rand()%10+3));

    printf("\n\nWyszukiwanie kontaktu:");
    start_clock();

    Contact * (*search) (PhoneBook *p, char* name, char* surname);
    search=dlsym(library,"searchContact");
    (* search) (p, "Jan2", "Kowalski2");
    end_clock();

    printf("\n\nUsuwanie jednego kontaktu:");
    start_clock();
    (*delete)(p, "Jan2", "Kowalski2");

    end_clock();







}

int main(int argc, char *argv[]) {


    test_list();
   // test_tree();
}
