// xutility internal header
#pragma once
#ifndef _VS2008_XUTILITY_
#define _VS2008_XUTILITY_


#ifndef RC_INVOKED
//#include <climits>
//#include <utility>
#include <crtdbg.h>
#include <yvals.h>
#include <xutility>

#ifdef _MSC_VER
#pragma pack(push,_CRT_PACKING)
#pragma warning(push,3)
#endif  /* _MSC_VER */

//using namespace std;
namespace VS2008 { //_STD_BEGIN

	// RANGE CHECKED ITERATOR TAGS
	struct _Unchecked_iterator_tag
	{
	};
	struct _Range_checked_iterator_tag
	{
	};

	// CHECKED ITERATOR BASE TAGS
	struct _Unchanged_checked_iterator_base_type_tag
	{
	};

	struct _Different_checked_iterator_base_type_tag
	{
	};

	// INNER TYPE TAG
	struct _Undefined_inner_type_tag
	{
	};

	// MOVE OPERATION TAG
	struct _Undefined_move_tag
	{
	};

	struct _Swap_move_tag
	{
	};

	class _Iterator_base_universal
	{	// base of all iterators
	public:
		typedef _Unchecked_iterator_tag _Checked_iterator_category;
		typedef _Unchanged_checked_iterator_base_type_tag _Checked_iterator_base_type;
		typedef _Undefined_inner_type_tag _Inner_type;
	};

	// Used to initialize _Mycont when _SECURE_SCL is off
#define _IGNORE_MYCONT ((const _Container_base_secure *)-4)

	class _CRTIMP2_PURE _Container_base_secure
	{	// base of all strings
	};

	class _Iterator_base_secure : public _Iterator_base_universal
	{	// base of all iterators
	public:
		// SecureSCL needs the base container
		__CLR_OR_THIS_CALL _Iterator_base_secure()
			: _Mycont(0)
		{
		}

		const _Container_base_secure* __CLR_OR_THIS_CALL _Getmycont() const
		{	// This member function always exists when we can get a container pointer
			return _Mycont;
		}

		bool __CLR_OR_THIS_CALL _Same_container(const _Iterator_base_secure& _Other) const
		{	// This member function always exists when we can get a container pointer
			return _Mycont == _Other._Mycont;
		}

		bool __CLR_OR_THIS_CALL _Has_container() const
		{	// This member function always exists when we can get a container pointer
			return _Mycont != 0;
		}

		const _Container_base_secure* _Mycont;
	};

	struct _Int_iterator_tag
	{	// identifying tag for integer types, not an iterator
	};

	struct _Float_iterator_tag
	{	// identifying tag for floating point types, not an iterator
	};

	// POINTER ITERATOR TAGS
	struct _Nonscalar_ptr_iterator_tag
	{	// pointer to unknown type
	};
	struct _Scalar_ptr_iterator_tag
	{	// pointer to scalar type
	};

	class _Container_base_aux;

	class _Aux_cont
	{
	public:
		explicit _Aux_cont(const _Container_base_aux* const _Pcont)
			: _Mycontainer(_Pcont) { }

		const _Container_base_aux* _Getcont() const
		{
			return _Mycontainer;
		}

	private:
		const _Container_base_aux* _Mycontainer;
	};

	class _Container_base_aux
	{	// base of all containers
	public:
		_Container_base_aux()
		{
			// Do nothing: _Container_base_aux_alloc_real() will construct the aux object.
		}

		_Container_base_aux(const _Container_base_aux&)
		{
			// Do nothing: _Container_base_aux_alloc_real() will construct the aux object.
		}

		_Container_base_aux& operator=(const _Container_base_aux&)
		{
			// Do nothing: keep our aux object.
			return *this;
		}

		void _Swap_aux(_Container_base_aux& _Right)
		{
			_STD swap(_Myownedaux, _Right._Myownedaux);
			_STD swap(*_Myownedaux, *_Right._Myownedaux);
		}

		~_Container_base_aux()
		{
			// Do nothing: ~_Container_base_aux_alloc_real() previously destroyed the aux object.
		}

	protected:
		friend class _Iterator_base_aux;

		_Aux_cont* _Myownedaux;
	};

