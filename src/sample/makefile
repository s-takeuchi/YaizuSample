LDFLAGS = -L/usr/local/lib64 -L/usr/local/lib
LIBS    = -lssl -lcrypto

sample : sample.o ApiBase.o ApiGetServerInfo.o ApiPostServerInfo.o ApiGetAgentInfo.o ApiPostAgentInfo.o ApiGetCommandForStatus.o ApiGetCommandForOperation.o ApiGetCommand.o ApiPostCommand.o ApiDeleteCommand.o ApiGetFile.o ApiPostFile.o dataaccess.o
	cd ../../../YaizuComLib/src/stkpl; make
	cp ../../../YaizuComLib/src/stkpl/stkpl.a .
	cd ../../../YaizuComLib/src/stkwebapp; make
	cp ../../../YaizuComLib/src/stkwebapp/stkwebapp.a .
	cd ../../../YaizuComLib/src/stkwebapp_um; make
	cp ../../../YaizuComLib/src/stkwebapp_um/stkwebapp_um.a .
	cd ../../../YaizuComLib/src/commonfunc; make
	cp ../../../YaizuComLib/src/commonfunc/commonfunc.a .
	cd ../../../YaizuComLib/src/stksocket; make
	cp ../../../YaizuComLib/src/stksocket/stksocket.a .
	cd ../../../YaizuComLib/src/stkthread; make
	cp ../../../YaizuComLib/src/stkthread/stkthread.a .
	cd ../../../YaizuComLib/src/stkdata; make
	cp ../../../YaizuComLib/src/stkdata/stkdata.a .

	g++ -o sample sample.o ApiBase.o ApiGetServerInfo.o ApiPostServerInfo.o ApiGetAgentInfo.o ApiPostAgentInfo.o ApiGetCommandForStatus.o ApiGetCommandForOperation.o ApiGetCommand.o ApiPostCommand.o ApiDeleteCommand.o ApiGetFile.o ApiPostFile.o dataaccess.o stkpl.a stkwebapp.a stkwebapp_um.a commonfunc.a stksocket.a stkthread.a stkdata.a -std=c++1z -lstdc++fs -pthread -static-libgcc -static-libstdc++ $(LDFLAGS) $(LIBS)

ApiBase.o : ApiBase.cpp
	g++ -c ApiBase.cpp -std=c++1z

ApiGetServerInfo.o : ApiGetServerInfo.cpp
	g++ -c ApiGetServerInfo.cpp -std=c++1z

ApiPostServerInfo.o : ApiPostServerInfo.cpp
	g++ -c ApiPostServerInfo.cpp -std=c++1z

ApiGetAgentInfo.o : ApiGetAgentInfo.cpp
	g++ -c ApiGetAgentInfo.cpp -std=c++1z

ApiPostAgentInfo.o : ApiPostAgentInfo.cpp
	g++ -c ApiPostAgentInfo.cpp -std=c++1z

ApiGetCommandForStatus.o : ApiGetCommandForStatus.cpp
	g++ -c ApiGetCommandForStatus.cpp -std=c++1z

ApiGetCommandForApiGetCommandForOperation.o : ApiGetCommandForApiGetCommandForOperation.cpp
	g++ -c ApiGetCommandForApiGetCommandForOperation.cpp -std=c++1z

ApiGetCommand.o : ApiGetCommand.cpp
	g++ -c ApiGetCommand.cpp -std=c++1z

ApiPostCommand.o : ApiPostCommand.cpp
	g++ -c ApiPostCommand.cpp -std=c++1z

ApiDeleteCommand.o : ApiDeleteCommand.cpp
	g++ -c ApiDeleteCommand.cpp -std=c++1z

ApiGetFile.o : ApiGetFile.cpp
	g++ -c ApiGetFile.cpp -std=c++1z

ApiPostFile.o : ApiPostFile.cpp
	g++ -c ApiPostFile.cpp -std=c++1z

dataaccess.o : dataaccess.cpp
	g++ -c dataaccess.cpp -std=c++1z

sample.o : sample.cpp
	g++ -c sample.cpp -std=c++1z

clean :
	rm -f *.o *.a sample
	cd ../../../YaizuComLib/src/stkpl; make clean
	cd ../../../YaizuComLib/src/stkwebapp; make clean
	cd ../../../YaizuComLib/src/stkwebapp_um; make clean
	cd ../../../YaizuComLib/src/commonfunc; make clean
	cd ../../../YaizuComLib/src/stksocket; make clean
	cd ../../../YaizuComLib/src/stkthread; make clean
	cd ../../../YaizuComLib/src/stkdata; make clean

all : clean sample

