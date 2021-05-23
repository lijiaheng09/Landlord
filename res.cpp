// card.h
#ifndef CARD_H
#define CARD_H

#include <bits/stdc++.h>

constexpr int PLAYER_COUNT = 3;

enum class Stage
{
	BIDDING, // 叫分阶段
	PLAYING	 // 打牌阶段
};

enum class CardComboType
{
	PASS,		// 过
	SINGLE,		// 单张
	PAIR,		// 对子
	STRAIGHT,	// 顺子
	STRAIGHT2,	// 双顺
	TRIPLET,	// 三条
	TRIPLET1,	// 三带一
	TRIPLET2,	// 三带二
	BOMB,		// 炸弹
	QUADRUPLE2, // 四带二（只）
	QUADRUPLE4, // 四带二（对）
	PLANE,		// 飞机
	PLANE1,		// 飞机带小翼
	PLANE2,		// 飞机带大翼
	SSHUTTLE,	// 航天飞机
	SSHUTTLE2,	// 航天飞机带小翼
	SSHUTTLE4,	// 航天飞机带大翼
	ROCKET,		// 火箭
	INVALID		// 非法牌型
};

const int cardComboScores[] = {
	0,	// 过
	1,	// 单张
	2,	// 对子
	6,	// 顺子
	6,	// 双顺
	4,	// 三条
	4,	// 三带一
	4,	// 三带二
	10, // 炸弹
	8,	// 四带二（只）
	8,	// 四带二（对）
	8,	// 飞机
	8,	// 飞机带小翼
	8,	// 飞机带大翼
	10, // 航天飞机（需要特判：二连为10分，多连为20分）
	10, // 航天飞机带小翼
	10, // 航天飞机带大翼
	16, // 火箭
	0	// 非法牌型
};

#ifndef _BOTZONE_ONLINE
extern std::string cardComboStrings[];
#endif

// 用0~53这54个整数表示唯一的一张牌
using Card = short;
constexpr Card card_joker = 52;
constexpr Card card_JOKER = 53;

// 除了用0~53这54个整数表示唯一的牌，
// 这里还用另一种序号表示牌的大小（不管花色），以便比较，称作等级（Level）
// 对应关系如下：
// 3 4 5 6 7 8 9 10	J Q K	A	2	小王	大王
// 0 1 2 3 4 5 6 7	8 9 10	11	12	13	14
using Level = short;
constexpr Level MAX_LEVEL = 15;
constexpr Level MAX_STRAIGHT_LEVEL = 11;
constexpr Level level_joker = 13;
constexpr Level level_JOKER = 14;

/**
* 将Card变成Level
*/
constexpr Level card2level(Card card)
{
	return card / 4 + card / 53;
}

// 牌的组合，用于计算牌型
struct CardCombo
{
	// 表示同等级的牌有多少张
	// 会按个数从大到小、等级从大到小排序
	struct CardPack
	{
		Level level;
		short count;

		bool operator<(const CardPack &b) const
		{
			if (count == b.count)
				return level > b.level;
			return count > b.count;
		}
	};
	std::vector<Card> cards;		 // 原始的牌，未排序
	std::vector<CardPack> packs;	 // 按数目和大小排序的牌种
	CardComboType comboType; // 算出的牌型
	Level comboLevel = 0;	 // 算出的大小序

	/**
	* 检查个数最多的CardPack递减了几个
	*/
	int findMaxSeq() const;

	/**
	* 这个牌型最后算总分的时候的权重
	*/
	int getWeight() const;

	// 创建一个空牌组
	CardCombo() : comboType(CardComboType::PASS) {}

	CardCombo(const std::initializer_list<Card> &c) : CardCombo(c.begin(), c.end()) { }

	CardCombo(const std::vector<Card> &c) : CardCombo(c.begin(), c.end()) {}

	/**
	* 通过Card（即short）类型的迭代器创建一个牌型
	* 并计算出牌型和大小序等
	* 假设输入没有重复数字（即重复的Card）
	*/
	template <typename CARD_ITERATOR>
	CardCombo(CARD_ITERATOR begin, CARD_ITERATOR end);

	/**
	* 判断指定牌组能否大过当前牌组（这个函数不考虑过牌的情况！）
	*/
	bool canBeBeatenBy(const CardCombo &b) const;

