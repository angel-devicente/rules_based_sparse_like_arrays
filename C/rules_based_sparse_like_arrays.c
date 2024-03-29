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
#include <time.h>
#include "hdf5.h"
#include "hdf5_hl.h"
#include "rules_based_sparse_like_arrays.h"

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#define AT __FILE__ ":" TOSTRING(__LINE__)
#define ERROR { fprintf(stderr, "ERROR at %s: %s\n", AT,strerror( errno )); exit(EXIT_FAILURE); }

#define R1_SIZE 3
#define R2_SIZE 5
#define R3_SIZE 7
#define R4_SIZE 9

herr_t op_func3D (hid_t loc_id, const char *name, void *operator_data);
herr_t op_func5D (hid_t loc_id, const char *name, void *operator_data);

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

void get_dims_order_h5(hid_t f_id, rba *a) {
  H5LTget_attribute_int(f_id, "/", "ndims", &(a->ndims));
  a->dims  = malloc(sizeof(int) * a->ndims);
  a->order = malloc(sizeof(int) * a->ndims);
  
  H5LTget_attribute_int(f_id, "/", "dims", a->dims);
  H5LTget_attribute_int(f_id, "/", "order", a->order);

#ifdef VERFPython
  for (int i = 0; i < a->ndims ; i++) {
    a->order[i] = i;
  }
#endif
}

void get_rules(hid_t f_id, rba *a) {
  hid_t rules_id;
  hid_t dset,aspace;
  hsize_t sdim[2];
  double *m; 

  char *d_n[4] = {"d1","d2","d3","d4"};

  a->nrules = malloc(sizeof(int) * (a->ndims - 1));
  
  rules_id = H5Gopen(f_id, "rules", H5P_DEFAULT); 

  // If ndims=3 we want di to become: 0,1     (i.e. "d1" and "d2")
  // If ndims=5 we want di to become: 0,1,2,3 (i.e. "d1", "d2", "d3" and "d4")
  for (int di = 0 ; di < a->ndims - 1 ; di++) {
      // get dimensions of dataset
    dset = H5Dopen(rules_id,d_n[di],H5P_DEFAULT);
    aspace = H5Dget_space(dset);
    H5Sget_simple_extent_dims(aspace, sdim, NULL);
    H5Sclose(aspace);
    H5Dclose(dset);

    switch (di) {
    case 0:
      a->nrules[di] = (int)sdim[0];
      a->rules_d1 = malloc(sizeof(r1) * a->nrules[di]);

      m = (double*)malloc(sizeof(double) * a->nrules[di] * R1_SIZE);
      if (H5LTread_dataset_double( rules_id, d_n[di],  m) < 0) ERROR;
      for (int r = 0; r < a->nrules[di]; r++) {
	a->rules_d1[r].d1s = (int)m[r * R1_SIZE];
	a->rules_d1[r].d1e = (int)m[r * R1_SIZE + 1];
	a->rules_d1[r].val = m[r * R1_SIZE + 2];
      }
      free(m);
      break;
    case 1:
      a->nrules[di] = (int)sdim[0];
      a->rules_d2 = malloc(sizeof(r2) * a->nrules[di]);

      m = (double*)malloc(sizeof(double) * a->nrules[di] * R2_SIZE);
      if (H5LTread_dataset_double( rules_id, d_n[di],  m) < 0) ERROR;
      for (int r = 0; r < a->nrules[di]; r++) {
	a->rules_d2[r].d1s = (int)m[r * R2_SIZE];
	a->rules_d2[r].d1e = (int)m[r * R2_SIZE + 1];
	a->rules_d2[r].d2s = (int)m[r * R2_SIZE + 2];
	a->rules_d2[r].d2e = (int)m[r * R2_SIZE + 3];
	a->rules_d2[r].val = m[r * R2_SIZE + 4];
      }
      free(m);
      break;
    case 2:
      a->nrules[di] = (int)sdim[0];
      a->rules_d3 = malloc(sizeof(r3) * a->nrules[di]);

      m = (double*)malloc(sizeof(double) * a->nrules[di] * R3_SIZE);
      if (H5LTread_dataset_double( rules_id, d_n[di],  m) < 0) ERROR;
      for (int r = 0; r < a->nrules[di]; r++) {
	a->rules_d3[r].d1s = (int)m[r * R3_SIZE];
	a->rules_d3[r].d1e = (int)m[r * R3_SIZE + 1];
	a->rules_d3[r].d2s = (int)m[r * R3_SIZE + 2];
	a->rules_d3[r].d2e = (int)m[r * R3_SIZE + 3];
	a->rules_d3[r].d3s = (int)m[r * R3_SIZE + 4];
	a->rules_d3[r].d3e = (int)m[r * R3_SIZE + 5];
	a->rules_d3[r].val = m[r * R3_SIZE + 6];
      }
      free(m);
      break;
    case 3:
      a->nrules[di] = (int)sdim[0];
      a->rules_d4 = malloc(sizeof(r4) * a->nrules[di]);

      m = (double*)malloc(sizeof(double) * a->nrules[di] * R4_SIZE);
      if (H5LTread_dataset_double( rules_id, d_n[di],  m) < 0) ERROR;
      for (int r = 0; r < a->nrules[di]; r++) {
	a->rules_d4[r].d1s = (int)m[r * R4_SIZE];
	a->rules_d4[r].d1e = (int)m[r * R4_SIZE + 1];
	a->rules_d4[r].d2s = (int)m[r * R4_SIZE + 2];
	a->rules_d4[r].d2e = (int)m[r * R4_SIZE + 3];
	a->rules_d4[r].d3s = (int)m[r * R4_SIZE + 4];
	a->rules_d4[r].d3e = (int)m[r * R4_SIZE + 5];
	a->rules_d4[r].d4s = (int)m[r * R4_SIZE + 6];
	a->rules_d4[r].d4e = (int)m[r * R4_SIZE + 7];
	a->rules_d4[r].val = m[r * R4_SIZE + 8];
      }
      free(m);
      break;
    default:
      // should not be here!
      ERROR;
    }
  }

  H5Gclose(rules_id); 
}



