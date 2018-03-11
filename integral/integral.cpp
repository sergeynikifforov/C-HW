#include <iostream>
#include <thread>
#include <math.h>
#include <mutex>
#include <vector>
#include <sstream>


double funcVal(double a) {
	return a * a*tan(a)*exp(a) - sin(a)*cos(tan(sqrt(a)));
}
void calculate(double left_edge, double right_edge, int step_func, double& perem_res) {
	double var_res = 0;
	double *var_lim = new double[step_func + 1];
	for (int j = 0; j <= step_func; j++) {
		var_lim[j] = left_edge + j * (right_edge - left_edge) / step_func;
	}
	for (int j = 0; j<step_func; j++) {
		var_res += (var_lim[j + 1] - var_lim[j]) * (funcVal(var_lim[j]) + funcVal(var_lim[j + 1])) / 2;
	}
	perem_res = var_res;
}
int main(int argc, char *argv[])
{
	if (argc != 4) {
		std::cerr << "wrong input";
	}
	else {
		double result = 0;
		int n;
		double lower_limit;
		double upper_limit;
		const int const_step = 1000000;
		std::stringstream new_stream;
		new_stream << argv[1] << ' ' << argv[2] << ' ' << argv[3];
		new_stream >> n >> lower_limit >> upper_limit;
		const int step1 = const_step / n;
		const int step2 = const_step - step1 * (n - 1);
		double *res_lim = new double[n + 1];
		auto start = std::chrono::high_resolution_clock::now();
		double h = (upper_limit - lower_limit) / n;
		for (int k = 0; k <= n; k++) {
			res_lim[k] = lower_limit + k * h;
		}
		double *k = new double[n];
		std::vector<std::thread> threads;
		for (int i = 0; i < n; i++) {
			if (i != n - 1) {
				threads.push_back(std::thread(calculate, res_lim[i], res_lim[i + 1], step1, std::ref(k[i])));
			}
			else {
				threads.push_back(std::thread(calculate, res_lim[i], res_lim[i + 1], step2, std::ref(k[i])));
			}
		}
		for (auto & th : threads)
			th.join();
		for (int i = 0; i < n; i++) {
			result += k[i];
		}
		auto end = std::chrono::high_resolution_clock::now(); //вывод времени
		std::cout << result << std::endl;
		std::cerr << std::chrono::duration<unsigned long int, std::nano>(end - start).count() << std::endl;
	}
	return 0;
}