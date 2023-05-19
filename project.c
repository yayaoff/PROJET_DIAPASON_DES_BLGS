#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include "../gmsh-sdk/include/gmshc.h"
#include "../gmsh-4.11.1-Windows64-sdk/include/gmshc.h"
#include "headers/matrix.h"
#include "headers/elasticity.h"
#include "math.h"
#include "headers/lu.h"
#include "headers/design.h"
#include "headers/opti.h"
// #include "headers/freq.h"

#define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

int main (int argc, char *argv[]) {

  if (argc < 3){
    printf("Usage: \n"
			"./project <k> <out> <half>\n" 
			"---------------------------- \n\n"
			"- k is the number of frequencies to compute. \n "
			"- out is the output file to write the frequencies. \n "
      "- <half> is 1 if half designed, 0 otherwise"
      "\n");
		return -1;
  } 

  int tag_half = atoi(argv[3]);
  printf("tag half = : %d\n",tag_half);

  // We want fabs(freq-f_target) <= EPS

  // Parameters to optimize : r1, r2, e, l
  // Ici, on optimise le paramètre r1 et on fixe les autres arbitrairement-> Tester pour les 4 et voir
  // pour quel paramètre on converge le plus vite ?
  // Rmq : attention aux contraintes : r1 < r2
  param_t *param_init = malloc(sizeof(param_t));
  param_init->argc = argc; param_init->argv = argv; param_init->k = atoi(argv[1]);
  param_init->r1=6e-3; param_init->r2=11e-3; param_init->e=38e-3; param_init->l=82e-3; param_init->meshSizeFactor=0.3;param_init->filename=NULL;

  // Compute frequencies 

  // If half designed:
  if (tag_half==1){
    double* frequencies = compute_band_freq_half(0,param_init);
    printf("\n------------------------------------------------------\n");
    printf("Frequence before optimisation = %f\n",frequencies[0]);
    printf("Frequence target = %f\n",f_target);
    printf("------------------------------------------------------\n");
    double lower_bound = 1e-6; double upper_bound = 99e-3;
    double param_biss = bissection_half_method(lower_bound,upper_bound,param_init);
    // Tester pour le nouveau paramètre
    param_init->l = param_biss;
    double* new_frequencies = compute_band_freq_half(1,param_init);
    printf("Freq final = %f\n",new_frequencies[0]);
    // for(int mode_k=0; mode_k < param->k; k++) printf("Frequence for mode %d = %f\n",mode_k, new_frequencies[mode_k]);
  }
  else{
      // double* frequencies = compute_freq(0,param_init);
      double* frequencies = compute_band_freq(0,param_init);
      printf("\n------------------------------------------------------\n");
      printf("Frequence before optimisation = %f\n",frequencies[0]);
      printf("Frequence target = %f\n",f_target);
      printf("------------------------------------------------------\n");
      double lower_bound = 1e-6; double upper_bound = 99e-3;
      double param_biss = bissection_method(lower_bound,upper_bound,param_init);
      // Tester pour le nouveau paramètre
      param_init->l = param_biss;
      double* new_frequencies = compute_band_freq(1,param_init);
      printf("Freq final = %f\n",new_frequencies[0]);
      // for(int mode_k=0; mode_k < param->k; k++) printf("Frequence for mode %d = %f\n",mode_k, new_frequencies[mode_k]);
  }
  return 0;
}