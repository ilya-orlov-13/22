#pragma once
#ifndef HEADER_H
#define HEADER_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stack>
#include <stdexcept>
#include <cmath>
#include <map>
#include <iomanip>

using namespace std;

// Структура узла для CalcTree1
struct NodeCT1 {
    int Data; //Может быть число (0-9) или код операции (-1, -2, -3)
    NodeCT1* Left;
    NodeCT1* Right;

    NodeCT1(int Val) : Data(Val), Left(nullptr), Right(nullptr) {}
};
typedef NodeCT1* PNodeCT1;

// Структура узла для CalcTree23
// Может хранить число, переменную 'x', или код операции
enum NodeTypeCT23 { NUMBER, VARIABLE_X, OPERATION };
struct NodeCT23 {
    NodeTypeCT23 Type;
    int Value;      // Для чисел или кодов операций
    char Op;        // Символ операции (для удобства вывода)
    //char VarName; // Если бы переменных было много, здесь 'x'
    NodeCT23* Left;
    NodeCT23* Right;

    // Конструктор для чисел
    NodeCT23(int Val) : Type(NUMBER), Value(Val), Op('\0'), Left(nullptr), Right(nullptr) {}
    // Конструктор для переменной 'x'
    NodeCT23() : Type(VARIABLE_X), Value(0), Op('\0'), Left(nullptr), Right(nullptr) {} // Value для 'x' не используется при построении
    // Конструктор для операций
    NodeCT23(char OperationChar, int OperationCode) : Type(OPERATION), Value(OperationCode), Op(OperationChar), Left(nullptr), Right(nullptr) {}
};
typedef NodeCT23* PNodeCT23;


// Объявления функций для CalcTree1
PNodeCT1 BuildTreeFromPostfix(const string& PostfixExpression);
int EvaluateSubtreeCT1(PNodeCT1 Node);
void TransformTreeCT1(PNodeCT1& Node); // Передаем по ссылке, т.к. узел может быть заменен
void DeleteTreeCT1(PNodeCT1 Node);
// Для отладки
void PrintTreeCT1Prefix(PNodeCT1 Node, ostream& Out);


// Объявления функций для CalcTree23
PNodeCT23 BuildTreeFromInfix(const string& InfixExpression);
int EvaluateTreeCT23(PNodeCT23 Node, int XValue);
void TransformTreeCT23(PNodeCT23& Node);
void PrintTreeCT23Prefix(PNodeCT23 Node, ostream& Out);
void PrintTreeCT23Postfix(PNodeCT23 Node, ostream& Out);
void PrintTreeCT23InfixExcessiveBrackets(PNodeCT23 Node, ostream& Out);
void DeleteTreeCT23(PNodeCT23 Node);
// Коды операций для CalcTree23
const int OP_ADD_CT23 = -1;
const int OP_SUB_CT23 = -2;
const int OP_MUL_CT23 = -3;
const int OP_DIV_CT23 = -4;
const int OP_MOD_CT23 = -5;
const int OP_POW_CT23 = -6;


#endif // HEADER_H
