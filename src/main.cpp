#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>
#include <chrono>

using namespace std;

bool running = true;

atomic_flag job1taken = ATOMIC_FLAG_INIT;
atomic_flag job2taken = ATOMIC_FLAG_INIT;
bool job1finished = false;
bool job2finished = false;
mutex console;
mutex sync;

void work()
{
	if(!job1taken.test_and_set())
	{
		console.lock();
		cout << "Starting to do job1" << endl;
		console.unlock();
		std::this_thread::sleep_for(25s);
		console.lock();
		cout << "Finishing job1" << endl;
		console.unlock();
		job1finished = true;
	}
	if(!job2taken.test_and_set())
	{
		console.lock();
		cout << "Starting to do job2" << endl;
		console.unlock();
		std::this_thread::sleep_for(25s);
		console.lock();
		cout << "Finishing job2" << endl;
		console.unlock();
		job2finished = true;
	}
}

void worker()
{
	while(running)
	{
		work();
		sync.lock();
		sync.unlock();
    }
}

int main()
{
	cout << "Starting " << std::thread::hardware_concurrency() << " jobs" << endl;
	sync.lock();
	for(unsigned int i=2; i<=std::thread::hardware_concurrency(); i++)
	{
		thread(worker).detach();
	}
	while(!job1finished || !job2finished)
	{
		work();
	}
	sync.unlock();
	running = false;
	return 0;
}
