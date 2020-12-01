/* An implementation of a treemap, provides functionality and usability to the functions in treemap_funcs.c */

#include "treemap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    int echo = 0;                                // controls echoing, 0: echo off, 1: echo on
    if(argc > 1 && strcmp("-echo",argv[1])==0) { // turn echoing on via -echo command line option
        echo=1;
    }

    printf("TreeMap Editor\n");
    printf("Commands:\n");
    printf("  quit:            exit the program\n");
    printf("  print:           shows contents of the tree in reverse sorted order\n");
    printf("  add <key> <val>: inserts the given key/val into the tree, duplicate keys are ignored\n");
    printf("  get <key>:       prints FOUND if the name is in the tree, NOT FOUND otherwise\n");
    printf("  clear:           eliminates all key/vals from the tree\n");
    printf("  preorder:        prints contents of the tree in pre-order which is how it will be saved\n");
    printf("  save <file>:     writes the contents of the tree in pre-order to the given file\n");
    printf("  load <file>:     clears the current tree and loads the one in the given file\n");

    char cmd[128];
    treemap_t tree;
    int success;
    treemap_init(&tree);

    while(1) {
        printf("TM> ");
        success = fscanf(stdin, "%s", cmd);
        if(success==EOF) {
            printf("\n");
            treemap_clear(&tree);
            break;
        }

        if(strcmp("quit", cmd) == 0){     // check for quit command
            if(echo) {
                printf("quit\n");
            }
            treemap_clear(&tree);         // free all nodes
            break;                          // break from loop
        } 
        
        else if(strcmp("print", cmd) == 0) {    // check for print command
            if(echo) {
                printf("print\n");
            }
            treemap_print_revorder(&tree);
        } 

        else if(strcmp("add", cmd) == 0) {      // check for add command
            char key[128];                  // init. key and value to add
            char val[128];
            
            fscanf(stdin, "%s %s", key, val);   // scan in the key and value
                        
            if(echo) {
                printf("add %s %s\n", key, val);
            }

            success = treemap_add(&tree, key, val);     // Add in the key and val

            if (!success) {
                printf("modified existing key\n");      // The key already existed
            }

        }

        else if(strcmp("get", cmd) == 0) {      // check for get command

            fscanf(stdin, " %s", cmd);      // scan for target key (reuse cmd)

            if(echo) {
                printf("get %s\n", cmd);
            }

            char *val = treemap_get(&tree, cmd);

            if (val == NULL) {
                printf("NOT FOUND\n");          // key not found
            }   
            else {
                printf("FOUND: %s\n", val);     // print value of found key
            }
        }

        else if(strcmp("clear", cmd) == 0) {    // check for clear command
            if (echo) {
                printf("clear\n");
            }

            treemap_clear(&tree);               // clear tree
        }

        else if(strcmp("preorder", cmd) == 0) { // check for preorder command
            if (echo) {
                printf("preorder\n");
            }
            treemap_print_preorder(&tree);      // print in preorder
        }

        else if(strcmp("save", cmd) == 0) {     // check for save command

            fscanf(stdin, "%s", cmd);         // Scan in filename (reuse cmd)

            if (echo) {
                printf("save %s\n", cmd);
            }

            treemap_save(&tree, cmd);         // Save the tree into fname
        }

        else if (strcmp("load", cmd) == 0) {    // check for load command

            fscanf(stdin, "%s", cmd);           // Scan in filename 
            if (echo) {
                printf("load %s\n", cmd);
            }

            success = treemap_load(&tree, cmd); // To check if the file was loaded correctly

            if (!success) {
                printf("ERROR: could not open file '%s'\nload failed\n", cmd);
            }
        }

        else {                                  // command not in list of commands
            printf("unknown command\n");
        }
        
    }
}

