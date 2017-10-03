#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>
#include <chrono>

bool running = true;

class job{
    public:
    std::atomic_flag taken = ATOMIC_FLAG_INIT;
    bool finished = false;
};

job job1;
job job2;

std::mutex console;
std::mutex sync;

void work()
{
	if(!job1.taken.test_and_set())
	{
		console.lock();
		std::cout << "Starting to do job1" << std::endl;
		console.unlock();
		std::this_thread::sleep_for(std::chrono::seconds(1));
		console.lock();
		std::cout << "Finishing job1" << std::endl;
		console.unlock();
		job1.finished = true;
	}
	if(!job2.taken.test_and_set())
	{
		console.lock();
		std::cout << "Starting to do job2" << std::endl;
		console.unlock();
		std::this_thread::sleep_for(std::chrono::seconds(1));
		console.lock();
		std::cout << "Finishing job2" << std::endl;
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
	std::cout << "Starting " << std::thread::hardware_concurrency() << " job(s)" << std::endl;
	sync.lock();
	for(unsigned int i=2; i<=std::thread::hardware_concurrency(); i++)
	{
		std::thread(worker).detach();
	}
	while(!job1.finished || !job2.finished)
	{
		work();
	}
	sync.unlock();
	running = false;
	return 0;
}
