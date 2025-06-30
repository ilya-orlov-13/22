#include "header.h"

// Построение дерева из обратной польской записи
PNodeCT1 BuildTreeFromPostfix(const string& PostfixExpression) {
    stack<PNodeCT1> s;
    for (char token : PostfixExpression) {
        if (isdigit(token)) {
            s.push(new NodeCT1(token - '0'));
        }
        else {
            if (s.size() < 2) {
                while (!s.empty()) { DeleteTreeCT1(s.top()); s.pop(); }
                throw runtime_error("Invalid postfix expression: not enough operands for operator.");
            }
            PNodeCT1 RightOperand = s.top(); s.pop();
            PNodeCT1 LeftOperand = s.top(); s.pop();
            PNodeCT1 OpNode = nullptr;
            if (token == '+') OpNode = new NodeCT1(-1);
            else if (token == '-') OpNode = new NodeCT1(-2);
            else if (token == '*') OpNode = new NodeCT1(-3);
            else {
                DeleteTreeCT1(LeftOperand);
                DeleteTreeCT1(RightOperand);
                while (!s.empty()) { DeleteTreeCT1(s.top()); s.pop(); }
                throw runtime_error("Invalid postfix expression: unknown operator.");
            }
            OpNode->Left = LeftOperand;
            OpNode->Right = RightOperand;
            s.push(OpNode);
        }
    }
    if (s.size() != 1) {
        while (!s.empty()) { DeleteTreeCT1(s.top()); s.pop(); }
        throw runtime_error("Invalid postfix expression: stack has incorrect number of items at the end.");
    }
    return s.top();
}

// Вычисление значения поддерева
int EvaluateSubtreeCT1(PNodeCT1 Node) {
    if (Node == nullptr) return 0;
    if (Node->Data >= 0) return Node->Data;
    int LeftVal = EvaluateSubtreeCT1(Node->Left);
    int RightVal = EvaluateSubtreeCT1(Node->Right);
    if (Node->Data == -1) return LeftVal + RightVal;
    if (Node->Data == -2) return LeftVal - RightVal;
    if (Node->Data == -3) return LeftVal * RightVal;
    throw runtime_error("Unknown operation code in EvaluateSubtreeCT1.");
}

// Замена всех вычитаний их значением
void TransformTreeCT1(PNodeCT1& Node) {
    if (Node == nullptr) return;
    TransformTreeCT1(Node->Left);
    TransformTreeCT1(Node->Right);
    if (Node->Data == -2) {
        int SubtreeValue = EvaluateSubtreeCT1(Node);
        DeleteTreeCT1(Node->Left);
        DeleteTreeCT1(Node->Right);
        Node->Left = nullptr;
        Node->Right = nullptr;
        Node->Data = SubtreeValue;
    }
}

// Удаление дерева
void DeleteTreeCT1(PNodeCT1 Node) {
    if (Node == nullptr) return;
    DeleteTreeCT1(Node->Left);
    DeleteTreeCT1(Node->Right);
    delete Node;
}

// Префиксный обход
void PrintTreeCT1Prefix(PNodeCT1 Node, ostream& Out) {
    if (Node == nullptr) return;
    Out << Node->Data << " ";
    PrintTreeCT1Prefix(Node->Left, Out);
    PrintTreeCT1Prefix(Node->Right, Out);
}

// Вывод дерева боком
void PrintSideways(PNodeCT1 root, int space) {
    if (root == nullptr) {
        return;
    }

    space += 5;

    PrintSideways(root->Right, space);

    cout << endl;
    for (int i = 5; i < space; i++) {
        cout << " ";
    }
    if (root->Data == -1) cout << "+"<< endl;
    else if (root->Data == -2) cout << "-"<< endl;
    else if (root->Data == -3) cout << "*"<< endl;
    else cout << root->Data << endl;

    PrintSideways(root->Left, space);
}