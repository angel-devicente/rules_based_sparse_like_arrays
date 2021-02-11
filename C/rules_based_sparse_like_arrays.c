/* Copyright (C) 2021 by Angel de Vicente, angel.de.vicente@iac.es 
   https://github.com/angel-devicente/                          */

/******************************************************************
* Angel de Vicente 
*
* A simple test program to test rules based sparse like arrays
* 
******************************************************************
* TO DO: 
*   
******************************************************************/


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <float.h>
#include "hdf5.h"
#include "hdf5_hl.h"
#include "rules_based_sparse_like_arrays.h"

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#define AT __FILE__ ":" TOSTRING(__LINE__)
#define ERROR { fprintf(stderr, "ERROR at %s: %s\n", AT,strerror( errno )); exit(EXIT_FAILURE); }

#define R1_SIZE 3
#define R2_SIZE 5

herr_t op_func (hid_t loc_id, const char *name, void *operator_data);

/************************************************************** 
*
* Parsing HDF5 file functions
*
**************************************************************/

hid_t open_h5(const char *fname) {
  hid_t f_id;     /* file identifier */
  
  f_id = H5Fopen(fname, H5F_ACC_RDONLY, H5P_DEFAULT);
  if (f_id < 0) ERROR;
  return f_id;
}

void close_h5(hid_t f_id) {
  if (H5Fclose(f_id) < 0) ERROR;
}

void get_dims_h5(hid_t f_id, rba *a) {
  int i[3];

  H5LTget_attribute_int(f_id, "/", "dims", i);
  a->d1 = i[0];
  a->d2 = i[1];
  a->d3 = i[2];
}

void get_rules(hid_t f_id, char *dname, rba *a) {
  hid_t attr,aspace;
  herr_t ret;
  hsize_t sdim[2];

  double *m;
  
  attr = H5Aopen_name(f_id,dname);
  aspace = H5Aget_space(attr);
  ret = H5Sget_simple_extent_dims(aspace, sdim, NULL);

  if (! strcmp(dname,"d1")) {
    a->rd1 = (int)sdim[0];
    a->rules_d1 = malloc(sizeof(r1) * a->rd1);
    
    // create temp array for rules
    m = (double*)malloc(sizeof(double) * a->rd1 * R1_SIZE);
    if (H5LTget_attribute_double( f_id, "/", "d1",  m) < 0) ERROR;
    for (int r = 0; r < a->rd1; r++) {
      a->rules_d1[r].d1s = (int)m[r * R1_SIZE];
      a->rules_d1[r].d1e = (int)m[r * R1_SIZE + 1];
      a->rules_d1[r].val = m[r * R1_SIZE + 2];
    }
    free(m);
    
  } else if (! strcmp(dname,"d2")) {
    a->rd2 = (int)sdim[0];
    a->rules_d2 = malloc(sizeof(r2) * a->rd2);
    
    // create temp array for rules
    m = (double*)malloc(sizeof(double) * a->rd2 * R2_SIZE);
    if (H5LTget_attribute_double( f_id, "/", "d2",  m) < 0) ERROR;
    for (int r = 0; r < a->rd2; r++) {
      a->rules_d2[r].d1s = (int)m[r * R2_SIZE];
      a->rules_d2[r].d1e = (int)m[r * R2_SIZE + 1];
      a->rules_d2[r].d2s = (int)m[r * R2_SIZE + 2];
      a->rules_d2[r].d2e = (int)m[r * R2_SIZE + 3];
      a->rules_d2[r].val = m[r * R2_SIZE + 4];

    }
    free(m);
    
  } else {
    ERROR;
  }
  
  ret = H5Sclose(aspace);
  ret = H5Aclose(attr);
}



