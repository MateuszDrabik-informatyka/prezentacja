module;
#include <iostream>
#include <vector>
#include <thread>
#include <atomic>
#include <mutex>
#include <fstream>
#include <algorithm>

export module zadanie5;

static bool is_prime(int number) {
    if (number <= 1) return false;

    for (int i = 2; i * i <= number; i++) {
        if (number % i == 0) {
            return false;
        }
    }
    return true;
}

static void check_primes(int start, int end, std::vector<std::atomic<int>>& primes) {
    for (int i = start; i <= end; ++i) {
        if (is_prime(i)) {
            primes[i].store(i);
            std::this_thread::yield();
        }
    }
}

export void zad5(int n) {
    const int num_threads = std::thread::hardware_concurrency();
    int points_per_thread = n / num_threads;
    int remainder = n % num_threads;

    std::vector<std::jthread> threads;
    std::vector<std::atomic<int>> primes(n + 1);
    int start_point = 0;

    for (int i = 0; i < num_threads; ++i) {
        int end_point = start_point + points_per_thread - 1;
        if (i == num_threads - 1) {
            end_point += remainder;
        }
        threads.emplace_back(check_primes, start_point, end_point, std::ref(primes));
        start_point = end_point + 1;
    }

    for (auto& t : threads) {
        t.join();
    }

    std::ofstream output_file("prime.txt");
    if (output_file.is_open()) {
        for (int i = 0; i < primes.size(); ++i) {
            if (primes[i].load() != 0) {
                output_file << primes[i].load() << std::endl;
            }
        }
        output_file.close();
    }
    else {
        std::cerr << "Nie mozna na zapisac liczb do pliku" << std::endl;
    }
}