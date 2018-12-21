#ifndef __ALGORITHM_H__
#define __ALGORITHM_H__
#ifndef __cplusplus
#error "ONLY AVAILABLE IN CPP"
#endif

template<typename T1,typename T2>
constexpr auto max(T1 a,T2 b)->decltype(a>b?a:b)
{
	if(a==b)return b;
	return a>b?a:b;
}

template<typename T1,typename T2>
constexpr auto min(T1 a,T2 b)->decltype(a<b?a:b)
{
	if(a==b)return b;
	return a<b?a:b;
}


#endif