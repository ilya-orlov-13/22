#pragma once
#ifndef HEADER_H
#define HEADER_H

#include <iostream>
#include <fstream>
#include <string>
#include <stack>
#include <stdexcept>
using namespace std;

// Структура узла для CalcTree1
struct NodeCT1 {
    int Data; // Число (0-9) или код операции (-1, -2, -3)
    NodeCT1* Left;
    NodeCT1* Right;
    NodeCT1(int Val) : Data(Val), Left(nullptr), Right(nullptr) {}
};
typedef NodeCT1* PNodeCT1;

// Объявления функций для CalcTree1
PNodeCT1 BuildTreeFromPostfix(const string& PostfixExpression);
int EvaluateSubtreeCT1(PNodeCT1 Node);
void TransformTreeCT1(PNodeCT1& Node);
void DeleteTreeCT1(PNodeCT1 Node);
void PrintTreeCT1Prefix(PNodeCT1 Node, ostream& Out);
void PrintSideways(PNodeCT1 root, int space);

#endif // HEADER_H
