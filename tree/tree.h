#ifndef TREE_H_
#define TREE_H_

#include <stdlib.h>

struct node
{
        struct node *parent;
        struct node **children;
        int num_children;
};

struct dfs_context {
        const int *depth;
        const int *child_index_stack;
        void *additional_context;
};

struct node *new_tree(void);
struct node *tree_new_child(struct node *parent);
int tree_traverse_dfs(struct node *root, void (*callback)(struct node *, struct dfs_context *), void * additional_context);

#endif // TREE_H_