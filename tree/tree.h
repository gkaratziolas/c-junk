#ifndef TREE_H_
#define TREE_H_

#include <stdlib.h>
#include "stack.h"

#define DFS_INITIAL_STACK_DEPTH 16u

struct node
{
        struct node *parent;
        struct node **children;
        int num_children;
};

struct dfs_context {
        struct stack *dfs_stack;
        void *additional_context;
};

struct node *tree_create(void);
struct node *tree_add_child(struct node *parent);
int tree_traverse_dfs(struct node *root, void (*callback)(struct node *, struct dfs_context *), void * additional_context);

#endif // TREE_H_