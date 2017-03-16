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
    void* library = dlopen("./lib/libshared.so", RTLD_LAZY);
    PhoneBook * (createPB) (PhoneBook);

    createPB = (PhoneBook * (PhoneBook)) dlsym(library, "createPB");
    PhoneBook * p= ((*createPB));
    

}

int main(int argc, char *argv[]) {


    test_list();
   // test_tree();
}
