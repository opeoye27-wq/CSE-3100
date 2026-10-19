#include <stdio.h>
#include <stdlib.h>

typedef struct leaf {
  int v;
  struct leaf *lchild;
  struct leaf *rchild;
} Leaf;

Leaf *createChild(int v) {
  Leaf *child = malloc(sizeof(Leaf));
  if (child == NULL) {
    printf("Mallco failed\n");
    return NULL;
  }
  child->v = v;
  child->lchild = NULL;
  child->rchild = NULL;
  return child;
}

Leaf *add(Leaf *tree, int v) {
  printf("address of tree = %p\n", tree);
  if (tree == NULL) {
    printf("Tree is null\n");
    return createChild(v);
  }
  Leaf *temp = tree;
  while (1) {
    if (v > temp->v) {
      if (temp->rchild == NULL) {
        temp->rchild = createChild(v);
        break;
      } else {
        temp = temp->rchild;
      }
    } else {
      if (temp->lchild == NULL) {
        temp->lchild = createChild(v);
        break;
      } else {
        temp = temp->lchild;
      }
    }
  }

  return tree;
}
void inorder(Leaf *tree) {
  if (tree != NULL) {
    inorder(tree->lchild);
    printf("%d\t", tree->v);
    inorder(tree->rchild);
  }
}

void postorder(Leaf *tree) {
  if (tree != NULL) {
    postorder(tree->lchild);
    postorder(tree->rchild);
    printf("%d\t", tree->v);
  }
}

void free_tree(Leaf *tree) {
  if (tree != NULL) {
    free_tree(tree->lchild);
    free_tree(tree->rchild);
    free(tree);
  }
}

int main() {
  Leaf *tree = NULL;

  tree = add(tree, 5);
  add(tree, 6);
  add(tree, 2);
  add(tree, 3);
  add(tree, 9);
  add(tree, 1);
  add(tree, 8);
  inorder(tree);
  printf("\n");
  postorder(tree);

  printf("\n");
  free_tree(tree);
  return 0;
}
