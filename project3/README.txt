This folder contains the source code for the RPC Proxy Web Server with Cache Replacement Algorithms.

Code Organization
------------------
The code has been organized as follows
1. RPCProxyServer.cpp - RPC Proxy Server implementation
2. RPCProxyClient.cpp - RPC Proxy Client implementation
3. AbstractCache.cpp - Abstract Cache Implementation
4. FIFOCache.cpp - Cache with FIFO Cache Replacement Policy
5. LRUCache.cpp - Cache with LRU Cache Replacement Policy
6. LMUCache.cpp - Cache with LMU Cache Replacement Policy
7. RandomCache.cpp - Cache with Random Cache Replacement Policy

More source code documentation can be found in <installation_folder>/rpcproxy/html/index.html

Test Code
---------
Our Testcode is placed in the Main.cpp

Compiling the Code
------------------
A Makefile has been placed for your convenience.

Run the following commands

make - This command will clean up any previously generated object files, and generate object files for the code.

make clean - This command will clean up any previously generated object files and executables.

Running the Code
-----------------

RPCProxyServer
==============
./RPCProxyServer <cache-replacement-algorithm> <cache-size> <port>

The values for the cache-replacement-algorithm parameter are FIFO/LRU/MAXS/RANDOM. Please note that these values must be in capital letters.

The syntax is printed in case of any issues.

RPCProxyClient
==============

To request web content for a single URL, run the following command
./RPCProxyClient -url <url> <ServerIPAddress> <Port>

To request web content for a list of URLs, run the following command
./RPCProxyClient -urlList <url-file> <ServerIPAddress> <Port>

Replace the <url> with the URL of your choice.
Replace the <url-file> with the path of the file that contains your URLs.
Replace the <ServerIPAddress> - RPC Proxy Server IP Address
Replace the <Port> - Port where the RPC Proxy Server is hosted

A sample URL File is shown below

urlFileSample.txt
------------------------

http://www.google.com
http://www.linkedin.com
 
------------------------

Sample Datasets used for Metrics collection and evaluation can be found in 
<installation_folder>/rpcproxy/datasets folder

