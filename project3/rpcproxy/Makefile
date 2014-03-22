BOOST_DIR = /usr/include/boost
THRIFT_DIR = /usr/local/include/thrift
LIB_DIR = /usr/local/lib
CPP_SRC_LIBS=.libs
GEN_SRC = gen-cpp/RPCProxy_constants.cpp gen-cpp/RPCProxy.cpp gen-cpp/RPCProxy_types.cpp

default: server client cache FIFO LRU main temp

server: RPCProxyServer.cpp
	g++ -o RPCProxyServer -I${THRIFT_DIR} -I${BOOST_DIR}  -I/gen-cpp RPCProxyServer.cpp ${GEN_SRC} -lthrift  -L${LIB_DIR} -Wl,-rpath=${CPP_SRC_LIBS} -lcurl -L/usr/lib/x86_64-linux-gnu

client: RPCProxyClient.cpp
	g++ -o RPCProxyClient -I${THRIFT_DIR} -I${BOOST_DIR}  -I/gen-cpp RPCProxyClient.cpp ${GEN_SRC} -lthrift  -L${LIB_DIR} -Wl,-rpath=${CPP_SRC_LIBS}

cache: AbstractCache.cpp
	g++ -c -std=c++0x AbstractCache.cpp AbstractCache.h 
 
FIFO: FIFOCache.cpp
	g++ -c -std=c++0x FIFOCache.cpp FIFOCache.h AbstractCache.h 

LRU: LRUCache.cpp
	g++ -c -std=c++0x LRUCache.cpp LRUCache.h AbstractCache.h 

main: Main.cpp
	g++ -c -std=c++0x Main.cpp FIFOCache.h LRUCache.h

temp: 
	g++ Main.o AbstractCache.o FIFOCache.o LRUCache.o -o temp


clean:
	$(RM) -r RPCProxyClient RPCProxyServer AbstractCache FIFO LRU main temp *.o


