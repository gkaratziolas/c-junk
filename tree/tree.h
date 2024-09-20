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
        struct stack *stack;
        void *additional_context;
};

struct node *tree_create(void);
int tree_destroy(struct node *root);
int tree_count_nodes(struct node *root);
int tree_max_depth(struct node *root);

struct node *tree_add_child(struct node *parent);

int tree_traverse_preorder(struct node *root, 
                           void (*callback)(struct node *, struct dfs_context *),
                           void *additional_context);

int tree_traverse_postorder(struct node *root, 
                            void (*callback)(struct node *, struct dfs_context *),
                            void *additional_context);

int tree_traverse_prepost(struct node *root, 
                          void (*prefix_callback)(struct node *, struct dfs_context *),
                          void (*postfix_callback)(struct node *, struct dfs_context *),
                          void *additional_context);

#endif // TREE_H_