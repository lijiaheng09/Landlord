#include <bits/stdc++.h>
#define pb push_back
#define mp make_pair
#define fi first
#define se second
#define SZ(x) ((int)x.size())
#define FOR(i,a,b) for (int i=a;i<=b;++i)
#define FORD(i,a,b) for (int i=a;i>=b;--i)
using namespace std;
typedef long long LL;
typedef pair<int,int> pa;
typedef vector<int> vec;
typedef double VL;
#include "card.h"
#include "main.h"


const double tradeoff = 0.8;
const VL VLRNG = 1000000000;
VL mxvl;
int ts;
int tmp[50];
int ts1;
int tmp1[50];
int ts2;
int tmp2[50];
int one[50];
int two[50];
int pai[50]={0};

inline VL PASSVL(){
	return (lastValidComboPosition!=landlordPosition&&
	myPosition!=landlordPosition)?0:-3;
}
inline VL SINGLEVL(double x){
	if (x<13) return x/2-3;
	else if (x==13) return 4;
	else return 5;
}
inline VL PAIRVL(double x){
	if (x<13) return x/2-1;
	else assert(0);
}
inline VL STRAIGHTVL(double l, double len){
	return 0;
}
inline VL STRAIGHT2VL(double l,double len){
	return 0;
}
inline VL TRIPLETVL(double x){
	return (x/2+1);
}
inline VL TRIPLET1VL(double x){
	return TRIPLETVL(x);
}
inline VL TRIPLET2VL(double x){
	return TRIPLETVL(x);
}
inline VL BOMBVL(double x){
	return (8+x/3);
}
inline VL QUADRUPLE2VL(double x){
	return 1;
}
inline VL QUADRUPLE4VL(double x){
	return 1;
}
inline VL PLANEVL(double x){
	return (x/6+4);
}
inline VL PLANE1VL(double x){
	return 5;
}
inline VL PLANE2VL(double x){
	return 6;
}
inline VL SSHUTTLEVL(double x){
	return (x/6+5);
}
inline VL SSHUTTLE2VL(double x){
	return 6;
}
inline VL SSHUTTLE4VL(double x){
	return 6;
}
inline VL ROCKETVL(){
	return 13;
}
inline VL INVALIDVL(){
	return -100;
}


void suan(VL vl){
	VL t = vl;
	int twos = 0;
	int ones = 0;
	int thres = 0;
	FOR(i,0,12)
		if (pai[i]){
			if (pai[i]==1) t+=(one[++ones]=SINGLEVL(i));
			else if (pai[i]==2) t+=(two[++twos]=PAIRVL(i));
			else if (pai[i]==3) t+=TRIPLETVL(i),++thres;
			else if (pai[i]==4) t+=BOMBVL(i);
		}
	FOR(i,1,ones){
		one[i]-=ones-i+1; t-=ones-i+1;
	}
	if (thres){
		int p1 = 1;
		int p2 = 1;
		one[ones+1]=0;
		two[twos+1]=0;
		while (thres--){
			if (one[p1]<0 || two[p2]<0){
				if (one[p1]<two[p2]) t -= one[p1], ++p1;
				else t -= two[p2], ++p2;
			}
			else break;
		}
	}
	if (pai[13] && pai[14]) t += ROCKETVL();
	else if (pai[13]) t += SINGLEVL(13);
	else if (pai[14]) t += SINGLEVL(14);
	mxvl=max(mxvl,t);
}
void shun2(VL vl, bool o){
	ts2=0;
	FOR(i,0,11) if (pai[i]){
		tmp2[++ts2]=i;
	}
	if (o){
		FOR(i,1,ts2-4){
			int r=i-1;
			FOR(j,i+4,ts2)
				if (tmp[j]-tmp[i]==j-i){
					while (1){
						--pai[++r];
						if (r==j) break;
					}
					suan(vl+STRAIGHTVL(tmp[i],j-i+1));
				}
				else break;
			FOR(t,i,r) ++pai[t];
		}
	}
	suan(vl);
}
void shun1(VL vl){
	ts1=0;
	FOR(i,0,11) if (pai[i]){
		tmp1[++ts1]=i;
	}
	FOR(i,1,ts1-4){
		int r=i-1;
		FOR(j,i+4,ts1)
			if (tmp[j]-tmp[i]==j-i){
				while (1){
					--pai[++r];
					if (r==j) break;
				}
				shun2(vl+STRAIGHTVL(tmp[i],j-i+1),1);
			}
			else break;
		FOR(t,i,r) ++pai[t];
	}
	shun2(vl,0);
}

