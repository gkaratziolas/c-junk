#include "tree.h"

struct node *tree_create(void)
{
        struct node *np = malloc(sizeof(struct node));
        if (np == NULL)
                return NULL;
        np->parent = NULL;
        np->children = NULL;
        np->num_children = 0;
        return np;
}

struct node *tree_add_child(struct node *parent)
{
        // Allocate memory for new child
        struct node *child = malloc(sizeof(struct node));
        if (child == NULL)
                return NULL;

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

int tree_traverse_dfs(struct node *root, void (*callback)(struct node *, struct dfs_context *), void *additional_context)
{
        struct node *head = root;
        int zero = 0;

        struct stack *child_index_stack = stack_create(sizeof(int), DFS_INITIAL_STACK_DEPTH);
        int *stack_top_ptr = NULL;

        stack_push(child_index_stack, &zero);

        struct dfs_context ctx = {
                .dfs_stack = child_index_stack,
                .additional_context = additional_context
        };

        if (callback) {
                callback(head, &ctx);
        }

        while (1) {
                // If head has children remaining, push to stack and increase search depth
                if (*(int *)stack_top(child_index_stack) < head->num_children) {
                        // Broken link in tree
                        if (head->children[*(int *)stack_top(child_index_stack)]->parent != head) {
                                stack_destroy(child_index_stack);
                                return -1;
                        }

                        head = head->children[*(int *)stack_top(child_index_stack)];
                        stack_push(child_index_stack, &zero);

                        if (callback) {
                                callback(head, &ctx);
                        }
                        continue;
                }
                // Out of children, return to parent
                if (head->parent == NULL) {
                        stack_destroy(child_index_stack);
                        return 0;
                }
                head = head->parent;
                stack_pop(child_index_stack, NULL);
                (*(int *)stack_top(child_index_stack))++;
        }

        stack_destroy(child_index_stack);
        return 0;
cleanup:
        stack_destroy(child_index_stack);
        return -1;
}

