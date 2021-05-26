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
						return fl;

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

					function<void (unsigned, Level)> searchOthers = [&](unsigned j, Level from) {
						if (j < pre.packs.size()) {
							Level k;
							for (k = from; k <= MAX_LEVEL; k++) {
								if (requiredCounts[k] || counts[k] < pre.packs[j].count)
									continue;
								requiredCounts[k] = pre.packs[j].count;
								searchOthers(j + 1, k + 1);
								requiredCounts[k] = 0;
							}
						} else {
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
							for (Card c : solve)
								requiredCounts[card2level(c)]++;
						}
					};
					searchOthers(mainPackCount, 0);
				}

			next:; // 再增大
			}
			return fl;
		}
	};

	if (lastValidCombo.comboType == CardComboType::PASS) {
		addCandidate({0}, 0);
		bool pa = addCandidate({0, 1}, 0);
		vector<Card> v;
		v = {0, 4, 8, 12};
		for (Card i = 4; i <= 11; i++) {
			v.push_back(i * 4);
			if (!addCandidate(v, 0))
				break;
		}
		if (pa) {
			v = {0, 1, 4, 5};
			for (Card i = 2; i <= 11; i++) {
				v.push_back(i * 4);
				v.push_back(i * 4 + 1);
				if (!addCandidate(v, 0))
					break;
			}

			if (addCandidate({0, 1, 2}, 0)) {
				if (addCandidate({0, 1, 2, 4}, 0))
					addCandidate({0, 1, 2, 4, 5}, 0);
				if (addCandidate({0, 1, 2, 3, 4, 8}, 0))
					addCandidate({0, 1, 2, 3, 4, 5, 8, 9}, 0);
				if (addCandidate({0, 1, 2, 4, 5, 6}, 0))
					if (addCandidate({0, 1, 2, 4, 5, 6, 8, 12}, 0))
						addCandidate({0, 1, 2, 4, 5, 6, 8, 9, 12, 13}, 0);
				if (addCandidate({0, 1, 2, 3, 4, 5, 6, 7}, 0))
					if (addCandidate({0, 1, 2, 3, 4, 5, 6, 7, 8, 12, 16, 20}, 0))
						addCandidate({0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 12, 13, 16, 17, 20, 21}, 0);
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

inline bool le_first(const pair<double, CardCombo> &a, const pair<double, CardCombo> &b) {
	return a.first < b.first;
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

/*
double evalProb(const CardCombo &c0) {
	double s = 0.0, r = 0.0;
	for (auto &&c : getCandidates()) {
		double v = exp(eval(c));
		if (c.packs == c0.packs)
			r = v;
		s += v;
	}
	return r / s;
}
*/

vector<pair<double, CardCombo>> getCandidatesEval(int num, int &max_sc) {
	max_sc = 1;
	vector<pair<double, CardCombo>> candidates;
	double s = 0.0;
	for (auto &&c : getCandidates()) {
		double v = exp(eval(c));
		candidates.push_back({v, c});
		s += v;
		if (c.comboType == CardComboType::BOMB || c.comboType == CardComboType::ROCKET)
			max_sc <<= 1;
	}
	for (auto &c : candidates)
		c.first /= s;
	sort(candidates.begin(), candidates.end(), gt_first);
	if (num != -1 && candidates.size() > num)
		candidates.resize(num);
	return candidates;
}

vector<pair<double, CardCombo>> getCandidatesEval(int num) {
	int dum;
	return getCandidatesEval(num, dum);
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
			break;
		}
	}
}

struct Node {
	int sc, max_sc;
	double v;
	CardCombo c;
	Node *fa;

	Node() : sc(-INF), v(0.0), c(), fa(0) { }
	Node(double a_v, const CardCombo &a_c, Node *a_fa) : sc(-INF), v(a_v), c(a_c), fa(a_fa) { }
} *root;

struct CmpNode {
	bool operator () (const Node *a, const Node *b) const {
		return a->v < b->v;
	}
};

vector<Node *> candidateNodes;
map<CardCombo, double> resEval;
map<CardCombo, double> rawEval;
priority_queue<Node *, vector<Node *>, CmpNode> Q;

template <int Init = 0>
void extendDown(Node *p) {
	int prevPosition = (myPosition + PLAYER_COUNT - 1) % PLAYER_COUNT;
	if (dist[prevPosition].empty()) {
		bool is_enemy = myPosition == landlordPosition || prevPosition % PLAYER_COUNT == landlordPosition;
		p->max_sc = p->sc = is_enemy ? -1 : 1;
	} else {
		bool is_enemy = myPosition == landlordPosition || (myPosition + 1) % PLAYER_COUNT == landlordPosition;
		auto candidates = getCandidatesEval(-1, p->max_sc);
		bool def = 1;
		for (auto &&c : candidates) {
			Node *t = new Node(p->v + log(c.first), c.second, p);
			if (Init) {
				candidateNodes.push_back(t);
				rawEval[c.second] = c.first;
			}
			if (Init || def) {
				int mul_sc = c.second.comboType == CardComboType::BOMB || c.second.comboType == CardComboType::ROCKET ? 2 : 1;
				doCombo(c.second);
				extendDown(t);
				undoCombo();
				p->sc = max(p->sc, mul_sc * (is_enemy ? -t->sc : t->sc));
				def = 0;
			}
			else
				Q.push(t);
		}
	}
}

void doNode(Node *t) {
	Node *p = t->fa;
	if (p) {
		const CardCombo &c = t->c;
		doNode(p);
		doCombo(c);
	}
}

void upward(Node *t) {
	Node *p = t->fa;
	if (p) {
		const CardCombo &c = t->c;
		int mul_sc = c.comboType == CardComboType::BOMB || c.comboType == CardComboType::ROCKET ? 2 : 1;
		undoCombo();
		bool is_enemy = myPosition == landlordPosition || (myPosition + 1) % PLAYER_COUNT == landlordPosition;
		p->sc = max(p->sc, mul_sc * (is_enemy ? -t->sc : t->sc));
		upward(p);
	}
}

void extend(Node *p) {
	doNode(p);
	extendDown(p);
	upward(p);
}

void update(double TL) {
	int cnt = 0;
	while (!Q.empty()) {
		if ((++cnt & 127) == 0 && clock() > TL * CLOCKS_PER_SEC)
			break;
		Node *p = Q.top();
		Q.pop();
		extend(p);
	}
}

int search(double TL) {
	while (!Q.empty())
		Q.pop();
	candidateNodes.clear();
	root = new Node();
	extendDown<1>(root);
	update(TL);
	return root->sc;
}

void searchCandidates(double w, double TL) {
	search(TL);
	bool is_enemy = myPosition == landlordPosition || (myPosition + 1) % PLAYER_COUNT == landlordPosition;
	for (Node *t : candidateNodes) {
		const CardCombo &c = t->c;
		int mul_sc = c.comboType == CardComboType::BOMB || c.comboType == CardComboType::ROCKET ? 2 : 1;
		double sc = mul_sc * (is_enemy ? -t->sc : t->sc);
		/*if (sc >= 0)
			sc = pow(sc, 0.8);
		else
			sc = -pow(-sc, 1.2);*/
#ifdef _LOG
		cerr << w << ' ' << sc << endl;
#endif
		if (!resEval.count(c))
			resEval[c] = 0.0;
		resEval[c] += w * sc;
	}
}

CardCombo getAction() {
	static const int DIST_NUM = 20;

	auto dists = randCards(DIST_NUM);
	double startTime = (double)clock() / CLOCKS_PER_SEC;
#ifdef _LOG
	cerr << "Rand Time: " << startTime << endl;
#endif
	resEval.clear();
	rawEval.clear();
	int num = dists.size();
	double vTime = (0.9 - startTime) / num;
	for (int i = 0; i < num; i++) {
		dist = dists[i].first;
		searchCandidates(dists[i].second, startTime + vTime * (i + 1));
		if (clock() > 0.85 * CLOCKS_PER_SEC)
			break;
	}
	double val = -INFINITY;
	CardCombo res;
	for (auto &&p : resEval) {
#ifdef _LOG
		p.first.debugPrint();
		cerr << rawEval[p.first] << ' ' << log(rawEval[p.first]) << ' ' << p.second << endl;
#endif
		p.second += 0.02 * cardRemaining[myPosition] * log(rawEval[p.first]);
		if (p.second > val) {
			res = p.first;
			val = p.second;
		}
	}
	return res;
}

/*
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
*/
