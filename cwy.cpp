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
int pai[50]={0};
int fl;
inline VL PASSVL(){
	return (lastValidComboPosition!=landlordPosition&&
	myPosition!=landlordPosition)?0:-5;
}
inline double f(double x){
	if(x<=8)return x/4;
	if(x<=11)return 2+(x-8)/2;
	return x-12+5;
}
inline VL SINGLEVL(double x){
	return f(x)-2;
}
inline VL PAIRVL(double x){
	if (x<13) return x==12?6:f(x)*1.2-1;
	else assert(0);
}
inline VL STRAIGHTVL(double l, double len){
	return (l+len/2)/4;
}
inline VL STRAIGHT2VL(double l,double len){
	return l/4;
}
inline VL TRIPLETVL(double x){
	return max(PAIRVL(x)+SINGLEVL(x),f(x)*1.2-1)-(fl&&x==12?5:0);
}
inline VL TRIPLET1VL(double x){
	return TRIPLETVL(x);
}
inline VL TRIPLET2VL(double x){
	return TRIPLETVL(x);
}
inline VL BOMBVL(double x){
	return max(PAIRVL(x)+PAIRVL(x),4+x/4)-(fl*5);
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
	return 9-(fl*5);
}
inline VL INVALIDVL(){
	return -100;
}

int zs;
void suan(VL vl){
	static double one[50];
	static double two[50];
	VL t = vl;
	int twos = 0;
	int ones = 0;
	int thres = 0,zz=0;
	FOR(i,0,12)
		if (pai[i]){
			if (pai[i]==1) {t+=(one[++ones]=SINGLEVL(i)); 
			if(i>10)ones--;}
			else if (pai[i]==2) 
				t+=(two[++twos]=PAIRVL(i));
			else if (pai[i]==3) {t+=TRIPLETVL(i);
			if(i<12)++thres;}
			else if (pai[i]==4) t+=BOMBVL(i);
			if(i>11)zz+=pai[i];
		}
	//FOR(i,0,12)cerr<<pai[i]<<" ";
	//cerr<<t<<" "<<ones<<" fjz ";
	
	FOR(i,1,ones){
		int tt=max(0,ones-i-zz+zs+1);
		one[i]-=tt; t-=tt;
	}
	//cerr<<t<<" ";
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
	//cerr<<t<<" "<<ones<<" "<<SINGLEVL(1)<<" "<<ones<<endl;
	mxvl=max(mxvl,t);
}
void shun2(VL vl, bool o){
	static int ts2;
	static int tmp2[50];
	ts2=0;
	FOR(i,0,11) if (pai[i]){
		tmp2[++ts2]=i;
	}
	if (o){
		FOR(i,1,ts2-4){
			int r=tmp2[i]-1;
			FOR(j,i+4,ts2)
				if (tmp2[j]-tmp2[i]==j-i){
					while (1){
						--pai[++r];
						if (r==tmp2[j]) break;
					}
					suan(vl+STRAIGHTVL(tmp2[i],j-i+1));
				}
				else break;
			FOR(t,tmp2[i],r) ++pai[t];
		}
	}
	suan(vl);
}
void shun1(VL vl){
	static int ts1;
	static int tmp1[50];
	ts1=0;
	FOR(i,0,11) if (pai[i]){
		tmp1[++ts1]=i;
	}
	FOR(i,1,ts1-4){
		int r=tmp1[i]-1;
		FOR(j,i+4,ts1)
			if (tmp1[j]-tmp1[i]==j-i){
				while (1){
					--pai[++r];
					if (r==tmp1[j]) break;
				}
				shun2(vl+STRAIGHTVL(tmp1[i],j-i+1),1);
			}
			else break;
		FOR(t,tmp1[i],r) ++pai[t];
	}
	shun2(vl,0);
}

void lian(){
	static int ts;
	static int tmp[50];
	ts=0;
	FOR(i,0,11) if (pai[i]>=2){
		tmp[++ts]=i;
	}
	FOR(i,1,ts-2){
		int r=tmp[i]-1;
		FOR(j,i+2,ts)
			if (tmp[j]-tmp[i]==j-i){
				while (1){
					pai[++r] -= 2;
					if (r==tmp[j]) break;
				}
				shun1(STRAIGHT2VL(tmp[i],j-i+1));
			}
			else break;
		FOR(t,tmp[i],r) pai[t] += 2;
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
	//FOR(i,0,14)cerr<<pai[i]<<" "; cerr<<endl;
	VL CHUVL(0); fl=1;
	if(PAI.comboType==CardComboType::SINGLE&&card2level(PAI.cards[0])<=10)zs=1; else zs=0;
	if (PAI.comboType == CardComboType::PASS) CHUVL += min(PASSVL(),
	(lastValidCombo.comboType==CardComboType::SINGLE?-0.5:0));
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
	fl=0; mxvl = -VLRNG;
//	for(auto i:PAI.cards)cerr<<i<<" "; cerr<<" alddddddd\n";
	lian();
	//FOR(i,0,14)cerr<<pai[i]<<" "; cerr<<endl;
	//cerr<<mxvl<<" fjzq "<<CHUVL<<" "<<zs<<" "<<mxvl+tradeoff*CHUVL<<" "<<TRIPLETVL(12)<<endl;
	return mxvl+tradeoff*CHUVL;
	
}
