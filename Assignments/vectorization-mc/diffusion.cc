#include <mkl.h>
#include "distribution.h"


//vectorize this function based on instruction on the lab page
int diffusion(const int n_particles, 
              const int n_steps, 
              const float x_threshold,
              const float alpha, 
              VSLStreamStatePtr rnStream) {
  int n_escaped=0;
  // Store the Distances of each particle -- Initialize to Zero 
  float x[n_particles];
  x[0:n_particles]=0.0f;
  
  // Store Random Numbers for each particle 
  float rn[n_particles];
  //Intel MKL function to generate random numbers
  vsRngUniform(VSL_RNG_METHOD_UNIFORM_STD, rnStream, n_particles, rn, -1.0, 1.0);
 
  // Outer loop will be over n steps 
  for (int i = 0; i < n_steps; i++) {

    for (int j = 0; j < n_particles; j++) { 
      
      x[j] += dist_func(alpha, rn[j]);
	  
	  //if (x[j] > x_threshold) n_escaped++; 
    }
  }

  for(int i = 0; i < n_particles; i++)
	if(x[i] > x_threshold) n_escaped++;
 
  return n_escaped;
}
