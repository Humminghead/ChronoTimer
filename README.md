# ChronoTimer
This is a simple stopwatch class. The main purpose of this class is to enable a developer to estimate a performance of hotspots in their C++ code.

# Requrements:
- Operating system on Linux
- Compiler what support C++ 17

# Installation:
Clone repository somewere and perform commands:
```
cd ./ChronoTimer
cmake ./CMakeLists.txt
make install
```

# Example of usage:
```C++
#include <iostream>
#include <mutex>
#include <sstream>

#include "chronotimer.h"

using namespace std;

// Measured function
auto TestFunction() {
    cout << "Im a test function! Hello World!" << endl;
    size_t n = 1'000'000;
    do{
        n--;
    }while(n>0);
    return "Well done!";
}

// Measured function with arguments
auto TestFunctionWithArg(std::string ret) {
    cout << "Im a test function With arguments!" << endl;
    size_t n = 2'000'000;
    do{
        n--;
    }while(n>0);
    return ret;
}

int main() {
  // Example of usage 1
  {
    Estimate::ChronoTimer timer(TestFunction);
    cout << timer.Run() << endl << timer.GetTime<>() << endl;
  }
  // Example of usage 2
  {
    cout << Estimate::ChronoTimer(TestFunction).RunAndPrint(cout) << endl;
  }
  // Example of usage 3
  {
    Estimate::ChronoTimer timer(TestFunctionWithArg,"Good args!");
    cout << timer.Run() << endl << timer.GetTime<>() << endl;
  }
  // Example of usage 4
  {
    cout << Estimate::ChronoTimer(TestFunctionWithArg,"Good args!").RunAndPrint(cout) << endl;
  }
  return 0;
}

```
