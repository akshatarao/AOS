#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
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

void printSyntax(char* executablePath)
{
	printf("\nSyntax: %s -url <url> <IP_Address> <Port>", executablePath);
	printf("\nSyntax: %s -urlList <url-file> <IP_Address> <Port>", executablePath);
	printf("\n");
}
int main(int argc, char** argv) {

  char* ipAddress;
  int port;
  bool isList = false;
  const char* url;
  const char* urlFileName;


  if(argc < 5)
  {
	printSyntax(argv[0]);
	exit(1); 
  }

  const char* option = argv[1];
  
  if(strcmp(option, "-url") == 0)
  {
	url = argv[2]; 
  }
  else if(strcmp(option, "-urlList") == 0)
  {
	urlFileName = argv[2];
	isList = true;
  }
  else
  {
	printSyntax(argv[0]);
	exit(1);
  }	

  ipAddress = argv[3];
  port = atoi(argv[4]);
		 
  shared_ptr<TTransport> socket(new TSocket(ipAddress, port));
  shared_ptr<TTransport> transport(new TBufferedTransport(socket));
  shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));

  RPCProxyClient client(protocol);

  try {
    transport->open();

    if(isList)
    {
	ifstream in_stream;

	string line;

	in_stream.open(urlFileName);

	while(!in_stream.eof())
	{
    		in_stream >> line;
    		
		string content;
		cout << "\nURL: " << line;
		client.fetchURLContent(content, line);
	}

	in_stream.close();

    }
    else
    {
	string content;
	client.fetchURLContent(content, url);
	cout << "URL Output: " << content << "\n";	
    }	

    client.printServerStats();	
     transport->close();
  } catch (TException &tx) {
    printf("ERROR: %s\n", tx.what());
  }

}
