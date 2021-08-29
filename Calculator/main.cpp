#include <iostream>
#include "calculator.h"
using namespace std;
int main()
{
	string str;
	getline(cin, str);
	cout << calculate(str) << endl;
	return 0;
}