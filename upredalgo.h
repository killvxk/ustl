// This file is part of the ustl library, an STL implementation.
// Copyright (C) 2003 by Mike Sharov <msharov@talentg.com>
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Library General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public
// License along with this library; if not, write to the 
// Free Software Foundation, Inc., 59 Temple Place - Suite 330, 
// Boston, MA  02111-1307  USA.
//
// ualgo.h
//
// Implementation of STL algorithms with custom predicates.
//
// The function prototypes are copied
// exactly from the SGI version of STL documentation along with comments about
// their use. The code is NOT the same, though the functionality usually is.
//

#ifndef UPREDALGO_H
#define UPREDALGO_H

namespace ustl {

///
/// Copy_if copies elements from the range [first, last) to the range
/// [result, result + (last - first)) if pred(*i) returns true.
///
template <typename InputIterator, typename OutputIterator, typename Predicate>
inline OutputIterator copy_if (InputIterator first, InputIterator last, OutputIterator result, Predicate pred)
{
    for (; first != last; ++first) {
	if (pred(*first)) {
	    *result = *first;
	    ++ result;
	}
    }
    return (result);
}

///
/// Returns the first iterator i in the range [first, last) such that
/// pred(*i) is true. Returns last if no such iterator exists.
///
template <typename InputIterator, typename Predicate>
inline InputIterator find_if (InputIterator first, InputIterator last, Predicate pred)
{
    while (first != last && !pred (*first))
	++ first;
    return (first);
}

/// Returns the first iterator such that p(*i, *(i + 1)) == true.
template <typename ForwardIterator, typename BinaryPredicate>
inline ForwardIterator adjacent_find (ForwardIterator first, ForwardIterator last, BinaryPredicate p)
{
    if (first != last)
	for (ForwardIterator prev = first; ++first != last; ++ prev)
	    if (p (*prev, *first))
		return (prev);
    return (last);
}

///
/// Returns the pointer to the first pair of unequal elements.
///
template <typename InputIterator, typename BinaryPredicate>
inline pair<InputIterator,InputIterator>
mismatch (InputIterator first1, InputIterator last1, InputIterator first2, BinaryPredicate comp)
{
    while (first1 != last1 && comp(*first1, *first2))
	++ first1, ++ first2;
    return (make_pair (first1, first2));
}

///
/// Returns true if two ranges are equal.
/// This is an extension, present in uSTL and SGI STL.
///
template <typename InputIterator, typename BinaryPredicate>
inline bool equal (InputIterator first1, InputIterator last1, InputIterator first2, BinaryPredicate comp)
{
    return (mismatch (first1, last1, first2, comp).first == last1);
}

///
/// Count_if finds the number of elements in [first, last) that satisfy the
/// predicate pred. More precisely, the first version of count_if returns the
/// number of iterators i in [first, last) such that pred(*i) is true.
///
template <typename InputIterator, typename Predicate>
inline size_t count_if (InputIterator first, InputIterator last, Predicate pred)
{
    size_t total = 0;
    for (; first != last; ++first)
	if (pred (*first))
	    ++ total;
    return (total);
}

///
/// Replace_if replaces every element in the range [first, last) for which
/// pred returns true with new_value. That is: for every iterator i, if
/// pred(*i) is true then it performs the assignment *i = new_value.
///
template <typename ForwardIterator, typename Predicate, typename T>
inline void replace_if (ForwardIterator first, ForwardIterator last, Predicate pred, const T& new_value)
{
    for (; first != last; ++first)
	if (pred (*first))
	    *first = new_value;
}

///
/// Replace_copy_if copies elements from the range [first, last) to the range
/// [result, result + (last-first)), except that any element for which pred is
/// true is not copied; new_value is copied instead. More precisely, for every
/// integer n such that 0 <= n < last-first, replace_copy_if performs the
/// assignment *(result+n) = new_value if pred(*(first+n)),
/// and *(result+n) = *(first+n) otherwise.
///
template <typename InputIterator, typename OutputIterator, typename Predicate, typename T>
inline OutputIterator replace_copy_if (InputIterator first, InputIterator last, OutputIterator result, Predicate pred, const T& new_value) 
{
    for (; first != last; ++result, ++first)
        *result = pred(*first) ? new_value : *first;
}

///
/// Remove_copy_if copies elements from the range [first, last) to a range
/// beginning at result, except that elements for which pred is true are not
/// copied. The return value is the end of the resulting range. This operation
/// is stable, meaning that the relative order of the elements that are copied
/// is the same as in the range [first, last).
///
template <typename InputIterator, typename OutputIterator, typename Predicate>
inline OutputIterator remove_copy_if (InputIterator first, InputIterator last, OutputIterator result, Predicate pred)
{
    for (; first != last; ++first)
	if (pred (*first))
	    *result++ = *first;
    return (result);
}

///
/// Remove_if removes from the range [first, last) every element x such that
/// pred(x) is true. That is, remove_if returns an iterator new_last such that
/// the range [first, new_last) contains no elements for which pred is true.
/// The iterators in the range [new_last, last) are all still dereferenceable,
/// but the elements that they point to are unspecified. Remove_if is stable,
/// meaning that the relative order of elements that are not removed is
/// unchanged.
///
template <typename ForwardIterator, typename Predicate>
inline ForwardIterator remove_if (ForwardIterator first, ForwardIterator last, Predicate pred)
{
    return (remove_copy_if (first, last, first, pred));
}

///
/// Returns the furthermost iterator i in [first, last) such that,
/// for every iterator j in [first, i), comp(*j, value) is true.
/// Assumes the range is sorted.
///
template <typename ForwardIterator, typename T, typename StrictWeakOrdering>
ForwardIterator lower_bound (ForwardIterator first, ForwardIterator last, const T& value, StrictWeakOrdering comp)
{
    ForwardIterator mid;
    while (first != last) {
	mid = advance (first, distance (first,last) / 2);
	if (comp (*mid, value))
	    first = mid + 1;
	else
	    last = mid;
    }
    return (first);
}

///
/// Performs a binary search inside the sorted range.
///
template <typename ForwardIterator, typename T, typename StrictWeakOrdering>
inline ForwardIterator binary_search (ForwardIterator first, ForwardIterator last, const T& value, StrictWeakOrdering comp)
{
    ForwardIterator found = lower_bound (first, last, value, comp);
    return ((found == last || comp(value, *found)) ? last : found);
}

///
/// Returns the furthermost iterator i in [first,last) such that for
/// every iterator j in [first,i), comp(value,*j) is false.
///
template <typename ForwardIterator, typename T, typename StrictWeakOrdering>
ForwardIterator upper_bound (ForwardIterator first, ForwardIterator last, const T& value, StrictWeakOrdering comp)
{
    ForwardIterator mid;
    while (first != last) {
	mid = advance (first, distance (first,last) / 2);
	if (comp (value, *mid))
	    last = mid;
	else
	    first = mid + 1;
    }
    return (last);
}

///
/// Returns pair<lower_bound,upper_bound>
///
template <typename ForwardIterator, typename T, typename StrictWeakOrdering>
inline pair<ForwardIterator,ForwardIterator> equal_range (ForwardIterator first, ForwardIterator last, const T& value, StrictWeakOrdering comp)
{
    pair<ForwardIterator,ForwardIterator> rv;
    rv.second = rv.first = lower_bound (first, last, value, comp);
    while (rv.second != last && !comp(value, *(rv.second)))
	++ rv.second;
    return (rv);
}

}; // namespace ustl

#endif
