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


double tradeoff = 0.8;
const VL ZHAVL = 10;
const VL INFV = 50;
const VL VLRNG = 1000000000;
int nds[15],ndl[15];
int nd[6],ND[6];
bool guo;
int VEL[15],VES[15];
int VE[6],ve[6];
int DAS[15],DAL[15],DA[6];
VL mxvl;
int ves[15],vel[15];
int zha;
int da[5];
int tri,tri1,tri2;
int pia[20];
int pai[50]={0};
int fl,qj;
int fan;

CardCombo PAII;
bool O;

int count_zha(){
	int t=(pai[13] && pai[14]);
	FOR(i,0,12) if (pai[i]==4) ++t;
	return t;
}
inline int fh(int x){
	if (x>0) return 1;
	else if (x<0) return -1;
	else return 0;
}
inline VL PASSVL(){
	return (lastValidComboPosition!=landlordPosition&&
	myPosition!=landlordPosition)?0:-qj;
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
	if (x<13) return x==12?6:f(x)*1.2-1.5;
	else assert(0);
}
inline VL STRAIGHTVL(double l, double len){
	return (l+len/2)/4-(1-fl)*(len-5)/2;
}
inline VL STRAIGHT2VL(double l,double len){
	return l/4-(1-fl)*(len-3);
}
inline VL TRIPLETVL(double x){
	return max(PAIRVL(x)+SINGLEVL(x),f(x)*1.2-1)-(fl&&x==12?qj:0);
}
inline VL TRIPLET1VL(double x){
	return TRIPLETVL(x);
}
inline VL TRIPLET2VL(double x){
	return TRIPLETVL(x)+(fl*0.5);
}
inline VL BOMBVL(double x){
	return max(PAIRVL(x)+PAIRVL(x),3+x/4)-(fl*qj);
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
	return 9-(fl*qj);
}
inline VL INVALIDVL(){
	return -100;
}

int zs;
void suan(VL vl){

	VL shangvl = 0;
	for (int ald=int(guo);ald<=0;++ald){

		if (zha!=-1){
			static int s[5];
			s[1]=s[2]=s[3]=s[4]=0;
			int niu=0;
			int FAN=fan;
			FOR(i,0,12) if (pai[i]){
				if (pai[i]<=3) ++s[pai[i]];
				else if (i>zha) ++niu,++FAN;
				else ++s[4],++FAN;
			}
			if (pai[13] && pai[14]) ++niu; else s[1]+=pai[13]+pai[14],++FAN;
			int ci=s[4]+s[3]+max(s[1]+s[2]-s[3],0);
			FOR(i,5,12) ci-=ves[i];
			FOR(i,3,12) ci-=vel[i];
			FOR(i,1,4) ci-=ve[i];
			if (ci<=niu+1){
				shangvl = INFV+FAN*ZHAVL;
				break;
			}
		}
		else{

			

			static int s[5];
			s[1]=s[2]=s[3]=s[4]=0;
			FOR(i,0,14) if (pai[i]){
				++s[pai[i]];
			}
			int wozha=s[4];

			FOR(i,1,4) VE[i]=ve[i];
			FOR(i,1,4) ND[i]=nd[i];
			FOR(i,5,12) VES[i]=ves[i];
			FOR(i,3,12) VEL[i]=vel[i];




			FOR(i,0,12) if (pai[i]){
				if (pai[i]<=3){
					VE[pai[i]]+=fh(i-DA[pai[i]]);
					ND[pai[i]]-=(fh(i-DA[pai[i]])==1);
				}
			}
			if (pai[13] && pai[14]){
				
				++wozha;
			}
			else{
				if (pai[13]){
					VE[1]+=fh(13-DA[1]);
					ND[1]-=(fh(13-DA[1])==1);
				}
				if (pai[14]){
					VE[1]+=fh(14-DA[1]);
					ND[1]-=(fh(14-DA[1])==1);
				}
			}

			int FAN=fan+wozha;

			bool gg=0;
			FOR(i,3,12) if (ndl[i]>0){
				if (wozha) --wozha,++VEL[i];
				else {gg=1;break;}
			}
			if (gg) break;
			FOR(i,5,12) if (nds[i]>0){
				if (wozha) --wozha,++VES[i];
				else {gg=1;break;}
			}
			if (gg) break;


			FOR(i,1,3)
				if (ND[i]>0){
					if (wozha) --wozha,--ND[i],++VE[i];
					else{gg=1;break;}
				}
			if (gg) break;

			int san=s[3];
			if (VE[2]<0){
				int t=min(-VE[2],san);
				san-=t;
				VE[2]+=t;
			}
			if (VE[1]<0){
				int t=min(-VE[1],san);
				san-=t;
				VE[1]+=t;
			}


			int ci=min(0,VE[1])+min(VE[2],0)+min(0,VE[3])+wozha;
			FOR(i,5,12) ci+=min(0,VES[i]);
			FOR(i,3,12) ci+=min(0,VEL[i]);


			if (ci>=-1){
				shangvl = INFV+FAN*ZHAVL;
				break;
			}

		}
	}



	int cardcnt=0;
	FOR(i,0,14)cardcnt+=pai[i];

	static double one[50];
	static double two[50];
	VL t = vl;
	int twos = 0;
	int ones = 0;
	int thres = 0,zz=0;
	FOR(i,0,14)
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
	//cerr<<t<<" "<<ones<<" "<<zs<<" fjz ";
	zz=zs+1+ones-max(zz,1);
	FOR(i,1,ones){
		int tt=max(0,zz-i);
		one[i]-=tt; t-=tt; //cerr<<tt<<endl;
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
	//cerr<<t<<" "<<ones<<" "<<SINGLEVL(1)<<" "<<ones<<endl;
	mxvl=max(mxvl,shangvl+t-(cardcnt<myCards.size()?max(6-cardcnt,0):0));
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
					ves[j-i+1]+=fh(tmp2[j]-DAS[j-i+1]);
					nds[j-i+1]-=(fh(tmp2[j]-DAS[j-i+1])==1);
					suan(vl+STRAIGHTVL(tmp2[i],j-i+1));
					ves[j-i+1]-=fh(tmp2[j]-DAS[j-i+1]);
					nds[j-i+1]+=(fh(tmp2[j]-DAS[j-i+1])==1);
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
				ves[j-i+1]+=fh(tmp1[j]-DAS[j-i+1]);
				nds[j-i+1]-=(fh(tmp1[j]-DAS[j-i+1])==1);
				shun2(vl+STRAIGHTVL(tmp1[i],j-i+1),1);
				ves[j-i+1]-=fh(tmp1[j]-DAS[j-i+1]);
				nds[j-i+1]+=(fh(tmp1[j]-DAS[j-i+1])==1);
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

				vel[j-i+1]+=fh(tmp[j]-DAL[j-i+1]);
				ndl[j-i+1]-=(fh(tmp[j]-DAL[j-i+1])==1);
				shun1(STRAIGHT2VL(tmp[i],j-i+1) );
				vel[j-i+1]-=fh(tmp[j]-DAL[j-i+1]);
				ndl[j-i+1]+=(fh(tmp[j]-DAL[j-i+1])==1);
			}
			else break;
		FOR(t,tmp[i],r) pai[t] += 2;
	}

	shun1(0);
}

