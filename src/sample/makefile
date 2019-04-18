sample : sample.o ApiGetLogInfo.o ApiGetServerInfo.o sample_elem3.o ApiPostAgentInfo.o dataaccess.o
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
	cd ../../../YaizuComLib/src/stkdata; make
	cp ../../../YaizuComLib/src/stkdata/stkdata.a .

	g++ -o sample sample.o ApiGetLogInfo.o ApiGetServerInfo.o sample_elem3.o ApiPostAgentInfo.o dataaccess.o stkpl.a stkwebapp.a commonfunc.a stksocket.a stkthread.a stkdata.a -std=c++1z -lstdc++fs -pthread -static-libgcc -static-libstdc++

ApiGetLogInfo.o : ApiGetLogInfo.cpp
	g++ -c ApiGetLogInfo.cpp -std=c++1z

ApiGetServerInfo.o : ApiGetServerInfo.cpp
	g++ -c ApiGetServerInfo.cpp -std=c++1z

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
	cd ../../../YaizuComLib/src/stkdata; make clean

all : clean sample

