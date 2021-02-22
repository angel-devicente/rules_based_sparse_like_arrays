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

typedef struct rba {
  int d1,d2,d3;   // dims
  int o1,o2,o3;   // order
  int rd1, rd2;   // rd1 rules have 3 values, rd2 rules have 5 values
  r1 *rules_d1;
  r2 *rules_d2;
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
  strip2D  *s;
} strip3D;


hid_t open_h5(const char *fname);
void close_h5(hid_t f_id);
void get_dims_h5(hid_t f_id, rba *a);
void get_order_h5(hid_t f_id, rba *a);
void get_rules(hid_t f_id, char *dname, rba *a);
void populate_array(rba *Ar, strip3D *A, hid_t f_id);
double val_array(int *ids, int* or, strip3D *A);
int performance_rbsla(strip3D *A, int *dims, int* or,long nhits);

#endif
