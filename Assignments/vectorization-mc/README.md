## Vectorization of Random Walk simulations

#### Running the Code
Run the MakeFile on a platform that supports AVX512 to compile the code using Intel C++ Compiler.
```
>> make
```

This code simulates a `Random Walk` by performing a 1D Monte Carlo Simulation. There are `n_particles` number of particles, and each particle moves a certain distance based on a `dist_func` that randomly calculates the distance traveled by that particle.

If at the end of `n_steps`, a particle has crossed a particular threshold `x_threshold`, it is considered to have escaped.

The initial code was not vectorized and therefore had very slow performance.

The following optimizations were performed to speed up computation through vectorization.

- The `dist_func` was explicitly vectorized by updating the declaration and definition to -

```cpp
// In the distribution.h file --
#pragma omp declare simd
float dist_func(const float alpha, const float rn);

// In the distribution.cc file --
#pragma omp declare simd
float dist_func(const float alpha, const float rn) {
  return delta_max*sinf(alpha*rn)*expf(-rn*rn);
}
```

- The inner loop of `diffusion.cc` was vectorized by removing all the dependencies, namely interchanging the loops and moving the `if` statement out of the inner loop.

```cpp
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
```

The potential speed-up can be veiwed in the ``*.optrpt` files. An example is shown below -

```console
LOOP BEGIN at diffusion.cc(32,3)
   remark #15305: vectorization support: vector length 16
   remark #15399: vectorization support: unroll factor set to 8
   remark #15309: vectorization support: normalized vectorization overhead 0.364
   remark #15300: LOOP WAS VECTORIZED
   remark #15448: unmasked aligned unit stride loads: 1
   remark #15475: --- begin vector cost summary ---
   remark #15476: scalar cost: 12
   remark #15477: vector cost: 0.680
   remark #15478: estimated potential speedup: 13.130
   remark #15488: --- end vector cost summary ---
LOOP END

```