double evalCards(const CardSet & PAI){
	FOR(i,0,14) pai[i]=0; fl=0;
	for (auto x : PAI){
		++pai[card2level(x)];
	}
	guo=1;
	mxvl = -VLRNG;
	lian();
	return 1.0*mxvl;


}

double eval(const CardCombo & PAI, bool o){


	if (SZ(PAI.cards)==SZ(dist[myPosition])){
		int t=(PAI.comboType == CardComboType::BOMB || PAI.comboType == CardComboType::ROCKET);
		return INFV+t*ZHAVL;
	}



	zha=-1;
	DA[1]=DA[2]=DA[3]=-1;
	FOR(i,3,12) DAL[i]=DAS[i]=-1;
	FOR(id,0,2) if (id!=myPosition){
		FOR(j,0,14) pia[j]=0;
		for (auto x:dist[id]){
			++pia[card2level(x)];
		}
		if (o){
			FOR(k,id+1,2) if (k!=myPosition){
				for (auto x:dist[k]){
					++pia[card2level(x)];
				}
			}
		}
		int da[5];
		da[1]=da[2]=da[3]=da[4]=-1;
		FOR(j,0,14) da[pia[j]]=max(da[pia[j]],j);
		if (pia[13] && pia[14]) zha=15;
		zha=max(zha,da[4]);
		FOR(j,1,3) DA[j]=max(DA[j],da[j]);

		int ts=0;
		static int tmp[15];
		FOR(i,0,11) if (pia[i]>=2){
			tmp[++ts]=i;
		}
		if (ts){
			int t=1;
			FOR(i,2,ts) if (tmp[i]==tmp[i-1]+1){
				++t;
			}
			else{
				DAL[t]=max(DAL[t],tmp[i-1]);
				t=1;
			}
			DAL[t]=max(DAL[t],tmp[ts]);
			//liandui
		}

		ts=0;
		FOR(i,0,11) if (pia[i]>=1){
			tmp[++ts]=i;
		}
		if (ts){
			int t=1;
			FOR(i,2,ts) if (tmp[i]==tmp[i-1]+1){
				++t;
			}
			else{
				DAS[t]=max(DAS[t],tmp[i-1]);
				t=1;
			}
			DAS[t]=max(DAS[t],tmp[ts]);
			//shunzi
		}

		if (o) break;

	}

	if (zha!=-1){
		DA[3]=15;
		DAL[12]=15;
		DAS[12]=15;
	}
	DA[2]=max(DA[2],DA[3]);
	DA[1]=max(DA[1],DA[2]);
	FORD(i,11,3) DAL[i]=max(DAL[i],DAL[i+1]);
	FORD(i,11,5) DAS[i]=max(DAS[i],DAS[i+1]);

	FOR(i,3,12) ves[i]=vel[i]=0;
	ve[1]=0;
	ve[2]=0;
	ve[3]=0;
	ve[4]=0;
	guo=0;

	nd[1]=0;
	nd[2]=0;
	nd[3]=0;
	nd[4]=0;
	FOR(i,3,12) nds[i]=ndl[i]=0;
	fan=0;

	


	FOR(i,0,14) pai[i]=0;
	for (auto x :myCards) ++pai[card2level(x)];

	int t1 = count_zha();

	for (auto x :PAI.cards) --pai[card2level(x)];

	int t2 = count_zha();

	if (t1!=t2) guo=1;


	vector<Card> cwy; FOR(i,0,14)FOR(j,0,pai[i]-1)cwy.pb(i*4+j);
	qj=5; if((lastValidComboPosition+1)%3==myPosition)qj=3;
	if((CardCombo(cwy)).comboType!=CardComboType::INVALID)tradeoff=1.2;
	else tradeoff=0.8;
	//FOR(i,0,14)cerr<<pai[i]<<" "; cerr<<endl;
	VL CHUVL(0); fl=1;
	if(PAI.comboType==CardComboType::SINGLE&&card2level(PAI.cards[0])<=10)zs=1; else zs=0;
	if (PAI.comboType == CardComboType::PASS) CHUVL += min(PASSVL(),
	(lastValidCombo.comboType==CardComboType::SINGLE?-0.5:
	(lastValidCombo.comboType==CardComboType::PAIR?-0.25:0))),guo=1;
	else if (PAI.comboType == CardComboType::SINGLE) CHUVL += SINGLEVL(PAI.packs[0].level),ve[1]-=(PAI.packs[0].level<DA[1]),nd[1]+=(PAI.packs[0].level<DA[1]);
	else if (PAI.comboType == CardComboType::PAIR) CHUVL += PAIRVL(PAI.packs[0].level),ve[2]-=(PAI.packs[0].level<DA[2]),nd[2]+=(PAI.packs[0].level<DA[2]);
	else if (PAI.comboType == CardComboType::STRAIGHT) CHUVL += STRAIGHTVL(PAI.packs.back().level, SZ(PAI.packs)),ves[SZ(PAI.packs)]-=(PAI.packs[0].level<DAS[SZ(PAI.packs)]),nds[SZ(PAI.packs)]+=(PAI.packs[0].level<DAS[SZ(PAI.packs)]);
	else if (PAI.comboType == CardComboType::STRAIGHT2) CHUVL += STRAIGHT2VL(PAI.packs.back().level, SZ(PAI.packs)),vel[SZ(PAI.packs)]-=(PAI.packs[0].level<DAL[SZ(PAI.packs)]),ndl[SZ(PAI.packs)]+=(PAI.packs[0].level<DAL[SZ(PAI.packs)]);
	else if (PAI.comboType == CardComboType::TRIPLET) CHUVL += TRIPLETVL(PAI.packs[0].level),ve[3]-=(PAI.packs[0].level<DA[3]),nd[3]+=(PAI.packs[0].level<DA[3]);
	else if (PAI.comboType == CardComboType::TRIPLET1) CHUVL += TRIPLET1VL(PAI.packs[0].level),ve[3]-=(PAI.packs[0].level<DA[3]),nd[3]+=(PAI.packs[0].level<DA[3]);
	else if (PAI.comboType == CardComboType::TRIPLET2) CHUVL += TRIPLET2VL(PAI.packs[0].level),ve[3]-=(PAI.packs[0].level<DA[3]),nd[3]+=(PAI.packs[0].level<DA[3]);
	else if (PAI.comboType == CardComboType::BOMB) ++fan, CHUVL += BOMBVL(PAI.packs[0].level),ve[4]-=(PAI.packs[0].level<zha),nd[4]+=(PAI.packs[0].level<zha);
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
	// if(PAI.cards[0]!=43&&(PAI.cards[0]!=4||PAI.cards[1]!=6))return -100;
	// for(auto i:PAI.cards)cerr<<card2level(i)<<" "; cerr<<" alddddddd\n";
	lian();




	//FOR(i,0,14)cerr<<pai[i]<<" "; cerr<<endl;
	//cerr<<PAIRVL(5)<<" aaaaaaaaaa "<<SINGLEVL(10)<<endl;
	// cerr<<mxvl<<" fjzq "<<CHUVL<<" "<<zs<<" "<<mxvl+tradeoff*CHUVL<<endl;
	static const double mu = 0.1; // 估价每大 1, 出牌概率大 e^mu
	
	return (mxvl+tradeoff*CHUVL) * mu;
	
}