	template<class _Alloc>
	class _Container_base_aux_alloc_real
		: public _Container_base_aux
	{ // base class for containers to hold allocator _Alaux
	protected:
		explicit _Container_base_aux_alloc_real(_Alloc _Al)
			: _Alaux(_Al)
		{
			_Myownedaux = new (_Alaux.allocate(1)) _Aux_cont(this);
		}

		_Container_base_aux_alloc_real(const _Container_base_aux_alloc_real& _Right)
			: _Alaux(_Right._Alaux)
		{
			_Myownedaux = new (_Alaux.allocate(1)) _Aux_cont(this);
		}

		_Container_base_aux_alloc_real& operator=(const _Container_base_aux_alloc_real&)
		{
			// Do nothing: keep our aux object.
			return *this;
		}

		~_Container_base_aux_alloc_real()
		{
			_Myownedaux->~_Aux_cont();

			_Alaux.deallocate(_Myownedaux, 1);
		}

		typename _Alloc::template rebind<_Aux_cont>::other _Alaux; // allocator object for aux objects
	};

	class _Iterator_base_aux : public _Iterator_base_universal
	{	// base of all iterators
	public:
		_Iterator_base_aux()
			: _Myaux(0) { }

		void _Set_container(const _Container_base_aux* const _Pcont)
		{
			_Myaux = _Pcont->_Myownedaux;
		}

		const _Container_base_aux* _Getmycont() const
		{	// Go through the aux object to get the container
			return _Myaux ? _Myaux->_Getcont() : 0;
		}

		bool _Same_container(const _Iterator_base_aux& _Other) const
		{	// Accelerate the comparison by not going through the aux object
			return _Myaux == _Other._Myaux;
		}

		bool _Has_container() const
		{	// Accelerate the comparison by not going through the aux object
			return _Myaux != 0;
		}

	private:
		const _Aux_cont* _Myaux;
	};

	typedef _Container_base_aux _Container_base;
	typedef _Iterator_base_aux _Iterator_base;

#define _CONTAINER_BASE_AUX_ALLOC _Container_base_aux_alloc_real

	template<class _Category,
		class _Ty2,
		class _Diff = ptrdiff_t,
		class _Pointer = _Ty2*,
		class _Reference = _Ty2&,
		class _Base_class = _Iterator_base>
		struct _Iterator_with_base
		: public _Base_class

	{	// base type for all iterator classes
		typedef _Category iterator_category;
		typedef _Ty2 value_type;
		typedef _Diff difference_type;
		typedef _Diff distance_type;	// retained
		typedef _Pointer pointer;
		typedef _Reference reference;
	};

	template<class _Ty2,
		class _Diff,
		class _Pointer,
		class _Reference>
		struct _Bidit
		: public _Iterator_with_base<std::bidirectional_iterator_tag,
		_Ty2, _Diff, _Pointer, _Reference>
	{	// base type for container bidirectional iterator classes
	};

	template<class _Ty2,
		class _Diff,
		class _Pointer,
		class _Reference,
		class _Base_class>
		struct _Ranit_base
		: public _Base_class

	{	// base type for container random-access iterator classes
		typedef std::random_access_iterator_tag iterator_category;
		typedef _Ty2 value_type;
		typedef _Diff difference_type;
		typedef _Diff distance_type;	// retained
		typedef _Pointer pointer;
		typedef _Reference reference;
	};


	// TEMPLATE FUNCTION _Checked_cat
	template <class _Iter, bool _Inherits_from_iterator_base>
	class _Checked_iterator_category_helper
	{
	public:
		typedef _Unchecked_iterator_tag _Checked_cat;
	};

	template <class _Iter>
	class _Checked_iterator_category_helper<_Iter, true>
	{
	public:
		typedef typename _Iter::_Checked_iterator_category _Checked_cat;
	};

	template <class _Iter>
	class _Checked_iterator_category
	{
	public:
		typedef typename _Checked_iterator_category_helper<_Iter, __is_base_of(_Iterator_base_universal, _Iter)>::_Checked_cat _Checked_cat;
	};

	template<class _Iter>
	inline
		typename _Checked_iterator_category<_Iter>::_Checked_cat _Checked_cat(const _Iter&)
	{
		typename _Checked_iterator_category<_Iter>::_Checked_cat _Cat;
		return (_Cat);
	}

	// TEMPLATE FUNCTION _Ptr_cat
	template<class _T1, class _T2, class _Checked_Cat1, class _Checked_Cat2>
	struct _Ptr_cat_with_checked_cat_helper
	{
		typedef _Nonscalar_ptr_iterator_tag _Ptr_cat;
	};

