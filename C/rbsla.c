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
  hid_t f_id, rules_id;
  rba Ar;
  strip3D A;
  int dims[3], or[3];
  long nhits = 100000000;
  
  // Test 1
  printf("## Starting Test #1.\n");
  f_id = open_h5("../Test_Files/test1_rules.h5");

  get_dims_h5(f_id,&Ar);
  dims[0] = Ar.d1 ; dims[1] = Ar.d2 ; dims[2] = Ar.d3;
  printf("   Dimensions: %d %d %d\n",dims[0],dims[1],dims[2]);

  get_order_h5(f_id,&Ar);
  or[0] = Ar.o1 ; or[1] = Ar.o2 ; or[2] = Ar.o3;
  printf("   Order: %d %d %d\n",or[0],or[1],or[2]);
  
  rules_id = H5Gopen(f_id, "rules", H5P_DEFAULT);
  get_rules(rules_id, "d1", &Ar); 
  get_rules(rules_id, "d2", &Ar); 
  populate_array(&Ar, &A, f_id);  // this also adds the datasets that we find in the file */

  H5Gclose(rules_id);
  close_h5(f_id); 

#if defined VERF || defined VERFPython
  write_regular_file(&A,dims,or,"../Test_Files/test1_std_C.h5");
#endif

#ifdef PERF
  performance_rbsla(&A, dims,or, nhits);
#endif

  // Test 2
  printf("## Starting Test #2.\n");
  f_id = open_h5("../Test_Files/test2_rules.h5");

  get_dims_h5(f_id,&Ar);
  dims[0] = Ar.d1 ; dims[1] = Ar.d2 ; dims[2] = Ar.d3;
  printf("   Dimensions: %d %d %d\n",dims[0],dims[1],dims[2]);

  get_order_h5(f_id,&Ar);
  or[0] = Ar.o1 ; or[1] = Ar.o2 ; or[2] = Ar.o3;
  printf("   Order: %d %d %d\n",or[0],or[1],or[2]);
  
  rules_id = H5Gopen(f_id, "rules", H5P_DEFAULT);
  get_rules(rules_id, "d1", &Ar); 
  get_rules(rules_id, "d2", &Ar); 
  populate_array(&Ar, &A, f_id);  // this also adds the datasets that we find in the file */

  H5Gclose(rules_id);
  close_h5(f_id); 

#if defined VERF || defined VERFPython
  write_regular_file(&A,dims,or,"../Test_Files/test2_std_C.h5");
#endif

#ifdef PERF
  performance_rbsla(&A, dims,or, nhits);
#endif

  // Test 3
  printf("## Starting Test #3.\n");
  f_id = open_h5("../Test_Files/test3_rules.h5");

  get_dims_h5(f_id,&Ar);
  dims[0] = Ar.d1 ; dims[1] = Ar.d2 ; dims[2] = Ar.d3;
  printf("   Dimensions: %d %d %d\n",dims[0],dims[1],dims[2]);

  get_order_h5(f_id,&Ar);
  or[0] = Ar.o1 ; or[1] = Ar.o2 ; or[2] = Ar.o3;
  printf("   Order: %d %d %d\n",or[0],or[1],or[2]);
  
  rules_id = H5Gopen(f_id, "rules", H5P_DEFAULT);
  get_rules(rules_id, "d1", &Ar); 
  get_rules(rules_id, "d2", &Ar); 
  populate_array(&Ar, &A, f_id);  // this also adds the datasets that we find in the file */

  H5Gclose(rules_id);
  close_h5(f_id); 

#if defined VERF || defined VERFPython
  write_regular_file(&A,dims,or,"../Test_Files/test3_std_C.h5");
#endif

#ifdef PERF
  performance_rbsla(&A, dims,or, nhits);
#endif


  // Test 4
  printf("## Starting Test #4. \n");
  f_id = open_h5("../Test_Files/test4_rules.h5");

  get_dims_h5(f_id,&Ar);
  dims[0] = Ar.d1 ; dims[1] = Ar.d2 ; dims[2] = Ar.d3;
  printf("   Dimensions: %d %d %d\n",dims[0],dims[1],dims[2]);

  get_order_h5(f_id,&Ar);
  or[0] = Ar.o1 ; or[1] = Ar.o2 ; or[2] = Ar.o3;
  printf("   Order: %d %d %d\n",or[0],or[1],or[2]);
  
  rules_id = H5Gopen(f_id, "rules", H5P_DEFAULT);
  get_rules(rules_id, "d1", &Ar); 
  get_rules(rules_id, "d2", &Ar); 
  populate_array(&Ar, &A, f_id);  // this also adds the datasets that we find in the file */

  H5Gclose(rules_id);
  close_h5(f_id); 

#if defined VERF || defined VERFPython
  write_regular_file(&A,dims,or,"../Test_Files/test4_std_C.h5");
#endif

#ifdef PERF
  performance_rbsla(&A, dims,or, nhits);
#endif

  
  return 0;
}

