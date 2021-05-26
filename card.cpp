// 斗地主·改（FightTheLandlord2）样例程序
// 无脑策略
// 最后修改：2021-05-08，去除了有问题的isArray。
// 作者：zhouhy
// 游戏信息：http://www.botzone.org/games#FightTheLandlord2

#include <bits/stdc++.h>
#include "card.h"

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

void CardCombo::debugPrint() const
{
#ifndef _BOTZONE_ONLINE
	std::cerr << "【" << cardComboStrings[(int)comboType] << "共" << cards.size() << "张，大小序" << comboLevel << "】";
#endif
}

/* 状态 */
