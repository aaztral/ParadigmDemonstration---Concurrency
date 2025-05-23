// Armando Fuentes Silva - A01712074

#include <iostream>
#include <functional>
#include <thread>
#include <mutex>
#include <condition_variable>

using namespace std;

class ZeroEvenOdd {
private:
    int n;
    int count = 1;
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

    void zero(function<void(int)> printNumber) {
        for (int i = 1; i <= n; ++i) {
            unique_lock<mutex> lock(mtx); 
            cv.wait(lock, [this]() { return state == 0; });
            printNumber(0);
            state = (i % 2 == 1) ? 1 : 2;
            cv.notify_all();
        }
    }

    void even(function<void(int)> printNumber) {
        for (int i = 2; i <= n; i += 2) {
            unique_lock<mutex> lock(mtx);
            cv.wait(lock, [this]() { return state == 2; });
            printNumber(i);
            state = 0;
            cv.notify_all();
        }
    }

    void odd(function<void(int)> printNumber) {
        for (int i = 1; i <= n; i += 2) {
            unique_lock<mutex> lock(mtx);
            cv.wait(lock, [this]() { return state == 1; });
            printNumber(i);
            state = 0;
            cv.notify_all();
        }
    }
};

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
