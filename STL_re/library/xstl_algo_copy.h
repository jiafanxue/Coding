#ifndef LIBRARY_XSTL_ALGO_COPY_H__
#define LIBRARY_XSTL_ALGO_COPY_H__

namespace xstl
{
	namespace algo
	{
		template <typename InputIt, typename OutputIt>
		OutputIt copy(InputIt first, InputIt last, OutputIt d_first)
		{
			while(first != last) {
				*d_first++ = *first++;
			}
			return d_first;
		}

		/* Since C++17
		template <typename ExecutionPolicy, typename InputIt, typename OutputIt>
		OutputIt copy(ExecutionPolicy&& policy, InputIt first, InputIt last,
			OutputIt d_first)
		{

		}
		*/ 

		template <typename InputIt, typename OutputIt, typename UnaryPredicate>
		OutputIt copy_if(InputIt first, InputIt last,
			OutputIt d_first, UnaryPredicate pred)
		{
			while(first != last) {
				if(pred(*first)) {
					*d_first++ = *first;
				}
				*first++;
			}
			return d_first;
		}

		/* Since C++17
		template <typename ExecutionPolicy, typename InputIt, typename OutputIt, typename UnaryPredicate>
		OutputIt copy_if(ExecutionPolicy&& policy, InputIt first, InputIt last,
			OutputIt d_first, UnaryPredicate pred)
		{

		}
		*/

		template <typename InputIt, typename Size, typename OutputIt>
		OutputIt copy_n(InputIt first, Size count, OutputIt result)
		{
			if(count > 0) {
				*result++ = *first;
				for(Size i = 1; i < count; ++i) {
					*result++ = *++first;
				}

				/*
				why not ?
				for(Size = 0; i < count; ++i) {
					*result = *first++;
				}
				*/
			}
			return result;
		}

		/* Since C++17
		template <typename ExecutionPolicy, typename InputIt, typename Size, typename OutputIt>
		OutputIt copy_n(ExecutionPolicy&& policy, InputIt first, InputIt last, 
			Size count, OutputIt result)
		{

		}
		*/

		template <typename BidirIt1, typename BidirIt2>
		BidirIt2 copy_backward(BidirIt1 first, BidirIt1 last, BidirIt2 d_last)
		{
			while(first != last) {
				*(--d_last) = *(--last);
			}
			return d_last;
		}
	}
}

#endif // LIBRARY_XSTL_ALGO_COPY_H__