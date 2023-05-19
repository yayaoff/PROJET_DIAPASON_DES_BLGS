#ifndef LU_H // le header guard
#define LU_H

#include "matrix.h"

#define EPS 1e-8 // tolérance pour un pivot nul

typedef struct{
    int argc;
    int k;
    double r1;
    double r2;
    double e;
    double l; 
    double meshSizeFactor;
    char *filename;
    char **argv;
}param_t;

// Calcule la décomposition LU de A "in-place":
// la partie triangulaire supérieure (diagonale comprise) de A correspond à U,
// la partie triangulaire inférieure correspond à L
// (qui a des 1 sur sa diagonale).
// Retourne 0 si tout s'est bien passé, ou -1 si un pivot nul (< EPS) est rencontré
// (il ne faut pas implémenter le pivotage)
int lu(Matrix * A);

// Idem pour une matrice bande
int lu_band(BandMatrix * A);

// Résout le système L*U*x = y,
// avec LU la décomposition obtenue par la fonction lu().
// L'algorithme est "in-place": le vecteur y est modifié pour contenir la solution x.
// Retourne 0 si tout s'est bien passé.
int solve(Matrix * LU, double * y);

// Idem pour une matrice bande
int solve_band(BandMatrix * LU, double * y);

int bandwidth(double **A, int m);

double* permute_vect_inv(double* v,int* perm, int size) ;

double* permute_vect(double* v,int* perm, int n_nodes);

double** permute_matrix(double** matrix, int* perm, int size);

int compute_permutation(int * perm, double * coord, int n_nodes) ;

int cmp_y(const void * a, const void * b);

int cmp(const void * a, const void * b) ;

int is_symmetric(Matrix * K) ;


#endif