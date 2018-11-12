CXX=icpc
CXXFLAGS=-xMIC-AVX512 -qopenmp-simd -mkl
OPTRPT=-qopt-report=5

default : app

distribution.o : distribution.cc distribution.h
	icpc -c ${OPTRPT} ${CXXFLAGS} -o "$@" "$<"

diffusion.o : diffusion.cc distribution.o
	icpc -c ${OPTRPT} ${CXXFLAGS} -o "$@" "$<" distribution.o

app : main.cc diffusion.o distribution.o
	icpc ${OPTRPT} ${CXXFLAGS} -o "$@" "$<" diffusion.o distribution.o
queue:	app
	echo 'cd $$PBS_O_WORKDIR ; taskset -c 0 ./app $\ALPHA $\THRESHOLD' | qsub -l nodes=1:flat -l walltime=00:03:00 -N monte_carlo

clean :
	rm app diffusion.o distribution.o *.optrpt
