#include <bits/stdc++.h>
#include "card.h"
#include "main.h"

using namespace std;

vector<CardCombo> getCandidates() {
	vector<CardCombo> r;

	auto deck = vector<Card>(myCards.begin(), myCards.end()); // 手牌
	short counts[MAX_LEVEL + 1] = {};

	unsigned short kindCount = 0;

	// 先数一下手牌里每种牌有多少个
	for (Card c : deck)
		counts[card2level(c)]++;

	// 再数一下手牌里有多少种牌
	for (short c : counts)
		if (c)
			kindCount++;

	auto addCandidate = [&](const CardCombo &pre, bool strict = 1) -> bool {
		// 然后先看一下是不是火箭，是的话就过
		CardComboType type = pre.comboType;
		if (type == CardComboType::ROCKET || type == CardComboType::BOMB)
			return 0;

		// 现在打算从手牌中凑出同牌型的牌
		// 手牌如果不够用，直接不用凑了，看看能不能炸吧
		if (deck.size() < pre.cards.size())
			return 0;

		bool fl = 0;

		// 否则不断增大当前牌组的主牌，看看能不能找到匹配的牌组
		{
			// 开始增大主牌
			int mainPackCount = pre.findMaxSeq();
			bool isSequential =
				type == CardComboType::STRAIGHT ||
				type == CardComboType::STRAIGHT2 ||
				type == CardComboType::PLANE ||
				type == CardComboType::PLANE1 ||
				type == CardComboType::PLANE2 ||
				type == CardComboType::SSHUTTLE ||
				type == CardComboType::SSHUTTLE2 ||
				type == CardComboType::SSHUTTLE4;
			for (Level i = strict ? 1 : 0;; i++) // 增大多少
			{
				for (int j = 0; j < mainPackCount; j++)
				{
					int level = pre.packs[j].level + i;

					// 各种连续牌型的主牌不能到2，非连续牌型的主牌不能到小王，单张的主牌不能超过大王
					if ((type == CardComboType::SINGLE && level > MAX_LEVEL) ||
						(isSequential && level > MAX_STRAIGHT_LEVEL) ||
						(type != CardComboType::SINGLE && !isSequential && level >= level_joker))
						return 0;

					// 如果手牌中这种牌不够，就不用继续增了
					if (counts[level] < pre.packs[j].count)
						goto next;
				}

				{
					// 找到了合适的主牌，那么从牌呢？
					// 如果手牌的种类数不够，那从牌的种类数就不够，也不行
					if (kindCount < pre.packs.size())
						continue;

					// 好终于可以了
					// 计算每种牌的要求数目吧
					short requiredCounts[MAX_LEVEL + 1] = {};
					for (int j = 0; j < mainPackCount; j++)
						requiredCounts[pre.packs[j].level + i] = pre.packs[j].count;
					for (unsigned j = mainPackCount; j < pre.packs.size(); j++)
					{
						Level k;
						for (k = 0; k <= MAX_LEVEL; k++)
						{
							if (requiredCounts[k] || counts[k] < pre.packs[j].count)
								continue;
							requiredCounts[k] = pre.packs[j].count;
							break;
						}
						if (k == MAX_LEVEL + 1) // 如果是都不符合要求……就不行了
							goto next;
					}

					// 开始产生解
					std::vector<Card> solve;
					for (Card c : deck)
					{
						Level level = card2level(c);
						if (requiredCounts[level])
						{
							solve.push_back(c);
							requiredCounts[level]--;
						}
					}
					fl = 1;
					r.push_back(CardCombo(solve.begin(), solve.end()));
				}

			next:; // 再增大
			}
			return fl;
		}
	};

	if (lastValidCombo.comboType == CardComboType::PASS) {
		addCandidate({0}, 0);
		bool pa = addCandidate({0, 0}, 0);
		vector<Card> v;
		v = {0, 1, 2, 3};
		for (Card i = 4; i <= 11; i++) {
			v.push_back(i);
			if (!addCandidate(v, 0))
				break;
		}
		if (pa) {
			v = {0, 0, 1, 1};
			for (Card i = 2; i <= 11; i++) {
				v.push_back(i);
				v.push_back(i);
				if (!addCandidate(v, 0))
					break;
			}

			if (addCandidate({0, 0, 0}, 0)) {
				if (addCandidate({0, 0, 0, 1}, 0))
					addCandidate({0, 0, 0, 1, 1}, 0);
				if (addCandidate({0, 0, 0, 0, 1, 2}, 0))
					addCandidate({0, 0, 0, 0, 1, 1, 2, 2}, 0);
				if (addCandidate({0, 0, 0, 1, 1, 1}, 0))
					if (addCandidate({0, 0, 0, 1, 1, 1, 2, 3}, 0))
						addCandidate({0, 0, 0, 1, 1, 1, 2, 2, 3, 3}, 0);
				if (addCandidate({0, 0, 0, 0, 1, 1, 1, 1}))
					if (addCandidate({0, 0, 0, 0, 1, 1, 1, 1, 2, 3, 4, 5}))
						addCandidate({0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5});
			}
		}
	} else {
		r.push_back(CardCombo());
		addCandidate(lastValidCombo);
	}
	// 实在找不到啊
	// 最后看一下能不能炸吧

	for (Level i = 0; i < level_joker; i++)
		if (counts[i] == 4 && (lastValidCombo.comboType != CardComboType::BOMB || i > lastValidCombo.packs[0].level)) // 如果对方是炸弹，能炸的过才行
		{
			// 还真可以啊……
			Card bomb[] = {Card(i * 4), Card(i * 4 + 1), Card(i * 4 + 2), Card(i * 4 + 3)};
			r.push_back(CardCombo(bomb, bomb + 4));
		}

	// 有没有火箭？
	if (counts[level_joker] + counts[level_JOKER] == 2)
	{
		Card rocket[] = {card_joker, card_JOKER};
		r.push_back(CardCombo(rocket, rocket + 2));
	}

	// ……
	return r;
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

vector<pair<double, CardCombo>> getCandidatesEval(int num) {
	vector<pair<double, CardCombo>> candidates;
	for (auto &&c : getCandidates())
		candidates.push_back({eval(c), c});
	sort(candidates.begin(), candidates.end(), gt_first);
	if (candidates.size() > num)
		candidates.resize(num);
	return candidates;
}

void doCombo(const CardCombo &c) {
	whatTheyPlayed[myPosition].push_back(c);
	if (c.comboType != CardComboType::PASS) {
		lastValidCombo = c;
		lastValidComboPosition = myPosition;
	} else if (lastValidComboPosition == (myPosition + 1) % PLAYER_COUNT) {
		lastValidCombo = CardCombo();
		lastValidComboPosition = -1;
	}
	dist[myPosition] = cardSub(dist[myPosition], c);
	cardRemaining[myPosition] -= c.cards.size();
	myPosition = (myPosition + 1) % PLAYER_COUNT;
	myCards = dist[myPosition];
}

void undoCombo() {
	myPosition = (myPosition + PLAYER_COUNT - 1) % PLAYER_COUNT;
	const CardCombo &c = whatTheyPlayed[myPosition].back();
	cardRemaining[myPosition] += c.cards.size();
	myCards = dist[myPosition] = cardAdd(dist[myPosition], c);
	whatTheyPlayed[myPosition].pop_back();
	lastValidCombo = CardCombo();
	lastValidComboPosition = -1;
	for (int p = 1; p < PLAYER_COUNT; p++) {
		int pos = (myPosition + PLAYER_COUNT - p) % PLAYER_COUNT;
		const auto &v = whatTheyPlayed[pos];
		if (v.empty())
			break;
		const auto &d = v.back();
		if (d.comboType != CardComboType::PASS) {
			lastValidCombo = d;
			lastValidComboPosition = pos;
		}
	}
}

int procSearch(const CardCombo &c) {
	int ans;

	if (myCards.size() == c.cards.size())
		ans = 1;
	else {
		bool is_landlord = myPosition == landlordPosition;

		doCombo(c);
		ans = search();
		if (is_landlord || (myPosition == landlordPosition))
			ans = -ans;
		undoCombo();
	}
	
	if (c.comboType == CardComboType::BOMB || c.comboType == CardComboType::ROCKET)
		ans *= 2;

	return ans;
}

int search() {
	static const int NUM = 3;
	auto candidates = getCandidatesEval(NUM);
	int ans = -INF;
	for (auto &&cs : candidates)
		ans = max(ans, procSearch(cs.second));
	return ans;
}

CardCombo getAction() {
	static const int DIST_NUM = 20, CAND_NUM = 10;
	auto dists = randCards(DIST_NUM);
	auto candidates = getCandidatesEval(CAND_NUM);
	for (auto &c : candidates) {
		c.first = 0;
		for (auto &&d : dists) {
			dist = d.first;
			myCards = dist[myPosition];
			c.first += d.second * procSearch(c.second);
		}
	}
	return max_element(candidates.begin(), candidates.end(), gt_first)->second;
}

double getMean() {
	static const int DIST_NUM = 20;
	auto dists = randCards(DIST_NUM);
	double ans = 0;
	for (auto &&d : dists) {
		dist = d.first;
		myCards = dist[myPosition];
		ans += d.second * search();
	}
	return ans;
}
