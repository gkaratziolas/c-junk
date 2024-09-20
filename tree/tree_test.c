#include <stdio.h>
#include "tree.h"

void tree_dfs_print(struct node *node, struct dfs_context *ctx);

int main(void)
{
        struct node *A = tree_create();
        struct node *B, *C, *D, *E, *F, *G, *H, *I, *J, *K;

        B = tree_add_child(A);
        C = tree_add_child(A);
        D = tree_add_child(A);

        E = tree_add_child(B);
        F = tree_add_child(B);
        G = tree_add_child(B);

        H = tree_add_child(C);
        I = tree_add_child(C);

        J = tree_add_child(D);

        tree_traverse_preorder(A, tree_dfs_print, NULL);

        printf("Num Nodes = %d, Max Depth = %d\n", tree_count_nodes(A), tree_max_depth(A));
        K = tree_add_child(J);
        printf("Num Nodes = %d, Max Depth = %d\n", tree_count_nodes(A), tree_max_depth(A));


        tree_destroy(A);
        printf("Num Nodes = %d\n", tree_count_nodes(A));
        //printf("%d\n\n\n", root->num_children);
        tree_traverse_preorder(A, tree_dfs_print, NULL);
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