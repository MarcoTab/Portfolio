// treemap_funcs.c: Provides a small library of functions that operate on
// binary search trees mapping strings keys to string values.

#include "treemap.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Initialize the given tree to have a null root and have size 0.
void treemap_init(treemap_t *tree) {
    tree->root = NULL;
    tree->size = 0;
}

// Inserts given key/value into a binary search tree. Uses an
// ITERATIVE (loopy) approach to insertion which starts a pointer at
// the root of the tree and changes its location until the correct
// insertion point is located. If the key given already exists in the
// tree, no new node is created; the existing value is changed to the
// parameter 'val' and 0 is returned.  If no node with the given key
// is found, a new node is created and with the given key/val, added
// to the tree, and 1 is returned. Makes use of strcpy() to ease
// copying characters between memory locations.
int treemap_add(treemap_t *tree, char key[], char val[]) {
    if(tree->root == NULL) {
        tree->root = malloc(sizeof(node_t));
        strcpy(tree->root->key, key);
        strcpy(tree->root->val, val);
        tree->root->left = NULL;
        tree->root->right = NULL;
        tree->size += 1;
        return 1;
    }
    node_t *current = tree->root;
    while (1) {
        if (strcmp(key, current->key) < 0) {                   // If the target key is smaller than the current key...
            if (current->left == NULL) {                        // And if the next left node is null...
                current->left = malloc(sizeof(node_t));         
                strcpy(current->left->key, key);               // Copy the key 
                strcpy(current->left->val, val);               // Copy the value
                current->left->left = NULL;                     // Initialize left and right nodes
                current->left->right = NULL;
                tree->size += 1;                                // Increment tree size
                return 1;
            } 
            else {                                              // Next left node isn't null, so travel to it and repeat
                current = current->left;    
            }
        } 
        else if (strcmp(key, current->key) > 0) {              // If the target key is larger than the current key...
            if (current->right == NULL) {                       // And if the next right node is null...
                current->right = malloc(sizeof(node_t));
                strcpy(current->right->key, key);              // Copy the key
                strcpy(current->right->val, val);              // Copy the value
                current->right->left = NULL;                   // Init. left and right nodes
                current->right->right = NULL;
                tree->size += 1;                               // Increment tree size
                return 1;
            }
            else {                                              
                current = current->right;                       // Next right node isn't null, so travel to it and repeat
            } 
        }
        else {                                                  // If the target key is exactly the current key...
            strcpy(current->val, val);                         // Copy the new value. No need to increment tree size since no new nodes are added.
            return 0;
        }
    }

    
}


// Searches the tree for given 'key' and returns its associated
// value. Uses an ITERATIVE (loopy) search approach which starts a
// pointer at the root of the tree and changes it until the search key
// is found or determined not to be in the tree. If a matching key is
// found, returns a pointer to its value. If no matching key is found,
// returns NULL.
char *treemap_get(treemap_t *tree, char key[]) {
    if(tree->root == NULL) {
        return NULL;
    }
    node_t *current = tree->root;
    while (1) {
        if (strcmp(key, current->key) < 0) {                        // If key is smaller than current key
            if (current->left == NULL) {                            // If the next left node is empty
                return NULL;                                        // The key must not be in the treemap
            }
            current = current->left;                                // Otherwise go to the next left node
        }
        else if (strcmp(key, current->key) > 0) {                   // If key is larger than current key
            if (current->right == NULL) {                           // If the next right node is empty
                return NULL;                                        // The key must not be in the treemap
            }
            current = current->right;                               // Otherwise go to the next right node
        }
        else {                                                      // If the key is exactly the same as the current key
            return current->val;                                    // Return the value of the current node
        }
    }
}

// Eliminate all nodes in the tree setting its contents empty. Uses
// recursive node_remove_all() function to free memory for all nodes.
void treemap_clear(treemap_t *tree) {
    node_remove_all(tree->root);        // Free the nodes of the tree starting from the root
    treemap_init(tree);                 // Reinitialize the tree for further use
}

// Recursive helper function which visits all nodes in a tree and
// frees the memory associated with them. This requires a post-order
// traversal: visit left tree, visit right tree, then free the cur
// node.
void node_remove_all(node_t *cur) {
    if (cur != NULL) {                  // Just so we don't try to free a NULL ptr
        if (cur->left != NULL) {
            node_remove_all(cur->left); // Recursively remove left side of subtree
        } 
        if (cur->right != NULL) {
            node_remove_all(cur->right); // Recursively remove right side of subtree
        }
        free(cur);                      // Free the root of this subtree
    }
}