	/**
	* 从指定手牌中寻找第一个能大过当前牌组的牌组
	* 如果随便出的话只出第一张
	* 如果不存在则返回一个PASS的牌组
	*/
	template <typename CARD_ITERATOR>
	CardCombo findFirstValid(CARD_ITERATOR begin, CARD_ITERATOR end) const;

	void debugPrint();
};

using CardSet = std::set<Card>;
using CardDistrib = std::array<CardSet, PLAYER_COUNT>;

// 我的牌有哪些
extern std::set<Card> myCards;

// 地主明示的牌有哪些
extern std::set<Card> landlordPublicCards;

// 大家从最开始到现在都出过什么
extern std::vector<CardCombo> whatTheyPlayed[PLAYER_COUNT];

// 当前要出的牌需要大过谁
extern CardCombo lastValidCombo;
extern int lastValidComboPosition;

// 大家还剩多少牌
extern short cardRemaining[PLAYER_COUNT];

// 我是几号玩家（0-地主，1-农民甲，2-农民乙）
extern int myPosition;

// 地主位置
extern int landlordPosition;

// 地主叫分
extern int landlordBid;

// 阶段
extern Stage stage;

// 自己的第一回合收到的叫分决策
extern std::vector<int> bidInput;

extern CardDistrib dist;

// 实现
// #include "card.hpp"

#endif

// card.hpp
#ifndef CARD_HPP
#define CARD_HPP

// #include "card.h"

template <typename CARD_ITERATOR>
CardCombo::CardCombo(CARD_ITERATOR begin, CARD_ITERATOR end)
{
	using namespace std;
	// 特判：空
	if (begin == end)
	{
		comboType = CardComboType::PASS;
		return;
	}

	// 每种牌有多少个
	short counts[MAX_LEVEL + 1] = {};

	// 同种牌的张数（有多少个单张、对子、三条、四条）
	short countOfCount[5] = {};

	cards = std::vector<Card>(begin, end);
	for (Card c : cards)
		counts[card2level(c)]++;
	for (Level l = 0; l <= MAX_LEVEL; l++)
		if (counts[l])
		{
			packs.push_back(CardPack{l, counts[l]});
			countOfCount[counts[l]]++;
		}
	sort(packs.begin(), packs.end());

	// 用最多的那种牌总是可以比较大小的
	comboLevel = packs[0].level;

	// 计算牌型
	// 按照 同种牌的张数 有几种 进行分类
	std::vector<int> kindOfCountOfCount;
	for (int i = 0; i <= 4; i++)
		if (countOfCount[i])
			kindOfCountOfCount.push_back(i);
	sort(kindOfCountOfCount.begin(), kindOfCountOfCount.end());

	int curr, lesser;

	switch (kindOfCountOfCount.size())
	{
	case 1: // 只有一类牌
		curr = countOfCount[kindOfCountOfCount[0]];
		switch (kindOfCountOfCount[0])
		{
		case 1:
			// 只有若干单张
			if (curr == 1)
			{
				comboType = CardComboType::SINGLE;
				return;
			}
			if (curr == 2 && packs[1].level == level_joker)
			{
				comboType = CardComboType::ROCKET;
				return;
			}
			if (curr >= 5 && findMaxSeq() == curr &&
				packs.begin()->level <= MAX_STRAIGHT_LEVEL)
			{
				comboType = CardComboType::STRAIGHT;
				return;
			}
			break;
		case 2:
			// 只有若干对子
			if (curr == 1)
			{
				comboType = CardComboType::PAIR;
				return;
			}
			if (curr >= 3 && findMaxSeq() == curr &&
				packs.begin()->level <= MAX_STRAIGHT_LEVEL)
			{
				comboType = CardComboType::STRAIGHT2;
				return;
			}
			break;
		case 3:
			// 只有若干三条
			if (curr == 1)
			{
				comboType = CardComboType::TRIPLET;
				return;
			}
			if (findMaxSeq() == curr &&
				packs.begin()->level <= MAX_STRAIGHT_LEVEL)
			{
				comboType = CardComboType::PLANE;
				return;
			}
			break;
		case 4:
			// 只有若干四条
			if (curr == 1)
			{
				comboType = CardComboType::BOMB;
				return;
			}
			if (findMaxSeq() == curr &&
				packs.begin()->level <= MAX_STRAIGHT_LEVEL)
			{
				comboType = CardComboType::SSHUTTLE;
				return;
			}
		}
		break;
	case 2: // 有两类牌
		curr = countOfCount[kindOfCountOfCount[1]];
		lesser = countOfCount[kindOfCountOfCount[0]];
		if (kindOfCountOfCount[1] == 3)
		{
			// 三条带？
			if (kindOfCountOfCount[0] == 1)
			{
				// 三带一
				if (curr == 1 && lesser == 1)
				{
					comboType = CardComboType::TRIPLET1;
					return;
				}
				if (findMaxSeq() == curr && lesser == curr &&
					packs.begin()->level <= MAX_STRAIGHT_LEVEL)
				{
					comboType = CardComboType::PLANE1;
					return;
				}
			}
			if (kindOfCountOfCount[0] == 2)
			{
				// 三带二
				if (curr == 1 && lesser == 1)
				{
					comboType = CardComboType::TRIPLET2;
					return;
				}
				if (findMaxSeq() == curr && lesser == curr &&
					packs.begin()->level <= MAX_STRAIGHT_LEVEL)
				{
					comboType = CardComboType::PLANE2;
					return;
				}
			}
		}
		if (kindOfCountOfCount[1] == 4)
		{
			// 四条带？
			if (kindOfCountOfCount[0] == 1)
			{
				// 四条带两只 * n
				if (curr == 1 && lesser == 2)
				{
					comboType = CardComboType::QUADRUPLE2;
					return;
				}
				if (findMaxSeq() == curr && lesser == curr * 2 &&
					packs.begin()->level <= MAX_STRAIGHT_LEVEL)
				{
					comboType = CardComboType::SSHUTTLE2;
					return;
				}
			}
			if (kindOfCountOfCount[0] == 2)
			{
				// 四条带两对 * n
				if (curr == 1 && lesser == 2)
				{
					comboType = CardComboType::QUADRUPLE4;
					return;
				}
				if (findMaxSeq() == curr && lesser == curr * 2 &&
					packs.begin()->level <= MAX_STRAIGHT_LEVEL)
				{
					comboType = CardComboType::SSHUTTLE4;
					return;
				}
			}
		}
	}

	comboType = CardComboType::INVALID;
}

