/* Copyright (C) 2021 by Angel de Vicente, angel.de.vicente@iac.es 
   https://github.com/angel-devicente/                           */

/******************************************************************
* Angel de Vicente 
*
* A simple test function for rules_based_sparse_like_arrays.c
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
  strip3D A3D;
  strip5D A5D;

#ifdef PERF  
  long nhits = 100000000;
#endif

  /*----------------------------------------
   * 3D Tests
   *---------------------------------------*/ 
  
  // Test 1
  //-----------------------------------
  printf("## Starting Test #1.\n");
  f_id = open_h5("../Test_Files/test1_rules.h5");

  get_dims_order_h5(f_id,&Ar);
  printf("   Dimensions: %d %d %d\n",Ar.dims[0],Ar.dims[1],Ar.dims[2]);
  printf("   Order: %d %d %d\n",Ar.order[0],Ar.order[1],Ar.order[2]); 

  get_rules(f_id, &Ar);
  populate_array3D(&Ar, &A3D, f_id);  // this also adds the datasets that we find in the file 

  close_h5(f_id); 

#if defined VERF || defined VERFPython
  write_regular_file3D(&A3D,Ar.dims,Ar.order,"../Test_Files/test1_std_C.h5");
#endif

#ifdef PERF
  performance_rbsla3D(&A3D, Ar.dims,Ar.order, nhits);
#endif

  free_reg3D(&Ar,&A3D);
  free_rba(&Ar);
  

  // Test 2
  //-----------------------------------
  printf("## Starting Test #2.\n");
  f_id = open_h5("../Test_Files/test2_rules.h5");

  get_dims_order_h5(f_id,&Ar);
  printf("   Dimensions: %d %d %d\n",Ar.dims[0],Ar.dims[1],Ar.dims[2]);
  printf("   Order: %d %d %d\n",Ar.order[0],Ar.order[1],Ar.order[2]); 

  get_rules(f_id, &Ar);
  populate_array3D(&Ar, &A3D, f_id);  // this also adds the datasets that we find in the file 

  close_h5(f_id); 

#if defined VERF || defined VERFPython
  write_regular_file3D(&A3D,Ar.dims,Ar.order,"../Test_Files/test2_std_C.h5");
#endif

#ifdef PERF
  performance_rbsla3D(&A3D, Ar.dims,Ar.order, nhits);
#endif

  free_reg3D(&Ar,&A3D);
  free_rba(&Ar);


  // Test 3
  //-----------------------------------
  printf("## Starting Test #3.\n");
  f_id = open_h5("../Test_Files/test3_rules.h5");

  get_dims_order_h5(f_id,&Ar);
  printf("   Dimensions: %d %d %d\n",Ar.dims[0],Ar.dims[1],Ar.dims[2]);
  printf("   Order: %d %d %d\n",Ar.order[0],Ar.order[1],Ar.order[2]); 

  get_rules(f_id, &Ar);
  populate_array3D(&Ar, &A3D, f_id);  // this also adds the datasets that we find in the file 

  close_h5(f_id); 

#if defined VERF || defined VERFPython
  write_regular_file3D(&A3D,Ar.dims,Ar.order,"../Test_Files/test3_std_C.h5");
#endif

#ifdef PERF
  performance_rbsla3D(&A3D, Ar.dims,Ar.order, nhits);
#endif

  free_reg3D(&Ar,&A3D);
  free_rba(&Ar);


  // Test 4
  //-----------------------------------
  printf("## Starting Test #4. \n");
  f_id = open_h5("../Test_Files/test4_rules.h5");

  get_dims_order_h5(f_id,&Ar);
  printf("   Dimensions: %d %d %d\n",Ar.dims[0],Ar.dims[1],Ar.dims[2]);
  printf("   Order: %d %d %d\n",Ar.order[0],Ar.order[1],Ar.order[2]); 

  get_rules(f_id, &Ar);
  populate_array3D(&Ar, &A3D, f_id);  // this also adds the datasets that we find in the file 

  close_h5(f_id); 

#if defined VERF || defined VERFPython
  write_regular_file3D(&A3D,Ar.dims,Ar.order,"../Test_Files/test4_std_C.h5");
#endif

#ifdef PERF
  performance_rbsla3D(&A3D, Ar.dims,Ar.order, nhits);
#endif

  free_reg3D(&Ar,&A3D);
  free_rba(&Ar);


  /*----------------------------------------
   * 5D Tests
   *---------------------------------------*/ 
  
  // Test 5
  //-----------------------------------
  printf("## Starting Test #5.\n");
  f_id = open_h5("../Test_Files/test5_rules.h5");

  get_dims_order_h5(f_id,&Ar);
  printf("   Dimensions: %d %d %d %d %d\n",Ar.dims[0],Ar.dims[1],Ar.dims[2],Ar.dims[3],Ar.dims[4]);
  printf("   Order: %d %d %d %d %d\n",Ar.order[0],Ar.order[1],Ar.order[2],Ar.order[3],Ar.order[4]); 

  get_rules(f_id, &Ar);
  populate_array5D(&Ar, &A5D, f_id);  // this also adds the datasets that we find in the file 

  close_h5(f_id); 

#if defined VERF || defined VERFPython
  write_regular_file5D(&A5D,Ar.dims,Ar.order,"../Test_Files/test5_std_C.h5");
#endif

#ifdef PERF
  performance_rbsla5D(&A5D, Ar.dims,Ar.order, nhits);
#endif

  free_reg5D(&Ar,&A5D);
  free_rba(&Ar);


  // Test 6
  //-----------------------------------
  printf("## Starting Test #6.\n");
  f_id = open_h5("../Test_Files/test6_rules.h5");

  get_dims_order_h5(f_id,&Ar);
  printf("   Dimensions: %d %d %d %d %d\n",Ar.dims[0],Ar.dims[1],Ar.dims[2],Ar.dims[3],Ar.dims[4]);
  printf("   Order: %d %d %d %d %d\n",Ar.order[0],Ar.order[1],Ar.order[2],Ar.order[3],Ar.order[4]); 

  get_rules(f_id, &Ar);
  populate_array5D(&Ar, &A5D, f_id);  // this also adds the datasets that we find in the file 

  close_h5(f_id); 

#if defined VERF || defined VERFPython
  write_regular_file5D(&A5D,Ar.dims,Ar.order,"../Test_Files/test6_std_C.h5");
#endif

#ifdef PERF
  performance_rbsla5D(&A5D, Ar.dims,Ar.order, nhits);
#endif

  free_reg5D(&Ar,&A5D);
  free_rba(&Ar);

  
  return 0;
}

