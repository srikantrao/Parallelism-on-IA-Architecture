#### Stencil Code - Convolutions Demo

Running the convolutions code ( in this case an Edge Filter) to evaluate performance improvement with SIMD vectorization.

There is **4x** improvement in the runtime of the code using traditional SSE.

It increases to **16x** using AVX registers (512 bits).

Quick command to run using `Intel Compiler`

```console
icpc -c -qopenmp -qopt-report=5 -xMIC_AVX512 -o stencil main.o image.o stencil.o
```
