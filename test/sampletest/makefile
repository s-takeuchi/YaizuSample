LDFLAGS = -L/usr/local/lib64 -L/usr/local/lib
LIBS    = -lssl -lcrypto

sampletest : sampletest.o
	cd ../../../YaizuComLib/src/stkpl; make
	cp ../../../YaizuComLib/src/stkpl/stkpl.a .
	cd ../../../YaizuComLib/src/stkwebapp; make
	cp ../../../YaizuComLib/src/stkwebapp/stkwebapp.a .
	cd ../../../YaizuComLib/src/commonfunc; make
	cp ../../../YaizuComLib/src/commonfunc/commonfunc.a .
	cd ../../../YaizuComLib/src/stksocket; make
	cp ../../../YaizuComLib/src/stksocket/stksocket.a .

	g++ -o sampletest sampletest.o stkpl.a stkwebapp.a commonfunc.a stksocket.a -std=c++1z -lstdc++fs -pthread -static-libgcc -static-libstdc++ $(LDFLAGS) $(LIBS)


sampletest.o : sampletest.cpp
	g++ -c sampletest.cpp -std=c++1z

clean :
	rm -f *.o *.a sampletest
	cd ../../../YaizuComLib/src/stkpl; make clean
	cd ../../../YaizuComLib/src/stkwebapp; make clean
	cd ../../../YaizuComLib/src/commonfunc; make clean
	cd ../../../YaizuComLib/src/stksocket; make clean

all : clean sampletest

