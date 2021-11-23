#include <iostream>
#include <iomanip>
#include "calculator.h"
using namespace std;
int main()
{
	string str;
	getline(cin, str);
	while (str != "exit")
	{
		double result = calculate(str);
		cout << setprecision(14) << result << endl;
	GET:
		getline(cin, str);
		if (str == "cls")
		{
			system("cls");
			goto GET;
		}
	}
	return 0;
}
