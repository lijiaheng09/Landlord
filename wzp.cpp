#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef unsigned long long ull;
typedef double ld;
#define mp make_pair
#define PI pair<ll,ll>
#define poly vector<ll>
#define mem(a) memset((a),0,sizeof(a))
#define For(i,l,r) for(int i=(int)(l);i<=(int)(r);i++)
#define Rep(i,r,l) for(int i=(int)(r);i>=(int)(l);i--)
#define pb push_back
#define fi first
#define se second
#define SZ(x) ((int)(x.size()))
#define re resize
#include "card.h"
#include "main.h"
int getBidValue(int maxBid){
	
	int to[54]={1};
	for(auto i:myCards) to[i]--;
	vector<Card> v;
	For(i,0,53)if(to[i])v.pb(i);
	ld jb=clock();
	int cnt[2]={0};
	while((clock()-jb)/CLOCKS_PER_SEC<0.9){
		random_shuffle(v.begin(),v.end());
		landlordPublicCards.clear();
		For(i,0,2){
			landlordPublicCards.insert(v[i]);
		}
		ld zs[2];
		For(j,0,1){
			landlordPosition=(myPosition+j)%3;
			if(!j)for(auto k:landlordPublicCards)myCards.insert(k);
			landlordBid=3;
			cardRemaining[landlordPosition]=20;
			zs[j]=getMean(); if(!j)zs[0]*=2;
			cardRemaining[landlordPosition]=17;
		}
		cnt[zs[1]>zs[0]]++;
	}
	return cnt[1]>cnt[0]?0:3;
}

const ld sigma=1.0;
ld sqr(ld x){return x*x;}
bool cmp(const pair<CardDistrib, double> &a,
const pair<CardDistrib, double> &b){
	return a.se>b.se;
}
std::vector<std::pair<CardDistrib, double>> randCards(int num){
	double ti=clock();
	int to[54];
	for(int i = 0; i < 54; i++)
		to[i] = 1;
	For(i,0,2){
		for(auto j:whatTheyPlayed[i]){
			for(auto k:j.cards){
				to[k]=0;
			}
		}
	}
	for(auto i:landlordPublicCards) to[i]=0;
	for(auto i:myCards) to[i]=0;
	vector<Card> v;
	For(i,0,53)if(to[i])v.pb(i);
	vector<pair<CardDistrib, double>> res;
	For(o,1,2000){
		random_shuffle(v.begin(),v.end());
		int dq=0;
		CardDistrib ans;
		For(i,0,2){
			if(i==myPosition){
				ans[i]=myCards;
			}else{
				if(i==landlordPosition)
					for(auto j:landlordPublicCards)
						ans[i].insert(j);
				For(j,0,cardRemaining[i]-1-(i==landlordPosition)*3)
					ans[i].insert(v[dq++]);
			}
		}
		res.pb(mp(ans,1));
	}
	sort(res.begin(),res.end());
	res.resize(unique(res.begin(),res.end()) - res.begin());
	for(auto &i:res){
		dist = i.first;
		ld t=1;
		int dq=myPosition;
		vector<CardCombo> zs;
		
		while(cardRemaining[landlordPosition]<20){
			zs.pb(whatTheyPlayed[(myPosition+2)%3].back());
			undoCombo();
			t*=exp(
			-sqr(getCandidatesEval(1)[0].fi-eval(zs.back()))/
			(2*sigma*sigma))
			/sqrt(2*M_PI)/sigma;
		}
		for(;zs.size();zs.pop_back())doCombo(zs.back()); 
		i.se=t;
	}
	sort(res.begin(),res.end(),cmp);
	if(res.size()>num)res.resize(num);
	ld sum=0; for(auto &i:res)sum+=i.se;
	for(auto &i:res)i.se/=sum;
#ifdef _LOG
	cerr << "Sum: " << sum << endl;
#endif
	return res;
}