	template<class _T1, class _T2>
	struct _Ptr_cat_helper
	{
		typedef typename _Ptr_cat_with_checked_cat_helper<_T1, _T2,
			typename _Checked_iterator_category<_T1>::_Checked_cat,
			typename _Checked_iterator_category<_T2>::_Checked_cat>::_Ptr_cat _Ptr_cat;
	};

	template<class _T1, class _T2, class _Checked_Cat1>
	struct _Ptr_cat_with_checked_cat_helper<_T1, _T2, _Checked_Cat1, _Range_checked_iterator_tag>
	{
		typedef typename _Ptr_cat_helper<_T1, typename _T2::_Inner_type>::_Ptr_cat _Ptr_cat;
	};

	template<class _T1, class _T2, class _Checked_Cat2>
	struct _Ptr_cat_with_checked_cat_helper<_T1, _T2, _Range_checked_iterator_tag, _Checked_Cat2>
	{
		typedef typename _Ptr_cat_helper<typename _T1::_Inner_type, _T2>::_Ptr_cat _Ptr_cat;
	};

	template<class _T1, class _T2>
	struct _Ptr_cat_with_checked_cat_helper<_T1, _T2, _Range_checked_iterator_tag, _Range_checked_iterator_tag>
	{
		typedef typename _Ptr_cat_helper<typename _T1::_Inner_type, typename _T2::_Inner_type>::_Ptr_cat _Ptr_cat;
	};

	template<class _T1>
	struct _Ptr_cat_helper<_T1, _Undefined_inner_type_tag>
	{
		typedef _Nonscalar_ptr_iterator_tag _Ptr_cat;
	};

	template<class _T2>
	struct _Ptr_cat_helper<_Undefined_inner_type_tag, _T2>
	{
		typedef _Nonscalar_ptr_iterator_tag _Ptr_cat;
	};

	template<>
	struct _Ptr_cat_helper<_Undefined_inner_type_tag, _Undefined_inner_type_tag>
	{
		typedef _Nonscalar_ptr_iterator_tag _Ptr_cat;
	};

	// INTEGER FUNCTION _Ptr_cat
	template<>
	struct _Ptr_cat_helper<bool*, bool*>
	{	// return pointer category from pointer to pointer arguments
		typedef _Scalar_ptr_iterator_tag _Ptr_cat;
	};

	template<>
	struct _Ptr_cat_helper<const bool*, bool*>
	{	// return pointer category from pointer to pointer arguments
		typedef _Scalar_ptr_iterator_tag _Ptr_cat;
	};

	template<>
	struct _Ptr_cat_helper<char*, char*>
	{	// return pointer category from pointer to pointer arguments
		typedef _Scalar_ptr_iterator_tag _Ptr_cat;
	};

	template<>
	struct _Ptr_cat_helper<const char*, char*>
	{	// return pointer category from pointer to pointer arguments
		typedef _Scalar_ptr_iterator_tag _Ptr_cat;
	};

	template<>
	struct _Ptr_cat_helper<signed char*, signed char*>
	{	// return pointer category from pointer to pointer arguments
		typedef _Scalar_ptr_iterator_tag _Ptr_cat;
	};

	template<>
	struct _Ptr_cat_helper<const signed char*, signed char*>
	{	// return pointer category from pointer to pointer arguments
		typedef _Scalar_ptr_iterator_tag _Ptr_cat;
	};

	template<>
	struct _Ptr_cat_helper<unsigned char*, unsigned char*>
	{	// return pointer category from pointer to pointer arguments
		typedef _Scalar_ptr_iterator_tag _Ptr_cat;
	};

	template<>
	struct _Ptr_cat_helper<const unsigned char*, unsigned char*>
	{	// return pointer category from pointer to pointer arguments
		typedef _Scalar_ptr_iterator_tag _Ptr_cat;
	};

#ifdef _NATIVE_WCHAR_T_DEFINED

	template<>
	struct _Ptr_cat_helper<wchar_t*, wchar_t*>
	{	// return pointer category from pointer to pointer arguments
		typedef _Scalar_ptr_iterator_tag _Ptr_cat;
	};

	template<>
	struct _Ptr_cat_helper<const wchar_t*, wchar_t*>
	{	// return pointer category from pointer to pointer arguments
		typedef _Scalar_ptr_iterator_tag _Ptr_cat;
	};

#endif /* _NATIVE_WCHAR_T_DEFINED */
	template<>
	struct _Ptr_cat_helper<short*, short*>
	{	// return pointer category from pointer to pointer arguments
		typedef _Scalar_ptr_iterator_tag _Ptr_cat;
	};

