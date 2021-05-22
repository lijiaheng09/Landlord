#include <bits/stdc++.h>
#include "card.h"
#include "main.h"

std::vector<CardCombo> candidates() {
	return {};
}

int search(int score) {
}

CardCombo getAction() {
	return lastValidCombo.findFirstValid(myCards.begin(), myCards.end());
}
