#ifndef LIBRARY_XSTL_ALGO_EQUAL_H__
#define LIBRARY_XSTL_ALGO_EQUAL_H__

namespace xstl
{
	namespace algo
	{
		template <typename InputIt1, typename InputIt2>
		bool equal(InputIt1 first1, InputIt1 last1,
			InputIt2 first2)
		{
			for(; first1 != last1; ++first1, ++first2) {
				if(!(*first1 == *first2)) {
					return false;
				}
			}
			return true;
		}

		/* Since C++17
		template <typename ExecutionPolicy, typename ForwardIt1, typename ForwardIt2>
		bool equal(ExecutionPolicy&& policy, ForwardIt1 first1, ForwardIt1 last1,
			ForwardIt2 first2)
		{

		}
		*/

		template <typename InputIt1, typename InputIt2, typename BinaryPredicate>
		bool equal(InputIt1 first1, InputIt1 last1,
			InputIt2 first2, BinaryPredicate p)
		{
			for(; first1 != last1; ++first1, ++first2) {
				if(!p(*first1, *first2)) {
					return false;
				}
			}
			return true;
		}

		/* Since C++17
		template <typename ExecutionPolicy, typename ForwardIt1, typename ForwardIt2, typename BinaryPredicate>
		bool equal(ExecutionPolicy&& policy, ForwardIt1 first1, ForwardIt1 last1,
			ForwardIt2 first2, BinaryPredicate p)
		{

		}
		*/

		// Since C++14
		template <typename InputIt1, typename InputIt2>
		bool equal(InputIt1 first1, InputIt1 last1,
			InputIt2 first2, InputIt2 last2)
		{
			int size1 = last1 - first1;
			int size2 = last2 - first2;
			if(size1 != size2) {
				return false;
			}
			for(; first1 != last1; ++first1, ++first2) {
				if(!(*first1 == *first2)) {
					return false;
				}
			}
			return true;
		}
		

		/* Since C++17
		template <typename ExecutionPolicy, typename ForwardIt1, typename ForwardIt2>
		bool equal(ExecutionPolicy%% policy, ForwardIt1 first1, ForwardIt1 last1,
			ForwardIt2 first2, ForwardIt2 last2)
		{

		}
		*/

		/* Since C++14
		template <typename InputIt1, typename InputIt2, typename BinaryPredicate>
		bool equal(InputIt1 first1, InputIt1 last1, 
			InputIt2 first2, InputIt2 last2,
			BinaryPredicate p)
		{

		}
		*/

		/* Since C++17
		template <typename ExecutionPolicy, typename ForwardIt1, typename ForwardIt2, typename BinaryPredicate>
		bool equal(ExecutionPolicy&& policy, ForwardIt1 first1, ForwardIt1 last1,
			ForwardIt2 first2, ForwardIt2 last2,
			BinaryPredicate p)
		{

		}
		*/
	}
}

#endif // LIBRARY_XSTL_ALGO_EQUAL_H__