#endif

// main.h
#ifndef MAIN_H
#define MAIN_H

#include <bits/stdc++.h>
// #include "card.h"

int getBidValue(int maxBid); // 叫分
CardCombo getAction(); // 出牌, 搜索记得维护全局变量
double getMean(); // 根据当前牌得到最优策略得分期望

int search(); // 最优得分
std::vector<CardCombo> getCandidates();
std::vector<std::pair<CardDistrib, double>> randCards(int num);
std::vector<std::pair<double, CardCombo>> getCandidatesEval(int num);
double evalCards(const CardSet &); // 期望得分
double eval(const CardCombo &); // 期望得分

CardSet cardSub(const CardSet &s, const CardCombo &c);
CardSet cardAdd(const CardSet &s, const CardCombo &c);

void doCombo(const CardCombo &c);
void undoCombo();

#endif

// card.cpp
// 斗地主·改（FightTheLandlord2）样例程序
// 无脑策略
// 最后修改：2021-05-08，去除了有问题的isArray。
// 作者：zhouhy
// 游戏信息：http://www.botzone.org/games#FightTheLandlord2

#include <bits/stdc++.h>
// #include "card.h"

#ifndef _BOTZONE_ONLINE
std::string cardComboStrings[] = {
	"PASS",
	"SINGLE",
	"PAIR",
	"STRAIGHT",
	"STRAIGHT2",
	"TRIPLET",
	"TRIPLET1",
	"TRIPLET2",
	"BOMB",
	"QUADRUPLE2",
	"QUADRUPLE4",
	"PLANE",
	"PLANE1",
	"PLANE2",
	"SSHUTTLE",
	"SSHUTTLE2",
	"SSHUTTLE4",
	"ROCKET",
	"INVALID"};
#endif

using std::set;
using std::sort;
using std::string;
using std::unique;
using std::vector;

std::set<Card> myCards;

// 地主明示的牌有哪些
std::set<Card> landlordPublicCards;

// 大家从最开始到现在都出过什么
std::vector<CardCombo> whatTheyPlayed[PLAYER_COUNT];

// 当前要出的牌需要大过谁
CardCombo lastValidCombo;
int lastValidComboPosition = -1;

// 大家还剩多少牌
short cardRemaining[PLAYER_COUNT] = {17, 17, 17};

