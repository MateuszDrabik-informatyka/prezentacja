module;
#include <thread>
#include <iostream>
#include <mutex>
#include <condition_variable>
export module zadanie1;


class FizzBuzz
{
	int n;
	int current;
	std::mutex mtx;
	std::condition_variable cv;
	void fizz()
	{
		std::unique_lock<std::mutex> lock(mtx);
		while (current <= n)
		{
			if (!(current % 3) && (current % 5))
			{
				std::cout << "fizz" << std::endl;
				++current;
				cv.notify_all();
			}
			else
			{
				cv.wait(lock);
			}
		}
	}
	void buzz()
	{
		std::unique_lock<std::mutex> lock(mtx);
		while (current <= n)
		{
			if ((current % 3) && !(current % 5))
			{
				std::cout << "buzz" << std::endl;
				++current;
				cv.notify_all();
			}
			else
			{
				cv.wait(lock);
			}
		}
	}
	void fizzbuzz()
	{
		std::unique_lock<std::mutex> lock(mtx);
		while (current <= n)
		{
			if (!(current % 3) && !(current % 5))
			{
				std::cout << "fizzbuzz" << std::endl;
				++current;
				cv.notify_all();
			}
			else
			{
				cv.wait(lock);
			}
		}
	}
	void number()
	{
		std::unique_lock<std::mutex> lock(mtx);
		while (current <= n)
		{
			
			if ((current % 3) && (current % 5))
			{
				std::cout << current << std::endl;
				++current;
				cv.notify_all();
			}
			else
			{
				cv.wait(lock);
			}
		}
	}
public:
	FizzBuzz(int x)
	{
		n = x;
		current = 1;
	}
	FizzBuzz() :FizzBuzz(0) {}
	void run()
	{
		std::jthread f([this]{this->fizz(); });
		std::jthread b([this] {this->buzz(); });
		std::jthread fb([this] {this->fizzbuzz(); });
		std::jthread n([this] {this->number(); });
	}
};
export void zad1()
{
	FizzBuzz f{ 1000 };
	f.run();
}