	template<>
	struct _Ptr_cat_helper<const short*, short*>
	{	// return pointer category from pointer to pointer arguments
		typedef _Scalar_ptr_iterator_tag _Ptr_cat;
	};

	template<>
	struct _Ptr_cat_helper<unsigned short*, unsigned short*>
	{	// return pointer category from pointer to pointer arguments
		typedef _Scalar_ptr_iterator_tag _Ptr_cat;
	};

	template<>
	struct _Ptr_cat_helper<const unsigned short*, unsigned short*>
	{	// return pointer category from pointer to pointer arguments
		typedef _Scalar_ptr_iterator_tag _Ptr_cat;
	};

	template<>
	struct _Ptr_cat_helper<int*, int*>
	{	// return pointer category from pointer to pointer arguments
		typedef _Scalar_ptr_iterator_tag _Ptr_cat;
	};

	template<>
	struct _Ptr_cat_helper<const int*, int*>
	{	// return pointer category from pointer to pointer arguments
		typedef _Scalar_ptr_iterator_tag _Ptr_cat;
	};

	template<>
	struct _Ptr_cat_helper<unsigned int*, unsigned int*>
	{	// return pointer category from pointer to pointer arguments
		typedef _Scalar_ptr_iterator_tag _Ptr_cat;
	};

	template<>
	struct _Ptr_cat_helper<const unsigned int*, unsigned int*>
	{	// return pointer category from pointer to pointer arguments
		typedef _Scalar_ptr_iterator_tag _Ptr_cat;
	};

	template<>
	struct _Ptr_cat_helper<long*, long*>
	{	// return pointer category from pointer to pointer arguments
		typedef _Scalar_ptr_iterator_tag _Ptr_cat;
	};

	template<>
	struct _Ptr_cat_helper<const long*, long*>
	{	// return pointer category from pointer to pointer arguments
		typedef _Scalar_ptr_iterator_tag _Ptr_cat;
	};

	template<>
	struct _Ptr_cat_helper<unsigned long*, unsigned long*>
	{	// return pointer category from pointer to pointer arguments
		typedef _Scalar_ptr_iterator_tag _Ptr_cat;
	};

	template<>
	struct _Ptr_cat_helper<const unsigned long*, unsigned long*>
	{	// return pointer category from pointer to pointer arguments
		typedef _Scalar_ptr_iterator_tag _Ptr_cat;
	};

	template<>
	struct _Ptr_cat_helper<float*, float*>
	{	// return pointer category from pointer to pointer arguments
		typedef _Scalar_ptr_iterator_tag _Ptr_cat;
	};

	template<>
	struct _Ptr_cat_helper<const float*, float*>
	{	// return pointer category from pointer to pointer arguments
		typedef _Scalar_ptr_iterator_tag _Ptr_cat;
	};

	template<>
	struct _Ptr_cat_helper<double*, double*>
	{	// return pointer category from pointer to pointer arguments
		typedef _Scalar_ptr_iterator_tag _Ptr_cat;
	};

	template<>
	struct _Ptr_cat_helper<const double*, double*>
	{	// return pointer category from pointer to pointer arguments
		typedef _Scalar_ptr_iterator_tag _Ptr_cat;
	};

	template<>
	struct _Ptr_cat_helper<long double*, long double*>
	{	// return pointer category from pointer to pointer arguments
		typedef _Scalar_ptr_iterator_tag _Ptr_cat;
	};

	template<>
	struct _Ptr_cat_helper<const long double*, long double*>
	{	// return pointer category from pointer to pointer arguments
		typedef _Scalar_ptr_iterator_tag _Ptr_cat;
	};

#ifdef _LONGLONG

	template<>
	struct _Ptr_cat_helper<_LONGLONG*, _LONGLONG*>
	{	// return pointer category from pointer to pointer arguments
		typedef _Scalar_ptr_iterator_tag _Ptr_cat;
	};

	template<>
	struct _Ptr_cat_helper<const _LONGLONG*, _LONGLONG*>
	{	// return pointer category from pointer to pointer arguments
		typedef _Scalar_ptr_iterator_tag _Ptr_cat;
	};