// 我是几号玩家（0-地主，1-农民甲，2-农民乙）
int myPosition;

// 地主位置
int landlordPosition = -1;

// 地主叫分
int landlordBid = -1;

// 阶段
Stage stage = Stage::BIDDING;

// 自己的第一回合收到的叫分决策
std::vector<int> bidInput;

CardDistrib dist;

int CardCombo::findMaxSeq() const
{
	for (unsigned c = 1; c < packs.size(); c++)
		if (packs[c].count != packs[0].count ||
			packs[c].level != packs[c - 1].level - 1)
			return c;
	return packs.size();
}

int CardCombo::getWeight() const
{
	if (comboType == CardComboType::SSHUTTLE ||
		comboType == CardComboType::SSHUTTLE2 ||
		comboType == CardComboType::SSHUTTLE4)
		return cardComboScores[(int)comboType] + (findMaxSeq() > 2) * 10;
	return cardComboScores[(int)comboType];
}

bool CardCombo::canBeBeatenBy(const CardCombo &b) const
{
	if (comboType == CardComboType::INVALID || b.comboType == CardComboType::INVALID)
		return false;
	if (b.comboType == CardComboType::ROCKET)
		return true;
	if (b.comboType == CardComboType::BOMB)
		switch (comboType)
		{
		case CardComboType::ROCKET:
			return false;
		case CardComboType::BOMB:
			return b.comboLevel > comboLevel;
		default:
			return true;
		}
	return b.comboType == comboType && b.cards.size() == cards.size() && b.comboLevel > comboLevel;
}

