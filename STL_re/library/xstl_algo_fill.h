#ifndef LIBRARY_XSTL_ALGO_FILL_H__
#define LIBRARY_XSTL_ALGO_FILL_H__

namespace xstl
{
	namespace algo
	{
		template <typename ForwardIt, typename T>
		void fill(ForwardIt first, ForwardIt last, const T& value)
		{
			for(; first != last; ++first) {
				*first = value;
			}
		}

		/* Since C++17
		template <typename ExecutionPolicy, typename ForwardIt, typename T>
		void fill(ExecutionPolicy&& policy, ForwardIt first, ForwardIt last, const T& value)
		{

		}
		*/

		/* Unitl C++11
		template <typename OutputIt, typename Size, typename T>
		void fill_n(OutputIt first, Size count, const T& value)
		{
			for(Size i = 0; i < count; ++i) {
				*first++ = value;
			}
		}
		*/

		template <typename OutputIt, typename Size, typename T>
		OutputIt fill_n(OutputIt first, Size count, const T& value)
		{
			for(Size i = 0; i < count; ++i) {
				*first++ = value;
			}
			return first;
		}

		/* Since C++17
 		template <typename ExecutionPolicy, typenamep OutputIt, typename Size, typename T>
		OutputIt fill_n(ExecutionPolicy&& policy, OutputIt first, OutputIt last,
			Size count, const T& value)
		{

		}
		*/
	}
}

#endif // LIBRARY_XSTL_ALGO_FILL_H__