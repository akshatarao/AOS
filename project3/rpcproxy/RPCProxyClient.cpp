#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>

#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>

#include "gen-cpp/RPCProxy.h"

using namespace std;
using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

using namespace rpcproxy;

using namespace boost;

int main(int argc, char** argv) {
  shared_ptr<TTransport> socket(new TSocket("localhost", 9090));
  shared_ptr<TTransport> transport(new TBufferedTransport(socket));
  shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));

  RPCProxyClient client(protocol);

  try {
    transport->open();

    client.hello();

    string content;
    const char* url = "abc";
	 
    client.fetchURLContent(content, url);	

     transport->close();
  } catch (TException &tx) {
    printf("ERROR: %s\n", tx.what());
  }

}
