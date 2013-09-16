PLUGIN_CXX_FLAGS=-fpic -fPIC -DPIC -shared -O6 -g
RANDPLUGIN_CXX_FLAGS=`gsl-config --cflags`
CRYPTPLUGIN_LD_FLAGS=-lgcrypt -lgpg-error
RANDPLUGIN_LD_FLAGS=`gsl-config --libs` -lgcrypt -lgpg-error
FINAL_LD_FLAGS=-ldl -lboost_regex -lboost_filesystem -lboost_system
PRIMARY_CXX_FLAGS=-fpic -fPIC -DPIC -O6 -g
USERSPEC_CXX_FLAGS=
USERSPEC_LD_FLAGS=

passgen: main.o loader.o config.o section.o key.o library.o randplugin.so cryptplugin.so userspec.so
	g++ ${PRIMARY_CXX_FLAGS} -o passgen main.o loader.o config.o section.o key.o library.o ${FINAL_LD_FLAGS}


randplugin.so: random_plugin.cpp config.cpp section.cpp key.cpp plugin.h random_plugin.h
	g++ ${PLUGIN_CXX_FLAGS} ${RANDPLUGIN_CXX_FLAGS} -o randplugin.so ${RANDPLUGIN_LD_FLAGS} random_plugin.cpp config.o section.o key.o

cryptplugin.so: crypt_plugin.cpp config.cpp section.cpp key.cpp plugin.h crypt_plugin.h
	g++ ${PLUGIN_CXX_FLAGS} ${CRYPTPLUGIN_CXX_FLAGS} -o cryptplugin.so ${CRYPTPLUGIN_LD_FLAGS} crypt_plugin.cpp config.o section.o key.o

userspec.so: userspec.cpp config.cpp section.cpp key.cpp plugin.h userspec.h
	g++ ${PLUGIN_CXX_FLAGS} ${USERSPEC_CXX_FLAGS} -o userspec.so ${USERSPEC_LD_FLAGS} userspec.cpp config.o section.o key.o

install:
	mkdir -p /usr/lib/passgen
	install cryptplugin.so /usr/lib/passgen
	install randplugin.so /usr/lib/passgen
	install userspec.so /usr/lib/passgen
	install passgen /usr/bin

clean:
	rm *.o *.so *~ passgen

main.o: main.cpp loader.cpp config.cpp loader.h config.h
	g++ ${PRIMARY_CXX_FLAGS} -c -o main.o main.cpp

loader.o: loader.cpp library.cpp loader.h library.h
	g++ ${PRIMARY_CXX_FLAGS} -c -o loader.o loader.cpp -lboost_filesystem

config.o: config.cpp section.cpp key.cpp config.h section.h key.h
	g++ ${PRIMARY_CXX_FLAGS} -c -o config.o config.cpp

section.o: section.cpp key.cpp section.h key.h
	g++ ${PRIMARY_CXX_FLAGS} -c -o section.o section.cpp

key.o: key.cpp key.h
	g++ ${PRIMARY_CXX_FLAGS} -c -o key.o key.cpp

library.o: library.cpp library.h
	g++ ${PRIMARY_CXX_FLAGS} -c -o library.o library.cpp
