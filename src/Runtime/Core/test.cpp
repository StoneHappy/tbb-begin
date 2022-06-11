#include "Core/test.h"
#include <tbb/tbb.h>
#include <iostream>

static const std::size_t N = 22;

struct ProcessBar
{
	ProcessBar()
	{
		std::cout << "------------------------------" << std::endl;
	}

	unsigned long operator+=(unsigned long increment)
	{
		for (size_t i = 0; i < increment; i++)
		{
			std::cout << "*" << std::flush;
		}
		count += increment;
		return count;
	}
	unsigned long count = 0;
};


class SubStringFinder {
	const char* str;
	const std::size_t len;
	std::size_t* max_array;
	std::size_t* pos_array;

public:
	void operator()(const oneapi::tbb::blocked_range<std::size_t>& r) const {
		for (std::size_t i = r.begin(); i != r.end(); ++i) {
			std::size_t max_size = 0, max_pos = 0;
			for (std::size_t j = 0; j < len; ++j) {
				if (j != i) {
					std::size_t limit = len - (std::max)(i, j);
					for (std::size_t k = 0; k < limit; ++k) {
						if (str[i + k] != str[j + k])
							break;
						if (k > max_size) {
							max_size = k;
							max_pos = j;
						}
					}
				}
			}
			max_array[i] = max_size;
			pos_array[i] = max_pos;
		}
	}
	// We do not use std::vector for compatibility with offload execution
	SubStringFinder(const char* s, const std::size_t s_len, std::size_t* m, std::size_t* p)
		: str(s),
		len(s_len),
		max_array(m),
		pos_array(p) {}
};


int SumWithnoLock()
{
	int rnt = 0;
	tbb::parallel_for(tbb::blocked_range<size_t>(1, 101), [&](const tbb::blocked_range<size_t>& r) {
		for (size_t i = r.begin(); i != r.end(); i++)
		{
			rnt ++;
		}
		});
	return rnt;
}

int SumWithLock()
{
	int process = 0;
	int rnt = 0;
	tbb::null_mutex mtx;
	tbb::parallel_for(tbb::blocked_range<size_t>(1, 101), [&](const tbb::blocked_range<size_t>& r) {
		for (size_t i = r.begin(); i != r.end(); i++)
		{
			tbb::null_mutex::scoped_lock lock(mtx);
			process += i; 
			rnt += i;
		}
		});

	return rnt;
}