void lian(){
	ts=0;
	FOR(i,0,11) if (pai[i]>=2){
		tmp[++ts]=i;
	}
	FOR(i,1,ts-2){
		int r=i-1;
		FOR(j,i+2,ts)
			if (tmp[j]-tmp[i]==j-i){
				while (1){
					pai[++r] -= 2;
					if (r==j) break;
				}
				shun1(STRAIGHT2VL(tmp[i],j-i+1));
			}
			else break;
		FOR(t,i,r) pai[t] += 2;
	}

	shun1(0);
}

double evalCards(const CardSet & PAI){
	FOR(i,0,14) pai[i]=0;
	for (auto x : PAI){
		++pai[card2level(x)];
	}
	mxvl = -VLRNG;
	lian();
	return 1.0*mxvl;


}

double eval(const CardCombo & PAI){
	FOR(i,0,14) pai[i]=0;
	
	for (auto x :myCards) ++pai[card2level(x)];
	for (auto x :PAI.cards) --pai[card2level(x)];
	VL CHUVL(0);
	if (PAI.comboType == CardComboType::PASS) CHUVL += PASSVL()-
	(lastValidCombo.comboType==CardComboType::SINGLE?-0.5:0);
	else if (PAI.comboType == CardComboType::SINGLE) CHUVL += SINGLEVL(PAI.packs[0].level);
	else if (PAI.comboType == CardComboType::PAIR) CHUVL += PAIRVL(PAI.packs[0].level);
	else if (PAI.comboType == CardComboType::STRAIGHT) CHUVL += STRAIGHTVL(PAI.packs.back().level, SZ(PAI.packs));
	else if (PAI.comboType == CardComboType::STRAIGHT2) CHUVL += STRAIGHT2VL(PAI.packs.back().level, SZ(PAI.packs));
	else if (PAI.comboType == CardComboType::TRIPLET) CHUVL += TRIPLETVL(PAI.packs[0].level);
	else if (PAI.comboType == CardComboType::TRIPLET1) CHUVL += TRIPLET1VL(PAI.packs[0].level);
	else if (PAI.comboType == CardComboType::TRIPLET2) CHUVL += TRIPLET2VL(PAI.packs[0].level);
	else if (PAI.comboType == CardComboType::BOMB) CHUVL += BOMBVL(PAI.packs[0].level);
	else if (PAI.comboType == CardComboType::QUADRUPLE2) CHUVL += QUADRUPLE2VL(PAI.packs[0].level);
	else if (PAI.comboType == CardComboType::QUADRUPLE4) CHUVL += QUADRUPLE4VL(PAI.packs[0].level);
	else if (PAI.comboType == CardComboType::PLANE) CHUVL += PLANEVL(PAI.packs[0].level);
	else if (PAI.comboType == CardComboType::PLANE1) CHUVL += PLANE1VL(PAI.packs[0].level);
	else if (PAI.comboType == CardComboType::PLANE2) CHUVL += PLANE2VL(PAI.packs[0].level);
	else if (PAI.comboType == CardComboType::SSHUTTLE) CHUVL += SSHUTTLEVL(PAI.packs[0].level);
	else if (PAI.comboType == CardComboType::SSHUTTLE2) CHUVL += SSHUTTLE2VL(PAI.packs[0].level);
	else if (PAI.comboType == CardComboType::SSHUTTLE4) CHUVL += SSHUTTLE4VL(PAI.packs[0].level);
	else if (PAI.comboType == CardComboType::ROCKET) CHUVL += ROCKETVL();
	else if (PAI.comboType == CardComboType::INVALID) CHUVL += INVALIDVL();
	mxvl = -VLRNG;
	lian();
	return mxvl+tradeoff*CHUVL;
	
}
