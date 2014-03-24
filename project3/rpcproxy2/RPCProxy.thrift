namespace cpp rpcproxy

service RPCProxy
{
	void hello(),
	string fetchURLContent(1:string url),
	void printServerStats(),
}
