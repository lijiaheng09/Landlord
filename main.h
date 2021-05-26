#ifndef MAIN_H
#define MAIN_H

#include <bits/stdc++.h>
#include "card.h"

int getBidValue(int maxBid); // 叫分
CardCombo getAction(); // 出牌, 搜索记得维护全局变量
// double getMean(); // 根据当前牌得到最优策略得分期望

int search(double TL); // 最优得分
std::vector<CardCombo> getCandidates();
std::vector<std::pair<CardDistrib, double>> randCards(int num);
std::vector<std::pair<double, CardCombo>> getCandidatesEval(int num);
// double evalCards(const CardSet &); // 期望得分
double eval(const CardCombo &); // 期望得分
// double evalProb(const CardCombo &);

CardSet cardSub(const CardSet &, const CardCombo &);
CardSet cardAdd(const CardSet &, const CardCombo &);

void doCombo(const CardCombo &);
void undoCombo();

#endif
