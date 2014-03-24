#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

int main (int argc, char *argv[]) {


ifstream in_stream;

string line;

in_stream.open(argv[1]);

while(!in_stream.eof())
{
    in_stream >> line;
    cout << line << "\n";
}

in_stream.close();

}
