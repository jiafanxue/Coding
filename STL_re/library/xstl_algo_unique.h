#ifndef LIBRARY_XSTL_ALGO_UNIQUE_H__
#define LIBRARY_XSTL_ALGO_UNIQUE_H__
	
namespace xstl
{
	namespace algo
	{
		template <typename ForwardIt>
		ForwardIt unique(ForwardIt first, ForwardIt last)
		{
			if(first == last) {
				return last;
			}

			ForwardIt result = first;
			while(++first != last) {
				if(!(*result == *first) && ++result != first) {
					*result = std::move(*first);
				}
			}
			return result;
		}

		/* Since C++17
		template <typename ExecutionPolicy, typename ForwardIt>
		ForwardIt unique(ExecutionPolicy&& policy, ForwardIt first, ForwardIt last)
		{

		}
		*/

		template <typename ForwardIt, typename BinaryPredicate>
		ForwardIt unique(ForwardIt first, ForwardIt last, BinaryPredicate p)
		{
			if(first == last) {
				return last;
			}

			ForwardIt result = first;
			while(++first != last) {
				if(!p(*result, *first) && ++result != first) {
					*result = std::move(*first);
				}
			}
			return result;
		}

		/* Since C++17
		template <typename ExecutionPolicy, typename ForwardIt, typename BinaryPredicate>
		ForwardIt unique(ExecutionPolicy&& policy, ForwardIt first, ForwardIt last, BinaryPredicate p)
		{

		}
		*/ 

	}
}

#endif // LIBRARY_XSTL_ALGO_UNIQUE_H__