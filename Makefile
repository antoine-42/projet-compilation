CXX=g++-9
GCC=gcc-9

CFLAGS=-g -O3 -Wall -I/usr/lib/gcc/x86_64-pc-linux-gnu/9.3.0/plugin/include    -pthread -Wl,-rpath -Wl,/usr/lib/openmpi -Wl,--enable-new-dtags -L/usr/lib/openmpi -lmpi_cxx -lmpi
CXXFLAGS=-g -O3 -Wall -I/usr/lib/gcc/x86_64-pc-linux-gnu/9.3.0/plugin/include -fno-rtti    -pthread -Wl,-rpath -Wl,/usr/lib/openmpi -Wl,--enable-new-dtags -L/usr/lib/openmpi -lmpi_cxx -lmpi
OBJS=plugin.o
EXEC=plugin.so

plugin.o: plugin.cpp
	$(GCC) $(CXXFLAGS) -fPIC -c $< -o $@

plugin.so: $(OBJS)
	$(GCC) $(CXXFLAGS) -shared $< -o $@

test: $(EXEC)
	$(GCC) $(CFLAGS) -fplugin=./plugin.so ./test2.c

clean:
	rm -rf $(EXE) *.o *.so *.out
