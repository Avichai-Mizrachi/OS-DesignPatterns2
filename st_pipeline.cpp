#include <iostream>
#include <cstdlib>
#include <ctime>
#include <random>
#include <atomic>
#include <vector>
#include <mutex>
#include <condition_variable>
#include "sources/active.hpp"
#include "sources/queue.hpp"
#include "sources/shared_variables.hpp"

using namespace std;

std::vector<int *> allocatedPointers1;
std::vector<int *> allocatedPointers2;
std::vector<int *> allocatedPointers3;
std::vector<int *> allocatedPointers4;

int numOfPrimes = 0;

ActiveObject *pipee[4]{nullptr};

// Check if the number is prime.
int isPrime(unsigned int number)
{

    // Check if number is 2 (the only even prime number)
    if (number == 2)
    {
        return 1;
    }

    // Check if number is less than 2 or even
    if (number < 2 || number % 2 == 0)
    {
        return 0;
    }

    // Check for divisibility by odd numbers up to the square root of the number
    for (unsigned int i = 3; i * i <= number; i += 2)
    {
        if (number % i == 0)
        {
            return 0;
        }
    }

    return 1;
}

// Casting to unsigned int
int usePrime(int number)
{
    unsigned int num = (unsigned int)number;
    return isPrime(num);
}

// First active object.
void generateNumbers(void *arg)
{
    int *task = static_cast<int *>(arg);
    int N = task[0];
    unsigned int seed = static_cast<unsigned int>(task[1]);

    std::mt19937 generator(seed); // Mersenne Twister random number engine

    for (int i = 0; i < N; i++)
    {
        int *gNumber = new int;

        // Check if allocation failed
        if (gNumber == nullptr)
        {
            std::cout << "gNumber is null" << std::endl;
            exit(1);
        }

        std::uniform_int_distribution<int> distribution(100000, 999999);
        *gNumber = distribution(generator);

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        pipee[1]->getQueue()->enqueue(gNumber);

        allocatedPointers1.push_back(gNumber);
    }
}

// Second active object.
void checkPrime(void *arg)
{
    int *number = new int;
    if (number == nullptr)
    {
        cout << "number is null" << endl;
        exit(1);
    }

    *number = *static_cast<int *>(arg);
    std::cout << *number << std::endl;

    int ans = usePrime(*number);
    if (ans == 1)
    {
        numOfPrimes++;
    }

    bool isPrime = static_cast<bool>(ans);
    cout << boolalpha << isPrime << endl;

    *number += 11;

    pipee[2]->getQueue()->enqueue(number);
    allocatedPointers2.push_back(number);
}

// Third active object.
void printAndCheckPrime(void *arg)
{
    int *number = new int;
    if (number == nullptr)
    {
        std::cout << "number is null" << std::endl;
        exit(1);
    }

    *number = *static_cast<int *>(arg);
    std::cout << *number << std::endl;

    int ans = usePrime(*number);

    bool isPrime = static_cast<bool>(ans);
    std::cout << std::boolalpha << isPrime << std::endl;

    *number -= 13;

    pipee[3]->getQueue()->enqueue(number);
    allocatedPointers3.push_back(number);
}

// Forth active object.
void printAndAddTwo(void *arg)
{
    int *number = new int;
    *number = *static_cast<int *>(arg);
    std::cout << *number << std::endl;
    *number += 2;
    std::cout << *number << std::endl;
    std::cout << std::endl; // Print an empty line
    allocatedPointers4.push_back(number);
}

// Main.
int global_N;
int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cout << "Usage: pipeline_st <N> [seed]" << std::endl;
        return 1;
    }

    int N = std::stoi(argv[1]);
    global_N = N;
    int seed = (argc >= 3) ? std::stoi(argv[2]) : static_cast<int>(std::time(nullptr));

    if (seed <= 0)
    {
        std::cout << "Seed must be a positive integer" << std::endl;
        return 1;
    }

    // Create the pipeline of Active Objects
    pipee[0] = createActiveObject(generateNumbers);
    pipee[1] = createActiveObject(checkPrime);
    pipee[2] = createActiveObject(printAndCheckPrime);
    pipee[3] = createActiveObject(printAndAddTwo);

    int task_A[2];
    task_A[0] = N;
    task_A[1] = seed;
    pipee[0]->getQueue()->enqueue(&task_A);

    // Wait for the pipeline to finish
    std::unique_lock<std::mutex> lock(mtx);
    while (pipee[3]->getIteration() < N)
    {
        cond.wait(lock);
    }

    // Wait for the pipeline to finish

    // After using the pointers, deallocate the memory
    std::vector<int *> allocatedPointers1;
    std::vector<int *> allocatedPointers2;
    std::vector<int *> allocatedPointers3;
    std::vector<int *> allocatedPointers4;

    // Destroy the pipeline
    destroyActiveObject(pipee[0]);
    destroyActiveObject(pipee[1]);
    destroyActiveObject(pipee[2]);
    destroyActiveObject(pipee[3]);
    for (unsigned long i = 0; i < allocatedPointers1.size(); i++)
    {
        delete allocatedPointers1[i];
    }
    for (unsigned long i = 0; i < allocatedPointers2.size(); i++)
    {
        delete allocatedPointers2[i];
    }
    for (unsigned long i = 0; i < allocatedPointers3.size(); i++)
    {
        delete allocatedPointers3[i];
    }
    for (unsigned long i = 0; i < allocatedPointers4.size(); i++)
    {
        delete allocatedPointers4[i];
    }

    return 0;
}
