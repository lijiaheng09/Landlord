#include <bits/stdc++.h>
#include "card.h"
#include "main.h"

using namespace std;

std::vector<CardCombo> getCandidates() {
	return {};
}

inline bool gt_first(const pair<double, CardCombo> &a, const pair<double, CardCombo> &b) {
	return a.first > b.first;
}

constexpr int INF = 0x3f3f3f3f;

CardSet cardSub(const CardSet &s, const CardCombo &c) {
	CardSet r = s;
	for (Card v : c.cards)
		r.erase(v);
	return r;
}

CardSet cardAdd(const CardSet &s, const CardCombo &c) {
	CardSet r = s;
	for (Card v : c.cards)
		r.insert(v);
	return r;
}

int search(int score) {
	if (myCards.empty())
		return score;
	bool is_landlord = myPosition == landlordPosition;
	vector<pair<double, CardCombo>> candidates;
	for (auto &&c : getCandidates())
		candidates.push_back({eval(c), c});
	sort(candidates.begin(), candidates.end(), gt_first);
	static const int NUM = 3;
	if (candidates.size() > NUM)
		candidates.resize(NUM);
	int ans = -INF;
	auto r_lastValidCombo = lastValidCombo;
	for (auto &&cs : candidates) {
		const CardCombo &c = cs.second;
		whatTheyPlayed[myPosition].push_back(c);
		lastValidCombo = c;
		dist[myPosition] = cardSub(dist[myPosition], c);
		cardRemaining[myPosition] -= c.cards.size();
		myPosition = (myPosition + 1) % 3;
		myCards = dist[myPosition];

		if (is_landlord || (myPosition == landlordPosition))
			ans = max(ans, -search(score));
		else
			ans = max(ans, search(score));

		myPosition = (myPosition + 2) % 3;
		cardRemaining[myPosition] += c.cards.size();
		dist[myPosition] = cardAdd(dist[myPosition], c);
		whatTheyPlayed[myPosition].pop_back();
	}
	lastValidCombo = r_lastValidCombo;
	myCards = dist[myPosition];
}

CardCombo getAction() {
	static const int NUM = 20;
	auto dists = randCards(NUM);
}
