#include "library.h"

#include <stdio.h>
//PRIVATE;
//sel

Contact *createContact(char* name, char* surname, char* email, char *phone, char* date, char* address) {
    Contact *contact=malloc(sizeof(Contact));
    contact->name=(char*)malloc(strlen(name) * sizeof(char));
    contact->surname=(char*)malloc(strlen(surname) * sizeof(char));
    contact->address=(char*)malloc(strlen(address) * sizeof(char));
    contact->email= (char*)malloc(strlen(email) * sizeof(char));
    strcpy(contact->name, name);
    strcpy(contact->surname, surname);
    strcpy(contact->email, email);
    strcpy(contact->phone, phone);
    strcpy(contact->address, address);
    strcpy(contact->date, date);

    return contact;

}
//LIST:
        LinkedList * createList(){
        LinkedList *list=malloc(sizeof(LinkedList));
        list->head=NULL;
        list->tail=NULL;
        return list;

    }

//TREE:
        BinaryTree * createTree(){
    BinaryTree *tree=malloc(sizeof(BinaryTree));
    tree->root=malloc(sizeof(TreeNode));
    return tree;

}


//public
PhoneBook * createPB(char  basedon){
    if(basedon == 'l'){
        PhoneBook * phoneBook =malloc(sizeof(PhoneBook));
        phoneBook ->basedon = "l";
        phoneBook->list=createList();
        return phoneBook;
    }
    if(basedon == 't'){
        PhoneBook * phoneBook =malloc(sizeof(PhoneBook));
        phoneBook ->basedon = "t";
        phoneBook->list=createTree();
        return phoneBook;

    }
    return NULL;
} //create a phone  book based on l - list, t - tree
void addContactToPhoneBook(PhoneBook * phoneBook,char* name, char* surname, char* email, char *phone, char* date, char* address){
    if(phoneBook->basedon == 'l'){

    }else{
        if (phoneBook ->basedon =='t'){

        }
    }
}
Contact* searchContact(PhoneBook* phoneBook, char * name, char* surname){

}