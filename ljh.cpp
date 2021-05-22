#include <bits/stdc++.h>
#include "card.h"
#include "main.h"

std::vector<CardCombo> getCandidates() {
	return {};
}

int search(int score) {
	if (myCards.empty())
		return score;
	
}

CardCombo getAction() {
	return lastValidCombo.findFirstValid(myCards.begin(), myCards.end());
}
