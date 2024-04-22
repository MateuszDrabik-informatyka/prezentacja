module;
#include <iostream>
#include <thread>
#include <atomic>
#include <vector>
export module zadanie4;

static double function(double x)
{
    return(x * x + 2 * x);
    // output = 16.67
}

export void zad4()
{
    const int N = 1000;
    double fstart = 1, fend = 3;
    std::atomic<double> output(0);
    double dx;

    dx = (fend - fstart) / N;

    auto thread_function = [&](int start, int end) {
        double local_sum = 0;
        for (int i = start; i <= end; i++)
            local_sum += function(fstart + i * dx);
        output += local_sum;
        };

    const int num_threads = std::thread::hardware_concurrency();
    int points_per_thread = N / num_threads;
    int remainder = N % num_threads;
    std::vector<std::jthread> threads(num_threads);

    int start_point = 1;
    for (int i = 0; i < num_threads; i++)
    {
        int end_point = start_point + points_per_thread - 1;
        if (i == num_threads - 1)
            end_point += remainder;
        threads[i] = std::jthread(thread_function, start_point, end_point);
        start_point = end_point + 1;
    }

    for (auto& t : threads)
    {
        t.join();
    }

    std::cout << "Wartosc calki wynosi : " << output * dx << '\n';
}