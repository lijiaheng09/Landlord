#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef unsigned long long ull;
typedef long double ld;
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
	return 3;
}

std::vector<std::pair<CardDistrib, double>> randCards(int num){
	int to[54]={1};
	For(i,0,2){
		for(auto j:whatTheyPlayed[i]){
			for(auto k:j){
				to[k]--;
			}
		}
	}
	for(auto i:landlordPublicCards) to[i]--;
	for(auto i:myCards) to[i]--;
	vector<int> v;
	For(i,0,53)v.pb(i);
	random_shuffle(v.begin(),v.end());
	CardDistrib ans;
	For(i,0,2){
		if(i==myPosition){
			ans[i]=myCards;
		}else {
			if(i==landlordPosition)
				for(auto j:landlordPublicCards)
					ans[i].insert(j);
			For(j,0,cardRemaining[i]-1){
				ans[i].insert(v.back());
				v.pop_back();
			}
		}
	}
	vector<pair<CardDistrib, double>> res;
	res.pb(mp(ans,1));
	return res;
}