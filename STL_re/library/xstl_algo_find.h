#ifndef LIBRARY_XSTL_ALGO_FIND_H__
#define LIBRARY_XSTL_ALGO_FIND_H__

namespace xstl
{
	namespace algo
	{
		template <typename InputIt, typename T>
		InputIt find(InputIt first, InputIt last, const T& value)
		{
			for(; first != last; ++first) {
				if(*first == value) {
					return first;
				}
			}
			return last;
		}

		/* Since C++17
		template <typename ExecutionPolicy, typename InputIt, typename T>
		InputIt find(ExecutionPolicy&& policy, InputIt first, InputIt last, const T& value)
		{

		}
		*/

		template <typename InputIt, typename UnaryPredicate>
		InputIt find_if(InputIt first, InputIt last,
			UnaryPredicate p)
		{
			for(; first != last; ++first) {
				if(p(*first)) {
					return first;
				}
			}
			return last;
		}

		/* Since C++17
		template <typename ExecutionPolicy, typename InputIt, typename UnaryPredicate>
		InputIt find_if(ExecutionPolicy&& policy, InputIt first, InputIt last,
			UnaryPredicate p)
		{

		}
		*/

		template <typename InputIt, typename UnaryPredicate>
		InputIt find_if_not(InputIt first, InputIt last,
			UnaryPredicate q)
		{	
			for(; first != last; ++first) {
				if(!q(*first)) {
					return first;
				}
			}
			return last;
		}

		/* Since C++17
		template <typename ExecutionPolicy, typename InputIt, typename UnaryPredicate>
		InputIt find_if_not(ExecutionPolicy&& policy, InputIt first, InputIt last,
			UnaryPredicate q)
		{

		}
		*/
	}
}

#endif // LIBRARY_XSTL_ALGO_FIND_H__