template <typename CARD_ITERATOR>
CardCombo CardCombo::findFirstValid(CARD_ITERATOR begin, CARD_ITERATOR end) const
{
	if (comboType == CardComboType::PASS) // 如果不需要大过谁，只需要随便出
	{
		CARD_ITERATOR second = begin;
		second++;
		return CardCombo(begin, second); // 那么就出第一张牌……
	}

	// 然后先看一下是不是火箭，是的话就过
	if (comboType == CardComboType::ROCKET)
		return CardCombo();

	// 现在打算从手牌中凑出同牌型的牌
	auto deck = std::vector<Card>(begin, end); // 手牌
	short counts[MAX_LEVEL + 1] = {};

	unsigned short kindCount = 0;

	// 先数一下手牌里每种牌有多少个
	for (Card c : deck)
		counts[card2level(c)]++;

	// 手牌如果不够用，直接不用凑了，看看能不能炸吧
	if (deck.size() < cards.size())
		goto failure;

	// 再数一下手牌里有多少种牌
	for (short c : counts)
		if (c)
			kindCount++;

	// 否则不断增大当前牌组的主牌，看看能不能找到匹配的牌组
	{
		// 开始增大主牌
		int mainPackCount = findMaxSeq();
		bool isSequential =
			comboType == CardComboType::STRAIGHT ||
			comboType == CardComboType::STRAIGHT2 ||
			comboType == CardComboType::PLANE ||
			comboType == CardComboType::PLANE1 ||
			comboType == CardComboType::PLANE2 ||
			comboType == CardComboType::SSHUTTLE ||
			comboType == CardComboType::SSHUTTLE2 ||
			comboType == CardComboType::SSHUTTLE4;
		for (Level i = 1;; i++) // 增大多少
		{
			for (int j = 0; j < mainPackCount; j++)
			{
				int level = packs[j].level + i;

				// 各种连续牌型的主牌不能到2，非连续牌型的主牌不能到小王，单张的主牌不能超过大王
				if ((comboType == CardComboType::SINGLE && level > MAX_LEVEL) ||
					(isSequential && level > MAX_STRAIGHT_LEVEL) ||
					(comboType != CardComboType::SINGLE && !isSequential && level >= level_joker))
					goto failure;

				// 如果手牌中这种牌不够，就不用继续增了
				if (counts[level] < packs[j].count)
					goto next;
			}

			{
				// 找到了合适的主牌，那么从牌呢？
				// 如果手牌的种类数不够，那从牌的种类数就不够，也不行
				if (kindCount < packs.size())
					continue;

				// 好终于可以了
				// 计算每种牌的要求数目吧
				short requiredCounts[MAX_LEVEL + 1] = {};
				for (int j = 0; j < mainPackCount; j++)
					requiredCounts[packs[j].level + i] = packs[j].count;
				for (unsigned j = mainPackCount; j < packs.size(); j++)
				{
					Level k;
					for (k = 0; k <= MAX_LEVEL; k++)
					{
						if (requiredCounts[k] || counts[k] < packs[j].count)
							continue;
						requiredCounts[k] = packs[j].count;
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
				return CardCombo(solve.begin(), solve.end());
			}

		next:; // 再增大
		}
	}

failure:
	// 实在找不到啊
	// 最后看一下能不能炸吧

	for (Level i = 0; i < level_joker; i++)
		if (counts[i] == 4 && (comboType != CardComboType::BOMB || i > packs[0].level)) // 如果对方是炸弹，能炸的过才行
		{
			// 还真可以啊……
			Card bomb[] = {Card(i * 4), Card(i * 4 + 1), Card(i * 4 + 2), Card(i * 4 + 3)};
			return CardCombo(bomb, bomb + 4);
		}

	// 有没有火箭？
	if (counts[level_joker] + counts[level_JOKER] == 2)
	{
		Card rocket[] = {card_joker, card_JOKER};
		return CardCombo(rocket, rocket + 2);
	}

	// ……
	return CardCombo();
}

void CardCombo::debugPrint()
{
#ifndef _BOTZONE_ONLINE
	std::cout << "【" << cardComboStrings[(int)comboType] << "共" << cards.size() << "张，大小序" << comboLevel << "】";
#endif
}

/* 状态 */

// ljh.cpp
#include <bits/stdc++.h>
// #include "card.h"
// #include "main.h"

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
				if (addCandidate({0, 1, 2, 3, 4, 5}, 0))
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

bool term_flag = 0;

int search() {
	static int cnt = 0;
	int num = 4;
	if (myCards.size() > 3)
		num = 2;
	if (myCards.size() > 6)
		num = 1;
	auto candidates = getCandidatesEval(num);
	int ans = -INF;
	for (auto &&cs : candidates) {
		ans = max(ans, procSearch(cs.second));
		if (term_flag || (((++cnt) & 1024) == 0 && clock() > 0.9 * CLOCKS_PER_SEC)) {
#ifdef _LOG
			if (!term_flag)
				cerr << "TERM" << endl;
#endif
			term_flag = 1;
			return ans;
		}
		if (ans > 0)
			break;
	}
	return ans;
}

CardCombo getAction() {
	static const int DIST_NUM = 20, CAND_NUM = 10;
	auto dists = randCards(DIST_NUM);
	auto candidates = getCandidatesEval(CAND_NUM);
#ifdef _LOG
	for (auto &&d : dists) {
		cerr << d.second << endl;
	}
#endif
	for (auto &c : candidates) {
#ifdef _LOG
		cerr << c.first << endl;
		for (Card v : c.second.cards)
			cerr << v << ' ';
		cerr << endl;
#endif
		c.first = 0;
		for (auto &&d : dists) {
			dist = d.first;
			myCards = dist[myPosition];
			c.first += d.second * procSearch(c.second);
		}
#ifdef _LOG
		cerr << c.first << endl;
		cerr << "----------" << endl;
#endif
	}
	return max_element(candidates.begin(), candidates.end(), le_first)->second;
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

// cwy.cpp
#include <bits/stdc++.h>
#define pb push_back
#define mp make_pair
#define fi first
#define se second
#define SZ(x) ((int)x.size())
#define FOR(i,a,b) for (int i=a;i<=b;++i)
#define FORD(i,a,b) for (int i=a;i>=b;--i)
using namespace std;
typedef long long LL;
typedef pair<int,int> pa;
typedef vector<int> vec;
typedef int VL;
// #include "card.h"
// #include "main.h"


const double tradeoff = 0.8;
const VL VLRNG = 1000000000;
VL mxvl;
int ts;
int tmp[50];
int ts1;
int tmp1[50];
int ts2;
int tmp2[50];
int one[50];
int two[50];
int pai[50]={0};

inline VL PASSVL(){
	return (lastValidComboPosition!=landlordPosition&&
	myPosition!=landlordPosition)?0:-8;
}
inline VL SINGLEVL(int x){
	if (x<13) return x/2-3;
	else if (x==13) return 4;
	else return 5;
}
inline VL PAIRVL(int x){
	if (x<13) return x/2-1;
	else assert(0);
}
inline VL STRAIGHTVL(int l, int len){
	return len+l-4;
}
inline VL STRAIGHT2VL(int l,int len){
	return len*2+l-4;
}
inline VL TRIPLETVL(int x){
	return (x/2+1);
}
inline VL TRIPLET1VL(int x){
	return TRIPLETVL(x);
}
inline VL TRIPLET2VL(int x){
	return TRIPLETVL(x);
}
inline VL BOMBVL(int x){
	return (8+x/3);
}
inline VL QUADRUPLE2VL(int x){
	return 1;
}
inline VL QUADRUPLE4VL(int x){
	return 1;
}
inline VL PLANEVL(int x){
	return (x/6+4);
}
inline VL PLANE1VL(int x){
	return 5;
}
inline VL PLANE2VL(int x){
	return 6;
}
inline VL SSHUTTLEVL(int x){
	return (x/6+5);
}
inline VL SSHUTTLE2VL(int x){
	return 6;
}
inline VL SSHUTTLE4VL(int x){
	return 6;
}
inline VL ROCKETVL(){
	return 13;
}
inline VL INVALIDVL(){
	return -100;
}


void suan(VL vl){
	VL t = vl;
	int twos = 0;
	int ones = 0;
	int thres = 0;
	FOR(i,0,12)
		if (pai[i]){
			if (pai[i]==1) t+=(one[++ones]=SINGLEVL(i));
			else if (pai[i]==2) t+=(two[++twos]=PAIRVL(i));
			else if (pai[i]==3) t+=TRIPLETVL(i),++thres;
			else if (pai[i]==4) t+=BOMBVL(i);
		}
	if (thres){
		int p1 = 1;
		int p2 = 1;
		one[ones+1]=0;
		two[twos+1]=0;
		while (thres--){
			if (one[p1]<0 || two[p2]<0){
				if (one[p1]<two[p2]) t -= one[p1], ++p1;
				else t -= two[p2], ++p2;
			}
			else break;
		}
	}
	if (pai[13] && pai[14]) t += ROCKETVL();
	else if (pai[13]) t += SINGLEVL(13);
	else if (pai[14]) t += SINGLEVL(14);
	mxvl=max(mxvl,t);
}
void shun2(VL vl, bool o){
	ts2=0;
	FOR(i,0,11) if (pai[i]){
		tmp2[++ts2]=i;
	}
	if (o){
		FOR(i,1,ts2-4){
			int r=i-1;
			FOR(j,i+4,ts2)
				if (tmp[j]-tmp[i]==j-i){
					while (1){
						--pai[++r];
						if (r==j) break;
					}
					suan(vl+STRAIGHTVL(tmp[i],j-i+1));
				}
				else break;
			FOR(t,i,r) ++pai[t];
		}
	}
	suan(vl);
}
void shun1(VL vl){
	ts1=0;
	FOR(i,0,11) if (pai[i]){
		tmp1[++ts1]=i;
	}
	FOR(i,1,ts1-4){
		int r=i-1;
		FOR(j,i+4,ts1)
			if (tmp[j]-tmp[i]==j-i){
				while (1){
					--pai[++r];
					if (r==j) break;
				}
				shun2(vl+STRAIGHTVL(tmp[i],j-i+1),1);
			}
			else break;
		FOR(t,i,r) ++pai[t];
	}
	shun2(vl,0);
}

void lian(){
	ts=0;
	FOR(i,0,11) if (pai[i]>=2){
		tmp[++ts]=i;
	}
	FOR(i,1,ts-2){
		int r=i-1;
		FOR(j,i+2,ts)
			if (tmp[j]-tmp[i]==j-i){
				while (1){
					pai[++r] -= 2;
					if (r==j) break;
				}
				shun1(STRAIGHT2VL(tmp[i],j-i+1));
			}
			else break;
		FOR(t,i,r) pai[t] += 2;
	}

	shun1(0);
}

double evalCards(const CardSet & PAI){
	FOR(i,0,14) pai[i]=0;
	for (auto x : PAI){
		++pai[card2level(x)];
	}
	mxvl = -VLRNG;
	lian();
	return 1.0*mxvl;


}

double eval(const CardCombo & PAI){
	FOR(i,0,14) pai[i]=0;
	
	for (auto x :myCards) ++pai[card2level(x)];
	for (auto x :PAI.cards) --pai[card2level(x)];
	VL CHUVL(0);
	if (PAI.comboType == CardComboType::PASS) CHUVL += PASSVL();
	else if (PAI.comboType == CardComboType::SINGLE) CHUVL += SINGLEVL(PAI.packs[0].level);
	else if (PAI.comboType == CardComboType::PAIR) CHUVL += PAIRVL(PAI.packs[0].level);
	else if (PAI.comboType == CardComboType::STRAIGHT) CHUVL += STRAIGHTVL(PAI.packs.back().level, SZ(PAI.packs));
	else if (PAI.comboType == CardComboType::STRAIGHT2) CHUVL += STRAIGHT2VL(PAI.packs.back().level, SZ(PAI.packs));
	else if (PAI.comboType == CardComboType::TRIPLET) CHUVL += TRIPLETVL(PAI.packs[0].level);
	else if (PAI.comboType == CardComboType::TRIPLET1) CHUVL += TRIPLET1VL(PAI.packs[0].level);
	else if (PAI.comboType == CardComboType::TRIPLET2) CHUVL += TRIPLET2VL(PAI.packs[0].level);
	else if (PAI.comboType == CardComboType::BOMB) CHUVL += BOMBVL(PAI.packs[0].level);
	else if (PAI.comboType == CardComboType::QUADRUPLE2) CHUVL += QUADRUPLE2VL(PAI.packs[0].level);
	else if (PAI.comboType == CardComboType::QUADRUPLE4) CHUVL += QUADRUPLE4VL(PAI.packs[0].level);
	else if (PAI.comboType == CardComboType::PLANE) CHUVL += PLANEVL(PAI.packs[0].level);
	else if (PAI.comboType == CardComboType::PLANE1) CHUVL += PLANE1VL(PAI.packs[0].level);
	else if (PAI.comboType == CardComboType::PLANE2) CHUVL += PLANE2VL(PAI.packs[0].level);
	else if (PAI.comboType == CardComboType::SSHUTTLE) CHUVL += SSHUTTLEVL(PAI.packs[0].level);
	else if (PAI.comboType == CardComboType::SSHUTTLE2) CHUVL += SSHUTTLE2VL(PAI.packs[0].level);
	else if (PAI.comboType == CardComboType::SSHUTTLE4) CHUVL += SSHUTTLE4VL(PAI.packs[0].level);
	else if (PAI.comboType == CardComboType::ROCKET) CHUVL += ROCKETVL();
	else if (PAI.comboType == CardComboType::INVALID) CHUVL += INVALIDVL();
	mxvl = -VLRNG;
	lian();
	return mxvl+tradeoff*CHUVL;
	
}

// wzp.cpp
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
// #include "card.h"
// #include "main.h"
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

const ld sigma=1;
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

// main.cpp
// #include "main.h"
// #include "card.h"
#include "jsoncpp/json.h"

using namespace std;

namespace BotzoneIO
{
	using namespace std;
	void read()
	{
		// 读入输入（平台上的输入是单行）
		string line;
		getline(cin, line);
		Json::Value input;
		Json::Reader reader;
		reader.parse(line, input);

		// 首先处理第一回合，得知自己是谁、有哪些牌
		{
			auto firstRequest = input["requests"][0u]; // 下标需要是 unsigned，可以通过在数字后面加u来做到
			auto own = firstRequest["own"];
			for (unsigned i = 0; i < own.size(); i++)
				myCards.insert(own[i].asInt());
			if (!firstRequest["bid"].isNull())
			{
				// 如果还可以叫分，则记录叫分
				auto bidHistory = firstRequest["bid"];
				myPosition = bidHistory.size();
				for (unsigned i = 0; i < bidHistory.size(); i++)
					bidInput.push_back(bidHistory[i].asInt());
			}
		}

		// history里第一项（上上家）和第二项（上家）分别是谁的决策

		int turn = input["requests"].size();
		for (int i = 0; i < turn; i++)
		{
			auto request = input["requests"][i];
			auto llpublic = request["publiccard"];
			if (!llpublic.isNull())
			{
				// 第一次得知公共牌、地主叫分和地主是谁
				landlordPosition = request["landlord"].asInt();
				landlordBid = request["finalbid"].asInt();
				myPosition = request["pos"].asInt();
				cardRemaining[landlordPosition] += llpublic.size();
				for (unsigned i = 0; i < llpublic.size(); i++)
				{
					landlordPublicCards.insert(llpublic[i].asInt());
					if (landlordPosition == myPosition)
						myCards.insert(llpublic[i].asInt());
				}
			}

			auto history = request["history"]; // 每个历史中有上家和上上家出的牌
			if (history.isNull())
				continue;
			stage = Stage::PLAYING;
		
			int whoInHistory[] = {(myPosition - 2 + PLAYER_COUNT) % PLAYER_COUNT, (myPosition - 1 + PLAYER_COUNT) % PLAYER_COUNT};

			// 逐次恢复局面到当前
			int howManyPass = 0;
			for (int p = 0; p < 2; p++)
			{
				int player = whoInHistory[p];	// 是谁出的牌
				auto playerAction = history[p]; // 出的哪些牌
				vector<Card> playedCards;
				for (unsigned _ = 0; _ < playerAction.size(); _++) // 循环枚举这个人出的所有牌
				{
					int card = playerAction[_].asInt(); // 这里是出的一张牌
					playedCards.push_back(card);
				}
				whatTheyPlayed[player].push_back(playedCards); // 记录这段历史
				cardRemaining[player] -= playerAction.size();

				if (playerAction.size() == 0)
					howManyPass++;
				else {
					lastValidCombo = CardCombo(playedCards.begin(), playedCards.end());
					lastValidComboPosition = player;
				}
			}

			if (howManyPass == 2) {
				lastValidCombo = CardCombo();
				lastValidComboPosition = -1;
			}

			if (i < turn - 1)
			{
				// 还要恢复自己曾经出过的牌
				auto playerAction = input["responses"][i]; // 出的哪些牌
				vector<Card> playedCards;
				for (unsigned _ = 0; _ < playerAction.size(); _++) // 循环枚举自己出的所有牌
				{
					int card = playerAction[_].asInt(); // 这里是自己出的一张牌
					myCards.erase(card);				// 从自己手牌中删掉
					playedCards.push_back(card);
				}
				whatTheyPlayed[myPosition].push_back(playedCards); // 记录这段历史
				cardRemaining[myPosition] -= playerAction.size();
			}
		}
	}

	/**
	* 输出叫分（0, 1, 2, 3 四种之一）
	*/
	void bid(int value)
	{
		Json::Value result;
		result["response"] = value;

		Json::FastWriter writer;
		cout << writer.write(result) << endl;
	}

	/**
	* 输出打牌决策，begin是迭代器起点，end是迭代器终点
	* CARD_ITERATOR是Card（即short）类型的迭代器
	*/
	template <typename CARD_ITERATOR>
	void play(CARD_ITERATOR begin, CARD_ITERATOR end)
	{
		Json::Value result, response(Json::arrayValue);
		for (; begin != end; begin++)
			response.append(*begin);
		result["response"] = response;

		Json::FastWriter writer;
		cout << writer.write(result) << endl;
	}
}

int main()
{
	// srand(time(nullptr));
	BotzoneIO::read();

	if (stage == Stage::BIDDING)
	{
		// 做出决策（你只需修改以下部分）

		auto maxBidIt = std::max_element(bidInput.begin(), bidInput.end());
		int maxBid = maxBidIt == bidInput.end() ? -1 : *maxBidIt;

		int bidValue = getBidValue(maxBid); // rand() % (3 - maxBid) + maxBid + 1;

		// 决策结束，输出结果（你只需修改以上部分）

		BotzoneIO::bid(bidValue);
	}
	else if (stage == Stage::PLAYING)
	{
		// 做出决策（你只需修改以下部分）

		// findFirstValid 函数可以用作修改的起点
		CardCombo myAction = getAction();
		// lastValidCombo.findFirstValid(myCards.begin(), myCards.end());

		// 是合法牌
		assert(myAction.comboType != CardComboType::INVALID);

		assert(
			// 在上家没过牌的时候过牌
			(lastValidCombo.comboType != CardComboType::PASS && myAction.comboType == CardComboType::PASS) ||
			// 在上家没过牌的时候出打得过的牌
			(lastValidCombo.comboType != CardComboType::PASS && lastValidCombo.canBeBeatenBy(myAction)) ||
			// 在上家过牌的时候出合法牌
			(lastValidCombo.comboType == CardComboType::PASS && myAction.comboType != CardComboType::INVALID));

		// 决策结束，输出结果（你只需修改以上部分）

		BotzoneIO::play(myAction.cards.begin(), myAction.cards.end());
	}
}

