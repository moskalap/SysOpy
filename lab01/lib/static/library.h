#ifndef STATIC_LIBRARY_H
#define STATIC_LIBRARY_H

//--STRUCTURES
typedef struct Contact {
    char *name;
    char *surname;
    char *date[10]; //dd-mm-yyyy ->[10]
    char *email;
    char phone[9];
    char *address;
} Contact;
typedef struct Node {
    struct Node *previous;
    struct Contact *contact;
    struct Node *next;
} Node;
typedef struct LinkedList {
    struct Node *head;
    struct Node *tail;
} LinkedList;
typedef struct TreeNode {
    struct TreeNode *parent;
    struct Contact *contact;
    struct TreeNode *leftChild;
    struct TreeNode *rightChild;
} TreeNode;
typedef struct BinaryTree {
    struct TreeNode *root;
} BinaryTree;
typedef struct PhoneBook {
    char basedon; // 't' - tree 'l' - list
    struct LinkedList *list;
    struct BinaryTree *tree;
} PhoneBook;
//----
//-----------------PRIVATE;
//sel
Contact *createContact(char* name, char* surname, char* email, char *phone, char* date, char* address);
        LinkedList * createList();
//TREE:
        BinaryTree * createTree();
//---------------------/private;

//public
PhoneBook * createPB(char  basedon); //create a phone  book based on l - list, t - tree
void addContactToPhoneBook(PhoneBook * phoneBook,char* name, char* surname, char* email, char *phone, char* date, char* address);
Contact* searchContact(PhoneBook* phoneBook, char * name, char* surname);
void displayContact(Contact * contact);
void deleteConatact(PhoneBook * phoneBook, char * name, char * surname);
void deletePhoneBook(PhoneBook * phoneBook);
void sortPhoneBook(PhoneBook * phoneBook, char option);









#endif