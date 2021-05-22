#ifndef MAIN_H
#define MAIN_H

#include <bits/stdc++.h>
#include "card.h"

int getBidValue(int maxBid); // 叫分
CardCombo getAction(); // 出牌, 搜索记得维护全局变量

bool search();
std::vector<std::pair<CardDistrib, double>> randCards(int num);
double evalCards(const CardSet &);
double eval(const CardCombo &);

#endif
