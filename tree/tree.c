#include "tree.h"

static int _tree_traverse_dfs(struct node *root, 
                              void (*prefix_callback)(struct node *, struct dfs_context *),
                              void (*postfix_callback)(struct node *, struct dfs_context *),
                              void *additional_context);
static void _node_destroy(struct node *node, struct dfs_context *ctx);
static void _count_node(struct node *node, struct dfs_context *ctx);
static void _node_compare_depth(struct node *node, struct dfs_context *ctx);

struct node *tree_create(void)
{
        struct node *root = malloc(sizeof(struct node));
        if (root == NULL)
                return NULL;
        root->parent = NULL;
        root->children = NULL;
        root->num_children = 0;
        return root;
}

int tree_destroy(struct node *root)
{
        if (root == NULL)
                return -1;
        if (root->parent != NULL)
                root->parent->num_children = 0;
        tree_traverse_postorder(root, _node_destroy, NULL);
        free(root);
        root = NULL;
        return 0;
}

static void _node_destroy(struct node *node, struct dfs_context *ctx)
{
        free(node);
        node = NULL;
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

static int _tree_traverse_dfs(struct node *root, 
                              void (*prefix_callback)(struct node *, struct dfs_context *),
                              void (*postfix_callback)(struct node *, struct dfs_context *),
                              void *additional_context)
{
        if (root == NULL)
                return -1;
        struct node *head = root;
        struct node *parent = root->parent;
        struct node *root_parent = root->parent;

        struct stack *child_index_stack = stack_create(sizeof(int), DFS_INITIAL_STACK_DEPTH);
        int zero = 0;
        stack_push(child_index_stack, &zero);

        struct dfs_context ctx = {
                .stack = child_index_stack,
                .additional_context = additional_context
        };

        if (prefix_callback)
                prefix_callback(head, &ctx);

        while (1) {
                int *stack_top_ptr = (int *)stack_top(child_index_stack);

                // If head has children remaining, push to stack and increase search depth
                if (*stack_top_ptr < head->num_children) {
                        // Broken link in tree
                        if (head->children[*stack_top_ptr]->parent != head)
                                goto err;

                        head = head->children[*stack_top_ptr];
                        // Push to stack, verify push succeeds
                        if (stack_push(child_index_stack, &zero) != 0)
                                goto err;

                        if (prefix_callback)
                                prefix_callback(head, &ctx);

                        continue;
                }

                // Out of children, return to parent
                parent = head->parent;

                if (postfix_callback)
                        postfix_callback(head, &ctx);

                // Check whether we've returned to root
                if (parent == root_parent)
                        goto cleanup;

                head = parent;
                
                stack_pop(child_index_stack, NULL);
                (*(int *)stack_top(child_index_stack))++;
        }
cleanup:
        stack_destroy(child_index_stack);
        return 0;
err:
        stack_destroy(child_index_stack);
        return -1;
}

int tree_traverse_preorder(struct node *root, 
                           void (*callback)(struct node *, struct dfs_context *),
                           void *additional_context)
{
        return _tree_traverse_dfs(root, callback, NULL, additional_context);
}

int tree_traverse_postorder(struct node *root, 
                            void (*callback)(struct node *, struct dfs_context *),
                            void *additional_context)
{
        return _tree_traverse_dfs(root, NULL, callback, additional_context);
}

int tree_traverse_prepost(struct node *root, 
                          void (*prefix_callback)(struct node *, struct dfs_context *),
                          void (*postfix_callback)(struct node *, struct dfs_context *),
                          void *additional_context)
{
        return _tree_traverse_dfs(root, prefix_callback, postfix_callback, additional_context);
}

int tree_count_nodes(struct node *root)
{
        if (root == NULL)
                return 0;
        int count = 0;
        tree_traverse_preorder(root, _count_node, &count);
        return count;
}

static void _count_node(struct node *node, struct dfs_context *ctx)
{
        if (node == NULL)
                return;
        (*((int *)ctx->additional_context))++;
        return;
}

int tree_max_depth(struct node *root)
{
        if (root == NULL)
                return 0;
        int max_depth = 0;
        tree_traverse_preorder(root, _node_compare_depth, &max_depth);
        return ++max_depth;
}

static void _node_compare_depth(struct node *node, struct dfs_context *ctx)
{
        if (node == NULL)
                return;
        if (ctx->stack->top > *(int *)ctx->additional_context)
                *(int *)ctx->additional_context = ctx->stack->top;
        return;
}