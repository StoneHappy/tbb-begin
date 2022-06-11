#include <iostream>
#include <tbb/blocked_range.h>
#include <tbb/parallel_for.h>
#include <tbb/parallel_reduce.h>
//void func(int v) {
//	cout << "v = " << v << endl;
//}
//
//class Apply {
//public:
//	int* my_a;
//	void operator()(const tbb::blocked_range<size_t>& r) const {
//		cout << "r begin: " << r.begin() << ", end: " << r.end() << endl;
//		for (auto i = r.begin(); i != r.end(); ++i) {
//			func(my_a[i]);
//		}
//	}
//	Apply(int a[]) {
//		my_a = a;
//	}
//};

int main(void) {
	//const int N = 100;
	//int a[N];
	//for (int i = 0; i < N; ++i)
	//	a[i] = i * 2;

	//// 使用类的()重载实现
	// parallel_for(blocked_range<size_t>(0, N), Apply(a));		

	// 使用匿名函数进行实现
	/*parallel_for(blocked_range<size_t>(0, N), [&](const tbb::blocked_range<size_t>& r) {
		cout << "r begin: " << r.begin() << ", end: " << r.end() << endl;
		for (auto i = r.begin(); i != r.end(); ++i) {
			cout << " i = " << i << endl;
		}
		});*/

	int sum = oneapi::tbb::parallel_reduce(oneapi::tbb::blocked_range<int>(1, 101), 0,
		[](oneapi::tbb::blocked_range<int> const& r, int init) -> int {
			for (int v = r.begin(); v != r.end(); v++) {
				//std::cout << "v: " << v << std::endl;
				init += v;
			}
			return init;
		},
		[](int lhs, int rhs) -> int {
			std::cout << "sadas" << std::endl;
			return lhs + rhs;
		}
		);
	return 0;
}
