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

int search(int score) {
	if (myCards.empty())
		return score;
	vector<pair<double, CardCombo>> candidates;
	for (auto &&c : getCandidates())
		candidates.push_back({eval(c), c});
	sort(candidates.begin(), candidates.end(), gt_first);
	static const int NUM = 3;
	if (candidates.size() > NUM)
		candidates.resize(NUM);
	int ans = -INF;
	for (auto &&c : candidates) {
		whatTheyPlayed[myPosition].push_back(c.second);
		myPosition = (myPosition + 1) % 3;
		myCards = dist[myPosition];
		myPosition = (myPosition + 2) % 3;
	}
}

CardCombo getAction() {
	return lastValidCombo.findFirstValid(myCards.begin(), myCards.end());
}
