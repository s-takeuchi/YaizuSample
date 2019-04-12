sample : sample.o sample_elem1.o sample_elem2.o sample_elem3.o ApiPostAgentInfo.o dataaccess.o
	cd ../../../YaizuComLib/src/stkpl; make
	cp ../../../YaizuComLib/src/stkpl/stkpl.a .
	cd ../../../YaizuComLib/src/stkwebapp; make
	cp ../../../YaizuComLib/src/stkwebapp/stkwebapp.a .
	cd ../../../YaizuComLib/src/commonfunc; make
	cp ../../../YaizuComLib/src/commonfunc/commonfunc.a .
	cd ../../../YaizuComLib/src/stksocket; make
	cp ../../../YaizuComLib/src/stksocket/stksocket.a .
	cd ../../../YaizuComLib/src/stkthread; make
	cp ../../../YaizuComLib/src/stkthread/stkthread.a .
	g++ -o sample sample.o sample_elem1.o sample_elem2.o sample_elem3.o ApiPostAgentInfo.o dataaccess.o stkpl.a stkwebapp.a commonfunc.a stksocket.a stkthread.a  -std=c++1z -lstdc++fs -pthread -static-libgcc -static-libstdc++

sample_elem1.o : sample_elem1.cpp
	g++ -c sample_elem1.cpp -std=c++1z

sample_elem2.o : sample_elem2.cpp
	g++ -c sample_elem2.cpp -std=c++1z

sample_elem3.o : sample_elem3.cpp
	g++ -c sample_elem3.cpp -std=c++1z

ApiPostAgentInfo.o : ApiPostAgentInfo.cpp
	g++ -c ApiPostAgentInfo.cpp -std=c++1z

dataaccess.o : dataaccess.cpp
	g++ -c dataaccess.cpp -std=c++1z

sample.o : sample.cpp
	g++ -c sample.cpp -std=c++1z

clean :
	rm -f *.o *.a sample
	cd ../../../YaizuComLib/src/stkpl; make clean
	cd ../../../YaizuComLib/src/stkwebapp; make clean
	cd ../../../YaizuComLib/src/commonfunc; make clean
	cd ../../../YaizuComLib/src/stksocket; make clean
	cd ../../../YaizuComLib/src/stkthread; make clean

all : clean sample

