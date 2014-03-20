BOOST_DIR = /usr/include/boost
THRIFT_DIR = /usr/local/include/thrift
LIB_DIR = /usr/local/lib
CPP_SRC_LIBS=.libs
GEN_SRC = gen-cpp/RPCProxy_constants.cpp gen-cpp/RPCProxy.cpp gen-cpp/RPCProxy_types.cpp

default: server client

server: RPCProxyServer.cpp
	g++ -o RPCProxyServer -I${THRIFT_DIR} -I${BOOST_DIR}  -I/gen-cpp RPCProxyServer.cpp ${GEN_SRC} -lthrift  -L${LIB_DIR} -Wl,-rpath=${CPP_SRC_LIBS}

client: RPCProxyClient.cpp
	g++ -o RPCProxyClient -I${THRIFT_DIR} -I${BOOST_DIR}  -I/gen-cpp RPCProxyClient.cpp ${GEN_SRC} -lthrift  -L${LIB_DIR} -Wl,-rpath=${CPP_SRC_LIBS}

clean:
	$(RM) -r RPCProxyClient RPCProxyServer  


