# ParadigmDemonstration---Concurrency
Solution for the 4th Evidence of the TC2037 Course at ITESM. Demonstration of a Programing Paradigm. 
Armando Fuentes Silva - A01712074

## Paradigm definition - Concurrency
First of all, let's take a quick look to the definition of a paradigm, a programing paradigm can be defined as a methodology for problem-solving using the tools and techniques available to us, following a particular approach. There are many different paradigms that provide solutions to a diverse number of demands and challenges in software development. Particullary I'm going to focus in the paradigms we had the chance to look further into in this course. 

**1. Logic programming.**
Logic programming is a type of programming paradigm that uses logic circuits to control how facts and rules about the problems within the system are represented or expressed. It's totally based on formal logic, program statements usually express or represent facts and rules related to problems within a system of formal logic, its main aim is to allow machines to reason because it is very useful for representing knowledge is also data-driven and array-oriented.

Common programming languages for this paradigm: 
- Prolog
- Datalog

**2. Functional programming.**
In functional programming everything is done with the help of functions and using functions as its basic building blocks. In it, we simply try to bind each and everything in a purely mathematical functions style. Programs are generally written at a higher level and are therefore much easier to comprehend.

Common programming languages for this paradigm: 
Pure:
- Racket
- Scheme
- Lisp
Not pure:
- Javascript
- C++
- Python

**3. Concurrent programming.**
Concurrent programming, or *concurrency* simply means multiple computations are happening at the same time. This is allowed due to it being split into groups of tasks that can be executed significantly faster concurrently.

Its hard to talk about concurrency without mentioning threads. What is a thread in comparison to a process?
A process is just an instance of a program, this processes can spawn child-processes or threads to be able to handle various subtasks. Threads live inside processes and share the same memory address space (view of memory). In contrast to that, child-processes are independent forks of the main process with a separate memory address space, which means that a process runs independently and is isolated from other processes. It cannot directly access shared data in other processes. 

So *concurrency* is essentially applicable when at least two tasks can start, run and complete in overlapping time. It doesn’t necessarily mean, that they’ll ever be running at the same instant (eg. multiple threads on a single-core machine).

**Multi-threading**
With multi-threading, you get the ability to run concurrent tasks within the same process. This way of concurrent programming allows the threads to share state, and execute from the same memory pools.
Common programming languages for this paradigm: 
- C
- C++
- Java
- Python

**4. Parallel programming.** 
In contrast to concurrency, parallelism is when two or more tasks are running at the same time (e.g., multiple threads on a multicore processor). When we consider parallel programming, programs use parallel hardware to execute computation more quickly. To mention some examples:

- Multi-core processors
- Graphics processing unit (GPU)
- Field-programmable gate arrays (FPGAs)
- Distributed computer clusters

Parallel programming is mostly used to speed-up computational time by splitting up a task into multiple, simple, and independent sub-task which can be performed simultaneously.

Common programming languages for this paradigm: 
On GPU:
- CUDA
- OpenCL
On CPU
- C++
- Java
- Python

### Paradigm Choice: Concurrent programming. 
For this demonstration I'm gonna provide a solution to a programming problem using concurrency. 
## Problem Description.
This problem is taken from LeetCode: https://leetcode.com/problems/print-zero-even-odd/description/

### Print Zero Even Odd.

**Here is the problem description:**

You have a function printNumber that can be called with an integer parameter and prints it to the console. 

For example, calling printNumber(7) prints 7 to the console. 
You are given an instance of the class ZeroEvenOdd that has three functions: zero, even, and odd. The same instance of ZeroEvenOdd will be passed to three different threads: 

Thread A: calls zero() that should only output 0's. 
Thread B: calls even() that should only output even numbers. 
Thread C: calls odd() that should only output odd numbers. 
Modify the given class to output the series "010203040506..." where the length of the series must be 2n. 

Implement the ZeroEvenOdd class: 

ZeroEvenOdd(int n) Initializes the object with the number n that represents the numbers that should be printed. 
void zero(printNumber) Calls printNumber to output one zero. 
void even(printNumber) Calls printNumber to output one even number. 
void odd(printNumber) Calls printNumber to output one odd number. 

**Constraints:**

1 <= n <= 1000

