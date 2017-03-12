#include <stdio.h>
#include "library.h"
#include "library.c"

int main(){
    PhoneBook * phoneBook = createPB('l');
    addContactToPhoneBook(phoneBook,"przemek","moskala", "mail", "123123123","12-23-2222","adr");
    printContact(searchContact(phoneBook, "przemek", "moskala"));


}