// Prints the key/val pairs of the tree in reverse order at differing
// levels of indentation which shows all elements and their structure
// in the tree. Visually the tree can be rotated clockwise to see its
// structure. See the related node_print_revorder() for additional
// detals.
void treemap_print_revorder(treemap_t *tree) {
    node_print_revorder(tree->root, 0);         // Reverse print the tree, starting at depth 0
}


// Recursive helper function which prints all key/val pairs in the
// tree rooted at node 'cur' in reverse order. Traverses right
// subtree, prints cur node's key/val, then traverses left tree.
// Parameter 'indent' indicates how far to indent (2 spaces per indent
// level).
//
// For example: a if the root node "El" is passed into the function
// and it has the following structure:
// 
//         ___El->strange_____     
//        |                   |   
// Dustin->corny       ___Mike->stoic
//                    |              
//               Lucas->brash     
// 
// the recursive calls will print the following output:
// 
//   Mike -> stoic                 # root->right
//     Lucas -> brash              # root->right->left
// El -> strange                   # root
//   Dustin -> corny               # root->left
void node_print_revorder(node_t *cur, int indent){
    if (cur != NULL) {
        if (cur->right != NULL) {
            node_print_revorder(cur->right, indent+1);  // Recursively print the right side of the tree
        }

        for (int i = 0; i < indent; i++) {
            printf("  ");                               // Indent to print the node at the correct depth
        }
    
        printf("%s -> %s\n", cur->key, cur->val);       // Print the node

        if (cur->left != NULL) {
            node_print_revorder(cur->left, indent+1);   // Recursively print the left side of the tree
        }
    }
    
}


// Print all the data in the tree in pre-order with indentation
// corresponding to the depth of the tree. Makes use of
// node_write_preorder() for this.
void treemap_print_preorder(treemap_t *tree) {
    if (tree->root != NULL) {                       // Since we are directly referencing the root of the tree, we must make sure it is not NULL before trying to print it
        node_write_preorder(tree->root, stdout, 0); // Print the entire tree in preorder (root then left then right)
    }   
}


// Saves the tree by opening the named file, writing the tree to it in
// pre-order with node_write_preorder(), then closing the file.
void treemap_save(treemap_t *tree, char *fname) {
    FILE *fout = fopen(fname, "w");                 // First open the file
    if (tree->root != NULL) {
        node_write_preorder(tree->root, fout, 0);   // Similar to tree_print_preorder except we use fout so it writes to a file
    }
    fclose(fout);                                   // Close the file
}


// Recursive helper function which writes/prints the tree in pre-order
// to the given open file handle. The parameter depth gives how far to
// indent node data, 2 spaces per unit depth. Depth increases by 1 on
// each recursive call. The function prints the cur node data,
// traverses the left tree, then traverses the right tree.
void node_write_preorder(node_t *cur, FILE *out, int depth) {
    if (cur != NULL) {
        for (int i = 0; i < depth; i++) {
            fprintf(out, "  ");
        }
        fprintf(out, "%s %s\n", cur->key, cur->val);    // If the current node isn't NULL, print the correct indentation and then the node
    }
    if (cur->left != NULL) {
        node_write_preorder(cur->left, out, depth+1);   // Recursively write the left subtree
    }
    if (cur->right != NULL) {
        node_write_preorder(cur->right, out, depth+1);  // Recursively write the right subtree
    }
}


// Clears the given tree then loads new elements to it from the
// named. Repeated calls to treemap_insert() are used to add strings read
// from the file.  If the tree is stored in pre-order in the file, its
// exact structure will be restored.  Returns 1 if the tree is loaded
// successfully and 0 if opening the named file fails in which case no
// changes are made to the tree.
int treemap_load(treemap_t *tree, char *fname ) {
    FILE *fin = fopen(fname, "r");
    char key[128];
    char val[128];
    if (fin != NULL) {                                  // Run only if the file opened correctly
        treemap_clear(tree);
        while(1) {
            if(fscanf(fin, "%s %s", key, val) == EOF) { // Scan in the next to values as long as it's not the EOF
                break;
            }
            treemap_add(tree, key, val);                // Add key and val to the tree
        }
        fclose(fin);                                    // Close the file once we're done
        return 1;                                       // We've opened the file, return 1
    }
    return 0;                                           // Don't need to close a file I never opened... Thus return 0.
    
}

