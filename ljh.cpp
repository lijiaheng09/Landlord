#include <bits/stdc++.h>
#include "card.h"
#include "main.h"

int search() {
	return 1;
}

CardCombo getAction() {
	return lastValidCombo.findFirstValid(myCards.begin(), myCards.end());
}
