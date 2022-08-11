#include <iostream>
#include <fstream>
using namespace std;

int main()
{
fstream mcp9808; 
mcp9808.open("/dev/mcp9808",ios::in);   
if(!mcp9808) { 
cout<<"No such file";
}
else  {
char ch;
while (!mcp9808.eof()) 
{ mcp9808 >>ch; 
cout << ch;   
}
}
mcp9808.close();    
return 0;
}
			



