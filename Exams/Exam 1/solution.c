#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUFFER_SIZE 4096

typedef struct people //holds information of people
{
    char name; 
    struct leaf *lperson;
    struct leaf *rperson;
} People;

//BST implementation
People *createPerson(char name) {
  People *person = malloc(sizeof(People));
  if (person == NULL) {
    printf("Malloc failed\n");
    return NULL;
  }
  person->name = name;
  person->lperson = NULL;
  person->rperson = NULL;
  return person;
}
//adding people to the tree
People *add(People *tree, char name) {
  //printf("address of tree = %p\n", tree);
  if (tree == NULL) {
   // printf("Tree is null\n");
    return createPerson(name);
  }
  People *temp = tree;
  while (1) {
    if (name > temp->name) {
      if (temp->rperson == NULL) {
        temp->rperson = createPerson(name);
        break;
      } else {
        temp = temp->rperson;
      }
    } else {
      if (temp->lperson == NULL) {
        temp->lperson = createPerson(name);
        break;
      } else {
        temp = temp->lperson;
      }
    }
  }
  return tree;
}

void inorder(People *tree) {
  if (tree != NULL) {
    inorder(tree->lperson);
    printf("%d\t", tree->name);
    inorder(tree->rperson);
  }
}


//creating contacts
typedef struct contacts {
    char name;
    char contact;
    struct contacts *next;
} Contact;

//Linked List Implementation
Contact *createContact(char n)
{
    Contact *c = malloc(sizeof(Contact));
    c->name = n; //value
    c->next = NULL;
    c->contact = "";
    return c;
}

Contact *append(Contact *head, char n) {
	/* printf("Address stored in head = %p\n",head); */
  if (head == NULL) {
    return createContact(n);
  }
  Contact *last = find_last(head);
  last->next = createContact(n);
  return head;
}

Contact *findPerson(Contact *head, char name)
{
    while(head != NULL)
    {
        if(head->name == name)
            return head;
        head = head->next;
    }
    return head;
}

Contact *addContact(Contact *head, char name, char c)
{
    Contact *p = findPerson(head, name);
    p->contact = p->contact + " " + c;
    return p;
}



int main(char argc)
{
    FILE *fin = fopen("data.txt", "r");
    unsigned char buffer = BUFFER_SIZE;
    char people[] = fgets(buffer, 1, fin);

    for(int i = 0; i < sizeof(people); i++)
    {
        People community = *createPerson(people[i]); //adding people to the community
    }
    fseek(fin, sizeof(people), SEEK_CUR);
    //adding contacts
    while(!feof)
    {
        char p = fgetc(fin);
        fseek(fin, 1, SEEK_CUR);
        char c = fgetc(fin);

        Contact con = *createContact(p);
        addContact(p, p, c);
        fseek(fin, 1, SEEK_CUR);
    }

}