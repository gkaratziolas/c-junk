#ifndef TREE_H_
#define TREE_H_

struct node
{
        struct node *parent;
        struct node **children;
        int num_children;
};

struct node *new_tree(void);
struct node *tree_new_child(struct node *parent);
int tree_depth_first_traversal(struct node *root);

#endif // TREE_H_