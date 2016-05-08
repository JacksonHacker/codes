#ifndef ALGORITHM_H
#define ALGORITHM_H

template <typename InputIterator, typename Function>
Function for_each(InputIterator first, InputIterator last, Function f);
template <typename InputIterator, typename Function>
Function for_each(InputIterator first, InputIterator last, Function f) {
	for ( ; first != last; ++first)
		f(*first);
	return f;
}

#endif