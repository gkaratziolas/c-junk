#include "tree.h"
#include <stdio.h>

void tree_dfs_print(struct node *node, struct dfs_context *ctx);

int main(void)
{
        struct node *root = new_tree();
        struct node *child;
        child = tree_new_child(root);
        child = tree_new_child(root);
        child = tree_new_child(root);
        child = tree_new_child(root);
        struct node *child2;
        child2 = tree_new_child(child);
        child2 = tree_new_child(child);
        child2 = tree_new_child(child);

        child = tree_new_child(child2);
        child = tree_new_child(child2);
        child = tree_new_child(child2);

        child2 = tree_new_child(child);
        child2 = tree_new_child(child);
        child2 = tree_new_child(child);

        child = tree_new_child(child2);
        child = tree_new_child(child2);
        child = tree_new_child(child2);

        child2 = tree_new_child(child);
        child2 = tree_new_child(child);
        child2 = tree_new_child(child);

        child = tree_new_child(child);
        child = tree_new_child(child);
        child = tree_new_child(child);
        child = tree_new_child(child);
        child = tree_new_child(child);
        child = tree_new_child(child);
        child = tree_new_child(child);
        child = tree_new_child(child);
        child = tree_new_child(child);
        child = tree_new_child(child);
        child = tree_new_child(child);
        child = tree_new_child(child);
        child = tree_new_child(child);
        child = tree_new_child(child);
        child = tree_new_child(child);
        child = tree_new_child(child);
        child = tree_new_child(child);
        child = tree_new_child(child);
        child = tree_new_child(child);
        child = tree_new_child(child);
        child = tree_new_child(child);
        child = tree_new_child(child);
        child = tree_new_child(child);
        child = tree_new_child(child);
        child = tree_new_child(child);
        child = tree_new_child(child);
        child = tree_new_child(child);
        child = tree_new_child(child);
        child = tree_new_child(child);
        child = tree_new_child(child);
        child = tree_new_child(child);
        child = tree_new_child(child);
        child = tree_new_child(child);
        child = tree_new_child(child);
        child = tree_new_child(child);
        child = tree_new_child(child);
        child = tree_new_child(child);
        child = tree_new_child(child);
        child = tree_new_child(child);
        child = tree_new_child(child);
        child = tree_new_child(child);
        child = tree_new_child(child);
        child = tree_new_child(child);
        child = tree_new_child(child);
        child = tree_new_child(child);
        child = tree_new_child(child);
        child = tree_new_child(child);
        child = tree_new_child(child);
        child = tree_new_child(child);
        child = tree_new_child(child);
        child = tree_new_child(child);
        child = tree_new_child(child);
        child = tree_new_child(child);
        child = tree_new_child(child);
        child = tree_new_child(child);
        child = tree_new_child(child);
        child = tree_new_child(child);
        

        tree_traverse_dfs(root, tree_dfs_print, NULL);
        return 0;
}

void tree_dfs_print(struct node *node, struct dfs_context *ctx)
{
        if (node == NULL || ctx == NULL) {
                return;
        }
        printf("%d\n", *(ctx->depth));
        return;
}