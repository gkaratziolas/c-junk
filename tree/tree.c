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

int tree_delete_node(struct node *node)
{

}

int tree_traverse_dfs(struct node *root, void (*callback)(struct node *, struct dfs_context *), void *additional_context)
{
        struct node *head = root;
        int max_depth = 16;
        int *child_index_stack = malloc(sizeof(child_index_stack[0]) * max_depth);
        int *tmp;

        int depth = 0;
        child_index_stack[depth] = 0;

        struct dfs_context ctx = {
                .depth = &depth,
                .child_index_stack = child_index_stack,
                .additional_context = additional_context
        };

        if (callback) {
                callback(head, &ctx);
        }

        while (1) {
                // If head has children remaining, increase search depth
                if (child_index_stack[depth] < head->num_children) {
                        // Broken link in tree
                        if (head->children[child_index_stack[depth]]->parent != head) {
                                free(child_index_stack);
                                return -1;
                        }

                        head = head->children[child_index_stack[depth]];
                        depth++;
                        if (depth == max_depth) {
                                int *tmp = realloc(child_index_stack, sizeof(child_index_stack[0]) * max_depth * 2);
                                if (tmp == NULL) {
                                        free(child_index_stack);
                                        return -1;
                                }
                                child_index_stack = tmp;
                                ctx.child_index_stack = child_index_stack;
                                max_depth = max_depth * 2;
                        }
                        child_index_stack[depth] = 0;

                        if (callback) {
                                callback(head, &ctx);
                        }
                        continue;
                }
                // Out of children, return to parent
                if (head->parent == NULL) {
                        free(child_index_stack);
                        return 0;
                }
                head = head->parent;
                depth--;
                child_index_stack[depth]++;
        }

        free(child_index_stack);
        return 0;
}

