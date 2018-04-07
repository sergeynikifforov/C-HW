#include <iostream>
#include "ThreadPool.h"
#include "Integral.h"
#include <sstream>
int main(int argc, char *argv[]) {
    if (argc != 5) {
        std::cerr << "wrong input";
    }
    else {
        unsigned int n;
        double lower_limit;
        double upper_limit;
        unsigned int numofth;
        const int const_step = 1000000;
        std::stringstream new_stream;
        new_stream << argv[1] << ' ' << argv[2] << ' ' << argv[3]<< ' ' << argv[4];
        new_stream >> numofth >> n >> lower_limit >> upper_limit;
        const int step = const_step /n;
        double res_lim[n + 1];
        auto start = std::chrono::high_resolution_clock::now();
        double h = (upper_limit - lower_limit) / n;
        for (int k = 0; k <= n; k++) {
            res_lim[k] = lower_limit + k * h;
        }
        ThreadPool pools{numofth};
        auto f1 = pools.enqueue([&]{
                double result =  calculate(res_lim[0], res_lim[1], step);
                return result;
        }
        );
        decltype(f1) flows[n-1];
        for(int i = 0 ; i < n-1; i ++){
            flows[i] = pools.enqueue([&,i]{
                double result =  calculate(res_lim[i+1], res_lim[i+2], step);
                return result;
            });
        }
        auto res = f1.get();
        for(auto &each_flow : flows){
            res = res + each_flow.get();
        }
        std::cout << res << std::endl;
        auto end = std::chrono::high_resolution_clock::now(); //вывод времени
        std::cerr << std::chrono::duration<unsigned long int, std::nano>(end - start).count() << std::endl;
    }
    return 0;
}