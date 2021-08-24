#ifndef UTIL_H
#define UTIL_H

#pragma once

void SetByte(DWORD offset,BYTE value);
void SetByteUnsafe(DWORD offset,BYTE value);
void SetWord(DWORD offset,WORD value);
void SetDword(DWORD offset,DWORD value);
void SetFloat(DWORD offset,float value);
void SetDouble(DWORD offset,double value);
void SetCompleteHook(BYTE head,DWORD offset,...);
void SetCompleteHookUnsafe(BYTE head,DWORD offset,...);
void MemoryCpy(DWORD offset,void* value,DWORD size);
void MemorySet(DWORD offset,DWORD value,DWORD size);
LPVOID HookFunction(const LPVOID offset, LPVOID function, DWORD size);
void PacketArgumentEncrypt(char* out_buff,char* in_buff,int size);

namespace std
{
	template <typename T>
	inline std::string to_string(T value)
	{
		std::ostringstream os ;
		os << value ;
		return os.str() ;
	}
}

template<typename charT>
struct my_equal
{
    my_equal( const _locale_t& loc ) : loc_(loc) {}
    bool operator()(charT ch1, charT ch2) {
        return _toupper_l(ch1, loc_) == _toupper_l(ch2, loc_);
		
    }
private:
    const _locale_t& loc_;
};

template<typename T>
size_t FindString( const T& str1, const T& str2, const _locale_t& loc = _locale_t() )
{
    auto it = std::search( str1.begin(), str1.end(), 
        str2.begin(), str2.end(), my_equal<T::value_type>(loc) );
    if ( it != str1.end() ) return it - str1.begin();
    else return -1; // not found
}

inline void ToLowerCaseString(std::string& str)
{
    std::transform(str.begin(), str.end(), str.begin(), tolower);
}

void CreateParentDir(fs::path pPath);

void ReplaceString(std::string &src, std::string const& search, std::string const& replace);


template<class T>
const T& Clamp(const T& v, const T& min, const T& max)
{
#if _HAS_CXX17
    return std::clamp(v, min, max);
#else
    return (v < min) ? min : (v > max) ? max : v;
#endif
}

#endif