#ifndef TIMER_HPP
#define TIMER_HPP
#include <chrono>

class Timer
{
private:
   std::chrono::time_point<std::chrono::high_resolution_clock> begin;
   std::chrono::time_point<std::chrono::high_resolution_clock> end;
public:
   void tic()
   {
      begin = std::chrono::high_resolution_clock::now();
   }
   void toc()
   {
      end = std::chrono::high_resolution_clock::now();
   }
   double etime()
   {
      //regresa tiempo en mili segundos
      return std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count()/1000000.;
   }

};
#endif
