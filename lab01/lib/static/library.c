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
void printContact(Contact * c){
    if (c==NULL) {
        printf("Contact doesn't exist!");
        return;
    }
    printf("\n(surname: %s, ",c->surname);
    printf("name: %s,",c->name);
    printf("e-mail: %s, ",c->email);
    printf("phone: %s, ",c->phone);
    printf("birth date: %s, ",c->date);
    printf("address: %s)",c->address);
}
//LIST:
LinkedList * createList(){
        LinkedList *list=malloc(sizeof(LinkedList));
        list->head=NULL;
        list->tail=NULL;
        return list;

    }
void addContactToList(LinkedList * list, Contact *contact){
    Node * node = malloc(sizeof(Node));
    node->contact=contact;


    if (list->head == NULL && list->tail == NULL){
        list->head=node;
        list->tail=node;
        node->previous=NULL;
        node->next=NULL;

    }else {
        node->previous = list->tail;
        node->next=NULL;
        list->tail->next = node;
        list->tail = node;

    }


}
void printList(LinkedList *list){
    Node* tmp = list->head;
    while(tmp!=NULL) {
        printContact(tmp->contact);
        tmp = tmp->next;
    }

}
Node *searchNodeinList(char* name, char * surname ,LinkedList *list){
    Node * tmp=list->head;
    while(tmp!=NULL){

        if (strcmp(tmp->contact->surname,surname)==0 && strcmp(tmp->contact->name,name)==0)
            return tmp;
        tmp=tmp->next;
    }
    return NULL;

}
Contact *searchContactinList(char * name, char * surname, LinkedList *list){
    Node* res=searchNodeinList(name,surname,list);
    if(res== NULL) return NULL;
    else
        return res->contact;
}
void deleteContactInList(char * name, char * surname, LinkedList *list){
    Node* node =searchNodeinList(name,surname,list);
    int done=0;
    if(node==NULL) return;
    //There is no such contact in phoneBook
    if(list->head==node){
        list->head=node->next;
        if(node->next != NULL)
            list->head->previous=NULL;
        done=1;
    }
    if (list->tail == node) {
        list->tail == node->previous;
        if(node->previous!= NULL);
        list->tail->next = NULL;
        done=1;
    }
    if(done==0){
        node->previous->next=node->next;
        node->next->previous=node->previous;

    }

    free(node->contact);
    free(node);
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
        phoneBook ->basedon = 'l';
        phoneBook->list=createList();
        return phoneBook;
    }
    if(basedon == 't'){
        PhoneBook * phoneBook =malloc(sizeof(PhoneBook));
        phoneBook ->basedon = 't';
        phoneBook->list=createTree();
        return phoneBook;

    }
    return NULL;
} //create a phone  book based on l - list, t - tree
void addContactToPhoneBook(PhoneBook * phoneBook,char* name, char* surname, char* email, char *phone, char* date, char* address){
    Contact* contact = createContact(name, surname,email,phone,date,address);
    if(phoneBook->basedon == 'l'){
            addContactToList(phoneBook->list,contact);
    }else{
        if (phoneBook ->basedon =='t'){

        }else{
            printf("err");
        }
    }
}
Contact* searchContact(PhoneBook* phoneBook, char * name, char* surname){
    if(phoneBook->basedon == 'l'){
         return searchContactinList(name,surname,phoneBook->list);
    }else{
        if (phoneBook ->basedon =='t'){

        }else{
            printf("err");
        }
    }
    return NULL;
}
void displayContact(Contact * c){
    if (c==NULL) {
        printf("Contact doesn't exist!");
        return;
    }
    printf("\n(surname: %s, ",c->surname);
    printf("name: %s,",c->name);
    printf("e-mail: %s, ",c->email);
    printf("phone: %s, ",c->phone);
    printf("birth date: %s, ",c->date);
    printf("address: %s)",c->address);

}
void displayPhoneBook(PhoneBook * phoneBook){
    if(phoneBook->basedon == 'l'){
        printList(phoneBook->list);
    }else{
        if (phoneBook ->basedon =='t'){

        }else{
            printf("err");
        }
    }
}
void deleteContact(PhoneBook * phoneBook, char * name, char * surname){
    if(phoneBook->basedon == 'l'){
        deleteContactInList(name, surname, phoneBook->list);
    }else{
        if (phoneBook ->basedon =='t'){

        }else{
            printf("err");
        }
    }
}
void deletePhoneBook(PhoneBook * phoneBook){
    if(phoneBook->basedon == 'l'){

    }else{
        if (phoneBook ->basedon =='t'){

        }else{
            printf("err");
        }
    }
}
void sortPhoneBook(PhoneBook * phoneBook, char option){
    if(phoneBook->basedon == 'l'){

    }else{
        if (phoneBook ->basedon =='t'){

        }else{
            printf("err");
        }
    }
}