int free_rba(rba *a) {
  free(a->dims);
  free(a->order);
  free(a->nrules);
  for (int di = 0 ; di < a->ndims - 1 ; di++) {
    switch (di) {
    case 0:
      free(a->rules_d1);
      break;
    case 1:
      free(a->rules_d2);
      break;
    case 2:
      free(a->rules_d3);
      break;
    case 3:
      free(a->rules_d4);
      break;
    default:
      ERROR;
    }
  }

  return 0;
}


int free_reg1D(rba *ar, strip1D *A) {
  if (A->c) free(A->c);
  if (A->s) free(A->s);

  return 0;
}

int free_reg2D(rba *ar, strip2D *A) {
  int d = ar->ndims - 2;
  
  if (A->c) free(A->c);

  if (A->s) {
    for (int di = 0 ; di < ar->dims[d] ; di++) {
      free_reg1D(ar,&(A->s[di]));  
    }
    free(A->s);
  }

  return 0;
}

int free_reg3D(rba *ar, strip3D *A) {
  int d = ar->ndims - 3;
  
  if (A->c) free(A->c);

  if (A->s) {
    for (int di = 0 ; di < ar->dims[d] ; di++) {
      free_reg2D(ar,&(A->s[di]));  
    }
    free(A->s);
  }

  return 0;
}

int free_reg4D(rba *ar, strip4D *A) {
  int d = ar->ndims - 4;
  
  if (A->c) free(A->c);

  if (A->s) {
    for (int di = 0 ; di < ar->dims[d] ; di++) {
      free_reg3D(ar,&(A->s[di]));  
    }
    free(A->s);
  }

  return 0;
}

int free_reg5D(rba *ar, strip5D *A) {
  int d = ar->ndims - 5;
  
  if (A->c) free(A->c);

  if (A->s) {
    for (int di = 0 ; di < ar->dims[d] ; di++) {
      free_reg4D(ar,&(A->s[di]));  
    }
    free(A->s);
  }

  return 0;
}


  /*----------------------------------------
   * 3D Functions
   *---------------------------------------*/ 

