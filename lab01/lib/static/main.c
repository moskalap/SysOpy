#include <stdio.h>
#include <stdlib.h>
#include "library.h"
#include "library.c"


static char *rand_string(char *str, size_t size)
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
        rand_string(s, size);
    }
    return s;
}



void test_list(){
    PhoneBook * phoneBook = createPB('l');
    addContactToPhoneBook(phoneBook,"przemek2","moskala2", "mai2", "123123122","12-23-2222","adr2");
    addContactToPhoneBook(phoneBook,"przemek","moskala", "mail", "123123123","12-23-2222","adr");
    addContactToPhoneBook(phoneBook,"przemek1","moskala1", "mai12", "123123121","12-23-2221","adr1");
    for(int i =0 ;i <25; i++)
        addContactToPhoneBook(phoneBook,rand_string_alloc(rand()%10+3),rand_string_alloc(rand()%1+3),rand_string_alloc(rand()%10+3),rand_string_alloc(9),rand_string_alloc(10),rand_string_alloc(rand()%10+3));


    // printContact(searchContact(phoneBook, "przemek", "moskala"));
  //  deleteContact(phoneBook, "przemek", "moskala");
  //  printContact(searchContact(phoneBook, "przemek", "moskala"));
    //deletePhoneBook(phoneBook);
 sortPhoneBook(phoneBook, 'n');
    //displayPhoneBook(phoneBook);
    deleteContact(phoneBook, "przemek", "moskala");
    sortPhoneBook(phoneBook, 'p');
    displayContact(searchContact(phoneBook,"przemek","moskala"));
    printf("__________-");
    //displayPhoneBook(phoneBook);
}


void test_tree(){
    PhoneBook * phoneBook = createPB('t');

    for(int i =0 ;i <10; i++)
        addContactToPhoneBook(phoneBook,rand_string_alloc(rand()%10+3),rand_string_alloc(rand()%1+3),rand_string_alloc(rand()%10+3),rand_string_alloc(9),rand_string_alloc(10),rand_string_alloc(rand()%10+3));

    addContactToPhoneBook(phoneBook,"przemek","moskala", "mail", "123123123","12-23-2222","adr");

    printContact(searchContact(phoneBook, "przemek", "moskala"));


   // for(int i =0 ;i <2; i++)
     //   addContactToPhoneBook(phoneBook,rand_string_alloc(rand()%10+3),rand_string_alloc(rand()%1+3),rand_string_alloc(rand()%10+3),rand_string_alloc(9),rand_string_alloc(10),rand_string_alloc(rand()%10+3));
   //printContact(searchContact(phoneBook, "przemek", "moskala"));
    //deleteContact(phoneBook, "przemek", "moskala");
   // sortPhoneBook(phoneBook, 'n');
   /// displayPhoneBook(phoneBook);
    deleteContact(phoneBook, "przemek", "moskala");
    printf("\ndeleted");
    displayPhoneBook(phoneBook);
//  deletePhoneBook(phoneBook);

}




int main(){
    test_list();


}