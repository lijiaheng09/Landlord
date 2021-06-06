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
	
	int to[54];
	for(int &x:to) x=1;
	for(auto i:myCards) to[i]--;
	vector<Card> v;
	For(i,0,53)if(to[i])v.pb(i);
	ld jb=clock();
	ld zs[2] = {0.0L, 0.0L};
	int cnt=0;
	while((clock()-jb)/CLOCKS_PER_SEC<0.9){
		#ifdef _LOG 
			cnt++;
		#endif
		random_shuffle(v.begin(),v.end());
		landlordPublicCards.clear();

		int pc = 0;

		For(i,0,2){
			landlordPublicCards.insert(v[pc++]);
		}

		for (int i = 0; i < PLAYER_COUNT; i++)
			if (i == myPosition)
				dist[i] = myCards;
			else {
				dist[i].clear();
				for (int j = 0; j < cardRemaining[i]; j++)
					dist[i].insert(v[pc++]);
			}
		
		int myPos0 = myPosition;

		for (int isLandlord : {0, 1}) {
			int pc = 3;
			landlordPosition = isLandlord ? myPosition : (myPosition + 1) % PLAYER_COUNT;
			if(!isLandlord&&evalCards(dist[landlordPosition])<
				evalCards(dist[(landlordPosition+1)%3]))
				swap(dist[landlordPosition],dist[(landlordPosition+1)%3]);
			for (Card c : landlordPublicCards)
				dist[landlordPosition].insert(c);
			landlordBid = 3;
			cardRemaining[landlordPosition] = 20;

			myPosition = landlordPosition;
			myCards = dist[myPosition];
			double t=search();
			
			//cerr<<(isLandlord?t:-t)<<" "; if(isLandlord==1)cerr<<endl;
			if(t>1)t=1; else if(t<-1)t=-1;
			if(isLandlord==1) t=t>0?t*1:t;
			else t=t<0?t*0.7:t;
			zs[isLandlord] += t;
			//cerr<<zs[0]<<" "<<zs[1]<<endl;
			//cerr<<dist[1].size()<<endl; exit(0);

			cardRemaining[landlordPosition] = 17;
			for (Card c : landlordPublicCards)
				dist[landlordPosition].erase(c);
			myPosition = myPos0;
			myCards = dist[myPosition];
		}
	}
	#ifdef _LOG
		cerr<<cnt<<endl;
	#endif
	cerr<<"farmer:"<<-zs[0]<<" landlord:"<<2*zs[1]<<endl;
	return 2.0L * zs[1] > -zs[0]&&zs[1]>0 ? 3 : 0;
}

ld sigma=3.0;
ld sqr(ld x){return x*x;}
bool cmp(const pair<CardDistrib, double> &a,
const pair<CardDistrib, double> &b){
	return a.se>b.se;
}
std::vector<std::pair<CardDistrib, double>> randCards(int num, double TL){
	double ti=clock();
	int to[54];
	for(int i = 0; i < 54; i++)
		to[i] = 1;
	for(auto i:landlordPublicCards) to[i]=0;
	for(auto i:myCards) to[i]=0;
	For(i,0,2){
		for(auto j:whatTheyPlayed[i]){
			for(auto k:j.cards){
				to[k]=-1;
			}
		}
	}
	vector<Card> v;
	For(i,0,53)if(to[i]==1)v.pb(i);
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
						if(to[j]!=-1)ans[i].insert(j);
				while (ans[i].size() < cardRemaining[i])
					ans[i].insert(v[dq++]);
			}
		}
		res.pb(mp(ans,-INFINITY));
	}
	sort(res.begin(),res.end());
	res.resize(unique(res.begin(),res.end()) - res.begin());
	int cnt = 0;
	for(auto &i:res){
#ifndef _DEBUG
		if ((++cnt & 127) == 0 && clock() > TL * CLOCKS_PER_SEC) {
#ifdef _LOG
			cerr << "TERM DIST" << ' ' << cnt << endl;
#endif
			break;
		}
#endif
		dist = i.first;
		ld t=0;
		int dq=myPosition;
		vector<CardCombo> zs;
		while(cardRemaining[landlordPosition]<20){
			zs.pb(whatTheyPlayed[(myPosition+2)%3].back());
			undoCombo();
			sigma=max(1,(int)whatTheyPlayed[myPosition].size());
			if (myPosition != myRealPosition)
				t+=log(getCandidateProb(zs.back()));// eval(zs.back());
		}
		for(;zs.size();zs.pop_back())doCombo(zs.back()); 
		i.se=t;
	}
	sort(res.begin(),res.end(),cmp);
	num=min(num,(int)res.size());
	vector<pair<CardDistrib,double> >jb;
	For(i,num/2,num-1){
		swap(res[i],res[i+rand()%(res.size()-i)]);
		jb.pb(res[i]);
		//cerr<<res[i].fi[0].count(32)+res[i].fi[0].count(33)+res[i].fi[0].count(34)+res[i].fi[0].count(35)<<endl;
	}
	res.resize(num/2);
	//cerr<<"bound: "<<res[num-1].se<<endl;
	ld mint = INFINITY;
	for (auto &&i : res) mint = min(mint, i.second);
	for (auto &i : res) i.second = exp(i.second - mint);
	ld sum=0; for(auto &i:res)sum+=i.se;
	for(auto &i:res)i.se=max(min(i.se/sum,0.1),0.4/num);
	for(auto &i:jb)res.pb(mp(i.fi,0.4/num));
	sum=0; for(auto &i:res)sum+=i.se;
	for(auto &i:res)i.se/=sum;
#ifdef _LOG
	cerr << "Sum: " << sum << endl;
#endif
	return res;
}
