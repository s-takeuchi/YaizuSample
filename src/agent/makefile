LDFLAGS = -L/usr/local/lib64 -L/usr/local/lib
LIBS    = -lssl -lcrypto

servalagt : servalagt.o
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

	g++ -o servalagt servalagt.o stkpl.a stkwebapp.a commonfunc.a stksocket.a stkthread.a stkdata.a -std=c++1z -lstdc++fs -pthread -static-libgcc -static-libstdc++ $(LDFLAGS) $(LIBS)

servalagt.o : servalagt.cpp
	g++ -c servalagt.cpp -std=c++1z

clean :
	rm -f *.o *.a servalagt
	cd ../../../YaizuComLib/src/stkpl; make clean
	cd ../../../YaizuComLib/src/stkwebapp; make clean
	cd ../../../YaizuComLib/src/commonfunc; make clean
	cd ../../../YaizuComLib/src/stksocket; make clean
	cd ../../../YaizuComLib/src/stkthread; make clean
	cd ../../../YaizuComLib/src/stkdata; make clean

all : clean servalagt

