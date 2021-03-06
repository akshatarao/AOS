// This autogenerated skeleton file illustrates how to build a server.
// You should copy it to another filename to avoid overwriting it.

#include "gen-cpp/RPCProxy.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <iostream>
#include <string>
#include <curl/curl.h>

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

using boost::shared_ptr;

using namespace  ::rpcproxy;

class RPCProxyHandler : virtual public RPCProxyIf {
 public:
  RPCProxyHandler() {
    // Your initialization goes here
  }

  void hello() {
    // Your implementation goes here
    printf("hello\n");
  }

   static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
   {
	((std::string*)userp)->append((char*)contents, size * nmemb);
	return size * nmemb;
   }

   void fetchURLContent(std::string& _return, const std::string& url) 
   {
	//TODO: Input Validation for URL - NULL, Incorrect

	CURL *curl;
	CURLcode res;
 	std::string readBuffer;
	
  	curl = curl_easy_init();
  
	if(curl) 
	{

		const char* urlLink = url.data();
    		curl_easy_setopt(curl, CURLOPT_URL, urlLink);
    		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
    		res = curl_easy_perform(curl);
    		curl_easy_cleanup(curl);

    		std::cout << readBuffer;
	}	

	std::string out = readBuffer + std::string("\n");
	
	_return = out;  
  }
};

int main(int argc, char **argv) {
  int port = 9090;
  shared_ptr<RPCProxyHandler> handler(new RPCProxyHandler());
  shared_ptr<TProcessor> processor(new RPCProxyProcessor(handler));
  shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
  shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
  shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

  TSimpleServer server(processor, serverTransport, transportFactory, protocolFactory);
  server.serve();
  return 0;
}