	template<>
	struct _Ptr_cat_helper<_ULONGLONG*, _ULONGLONG*>
	{	// return pointer category from pointer to pointer arguments
		typedef _Scalar_ptr_iterator_tag _Ptr_cat;
	};

	template<>
	struct _Ptr_cat_helper<const _ULONGLONG*, _ULONGLONG*>
	{	// return pointer category from pointer to pointer arguments
		typedef _Scalar_ptr_iterator_tag _Ptr_cat;
	};

#endif /* _LONGLONG */

	template<class _Ty>
	struct _Ptr_cat_helper<_Ty**, _Ty**>
	{	// return pointer category from pointer to pointer arguments
		typedef _Scalar_ptr_iterator_tag _Ptr_cat;
	};

	template<class _Ty>
	struct _Ptr_cat_helper<_Ty**, const _Ty**>
	{	// return pointer category from pointer to pointer arguments
		typedef _Scalar_ptr_iterator_tag _Ptr_cat;
	};

	template<class _Ty>
	struct _Ptr_cat_helper<_Ty* const*, _Ty**>
	{	// return pointer category from pointer to pointer arguments
		typedef _Scalar_ptr_iterator_tag _Ptr_cat;
	};

	template<class _Ty>
	struct _Ptr_cat_helper<_Ty* const*, const _Ty**>
	{	// return pointer category from pointer to pointer arguments
		typedef _Scalar_ptr_iterator_tag _Ptr_cat;
	};

	/* use _Ptr_cat_helper to determine the type of the pointer category */
	template<class _T1, class _T2> inline
		typename _Ptr_cat_helper<_T1, _T2>::_Ptr_cat __CLRCALL_OR_CDECL _Ptr_cat(_T1&, _T2&)
	{
		typename _Ptr_cat_helper<_T1, _T2>::_Ptr_cat _Cat;
		return (_Cat);
	}




	// TEMPLATE FUNCTIONS distance and _Distance
	template<class _InIt,
		class _Diff> inline
		void __CLRCALL_OR_CDECL _Distance2(_InIt _First, _InIt _Last, _Diff& _Off,
			std::input_iterator_tag)
	{	// add to _Off distance between input iterators
		for (; _First != _Last; ++_First)
			++_Off;
	}

	template<class _FwdIt,
		class _Diff> inline
		void __CLRCALL_OR_CDECL _Distance2(_FwdIt _First, _FwdIt _Last, _Diff& _Off,
			std::forward_iterator_tag)
	{	// add to _Off distance between forward iterators (redundant)
		for (; _First != _Last; ++_First)
			++_Off;
	}

	template<class _BidIt,
		class _Diff> inline
		void __CLRCALL_OR_CDECL _Distance2(_BidIt _First, _BidIt _Last, _Diff& _Off,
			std::bidirectional_iterator_tag)
	{	// add to _Off distance between bidirectional iterators (redundant)
		for (; _First != _Last; ++_First)
			++_Off;
	}

	template<class _RanIt,
		class _Diff> inline
		void __CLRCALL_OR_CDECL _Distance2(_RanIt _First, _RanIt _Last, _Diff& _Off,
			std::random_access_iterator_tag)
	{	// add to _Off distance between random-access iterators

		_Off += _Last - _First;
	}

	template<class _InIt,
		class _Diff> inline
		void __CLRCALL_OR_CDECL _Distance(_InIt _First, _InIt _Last, _Diff& _Off)
	{	// add to _Off distance between iterators
		_Distance2(_First, _Last, _Off, _Iter_cat(_First));
	}


}//_STD_END
#ifdef _MSC_VER
#pragma warning(pop)
#pragma pack(pop)
#endif  /* _MSC_VER */

#endif /* RC_INVOKED */
#endif /* _XUTILITY_ */

/*
 * This file is derived from software bearing the following
 * restrictions:
 *
 * Copyright (c) 1994
 * Hewlett-Packard Company
 *
 * Permission to use, copy, modify, distribute and sell this
 * software and its documentation for any purpose is hereby
 * granted without fee, provided that the above copyright notice
 * appear in all copies and that both that copyright notice and
 * this permission notice appear in supporting documentation.
 * Hewlett-Packard Company makes no representations about the
 * suitability of this software for any purpose. It is provided
 * "as is" without express or implied warranty.
 */

 /*
  * Copyright (c) 1992-2008 by P.J. Plauger.  ALL RIGHTS RESERVED.
  * Consult your license regarding permissions and restrictions.
  V5.05:0009 */
