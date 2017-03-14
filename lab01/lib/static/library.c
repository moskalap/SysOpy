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
void deleteList(LinkedList * list){
    Node * tmp = list->head;
    while(tmp!=NULL){
        free(tmp->contact);
        tmp=tmp->next;
        if( tmp && tmp->previous!= NULL) free(tmp->previous);
    }
    free(list);
}
void deleteContactStruct(Contact * contact){
    free(contact->name);
    free(contact->surname);
    free(contact->address);
    free(contact->email);
    free(contact);

}
void swap(Node * a, Node * b){
    Contact *t = a ->contact;
    a->contact=b->contact;
   b->contact=t;
}
int compareContacts(Contact *a, Contact *b) {
    int res = strcmp(a->surname, b->surname);
    if (res == 0) {

        return strcmp(a->name, b->name);
    }
    else return res;

}
Node *lastNode(Node *root)
{
    while (root && root->next)
        root = root->next;
    return root;
}
/* Considers last element as pivot, places the pivot element at its
   correct position in sorted array, and places all smaller (smaller than
   pivot) to left of pivot and all greater elements to right of pivot */
char *getPivot(Node* h , char option){
    switch(option){
        case 'n':
            return h->contact->surname;

        case 'p':
            return h->contact->phone;

        case 'e':
            return h->contact->email;

        case 'd':
            return h->contact->date;

    }
}

int compareby(Contact * a, Contact * b, char option){
    switch(option){
        case 'n':
             return   strcmp(a->surname, b->surname);

        case 'p':
         printf(a->phone);
            printf("\n");
            return strcmp(a->phone, b->phone);

        case 'e':
            return strcmp(a->email, b->email);

        case 'd':
            return strcmp(a->date, b->date);

    }
}
Node* partition(Node *l, Node *h, char option)
{
    //char *x  = h->contact->surname;
    char *x = getPivot(h, option);


    Node *i = l->previous;
    for (Node *j = l; j != h; j = j->next){
        int f=compareby(j->contact, h->contact,option);
     char * v =j->contact->email;
     char * u =h->contact->email;


        if(compareby(j->contact, h->contact,option) <= 0)
        {
            i = (i == NULL)? l : i->next;
            swap(i, j);
        }
    }
    i = (i == NULL)? l : i->next; // Similar to i++
    swap(i, h);
    return i;
}
void _quickSort(Node* l, Node *h, char option){
    if (h != NULL && l != h && l != h->next){
        Node *p = partition(l, h,option);
        _quickSort(l, p->previous, option);
        _quickSort(p->next, h,option);
    }
}
void quickSort(Node *head, char option){
    Node *h = lastNode(head);
    _quickSort(head, h, option);
}

//TREE:
BinaryTree * createTree(){
    BinaryTree *tree=malloc(sizeof(BinaryTree));
    tree->root=malloc(sizeof(TreeNode));
    return tree;

}
void addTreeNode(TreeNode *root, Contact *contact, char option){
    if(root->contact==NULL){
        root->contact=contact;
        root->leftChild=malloc(sizeof(TreeNode));
        root->rightChild=malloc(sizeof(TreeNode));
    }
    else{
        if(compareby(root->contact, contact, option)>=0)
            addTreeNode(root->leftChild,contact, option);
        else
            addTreeNode(root->rightChild,contact, option);
    }
}
void addTreeByDate(TreeNode *root, Contact *contact){
    if(root->contact==NULL){
        printf("s");
        root->contact=contact;
        root->leftChild=malloc(sizeof(TreeNode));
        root->rightChild=malloc(sizeof(TreeNode));




    }
    else{
        if(strcmp(root->contact->date, contact->date)>=0)
            addTreeByDate(root->leftChild,contact);
        else
            addTreeByDate(root->rightChild,contact);

    }
}
Contact * deleteTNode(TreeNode * t){
    Contact * c= t->contact;
    free(t);
    return c;
}
void printTree(TreeNode* root){
    if(root!=NULL && root->contact!=NULL){
        printTree(root->leftChild);
        printContact(root->contact);
        printTree(root->rightChild);
    }
}
TreeNode * searchTreeNode(char * name, char * surname,TreeNode* root){

    if (root->contact!=NULL){

        printf(root->contact->surname);
        if (strcmp(root->contact->surname, surname) == 0 && strcmp(root->contact->name, name) == 0) {
            return root;
        }
        if(strcmp(root->contact->surname, surname)>0 || (strcmp(root->contact->surname, surname)==0 && strcmp(root->contact->name, name)>0))
            return searchTreeNode(name, surname, root->leftChild);
        if(strcmp(root->contact->surname, surname)<0) return searchTreeNode(name, surname, root->rightChild);
    } else return NULL;

}

BinaryTree * rebuildTreeby(TreeNode* binaryTree, char option, BinaryTree * new){
   if(binaryTree && binaryTree->contact) {

       Contact *contact = binaryTree->contact;
       rebuildTreeby(binaryTree->leftChild,option, new);
       rebuildTreeby(binaryTree->rightChild,option, new);
       addTreeNode(new->root,contact,option);

   }
    return new;

}
void deleteTree(TreeNode * node){
    if (node){
        deleteTree(node->leftChild);
        deleteTree(node->rightChild);
        if (node->contact) deleteContactStruct(node->contact);
    }
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
        phoneBook->tree=createTree();
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
            addTreeNode(phoneBook->tree->root, contact, 'n');
        }else{
            printf("err::can not add contact ");
        }
    }
}
Contact* searchContact(PhoneBook* phoneBook, char * name, char* surname){
    if(phoneBook->basedon == 'l'){
         return searchContactinList(name,surname,phoneBook->list);
    }else{
        if (phoneBook ->basedon =='t'){
           return searchTreeNode(name,surname, phoneBook->tree->root)->contact;
        }else{
            printf("Contact not in list!");
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
            printTree(phoneBook->tree->root);
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
            deleteTnode(searchTreeNode(name, surname, phoneBook->tree->root), phoneBook->tree->root);

        }else{
            printf("err");
        }
    }
}
void deletePhoneBook(PhoneBook * phoneBook){
    if(phoneBook->basedon == 'l'){
        deleteList(phoneBook->list);
        free(phoneBook);
    }else{
        if (phoneBook ->basedon =='t'){
            deleteTree(phoneBook->tree->root);
            free(phoneBook->tree);
            free(phoneBook);
        }else{
            printf("err");
        }
    }
}
void sortPhoneBook(PhoneBook * phoneBook, char option){
    if(phoneBook->basedon == 'l'){
       quickSort(phoneBook->list->head, option);
    }else{
        if (phoneBook ->basedon =='t'){
            BinaryTree * new=createTree();
        phoneBook->tree=rebuildTreeby(phoneBook->tree->root, option, new);

        }else{
            printf("err");
        }
    }
}