void populate_array3D(rba *Ar, strip3D *A, hid_t f_id) {
  A->c = NULL;
  
  // create first dimension of array
  if ( ! (A->s = (strip2D*)malloc(sizeof(strip2D)*Ar->dims[0]))) printf("ERROR \n");
  for (int idx = 0 ; idx < Ar->dims[0] ; idx++) {
    A->s[idx].c = NULL ;
    A->s[idx].s = NULL ;
  }
  
  // apply d1 rules
  for (int rule = 0 ; rule < Ar->nrules[0] ; rule++) {
    for (int idx = Ar->rules_d1[rule].d1s ; idx <= Ar->rules_d1[rule].d1e ; idx++) {
      A->s[idx].c = malloc(sizeof(double));
      *(A->s[idx].c) = Ar->rules_d1[rule].val;
    }
  }
  
  // create second dimension of array
  for (int idx = 0 ; idx < Ar->dims[0] ; idx++) {
    if (A->s[idx].c == NULL) {
      if ( ! (A->s[idx].s = (strip1D*)malloc(sizeof(strip1D)*Ar->dims[1]))) printf("ERROR \n");
  
      for (int idx2 = 0 ; idx2 < Ar->dims[1] ; idx2++) {
	A->s[idx].s[idx2].c = NULL;
	A->s[idx].s[idx2].s = NULL;
      }
    }
  }
  
  // apply d2 rules
  for (int rule = 0 ; rule < Ar->nrules[1] ; rule++) {
    for (int idx1 = Ar->rules_d2[rule].d1s ; idx1 <= Ar->rules_d2[rule].d1e ; idx1++) {
      for (int idx2 = Ar->rules_d2[rule].d2s ; idx2 <= Ar->rules_d2[rule].d2e ; idx2++) {
	A->s[idx1].s[idx2].c = malloc(sizeof(double));
	*(A->s[idx1].s[idx2].c) = Ar->rules_d2[rule].val;
      }
    }
  }
  
  
  // create third dimension of array
  for (int idx1 = 0 ; idx1 < Ar->dims[0] ; idx1++) {
    if (A->s[idx1].c != NULL) continue;
  
    for (int idx2 = 0 ; idx2 < Ar->dims[1] ; idx2++) {
      if (A->s[idx1].s[idx2].c != NULL) continue;
  
      if ( ! (A->s[idx1].s[idx2].s = (double*)malloc(sizeof(double)*Ar->dims[2]))) printf("ERROR \n");
  
      // assign DBL_MAX to positions that didn't have a value specified with d1 or d2 rules.
      // these should be overwritten by actual values from regular datasets in the rules file.
      // If there are any DBL_MAX values at the end it will mean that our rules file didn't
      // cover all the points of the array.
      for (int idx3 = 0 ; idx3 < Ar->dims[2] ; idx3++) {
	A->s[idx1].s[idx2].s[idx3] = DBL_MAX;
      }
    }
  }
  
  
  if (H5Giterate (f_id, "/dsets", NULL, op_func3D, A)) printf("ERROR \n");
}


