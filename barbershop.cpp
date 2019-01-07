#include "barbershop.h"
#include <conio.h>
#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

BarberShop::BarberShop(int chairNum) : chairNum(chairNum)
{
}

void BarberShop::process()
{
	using namespace chrono_literals;

	while (running)
	{
		// get the next client
		mtx.lock();
		int t = -1;
		if (clients.size())
		{
			t = clients.front();
			clients.pop();
		}
		mtx.unlock();

		if (t == -1) // no next client
		{
			// barber goes to sleep
			cout << "Barber goes to sleep.\n";
			unique_lock<mutex> lock(mtx);
			sleeping = true;
			if (cv.wait_for(lock, chrono::seconds(60), [this] { return !sleeping; }))
			{
				// barber is awake
				cout << "Barber is awake.\n";
			}
			else
			{
				// timeout
				cout << "Time out, stop running.\n";
				running = false;
			}
		}
		else // next client exist
		{
			// barber goes to work
			cout << "Barber begins to work.(" << t << "s)\n";
			unique_lock<mutex> lock(mtx);
			if (cv.wait_for(lock, chrono::seconds(t), [this] { return !running; }))
			{
				// stop running
				return;
			}
			else
			{
				// timeout(finishes work)
				cout << "Barber finishes work.\n";
			}
		}
	}
}

void BarberShop::addClient()
{
	char c = _getwch();
	while (c == '\n' || c == '\r')
	{
		// try to add a client
		mtx.lock();
		if (clients.size() == chairNum)
		{
			// shop is full
			cout << "No seat for new client.\n";
		}
		else
		{
			// add a client, give a random time
			cout << "Add a client.\n";
			clients.push(rand() % 3 + 5);
		}
		mtx.unlock();

		while (sleeping)
		{
			// try to wake up barber
			sleeping = false;
			cv.notify_one();
		}
		c = _getwch();
	}
	// that's the end of input

	while (running)
	{
		// try to stop running to finish process()
		running = false;
		cv.notify_one();
	}
	while (sleeping)
	{
		// try to stop sleeping to finish process()
		sleeping = false;
		cv.notify_one();
	}
}

void BarberShop::start()
{
	srand(time(NULL));

	cout << "BarberShop is on, press Enter to add a client and press other key to stop.\n";
	cout << "If barber sleeps over 60s, barbershop will be closed.\n\n";

	running = true;
	sleeping = true;
	thread th1(&BarberShop::addClient, this);
	thread th2(&BarberShop::process, this);
	th1.join();
	th2.join();

	cout << "BarberShop closes.\n";
}