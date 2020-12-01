#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "search.h"
#include <time.h>

// Complete this main to benchmark the search algorithms outlined in
// the project specification
int main(int argc, char *argv[]){
  int lin_search = 0;       // a
  int link_search = 0;      // l
  int bin_search = 0;       // b
  int bin_tree_search = 0;  // t

  if (argc < 2) {
    printf("usage: ./search_benchmark <minpow> <maxpow> <repeats> [which]\n");
    printf(" which is a combination of:\n");
    printf("  a : Linear Array Search\n");
    printf("  l : Linked List Search\n");
    printf("  b : Binary Array Search\n");
    printf("  t : Binary Tree Search\n");
    printf("  (default all)\n");
  }
  else if (argc < 5) {
    lin_search = 1;
    link_search = 1;
    bin_search = 1;
    bin_tree_search = 1;

  } else {
    char *algs_string = argv[4];
    for (int i = 0; i < strlen(algs_string); i++) {
      if (algs_string[i] == 'a'){
        lin_search = 1;
      } else if (algs_string[i] == 'l'){
        link_search = 1;
      } else if (algs_string[i] == 'b'){
        bin_search = 1;
      } else if (algs_string[i] == 't'){
        bin_tree_search = 1;
      }
    }
  }

  int minpow = atoi(argv[1]);
  int maxpow = atoi(argv[2]);
  int repeats = atoi(argv[3]);

  clock_t begin, end;

  int curlen = 1; // or 2^0

  for (int i = 0; i < minpow-1; i++) { // Get starting power
    curlen *= 2;
  }

  printf("%8s %8s","LENGTH","SEARCHES");

                      //vvvvvvvvvvvvvv-------------- Total # of tests to perform
  for (int i = -1; i < (maxpow - minpow + 1); i++) {
    if (i >= 0) {
      printf("%8d %8d", curlen, 2*curlen*repeats);
    }
    if (lin_search) {                                               // Run linear search
      if (i<0) {
        printf("%11s", "array");
      } else {
        int *bench = make_evens_array(curlen);                      // init benchmarking array
        begin = clock();                                            // Start the clock
        for (int j = 0; j < repeats; j++) {
          for (int k = 0; k < curlen; k++) {
            linear_array_search(bench, curlen, k);
          }
        }
        end = clock();                                              // Stop the clock
        double lin_time = ((double) (end-begin)) / CLOCKS_PER_SEC;  // Get time
        printf(" %1.4e", lin_time);                                 // print time
        free(bench);
      }

    } 
    
    if (link_search) {                                              // do linked linear search
      if (i<0) {
        printf("%11s", "list");
      } else {
        list_t *bench = make_evens_list(curlen);                    // init benchmarking list
        begin = clock();                                            // start the clock
        for (int j = 0; j < repeats; j++) {
          for (int k = 0; k < curlen; k++) {
            linkedlist_search(bench, curlen, k);
          }
        }
        end = clock();                                              // stop the clock

        double link_time = ((double) (end-begin)) / CLOCKS_PER_SEC; // Get time
        printf(" %1.4e", link_time);                                // print time
        list_free(bench);
      }

    } 
    
    if (bin_search) {                                               // do binary array search
      if (i<0) {
        printf("%11s", "binary");
      } else {
        int *bench = make_evens_array(curlen);                      // Init sorted array
        begin = clock();                                            // start the clock
        for (int j = 0; j < repeats; j++) {
          for (int k = 0; k < curlen; k++) {
            binary_array_search(bench, curlen, k);
          }
        }
        end = clock();                                              // stop the clock

        double bin_time = ((double) (end-begin)) / CLOCKS_PER_SEC;  // Get time
        printf(" %1.4e", bin_time);                                 // print time
        free(bench);
      }

    } 
    
    if (bin_tree_search) {                                               // do binary tree search
      if (i<0) {
        printf("%11s", "tree");
      } else {
        bst_t *bench = make_evens_tree(curlen);
        begin = clock();                                                 // start the clock
        for (int j = 0; j < repeats; j++) {
          for (int k = 0; k < curlen; k++) {
            binary_tree_search(bench, curlen, k);
          }
        }
        end = clock();                                                   // stop the clock

        double bin_tree_time = ((double) (end-begin)) / CLOCKS_PER_SEC;  // Get time
        printf(" %1.4e", bin_tree_time);                                 // print time
        bst_free(bench);
      }
      
    }

    printf("\n");

    curlen *= 2;

  }
  

  return 0;
}
