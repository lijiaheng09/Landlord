#ifndef MAGIC_HPP
#define MAGIC_HPP

#include <bits/stdc++.h>
#include "card.h"
#include "main.h"

typedef double VL;

int fl, qj;

inline VL f(int x){
	if(x<=8)return x/4.0;
	if(x<=11)return 2+(x-8)/2.0;
	return x-12+5;
}

namespace PreProcessing {
	inline VL SINGLEVL(int x){
		return f(x)-2;
	}
	inline VL PAIRVL(int x){
		if (x<13) return x==12?6:f(x)*1.2-1.5;
		else return 0;
	}

#define PREP(F) \
	const std::array<VL, MAX_LEVEL> F##_PRE = ([]() { \
		std::array<VL, MAX_LEVEL> R; \
		for (int i = 0; i < MAX_LEVEL; i++) \
			R[i] = PreProcessing::F(i); \
		return R; \
	})()

	PREP(SINGLEVL);
	PREP(PAIRVL);
#undef PREP
}

#define PREPD(F) \
	VL F(int x) { return PreProcessing::F##_PRE[x]; }

PREPD(SINGLEVL)
PREPD(PAIRVL)
#undef PREPD

inline VL PASSVL(){
	return (lastValidComboPosition!=landlordPosition&&
	myPosition!=landlordPosition)?0:-qj;
}
inline VL STRAIGHTVL(int l, int len){
	return (l+len/2.0)/4.0-(1-fl)*(len-5)/2.0;
}
inline VL STRAIGHT2VL(int l,int len){
	return l/4.0-(1-fl)*(len-3);
}
inline VL TRIPLETVL(int x){
	return std::max(PAIRVL(x)+SINGLEVL(x),f(x)*1.2-1)-(fl&&x==12?qj:0);
}
inline VL TRIPLET1VL(int x){
	return TRIPLETVL(x);
}
inline VL TRIPLET2VL(int x){
	return TRIPLETVL(x)+(fl*0.5);
}
inline VL BOMBVL(int x){
	return std::max (PAIRVL(x)+PAIRVL(x),3+x/4.0)-(fl*qj);
}
inline constexpr VL QUADRUPLE2VL(int x){
	return 1;
}
inline constexpr VL QUADRUPLE4VL(int x){
	return 1;
}
inline constexpr VL PLANEVL(int x){
	return (x/6.0+4);
}
inline constexpr VL PLANE1VL(int x){
	return 5;
}
inline constexpr VL PLANE2VL(int x){
	return 6;
}
inline constexpr VL SSHUTTLEVL(int x){
	return (x/6.0+5);
}
inline constexpr VL SSHUTTLE2VL(int x){
	return 6;
}
inline constexpr VL SSHUTTLE4VL(int x){
	return 6;
}
inline VL ROCKETVL(){
	return 9-(fl*qj);
}
inline constexpr VL INVALIDVL(){
	return -100;
}

#endif
