// optimized versions of matrix diagonal summing
#include "matvec.h"

// First implementation carelessly sets the sq to 0
// Primarily implements the algorithm given in specification
int matsquare_VER1(matrix_t mat, matrix_t matsq) {
  for(int i = 0; i<mat.rows; i++) {    
    for(int j=0; j<mat.cols; j++){
      MSET(matsq, i, j, 0);          // init all values to 0
    }
  }
  for(int i = 0; i<mat.rows; i++) {
    for(int j=0; j<mat.cols; j++){
      int lead = MGET(mat,i,j);           // lead vlue that will multiply all following values
      for (int k = 0; k<mat.rows; k++) {
        int mjk = MGET(mat, j, k);        // Get jk
        int cur = MGET(matsq, i, k);      // Get ik
        int new = cur + mjk*lead;         // calculate new val
        MSET(matsq, i, k, new);           // place in ik
      }
    }
  }
  return 0;  
}


// Second tries to access the memory less often, not very effective at reducing time really
int matsquare_VER2(matrix_t mat, matrix_t matsq) {
  for(int i = 0; i<mat.rows; i++) {
    for(int k = 0; k<mat.rows; k++) {
      MSET(matsq, i, k, 0);                 // initialize per row instead of all at one time. This is pointless.
    }
    for(int j=0; j<mat.cols; j++){
      int lead = MGET(mat,i,j);
      for (int k = 0; k<mat.rows; k++) {
        int mjk = MGET(mat, j, k);          // get jk
        int cur = MGET(matsq, i, k);        // get ik
        int new = cur + mjk*lead;           // calc new val
        MSET(matsq, i, k, new);             // place in ik
      }
    }
  }
  return 0;  
}

// Spec algorithm but don't use new... doesn't do much
int matsquare_VER3(matrix_t mat, matrix_t matsq) {
  for(int i = 0; i<mat.rows; i++) {
    for(int k = 0; k<mat.rows; k++) {
      MSET(matsq, i, k, 0);                   // init per row instead of all at one time.
    }
    for(int j=0; j<mat.cols; j++){
      int lead = MGET(mat,i,j);
      for (int k = 0; k<mat.rows; k++) {
        int mjk = MGET(mat, j, k);            // get jk
        int cur = MGET(matsq, i, k);          // get ik
        MSET(matsq, i, k, cur + mjk*lead);    // calc and place in ik
      }
    }
  }
  return 0;
}

// Spec algorithm w/ loop unrolling
int matsquare_VER4(matrix_t mat, matrix_t matsq) {
  for(int i = 0; i<mat.rows; i++) {
    for(int x = 0; x<mat.rows; x++) {
      MSET(matsq, i, x, 0);               // Initialize per row
    }
    for(int j=0; j<mat.cols; j++){
      int lead = MGET(mat,i,j);             // lead value to multiply with everything
      int k = 0;
      for (k = 0; k<mat.rows-4; k += 4) {   // 4x loop unrolling
        int mjk = MGET(mat, j, k);
        int cur = MGET(matsq, i, k);
        MSET(matsq, i, k, cur + mjk*lead);
        
        int mjk1 = MGET(mat, j, k+1);           // get jk
        int cur1 = MGET(matsq, i, k+1);         // get ik
        MSET(matsq, i, k+1, cur1 + mjk1*lead);  // place in ik

        int mjk2 = MGET(mat, j, k+2);
        int cur2 = MGET(matsq, i, k+2);
        MSET(matsq, i, k+2, cur2 + mjk2*lead);

        int mjk3 = MGET(mat, j, k+3);
        int cur3 = MGET(matsq, i, k+3);
        MSET(matsq, i, k+3, cur3 + mjk3*lead);
      }

      for(; k < mat.cols; k++) {            // finish off stragglers
        int mjk = MGET(mat, j, k);
        int cur = MGET(matsq, i, k);
        MSET(matsq, i, k, cur + mjk*lead);
      }
      
    }
  }
  return 0;
}

// Using the transpose of the matrix, we can access memory in an even nicer way (base algorithm but access everything by rows, no columns)
// Since transposing is O(n^2) we don't affect the multiplication runtime (for large n)
// since matrix multiplication is at LEAST O(n^2), but most often > O(n^2)
// Also we use MSET less in this version (at least during the 2nd bout of loops)
int matsquare_VER5(matrix_t mat, matrix_t matsq) {
  matrix_t transmat;
  matrix_init(&transmat, mat.rows, mat.cols); // Initialize the transpose matrix
  
  for(int i = 0; i < mat.rows; i++) {
    for(int j = 0; j < mat.cols; j++) {
      MSET(transmat, i, j, MGET(mat, j, i)); // Fill transpose matrix with required values
    }
  }

  for(int i = 0; i < mat.rows; i++) {
    for (int j = 0; j < mat.cols; j++) {
      int cur = 0;                             // Keep a running count
      for(int k = 0; k < mat.cols; k++) {
        cur += MGET(transmat, j, k)*MGET(mat, i, k); // Add the required multiplication to the sum
      }
      MSET(matsq, i, j, cur); // place sum in correct place in the matrix
    }
  }

  matrix_free_data(&transmat); // free transpose matrix to avoid memory errors

  return 0;
}

// transpose but with loop unrolling
int matsquare_VER6(matrix_t mat, matrix_t matsq) {
  matrix_t transmat;
  matrix_init(&transmat, mat.rows, mat.cols); // Initialize the transpose matrix
  
  for(int i = 0; i < mat.rows; i++) {
    for(int x = 0; x < mat.cols; x++) {
      MSET(transmat, i, x, MGET(mat, x, i)); // Fill transpose matrix with required values
    }
  }

  for(int i = 0; i < mat.rows; i++) {
    for (int j = 0; j < mat.cols; j++) {
      int cur = 0;                                       // Keep a running count
      int k = 0;
      for(k = 0; k < mat.cols-4; k+=4) {
        cur += MGET(transmat, j, k)*MGET(mat, i, k);     // Do the thing, but 4 times so that we unroll the loop
        cur += MGET(transmat, j, k+1)*MGET(mat, i, k+1);
        cur += MGET(transmat, j, k+2)*MGET(mat, i, k+2);
        cur += MGET(transmat, j, k+3)*MGET(mat, i, k+3);
      }
      for(; k < mat.cols; k++) {
        cur += MGET(transmat, j, k)*MGET(mat, i, k);     // Finish off the stragglers
      }

      MSET(matsq, i, j, cur);   // Place cur in the correct place in the return matrix
    }
  }

  matrix_free_data(&transmat); // free transpose matrix to avoid memory errors
  return 0;
}

int matsquare_OPTM(matrix_t mat, matrix_t matsq) {
  if(mat.rows != mat.cols ||                       // must be a square matrix to square it
     mat.rows != matsq.rows || mat.cols != matsq.cols)
  {
    printf("matsquare_OPTM: dimension mismatch\n");
    return 1;
  }
 

  // Call to some version of optimized code
  return matsquare_VER6(mat, matsq);
}
