/* Copyright (C) 2021 by Angel de Vicente, angel.de.vicente@iac.es 
   https://github.com/angel-devicente/                           */

/******************************************************************
* Angel de Vicente 
*
* A simple test function for rules_based_sparse_like_arrays.c
* 
******************************************************************
* TO DO: 
*
******************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include "hdf5.h"
#include "hdf5_hl.h"
#include "rules_based_sparse_like_arrays.h"

int main(int argc, char *argv[]) {
  hid_t f_id;
  rba Ar;
  strip3D A;
  int dims[3];

  // Test 1
  f_id = open_h5("../Test_Files/test1_rules.h5");
  get_dims_h5(f_id,&Ar);
  get_rules(f_id, "d1", &Ar);
  get_rules(f_id, "d2", &Ar);
  populate_array(&Ar, &A, f_id);  // this also adds the datasets that we find in the file
  close_h5(f_id);

#ifdef VERF  
  dims[0] = Ar.d1 ; dims[1] = Ar.d2 ; dims[2] = Ar.d3;
  write_regular_file(&A,dims,"../Test_Files/test1_std_C.h5");
#endif

  // Test 2
  f_id = open_h5("../Test_Files/test2_rules.h5");
  get_dims_h5(f_id,&Ar);
  get_rules(f_id, "d1", &Ar);
  get_rules(f_id, "d2", &Ar);
  populate_array(&Ar, &A, f_id);  // this also adds the datasets that we find in the file
  close_h5(f_id);

#ifdef VERF  
  dims[0] = Ar.d1 ; dims[1] = Ar.d2 ; dims[2] = Ar.d3;
  write_regular_file(&A,dims,"../Test_Files/test2_std_C.h5");
#endif

  // Test 3
  f_id = open_h5("../Test_Files/test3_rules.h5");
  get_dims_h5(f_id,&Ar);
  get_rules(f_id, "d1", &Ar);
  get_rules(f_id, "d2", &Ar);
  populate_array(&Ar, &A, f_id);  // this also adds the datasets that we find in the file
  close_h5(f_id);

#ifdef VERF    
  dims[0] = Ar.d1 ; dims[1] = Ar.d2 ; dims[2] = Ar.d3;
  write_regular_file(&A,dims,"../Test_Files/test3_std_C.h5");
#endif  
  
  return 0;
}

