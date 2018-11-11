#include "barbershop.h"
#include <iostream>

using namespace std;

int main()
{
	int n;

	cout << "Please input the number of barbershop's chairs:\n";
	cin >> n;

	BarberShop barberShop(10);
	barberShop.start();
	system("pause");
}