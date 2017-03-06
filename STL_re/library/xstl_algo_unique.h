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

		template <typename InputIt, typename OutputIt>
		OutputIt unique_copy(InputIt first, InputIt last,
			OutputIt d_first)
		{
			if(first == last) {
				return d_first;
			}

			InputIt result = first;
			*d_first = *first;
			while(++first != last) {
				if(!(*result == *first) && ++result != first) {
					*result = std::move(*first);
				}
				*++d_first = *result;
			}
			return ++d_first;
		}	

		/* Since C++17
		template <typename ExecutionPolicy, typename InputIt, typename OutputIt>
		OutputIt unqiue_copy(ExecutionPolicy&& policy, InputIt first, InputIt last,
			OutputIt d_first)
		{

		}
		*/

		template <typename InputIt, typename OutputIt, typename BinaryPredicate>
		OutputIt unique_copy(InputIt first, InputIt last,
			OutputIt d_first, BinaryPredicate p)
		{
			if(first == last) {
				return d_first;
			}

			InputIt result = first;
			*d_first = *first;
			while(++first != last) {
				if(!p(*result, *first) && ++result != first) {
					*result = std::move(*first);
				}
				*++d_first = *result;
			}
			return ++d_first;
		}

		/* Since C++17
		template <typename ExecutionPolicy, typename InputIt, typename OutputIt, typename BinaryPredicate>
		OutputIt unqiue_copy(ExecutionPolicy&& policy, InputIt first, InputIt last,
			OutputIt d_first, BinaryPredicate p)
		{

		}
		*/
	}
}

#endif // LIBRARY_XSTL_ALGO_UNIQUE_H__