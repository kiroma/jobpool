#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>
#include <chrono>

using namespace std;

bool running = true;

class job{
    public:
    atomic_flag taken = ATOMIC_FLAG_INIT;
    bool finished = false;
};

job job1;
job job2;

mutex console;
mutex sync;

void work()
{
	if(!job1.taken.test_and_set())
	{
		console.lock();
		cout << "Starting to do job1" << endl;
		console.unlock();
		std::this_thread::sleep_for(1s);
		console.lock();
		cout << "Finishing job1" << endl;
		console.unlock();
		job1.finished = true;
	}
	if(!job2.taken.test_and_set())
	{
		console.lock();
		cout << "Starting to do job2" << endl;
		console.unlock();
		std::this_thread::sleep_for(1s);
		console.lock();
		cout << "Finishing job2" << endl;
		console.unlock();
		job2.finished = true;
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
	cout << "Starting " << std::thread::hardware_concurrency() << " job(s)" << endl;
	sync.lock();
	for(unsigned int i=2; i<=std::thread::hardware_concurrency(); i++)
	{
		thread(worker).detach();
	}
	while(!job1.finished || !job2.finished)
	{
		work();
	}
	sync.unlock();
	running = false;
	return 0;
}
