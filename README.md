# ParadigmDemonstration---Concurrency
Solution for the 4th Evidence of the TC2037 Course at ITESM. Demonstration of a Programing Paradigm.

## Paradigm - Concurrency
First of all, let's take a quick look to the definition of a paradigm, a programing paradigm can be defined as a methodology for problem-solving using the tools and techniques available to us, following a particular approach. There are many different paradigms that provide solutions to a diverse number of demands and challenges in software development. Particullary I'm going to focus in the paradigms we had the chance to look further into in this course. 

**1. Logic programming.**

**2. Scripting.***

**3. Functional programming.**

**4. Concurrent programming.**

**5. Parallel programming.** 

### Paradigm Choice: Concurrent programming. 

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

## Solution and analysis. 


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

## Sources: 

GeeksforGeeks. (2025, April 8th). Introduction of Programming Paradigms. GeeksforGeeks. https://www.geeksforgeeks.org/introduction-of-programming-paradigms/

