#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <string_view>
#include <mutex>

// многопоточность в приложениях

// управляющий поток
// атомарные операции
// гонки данных
// векторизованные вычисления
using namespace std;
using namespace std::chrono;

void Fill(vector<double>* cources) {
    for (size_t i = 0; i < cources->capacity(); i++)
    {
        this_thread::sleep_for(milliseconds(15));
        cources->emplace_back(
            15 + (double)::rand() / (1 + ::rand())
        );
    }
}

mutex mt; // создёт массив резервирующих потоков, вносит-выносит данные

void Print(
    std::vector<double>::iterator start,
    std::vector<double>::iterator stop,
    string_view name
) 
{
    while (start <= stop)
    {
        mt.lock(); // атомарная область потока
        cout << "Value " << *start << '\n';
        mt.unlock();
        ++start;
    }
}

int main()
{
    cout << "Hello World!\n";
    //this_thread::get_id();
    this_thread::sleep_for(milliseconds(10));
    vector<double> udsCource;
    udsCource.reserve(540);
    vector<double> euroCource;
    euroCource.reserve(540);
    vector<double> phoundCource;
    phoundCource.reserve(540);
    thread tr1(Fill, &udsCource);
    //Fill(&udsCource);    
    thread tr2(Fill, &euroCource);
    thread tr3(Fill, &phoundCource);

    tr1.join();
    thread tr4(
        Print,
        udsCource.begin(),
        udsCource.begin() + 100,
        "Usd"
    );
    thread tr5(
        [&tr2, &euroCource]() {
            tr2.join();
            Print(
                euroCource.begin(),
                euroCource.begin() + 100,
                "Euro");
        }
    );
    
    tr3.join();
    Print(
        phoundCource.begin(),
        phoundCource.begin() + 100,
        "Pound"
    );
    tr5.join();
    tr4.join();
    return 0;
}