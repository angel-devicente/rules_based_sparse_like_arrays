#ifndef RULES_BASED_SPARSE_LIKE_ARRAYS_H
#define RULES_BASED_SPARSE_LIKE_ARRAYS_H

typedef struct r1 {
  int d1s, d1e;
  double val;
} r1;

typedef struct r2 {
  int d1s, d1e, d2s, d2e;
  double val;
} r2;

typedef struct r3 {
  int d1s, d1e, d2s, d2e, d3s, d3e;
  double val;
} r3;

typedef struct r4 {
  int d1s, d1e, d2s, d2e, d3s, d3e, d4s, d4e;
  double val;
} r4;

typedef struct rba {
  int ndims;    // number of dimensions
  int *dims;    // dimensions (size given by ndims)
  int *order;   // order (size given by ndims)
  int *nrules;  // number for rules (size given by ndims - 1)
  r1 *rules_d1; // Actual rules for each type. Can be NULL for some
  r2 *rules_d2;
  r3 *rules_d3;
  r4 *rules_d4;
} rba;

typedef struct strip1D {
  double  *c;  // constant value
  double  *s;
} strip1D;

typedef struct strip2D {
  double  *c;  // constant value
  strip1D  *s;
} strip2D;

typedef struct strip3D {
  double  *c;  // constant value  
  strip2D  *s;
} strip3D;

typedef struct strip4D {
  double  *c;  // constant value  
  strip3D  *s;
} strip4D;

typedef struct strip5D {
  double  *c;  // constant value  
  strip4D  *s;
} strip5D;


hid_t open_h5(const char *fname);
void close_h5(hid_t f_id);
void get_dims_order_h5(hid_t f_id, rba *a);
void get_rules(hid_t f_id, rba *a);

void populate_array3D(rba *Ar, strip3D *A, hid_t f_id);
double val_array3D(int *ids, int* or, strip3D *A);
void write_regular_file3D(strip3D *A, int *dims, int *ord, const char *fname);
int performance_rbsla3D(strip3D *A, int *dims, int* or,long nhits);

void populate_array5D(rba *Ar, strip5D *A, hid_t f_id);
void write_regular_file5D(strip5D *A, int *dims, int *ord, const char *fname);
double val_array5D(int *ids, int* or, strip5D *A);
int performance_rbsla5D(strip5D *A, int *dims, int* or,long nhits);

int free_rba(rba *a);
int free_reg1D(rba *a, strip1D *A);
int free_reg2D(rba *a, strip2D *A);
int free_reg3D(rba *a, strip3D *A);
int free_reg4D(rba *a, strip4D *A);
int free_reg5D(rba *a, strip5D *A);

#endif
