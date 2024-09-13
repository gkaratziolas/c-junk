#include "tree.h"

struct node *new_tree(void)
{
        struct node *np = malloc(sizeof(struct node));
        if (np == NULL)
                return NULL;
        np->parent = NULL;
        np->children = NULL;
        np->num_children = 0;
        return np;
}

struct node *tree_new_child(struct node *parent)
{
        // Allocate memory for new child
        struct node *child = malloc(sizeof(struct node));
        if (child == NULL)
                return NULL

        // Allocate space for additional child pointer
        struct node **tmp = realloc(parent->children, (parent->num_children+1) * sizeof(struct node*));
        if (tmp == NULL) {
                free(child);
                return NULL;
        }
        parent->children = tmp;

        parent->children[parent->num_children] = child;
        parent->num_children++;

        child->parent = parent;
        child->children = NULL;
        child->num_children = 0;

        return child;
}

int tree_depth_first_traversal(struct node *root)
{
        struct node *x = root;
        int max_depth = 16;
        int *child_indices = malloc(sizeof(child_indices[0]) * max_depth);

        int depth = 0;
        child_indices[depth] = 0;

        // acknowlege root

        while (1) {
                // If x has children remaining, increase search depth
                if (child_indices[depth] < x->num_children) {
                        // Broken link in tree
                        if (x->children[child_indices[depth]]->parent != x) {
                                return -1;
                        }
                        x = x->children[child_indices[depth]];
                        // acknowlege x
                        depth++;
                        if (depth == max_depth) {
                                int *tmp = realloc(child_indices, sizeof(child_indices[0]) * max_depth * 2);
                                if (tmp == NULL) {
                                        free(child_indices);
                                        return -1;
                                }
                                max_depth = max_depth * 2;
                        }
                        child_indices[depth] = 0;
                        continue;
                }
                // Out of children, return to parent
                if (x->parent == NULL) {
                        free(child_indices);
                        return 0;
                }
                x = x->parent;
                depth--;
                child_indices[depth]++;
        }

        free(child_indices);
        return 0;
}