void populate_array(rba *Ar, strip3D *A, hid_t f_id) {
    
  // create first dimension of array
  if ( ! (A->s = (strip2D*)malloc(sizeof(strip2D)*Ar->d1))) printf("ERROR \n");
  for (int idx = 0 ; idx < Ar->d1 ; idx++) {
    A->s[idx].c = NULL ;
    A->s[idx].s = NULL ;
  }

  // apply d1 rules
  for (int rule = 0 ; rule < Ar->rd1 ; rule++) {
    for (int idx = Ar->rules_d1[rule].d1s ; idx <= Ar->rules_d1[rule].d1e ; idx++) {
      A->s[idx].c = malloc(sizeof(double));
      *(A->s[idx].c) = Ar->rules_d1[rule].val;
    }
  }
  
  // create second dimension of array
  for (int idx = 0 ; idx < Ar->d1 ; idx++) {
    if (A->s[idx].c == NULL) {
      if ( ! (A->s[idx].s = (strip1D*)malloc(sizeof(strip1D)*Ar->d2))) printf("ERROR \n");
      
      for (int idx2 = 0 ; idx2 < Ar->d2 ; idx2++) {
	A->s[idx].s[idx2].c = NULL;
	A->s[idx].s[idx2].s = NULL;
      }
    }
  }

  // apply d2 rules
  for (int rule = 0 ; rule < Ar->rd2 ; rule++) {
    for (int idx1 = Ar->rules_d2[rule].d1s ; idx1 <= Ar->rules_d2[rule].d1e ; idx1++) {
      for (int idx2 = Ar->rules_d2[rule].d2s ; idx2 <= Ar->rules_d2[rule].d2e ; idx2++) {
	A->s[idx1].s[idx2].c = malloc(sizeof(double));
	*(A->s[idx1].s[idx2].c) = Ar->rules_d2[rule].val;
      }
    }
  }


  // create third dimension of array
  for (int idx1 = 0 ; idx1 < Ar->d1 ; idx1++) {
    if (A->s[idx1].c != NULL) continue;

    for (int idx2 = 0 ; idx2 < Ar->d2 ; idx2++) {
      if (A->s[idx1].s[idx2].c != NULL) continue;

      if ( ! (A->s[idx1].s[idx2].s = (double*)malloc(sizeof(double)*Ar->d3))) printf("ERROR \n"); 

      // assign DBL_MAX to positions that didn't have a value specified with d1 or d2 rules.
      // these should be overwritten by actual values from regular datasets in the rules file.
      // If there are any DBL_MAX values at the end it will mean that our rules file didn't
      // cover all the pints of the array.
      for (int idx3 = 0 ; idx3 < Ar->d3 ; idx3++) {
	A->s[idx1].s[idx2].s[idx3] = DBL_MAX;
      }
    }
  }


  if (H5Giterate (f_id, "/", NULL, op_func, A)) printf("ERROR \n");
}



herr_t op_func (hid_t loc_id, const char *name, void *operator_data) {
    herr_t          status;
    H5G_stat_t      statbuf;
    int             dimd1[2], dimd2[2], dims[3];
    strip3D         *A;
    double          *m;
    
    A = (strip3D*) operator_data;
    
    status = H5Gget_objinfo (loc_id, name, 0, &statbuf);
    switch (statbuf.type) {
        case H5G_DATASET:
            printf ("Reading dataset: %s\n", name);
	    H5LTget_attribute_int(loc_id, name, "d1", dimd1);
	    H5LTget_attribute_int(loc_id, name, "d2", dimd2);
	    H5LTget_attribute_int(loc_id, "/", "dims", dims);

	    m = (double*)malloc(sizeof(double) * (dimd1[1]-dimd1[0]+1) * (dimd2[1]-dimd2[0]+1) * dims[2]);
	    if (H5LTread_dataset_double(loc_id, name,  m) < 0) ERROR;

	    int idxm = 0;
	    for (int idx1 = dimd1[0] ; idx1 <= dimd1[1] ; idx1++) {
	      for (int idx2 = dimd2[0] ; idx2 <= dimd2[1] ; idx2++) {
		for (int idx3 = 0 ; idx3 < dims[2] ; idx3++) {
		  //		  printf("Doing %d %d %d %d \n",idx1,idx2,idx3,idxm);
		  A->s[idx1].s[idx2].s[idx3] = m[idxm++];
		}
	      }
	    }
	    free(m);
            break;
        default:
            printf ( " ERROR: we are only expecting datasets: %s\n", name);
    }

    return 0;
}


void write_regular_file(strip3D *A, int *dims, const char *fname) {
  hid_t f_id;  
  double *m;
  int idxm;
  
  m = (double*)malloc(sizeof(double) * (dims[0] * dims[1] * dims[2]));

  idxm = 0;
  for (int idx1 = 0 ; idx1 < dims[0] ; idx1++) {
    for (int idx2 = 0 ; idx2 < dims[1] ; idx2++) {
      for (int idx3 = 0 ; idx3 < dims[2] ; idx3++) {
	//	printf("Writing: %d %d %d %d \n",idx1,idx2,idx3,idxm);
	m[idxm++] = val_array(idx1,idx2,idx3,A);
      }
    }
  }


  hsize_t dimst[3] ; dimst[0] = dims[0] ; dimst[1] = dims[1] ; dimst[2] = dims[2];
  if ((f_id = H5Fcreate(fname, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT)) < 0) ERROR;
  H5LTmake_dataset_double ( f_id, "/data", 3, dimst, m);
  close_h5(f_id);
  free(m);
		      
}

double val_array(int i1, int i2, int i3, strip3D *A) {
  if (A->s[i1].c) return *(A->s[i1].c);
  if (A->s[i1].s[i2].c) return *(A->s[i1].s[i2].c);
  return A->s[i1].s[i2].s[i3];
}


