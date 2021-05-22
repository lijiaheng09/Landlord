#ifndef CARD_HPP
#define CARD_HPP

#include "card.h"

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