## Solution and code analysis. 
So the Leetcode test runners are gonna callled this 3 methods on parallel, I builded my own represenation of this calls using threads and solo the printNumber function: 
```
int main() {
    int n = 3; // Test cases
    ZeroEvenOdd zeo(n);

    auto printNumber = [](int x) {
        cout << x;
    };

    thread t1(&ZeroEvenOdd::zero, &zeo, printNumber);
    thread t2(&ZeroEvenOdd::even, &zeo, printNumber);
    thread t3(&ZeroEvenOdd::odd, &zeo, printNumber);

    t1.join();
    t2.join();
    t3.join();

    cout << endl;
    return 0;
}
```
So our job is to build the Zero, Even and Odd functions in a way that the print order is correct while having no control over the executiiom order. 

Imagine the sequence is Odd -> Even -> Zero, when Odd tries to execute and print an Odd number we gotta *lock it*, basically tell the function to wait it out, this is not their job, because we need to print a 0 firstThe exact thing we gotta do with the Even function, and when we finally reach Zero it should print. This should be the case no matter the oprder of execution. How do we do this? When the function executing has fiunished printing the number it should, it notifies the other functions to check out if its their turn, in this case Zero prints 0 and tells the other functions it finished, they check a variable that tells them if it's Odd's or Even's turn modified by Zero, and the choosen one executes next. This is why we lock it, when we are sharing the same variablewith the purpouse of modifying it we gotta lock it to prevent two threads to try and use the same variable and getting stuck. 

### Lets explain the code

Our constructor is passing the variables that we are goning to use. 
```
private:
    int n;
    mutex mtx;
    condition_variable cv;
    int state = 0; 
/*
0 = zero's turn
1 = odd's turn 
2 = even's turn
*/

public:
    ZeroEvenOdd(int n) {
        this->n = n;
    }
```

Our n is the number we want to print with our ZeroEvenOdd, basically how far we go. Both mtx and cv are used to wait and notify all threads with safety. Finally our state just determines who's turn is it. 

Our 3 functions are constructed in a similar manner. 
```
void zero(function<void(int)> printNumber) {
    for (int i = 1; i <= n; ++i) {
        unique_lock<mutex> lock(mtx);
        cv.wait(lock, [&]() { return state == 0; });
        printNumber(0);
        state = (i % 2 == 1) ? 1 : 2;
        cv.notify_all();
    }
}
```
We create a unique_lock with the help of mutex, this is easier and cleaner than just declaring a bunch of locks, we use conditional value to wait out our print opperation, we check if its our turn in this case the state must equal 0 to continue. We just print our 0, which it's always the case for Zero, we do a simple math equation to check if i is even or odd, to figure out our next state and we notify Even and Odd through our cv when we finish. 

```
void even(function<void(int)> printNumber) {
    for (int i = 2; i <= n; i += 2) {
        unique_lock<mutex> lock(mtx);
        cv.wait(lock, [this]() { return state == 2; });
        printNumber(i);
        state = 0;
        cv.notify_all();
    }
}
```
```
void odd(function<void(int)> printNumber) {
    for (int i = 1; i <= n; i += 2) {
        unique_lock<mutex> lock(mtx);
        cv.wait(lock, [this]() { return state == 1; });
        printNumber(i);
        state = 0;
        cv.notify_all();
    }
}
```

Both our even and odd functions are really similar to our Zero function and between each other, the key differences are that we add 2 to i everytime we get through the loop, this is because the even and odd functions are intertwined, basically, odd numbers: 1, 3, 5, 7, etc... which is the same as just adding 2. This way we can also just print i, and since both even and odd numbers should be followed by a 0 in our result we just make the state = 0 for both cases.

## Testing

Example 1:

Input: n = 2 
Output: "0102" 
Explanation: There are three threads being fired asynchronously. 
One of them calls zero(), the other calls even(), and the last one calls odd(). 
"0102" is the correct output. 

Example 2:

Input: n = 5 
Output: "0102030405" 

Example 3:

Input: n = 3 
Output: "010203" 
(the one in the code and not in leetcode)

## Complexity Analysis
The analysis for this solution is rather simple, our program just executes n number of times but we also have to execute the Zero function in between each numerical progrssion of n, so our Complexity comes down to O(2n), for example, n = 3, our result is 010203, we execute the Zero function 3 times, the odd function twice and the even function once, 3 + 2 + 1 = 6, 2 * 3 = 6.

## Sources: 

GeeksforGeeks. (2025, April 8th). Introduction of Programming Paradigms. GeeksforGeeks. https://www.geeksforgeeks.org/introduction-of-programming-paradigms/

## Learning resources (YouTube):

https://youtu.be/eh_9zUNmTig?si=x1qpSlTwYJx5Ooe2 

https://youtu.be/NsqqWDQHldo?si=8VOlUXog3bVtl49T

https://youtu.be/eZ8yKZo-PGw?si=YzCXPt1GeTIN17v3 

From: CppNuts
