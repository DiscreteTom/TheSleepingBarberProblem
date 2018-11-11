#pragma once

#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

using namespace std;

class BarberShop
{
private:
	int chairNum;
	queue<int> clients;

	// about thread
	mutex mtx; // to protect clients
	volatile bool running;
	condition_variable cv;
	volatile bool sleeping;

	// thread functions
	void addClient();
	void process();

public:
	BarberShop(int chairNum);
	void start();
};