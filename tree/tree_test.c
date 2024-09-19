#include <stdio.h>
#include "tree.h"

void tree_dfs_print(struct node *node, struct dfs_context *ctx);

int main(void)
{
        struct node *root = tree_create();
        struct node *child;
        child = tree_add_child(root);
        child = tree_add_child(root);
        child = tree_add_child(root);
        child = tree_add_child(root);
        struct node *child2;
        child2 = tree_add_child(child);
        child2 = tree_add_child(child);
        child2 = tree_add_child(child);

        child = tree_add_child(child2);
        child = tree_add_child(child2);
        child = tree_add_child(child2);

        child2 = tree_add_child(child);
        child2 = tree_add_child(child);
        child2 = tree_add_child(child);

        child = tree_add_child(child2);
        child = tree_add_child(child2);
        child = tree_add_child(child2);

        child2 = tree_add_child(child);
        child2 = tree_add_child(child);
        child2 = tree_add_child(child);

        child = tree_add_child(child);
        child = tree_add_child(child);
        child = tree_add_child(child);
        child = tree_add_child(child);
        child = tree_add_child(child);
        child = tree_add_child(child);
        child = tree_add_child(child);
        child = tree_add_child(child);
        child2 = tree_add_child(child);
        child2 = tree_add_child(child);
        child2 = tree_add_child(child);
        child2 = tree_add_child(child);
        child = tree_add_child(child);
        child = tree_add_child(child);
        child = tree_add_child(child2);
        child = tree_add_child(child);
        
        tree_traverse_dfs(root, tree_dfs_print, NULL);
        return 0;
}

void tree_dfs_print(struct node *node, struct dfs_context *ctx)
{
        if (node == NULL || ctx == NULL) {
                return;
        }

        printf("%d ( ", ctx->stack->top);
        for (int i = 0; i < ctx->stack->top; i++) {
                printf("%d ", ((int *)ctx->stack->array)[i]);
        }
        printf(")\n");
        return;
}