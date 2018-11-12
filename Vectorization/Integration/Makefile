CXX=icpc
CXXFLAGS=-c -qopenmp -qopt-report=5 -xMIC-AVX512
LDFLAGS=-qopenmp

OBJECTS=main.o library.o worker.o

integral: $(OBJECTS)
	$(CXX) $(LDFLAGS) -o integral $(OBJECTS)

all:	integral

run:	all
	./integral

queue:	all
	echo 'cd $$PBS_O_WORKDIR ; ./integral' | qsub -l nodes=1:flat -l walltime=00:03:00 -N numintegr

clean:
	rm -f *.optrpt *.o integral *~ numintegr.*
