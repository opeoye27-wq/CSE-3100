#include <stdio.h>
#include <stdlib.h>

typedef struct node {
  int value;
  struct node *next;
} Node;

// write a function, that takes value as input and returns a node:

Node *createNode(int v) {
  Node *n = malloc(sizeof(Node));
  n->value = v;
  n->next = NULL;
  return n;
}

Node *prepend(Node *head, int v) {
  /* printf("Address stored in head = %p\n", head); */
  Node *new = createNode(v);
  new->next = head;
  return new;
}



void prepend_dp(Node **head, int v) {
  /* printf("Address stored in head = %p\n", head); */
  Node *new = createNode(v);
  new->next = *head;
	*head = new;
}

Node *find_last(Node *head) {
  if (head != NULL) {
    while (head->next != NULL) {
      head = head->next;
    }
  }
  return head;
}

Node *append(Node *head, int v) {
	/* printf("Address stored in head = %p\n",head); */
  if (head == NULL) {
    return createNode(v);
  }
  Node *last = find_last(head);
  last->next = createNode(v);
  return head;
}

void append_dp(Node **head, int v) {
  /* printf("Address stored in head = %p\n", head); */
  if (*head == NULL) {
    *head = createNode(v);
  } else {

    Node *last = find_last(*head);
    last->next = createNode(v);
  }
}

void print_list(Node *head) {
  if (head == NULL) {
    printf("Empty list\n");
  }
  while (head != NULL) {
    printf("%d->", head->value);
    head = head->next;
  }
  printf("NULL\n");
}

void free_list(Node *head) {
  Node *temp = head;
  while (temp != NULL) {
    head = head->next;
    free(temp);
    temp = head;
  }
}

int main() {
  Node *l1 = NULL; // list is not pointing to anything
	Node *l2 = NULL;
  append_dp(&l1, 100);
  append_dp(&l1, 200);
  append_dp(&l1, 300);
	l2 = append(l2,123);
	append(l2,456); //notice I am not updating l2
	append(l2,354); //notice I am not updating l2
  prepend_dp(&l1,10);
  prepend_dp(&l1,20);
  prepend_dp(&l1,30);
  l2 = prepend(l2,20);
  l2 = prepend(l2,30);
  l2 = prepend(l2,40);
  print_list(l1);
  print_list(l2);

  free_list(l1);

  return 0;
}
