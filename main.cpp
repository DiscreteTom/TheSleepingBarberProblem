#include "barbershop.h"
#include <iostream>

using namespace std;

int main()
{
	int n;

	cout << "Please input the number of barbershop's chairs:\n";
	cin >> n;

	if (n <= 0)
	{
		n = 10;
		cout << "Invalid input, the number of barbershop's chairs will be set to 10.\n";
	}

	cout << endl;

	cin.clear();
	cin.sync();
	cin.ignore();

	BarberShop barberShop(n);
	barberShop.start();
	system("pause");
}