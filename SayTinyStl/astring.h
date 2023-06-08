#ifndef SAYSTL_ASTRING_H
#define SAYSTL_ASTRING_H

// ���� string wstring u16string u32string ��������
#include "basic_string.h"

namespace saystl {

	using string = saystl::basic_string<char>;
	using wstring = saystl::basic_string<wchar_t>;
	using u16string = saystl::basic_string<char16_t>;
	using u32string = saystl::basic_string<char32_t>;

}// !namespace saystl

#endif // !SAYSTL_ASTRING_H

