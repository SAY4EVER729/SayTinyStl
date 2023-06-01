#ifndef SAYTINYSTL_TYPE_TRAITS
#define SAYTINYSTL_TYPE_TRAITS
//���ͷ�ļ�������ȡ������Ϣ
#include <type_traits>
namespace saystl {

	template<class T, T val>
	struct say_intergral_constant {
		//constexpr�������κ������ṹ��
		/*���εĺ���ֻ�ܰ���return ��䡣
		���εĺ���ֻ������ȫ�ֲ��䳣����
		���εĺ���ֻ�ܵ�������constexpr���εĺ�����
		��������Ϊvoid ���ͺͣ�����prefix operation��v++����������֡�*/
		static constexpr T value = val;
	};
	//using����alias templates, ģ�����
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