herr_t op_func3D (hid_t loc_id, const char *name, void *operator_data) {
  H5G_stat_t      statbuf;
  int             dimd1[2], dimd2[2], dims[3];
  strip3D         *A;
  double          *m;
    
  A = (strip3D*) operator_data;
    
  H5Gget_objinfo (loc_id, name, 0, &statbuf);
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


void write_regular_file3D(strip3D *A, int *dims, int *ord, const char *fname) {
  hid_t f_id;
  double *m;
  int idxm;
  int ids[3];
  
  m = (double*)malloc(sizeof(double) * (dims[0] * dims[1] * dims[2]));
  
  idxm = 0;
  for (int idx1 = 0 ; idx1 < dims[ord[0]] ; idx1++) {
    for (int idx2 = 0 ; idx2 < dims[ord[1]] ; idx2++) {
      for (int idx3 = 0 ; idx3 < dims[ord[2]] ; idx3++) {
	//	printf("Writing: %d %d %d %d \n",idx1,idx2,idx3,idxm);
	ids[0] = idx1 ; ids[1] = idx2 ; ids[2] = idx3;
	m[idxm++] = val_array3D(ids,ord,A);
      }
    }
  }
  
  hsize_t dimst[3] ; dimst[0] = dims[0] ; dimst[1] = dims[1] ; dimst[2] = dims[2];
  if ((f_id = H5Fcreate(fname, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT)) < 0) ERROR;
  H5LTmake_dataset_double (f_id, "/data", 3, dimst, m);
  close_h5(f_id);
  free(m);
}

double val_array3D(int *ids, int *ord, strip3D *A) {
  if (A->s[ids[ord[0]]].c) return *(A->s[ids[ord[0]]].c);
  if (A->s[ids[ord[0]]].s[ids[ord[1]]].c) return *(A->s[ids[ord[0]]].s[ids[ord[1]]].c);
  return A->s[ids[ord[0]]].s[ids[ord[1]]].s[ids[ord[2]]];
}


int performance_rbsla3D(strip3D *A, int *dims, int *or, long nhits) {
  double *A_d;
  long idxm;
  int *hits;
  int ids[3];
  double sum;
  
  // timing
  clock_t t1,t2;
  double cpu_time = 0.0;
  
  // First, let's create the dense array
  A_d = (double*)malloc(sizeof(double) * (dims[0] * dims[1] * dims[2]));
  
  idxm = 0;
  for (int idx1 = 0 ; idx1 < dims[or[0]] ; idx1++) {
    for (int idx2 = 0 ; idx2 < dims[or[1]] ; idx2++) {
      for (int idx3 = 0 ; idx3 < dims[or[2]] ; idx3++) {
	//	printf("Writing: %d %d %d %d \n",idx1,idx2,idx3,idxm);
	ids[0] = idx1 ; ids[1] = idx2 ; ids[2] = idx3;
	A_d[idxm++] = val_array3D(ids,or,A);
      }
    }
  }
  
  // Let's create a list of random points to access in the array
  hits = (int*)malloc(sizeof(int) * nhits * 3);
  srand(time(NULL));   // Initialization, should only be called once.
  
  for (long idx = 0 ; idx < nhits ; idx++) {
    hits[3*idx]   = rand() % dims[0];
    hits[3*idx+1] = rand() % dims[1];
    hits[3*idx+2] = rand() % dims[2];
  }
  
  
  sum = 0;
  t1 = clock();
  for (long idx = 0 ; idx < nhits ; idx++) {
    sum += A_d[hits[3*idx]*dims[1]*dims[2] + hits[3*idx+1]*dims[2] + hits[3*idx+2]];
  }
  t2 = clock(); cpu_time = t2-t1;
  printf("Dense array sum: %10.6f (%f seconds)\n", sum, (double)cpu_time/CLOCKS_PER_SEC);
  
  
  sum = 0;
  t1 = clock();
  for (long idx = 0 ; idx < nhits ; idx++) {
    ids[0] = hits[3*idx] ; ids[1] = hits[3*idx+1] ; ids[2] = hits[3*idx+2];
    sum += val_array3D(ids,or,A);
  }
  t2 = clock(); cpu_time = t2-t1;
  printf("RBSLA array sum: %10.6f (%f seconds)\n\n", sum, (double)cpu_time/CLOCKS_PER_SEC);
  
  
  return 0;
}


  /*----------------------------------------
   * 5D Functions
   *---------------------------------------*/ 

void populate_array5D(rba *Ar, strip5D *A, hid_t f_id) {
  A->c = NULL;
  
  // create first dimension of array
  if ( ! (A->s = (strip4D*)malloc(sizeof(strip4D)*Ar->dims[0]))) printf("ERROR \n");
  for (int idx = 0 ; idx < Ar->dims[0] ; idx++) {
    A->s[idx].c = NULL ;
    A->s[idx].s = NULL ;
  }
  
  // apply d1 rules
  for (int rule = 0 ; rule < Ar->nrules[0] ; rule++) {
    for (int idx = Ar->rules_d1[rule].d1s ; idx <= Ar->rules_d1[rule].d1e ; idx++) {
      A->s[idx].c = malloc(sizeof(double));
      *(A->s[idx].c) = Ar->rules_d1[rule].val;
    }
  }

  
  // create second dimension of array
  for (int idx = 0 ; idx < Ar->dims[0] ; idx++) {
    if (A->s[idx].c == NULL) {
      if ( ! (A->s[idx].s = (strip3D*)malloc(sizeof(strip3D)*Ar->dims[1]))) printf("ERROR \n");
  
      for (int idx2 = 0 ; idx2 < Ar->dims[1] ; idx2++) {
	A->s[idx].s[idx2].c = NULL;
	A->s[idx].s[idx2].s = NULL;
      }
    }
  }
  
  // apply d2 rules
  for (int rule = 0 ; rule < Ar->nrules[1] ; rule++) {
    for (int idx1 = Ar->rules_d2[rule].d1s ; idx1 <= Ar->rules_d2[rule].d1e ; idx1++) {
      for (int idx2 = Ar->rules_d2[rule].d2s ; idx2 <= Ar->rules_d2[rule].d2e ; idx2++) {
	A->s[idx1].s[idx2].c = malloc(sizeof(double));
	*(A->s[idx1].s[idx2].c) = Ar->rules_d2[rule].val;
      }
    }
  }
  

  // create third dimension of array
  for (int idx1 = 0 ; idx1 < Ar->dims[0] ; idx1++) {
    if (A->s[idx1].c == NULL) {
      for (int idx2 = 0 ; idx2 < Ar->dims[1] ; idx2++) {
	if (A->s[idx1].s[idx2].c == NULL) {
	  if ( ! (A->s[idx1].s[idx2].s = (strip2D*)malloc(sizeof(strip2D)*Ar->dims[2]))) printf("ERROR \n");
  
	  for (int idx3 = 0 ; idx3 < Ar->dims[2] ; idx3++) {
	    A->s[idx1].s[idx2].s[idx3].c = NULL;
	    A->s[idx1].s[idx2].s[idx3].s = NULL;
	  }
	}
      }
    }
  }
  
  // apply d3 rules
  for (int rule = 0 ; rule < Ar->nrules[2] ; rule++) {
    for (int idx1 = Ar->rules_d3[rule].d1s ; idx1 <= Ar->rules_d3[rule].d1e ; idx1++) {
      for (int idx2 = Ar->rules_d3[rule].d2s ; idx2 <= Ar->rules_d3[rule].d2e ; idx2++) {
	for (int idx3 = Ar->rules_d3[rule].d3s ; idx3 <= Ar->rules_d3[rule].d3e ; idx3++) {
	  A->s[idx1].s[idx2].s[idx3].c = malloc(sizeof(double));
	  *(A->s[idx1].s[idx2].s[idx3].c) = Ar->rules_d3[rule].val;
	}
      }
    }
  }


  // create fourth dimension of array
  for (int idx1 = 0 ; idx1 < Ar->dims[0] ; idx1++) {
    if (A->s[idx1].c == NULL) {
      for (int idx2 = 0 ; idx2 < Ar->dims[1] ; idx2++) {
	if (A->s[idx1].s[idx2].c == NULL) {
	  for (int idx3 = 0 ; idx3 < Ar->dims[2] ; idx3++) {
	    if (A->s[idx1].s[idx2].s[idx3].c == NULL) {

	      if ( ! (A->s[idx1].s[idx2].s[idx3].s = (strip1D*)malloc(sizeof(strip1D)*Ar->dims[3]))) printf("ERROR \n");
  
	      for (int idx4 = 0 ; idx4 < Ar->dims[3] ; idx4++) {
		A->s[idx1].s[idx2].s[idx3].s[idx4].c = NULL;
		A->s[idx1].s[idx2].s[idx3].s[idx4].s = NULL;
	      }
	    }
	  }
	}
      }
    }
  }
  
  // apply d4 rules
  for (int rule = 0 ; rule < Ar->nrules[3] ; rule++) {
    for (int idx1 = Ar->rules_d4[rule].d1s ; idx1 <= Ar->rules_d4[rule].d1e ; idx1++) {
      for (int idx2 = Ar->rules_d4[rule].d2s ; idx2 <= Ar->rules_d4[rule].d2e ; idx2++) {
	for (int idx3 = Ar->rules_d4[rule].d3s ; idx3 <= Ar->rules_d4[rule].d3e ; idx3++) {
	  for (int idx4 = Ar->rules_d4[rule].d4s ; idx4 <= Ar->rules_d4[rule].d4e ; idx4++) {
	    A->s[idx1].s[idx2].s[idx3].s[idx4].c = malloc(sizeof(double));
	    *(A->s[idx1].s[idx2].s[idx3].s[idx4].c) = Ar->rules_d4[rule].val;
	  }
	}
      }
    }
  }


  // create fifth dimension of array
  for (int idx1 = 0 ; idx1 < Ar->dims[0] ; idx1++) {
    if (A->s[idx1].c != NULL) continue;
    for (int idx2 = 0 ; idx2 < Ar->dims[1] ; idx2++) {
      if (A->s[idx1].s[idx2].c != NULL) continue;
      for (int idx3 = 0 ; idx3 < Ar->dims[2] ; idx3++) {
	if (A->s[idx1].s[idx2].s[idx3].c != NULL) continue;
	for (int idx4 = 0 ; idx4 < Ar->dims[3] ; idx4++) {
	  if (A->s[idx1].s[idx2].s[idx3].s[idx4].c != NULL) continue;

	  if ( ! (A->s[idx1].s[idx2].s[idx3].s[idx4].s = (double*)malloc(sizeof(double)*Ar->dims[4]))) printf("ERROR \n");
  
	  // assign DBL_MAX to positions that didn't have a value specified with d1 or d2 rules.
	  // these should be overwritten by actual values from regular datasets in the rules file.
	  // If there are any DBL_MAX values at the end it will mean that our rules file didn't
	  // cover all the points of the array.
	  for (int idx5 = 0 ; idx5 < Ar->dims[4] ; idx5++) {
	    A->s[idx1].s[idx2].s[idx3].s[idx4].s[idx5] = DBL_MAX;
	  }
	}
      }
    }
  }
  
  if (H5Giterate (f_id, "/dsets", NULL, op_func5D, A)) printf("ERROR \n");
}


herr_t op_func5D (hid_t loc_id, const char *name, void *operator_data) {
  herr_t          status;
  H5G_stat_t      statbuf;
  int             dimd1[2], dimd2[2], dimd3[2], dimd4[2], dims[5];
  strip5D         *A;
  double          *m;
  
  A = (strip5D*) operator_data;
  
  status = H5Gget_objinfo (loc_id, name, 0, &statbuf);
  switch (statbuf.type) {
  case H5G_DATASET:
    printf ("Reading dataset: %s\n", name);
    H5LTget_attribute_int(loc_id, name, "d1", dimd1);
    H5LTget_attribute_int(loc_id, name, "d2", dimd2);
    H5LTget_attribute_int(loc_id, name, "d3", dimd3);
    H5LTget_attribute_int(loc_id, name, "d4", dimd4);
    H5LTget_attribute_int(loc_id, "/", "dims", dims);
  
    m = (double*)malloc(sizeof(double) * (dimd1[1]-dimd1[0]+1) * (dimd2[1]-dimd2[0]+1) * \
			(dimd3[1]-dimd3[0]+1) * (dimd4[1]-dimd4[0]+1) * \
			dims[4]);
    if (H5LTread_dataset_double(loc_id, name,  m) < 0) ERROR;
  
    int idxm = 0;
    for (int idx1 = dimd1[0] ; idx1 <= dimd1[1] ; idx1++) {
      for (int idx2 = dimd2[0] ; idx2 <= dimd2[1] ; idx2++) {
	for (int idx3 = dimd3[0] ; idx3 <= dimd3[1] ; idx3++) {
	  for (int idx4 = dimd4[0] ; idx4 <= dimd4[1] ; idx4++) {
	    for (int idx5 = 0 ; idx5 < dims[4] ; idx5++) {
	      //		  printf("Doing %d %d %d %d %d %d \n",idx1,idx2,idx3,idx4,idx5,idxm);
	      A->s[idx1].s[idx2].s[idx3].s[idx4].s[idx5] = m[idxm++];
	    }
	  }
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


void write_regular_file5D(strip5D *A, int *dims, int *ord, const char *fname) {
  hid_t f_id;
  double *m;
  int idxm;
  int ids[5];
  
  m = (double*)malloc(sizeof(double) * (dims[0] * dims[1] * dims[2] * dims[3] * dims[4]));
  
  idxm = 0;
  for (int idx1 = 0 ; idx1 < dims[ord[0]] ; idx1++) {
    for (int idx2 = 0 ; idx2 < dims[ord[1]] ; idx2++) {
      for (int idx3 = 0 ; idx3 < dims[ord[2]] ; idx3++) {
	for (int idx4 = 0 ; idx4 < dims[ord[3]] ; idx4++) {
	  for (int idx5 = 0 ; idx5 < dims[ord[4]] ; idx5++) {
	    //	printf("Writing: %d %d %d %d %d %d \n",idx1,idx2,idx3,idx4,idx5,idxm);
	    ids[0] = idx1 ; ids[1] = idx2 ; ids[2] = idx3; ids[3] = idx4; ids[4] = idx5;
	    m[idxm++] = val_array5D(ids,ord,A);
	  }
	}
      }
    }
  }
  
  hsize_t dimst[5] ; dimst[0] = dims[0] ; dimst[1] = dims[1] ; dimst[2] = dims[2]; dimst[3] = dims[3];dimst[4] = dims[4];
  if ((f_id = H5Fcreate(fname, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT)) < 0) ERROR;
  H5LTmake_dataset_double (f_id, "/data", 5, dimst, m);
  close_h5(f_id);
  free(m);
}

double val_array5D(int *ids, int *ord, strip5D *A) {
  if (A->s[ids[ord[0]]].c) return *(A->s[ids[ord[0]]].c);

  if (A->s[ids[ord[0]]].s[ids[ord[1]]].c) return *(A->s[ids[ord[0]]].s[ids[ord[1]]].c);

  if (A->s[ids[ord[0]]].s[ids[ord[1]]].s[ids[ord[2]]].c)
    return *(A->s[ids[ord[0]]].s[ids[ord[1]]].s[ids[ord[2]]].c);

  if (A->s[ids[ord[0]]].s[ids[ord[1]]].s[ids[ord[2]]].s[ids[ord[3]]].c)
    return *(A->s[ids[ord[0]]].s[ids[ord[1]]].s[ids[ord[2]]].s[ids[ord[3]]].c);
  
  return A->s[ids[ord[0]]].s[ids[ord[1]]].s[ids[ord[2]]].s[ids[ord[3]]].s[ids[ord[4]]];
}


int performance_rbsla5D(strip5D *A, int *dims, int *or, long nhits) {
  double *A_d;
  long idxm;
  int *hits;
  int ids[5];
  double sum;
  
  // timing
  clock_t t1,t2;
  double cpu_time = 0.0;
  
  // First, let's create the dense array
  A_d = (double*)malloc(sizeof(double) * (dims[0] * dims[1] * dims[2] * dims[3] * dims[4]));
  
  idxm = 0;
  for (int idx1 = 0 ; idx1 < dims[or[0]] ; idx1++) {
    for (int idx2 = 0 ; idx2 < dims[or[1]] ; idx2++) {
      for (int idx3 = 0 ; idx3 < dims[or[2]] ; idx3++) {
	for (int idx4 = 0 ; idx4 < dims[or[3]] ; idx4++) {
	  for (int idx5 = 0 ; idx5 < dims[or[4]] ; idx5++) {
	    //	printf("Writing: %d %d %d %d %d %d\n",idx1,idx2,idx3,idx4,idx5,idxm);
	    ids[0] = idx1 ; ids[1] = idx2 ; ids[2] = idx3; ids[3] = idx4; ids[4] = idx5;
	    A_d[idxm++] = val_array5D(ids,or,A);
	  }
	}
      }
    }
  }
  
  // Let's create a list of random points to access in the array
  hits = (int*)malloc(sizeof(int) * nhits * 5);
  srand(time(NULL));   // Initialization, should only be called once.
  
  for (long idx = 0 ; idx < nhits ; idx++) {
    hits[5*idx]   = rand() % dims[0];
    hits[5*idx+1] = rand() % dims[1];
    hits[5*idx+2] = rand() % dims[2];
    hits[5*idx+3] = rand() % dims[3];
    hits[5*idx+4] = rand() % dims[4];
  }
  
  
  sum = 0;
  t1 = clock();
  for (long idx = 0 ; idx < nhits ; idx++) {
    sum += A_d[hits[5*idx]*dims[1]*dims[2]*dims[3]*dims[4] + \
	       hits[5*idx+1]*dims[2]*dims[3]*dims[4] + \
	       hits[5*idx+2]*dims[3]*dims[4] + \
	       hits[5*idx+3]*dims[4] + \
	       hits[5*idx+4]];
  }
  t2 = clock(); cpu_time = t2-t1;
  printf("Dense array sum: %10.6f (%f seconds)\n", sum, (double)cpu_time/CLOCKS_PER_SEC);
  
  
  sum = 0;
  t1 = clock();
  for (long idx = 0 ; idx < nhits ; idx++) {
    ids[0] = hits[5*idx] ; ids[1] = hits[5*idx+1] ; ids[2] = hits[5*idx+2];
    ids[3] = hits[5*idx+3] ; ids[4] = hits[5*idx+4] ;
    sum += val_array5D(ids,or,A);
  }
  t2 = clock(); cpu_time = t2-t1;
  printf("RBSLA array sum: %10.6f (%f seconds)\n\n", sum, (double)cpu_time/CLOCKS_PER_SEC);
  
  
  return 0; 
}
