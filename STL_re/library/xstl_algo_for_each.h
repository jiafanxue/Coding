#ifndef LIBRARY_XSTL_ALGO_FOR_EACH_H__
#define LIBRARY_XSTL_ALGO_FOR_EACH_H__

namespace xstl
{
	namespace algo
	{
		template <typename InputIt, typename UnaryFunction>
		UnaryFunction for_each(InputIt first, InputIt last, UnaryFunction f)
		{
			for(; first != last; ++first) {
				f(*first);
			}
			return f;
		}

		/* Since C++17
		template <typename ExecutionPolicy, typename InputIt, typename UnaryFunction2>
		void for_each(ExecutionPolicy&& policy, InputIt first, InputIt last, UnaryFunction2 f)
		{

		}
		*/

	}
}

#endif // LIBRARY_XSTL_ALGO_FOR_EACH_H__