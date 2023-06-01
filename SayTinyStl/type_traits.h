#ifndef SAYTINYSTL_TYPE_TRAITS
#define SAYTINYSTL_TYPE_TRAITS
//这个头文件用于提取类型信息
#include <type_traits>
namespace saystl {

	template<class T, T val>
	struct say_intergral_constant {
		//constexpr可以修饰函数、结构体
		/*修饰的函数只能包括return 语句。
		修饰的函数只能引用全局不变常量。
		修饰的函数只能调用其他constexpr修饰的函数。
		函数不能为void 类型和，并且prefix operation（v++）不允许出现。*/
		static constexpr T value = val;
	};
	//using做：alias templates, 模板别名
	template <bool b>
	using say_bool_constant = say_intergral_constant<bool, b>;

	//using say_true_type = say_bool_constant<true>;
	//using say_false_type = say_bool_constant<false>;
	typedef say_bool_constant<true>  say_true_type;
	typedef say_bool_constant<false> say_false_type;


	//forward declaration
	template<class T1, class T2>
	struct pair;

	template<class T>
	struct is_pair : saystl::say_false_type {};

	template<class T1, class T2>
	struct is_pair<saystl::pair<T1, T2>> : saystl::say_true_type {};



}//namespace saystl

#endif // !SAYTINYSTL_TYPE_TRAITS


