#ifndef MAIN_H
#define MAIN_H

#include <bits/stdc++.h>
#include "card.h"

int getBidValue(int maxBid); // 叫分
CardCombo getAction(); // 出牌, 搜索记得维护全局变量

int search(int score = 1); // 最优得分
std::vector<CardCombo> candidates();
std::vector<std::pair<CardDistrib, double>> randCards(int num);
double evalCards(const CardSet &); // 期望得分
double eval(const CardCombo &); // 期望得分

#endif
