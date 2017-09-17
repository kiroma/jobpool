#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>
#include <chrono>

using namespace std;

bool running = true;

bool job1taken = false;
bool job2taken = false;
bool job1finished = false;
bool job2finished = false;
bool jobsdone = false;
static unsigned int n = std::thread::hardware_concurrency();
mutex mtx;

void work()
{
	if(!job1taken)
	{
		job1taken = true;
		mtx.lock();
		cout << "Starting to do job1" << endl;
		mtx.unlock();
		std::this_thread::sleep_for(1s);
		mtx.lock();
		cout << "Finishing job1" << endl;
		mtx.unlock();
		job1finished = true;
	}
	if(!job2taken)
	{
		job2taken = true;
		mtx.lock();
		cout << "Starting to do job2" << endl;
		mtx.unlock();
		std::this_thread::sleep_for(1s);
		mtx.lock();
		cout << "Finishing job2" << endl;
		mtx.unlock();
		job2finished = true;
	}
}

void worker()
{
	while(running)
	{
		work();
    }
}

int main()
{
	cout << "Starting " << n << " jobs" << endl;
	for(unsigned int i=1; i<=n; i++)
	{
		thread(worker).detach();
	}
	while(!job1finished || !job2finished)
	{
		work();
	}
	running = false;
	return 0;
}
