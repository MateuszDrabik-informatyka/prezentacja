module;
#include <complex>
#include <cmath>
#include <vector>
#include <thread>
#include <mutex>
#include <numbers>
#include <iostream>
export module Przyklad;

static void dft(const std::vector<std::complex<double>>& samples, std::vector<std::complex<double>>& harmonic)
{
	std::mutex mtx;
	std::vector<std::jthread> threads;
	if (!harmonic.empty())
	{
		harmonic.clear();
	}
	harmonic.resize(samples.size());
	for (size_t i = 0; i < samples.size(); ++i)
	{
		threads.push_back(std::jthread([&samples,&harmonic,i,&mtx]()
			{
				for (size_t j = 0; j < samples.size(); ++j)
				{
					std::lock_guard<std::mutex> lock(mtx);
					harmonic[i] += samples[j] * pow(std::numbers::e,std::complex<double>(0, - (double)i * (double)j*2*std::numbers::pi/samples.size()));
				}
			}));
	}
	for (auto& i : threads)
	{
		i.join();
	}
}

static void idft(const std::vector<std::complex<double>>& harmonic, std::vector<std::complex<double>>& samples)
{
	std::mutex mtx;
	std::vector<std::jthread> threads;
	if (!samples.empty())
	{
		samples.clear();
	}
	samples.resize(harmonic.size());
	for (size_t i = 0; i < samples.size(); ++i)
	{
		threads.push_back(std::jthread([&samples, &harmonic, i, &mtx]()
			{
				for (size_t j = 0; j < samples.size(); ++j)
				{
					std::lock_guard<std::mutex> lock(mtx);
					samples[i] += harmonic[j] * pow(std::numbers::e, std::complex<double>(0,(double)i * (double)j * 2 * std::numbers::pi / samples.size()))/(double)harmonic.size();
				}
			}));
	}
	for (auto& i : threads)
	{
		i.join();
	}
}


export void Przyklad()
{
	std::vector<std::complex<double>> samples{ 1,2,4,8 };
	std::vector<std::complex<double>> harmonic;
	dft(samples, harmonic);
	std::cout << harmonic.size() << std::endl;
	for (auto& i : harmonic)
	{
		std::cout << i<< std::endl;
	}
	idft(harmonic, samples);
	for (auto& i : samples)
	{
		std::cout << i << std::endl;
	}
}