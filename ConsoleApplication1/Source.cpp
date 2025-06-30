#include "header.h"

// CalcTree1

PNodeCT1 BuildTreeFromPostfix(const string& PostfixExpression) {
    stack<PNodeCT1> s;
    for (char token : PostfixExpression) {
        if (isdigit(token)) {
            s.push(new NodeCT1(token - '0')); // Преобразуем символ цифры в число
        }
        else {
            if (s.size() < 2) {
                // Очищаем стек, чтобы избежать утечек
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

int EvaluateSubtreeCT1(PNodeCT1 Node) {
    if (Node == nullptr) {
        return 0;
    }
    if (Node->Data >= 0) { // Операнд (число)
        return Node->Data;
    }
    // Операция
    int LeftVal = EvaluateSubtreeCT1(Node->Left);
    int RightVal = EvaluateSubtreeCT1(Node->Right);

    if (Node->Data == -1) return LeftVal + RightVal; // Сложение
    if (Node->Data == -2) return LeftVal - RightVal; // Вычитание
    if (Node->Data == -3) return LeftVal * RightVal; // Умножение

}

void TransformTreeCT1(PNodeCT1& Node) {
    if (Node == nullptr) {
        return;
    }

    // Сначала рекурсивно трансформируем потомков
    TransformTreeCT1(Node->Left);
    TransformTreeCT1(Node->Right);

    // Проверяем текущий узел
    if (Node->Data == -2) { // Если это операция вычитания
        int SubtreeValue = EvaluateSubtreeCT1(Node); // Вычисляем значение поддерева
        DeleteTreeCT1(Node->Left);    // Удаляем старых потомков
        DeleteTreeCT1(Node->Right);
        Node->Left = nullptr;
        Node->Right = nullptr;
        Node->Data = SubtreeValue;    // Заменяем узел на его значение
    }
    // Также нужно проверить, не стали ли потомки операциями вычитания, которые
    // были преобразованы. Это уже сделано рекурсивными вызовами выше.
    // Но если сама операция вычитания была в корне поддерева, которое нужно заменить,
    // то после рекурсивного вызова для Node->Left и Node->Right,
    // Node все еще будет узлом операции.
    // Поэтому проверка текущего узла Node->Data == -2 должна быть после рекурсивных вызовов.
}


void DeleteTreeCT1(PNodeCT1 Node) {
    if (Node == nullptr) {
        return;
    }
    DeleteTreeCT1(Node->Left);
    DeleteTreeCT1(Node->Right);
    delete Node;
}

void PrintTreeCT1Prefix(PNodeCT1 Node, ostream& Out) {
    if (Node == nullptr) {
        return;
    }
    Out << Node->Data << " ";
    PrintTreeCT1Prefix(Node->Left, Out);
    PrintTreeCT1Prefix(Node->Right, Out);
}


// CalcTree23

// Вспомогательная функция для определения приоритета операции
int GetOperationPriorityCT23(char Op) {
    if (Op == '+' || Op == '-') return 1;
    if (Op == '*' || Op == '/' || Op == '%') return 2;
    if (Op == '^') return 3; 
    return 0; // Для скобок или если не операция
}

// Парсер инфиксных выражений
PNodeCT23 BuildTreeFromInfix(const string& s) {
    // Удаляем пробелы
    string expr;
    for (char c : s) if (!isspace(c)) expr += c;
    if (expr.empty()) throw runtime_error("Пустое выражение");

    // Вспомогательная функция поиска парной открывающей скобки
    auto searchBracket = [](const string& s, int k) {
        int counter = 1;
        while (counter != 0) {
            k--;
            if (s[k] == ')') counter++;
            if (s[k] == '(') counter--;
        }
        return k;
        };
    // Вспомогательная функция поиска операции вне скобок с заданным приоритетом
    auto SearchOperation = [&](const string& s, int priority) {
        int k = (int)s.length() - 1;
        while (k >= 0) {
            if (s[k] == ')') k = searchBracket(s, k);
            else if (GetOperationPriorityCT23(s[k]) == priority) return k;
            else k--;
        }
        return -1;
        };
    // Если выражение в скобках целиком
    if (expr.front() == '(' && expr.back() == ')' && searchBracket(expr, (int)expr.length() - 1) == 0) {
        return BuildTreeFromInfix(expr.substr(1, expr.length() - 2));
    }
    // Поиск операций по приоритету (слева направо)
    for (int pr = 1; pr <= 3; ++pr) {
        int k = SearchOperation(expr, pr);
        if (k != -1) {
            char op = expr[k];
            int opCode = 0;
            switch (op) {
            case '+': opCode = OP_ADD_CT23; break;
            case '-': opCode = OP_SUB_CT23; break;
            case '*': opCode = OP_MUL_CT23; break;
            case '/': opCode = OP_DIV_CT23; break;
            case '%': opCode = OP_MOD_CT23; break;
            case '^': opCode = OP_POW_CT23; break;
            default: throw runtime_error("Неизвестная операция в инфиксном выражении");
            }
            PNodeCT23 node = new NodeCT23(op, opCode);
            node->Left = BuildTreeFromInfix(expr.substr(0, k));
            node->Right = BuildTreeFromInfix(expr.substr(k + 1));
            return node;
        }
    }
    // Если это переменная x
    if (expr == "x") return new NodeCT23();
    // Если это число
    try {
        int val = stoi(expr);
        if (val < 1 || val > 30) throw runtime_error("Число вне диапазона [1;30]");
        return new NodeCT23(val);
    }
    catch (...) {
        throw runtime_error("Ошибка разбора числа или переменной в инфиксном выражении: '" + expr + "'");
    }
}

int EvaluateTreeCT23(PNodeCT23 Node, int XValue) {
    if (Node == nullptr) {
        throw runtime_error("Cannot evaluate NULL subtree in CT23.");
    }
    switch (Node->Type) {
    case NUMBER:
        return Node->Value;
    case VARIABLE_X:
        return XValue;
    case OPERATION: {
        int LeftVal = EvaluateTreeCT23(Node->Left, XValue);
        int RightVal = EvaluateTreeCT23(Node->Right, XValue);
        switch (Node->Value) { // Используем код операции
        case OP_ADD_CT23: return LeftVal + RightVal;
        case OP_SUB_CT23: return LeftVal - RightVal;
        case OP_MUL_CT23: return LeftVal * RightVal;
        case OP_DIV_CT23:
            if (RightVal == 0) throw runtime_error("Division by zero.");
            return LeftVal / RightVal; // Целочисленное деление
        case OP_MOD_CT23:
            if (RightVal == 0) throw runtime_error("Modulo by zero.");
            return LeftVal % RightVal;
        case OP_POW_CT23:
            if (RightVal < 0) throw runtime_error("Negative exponent in power operation.");
            return static_cast<int>(pow(LeftVal, RightVal));
        default:
            throw runtime_error("Unknown operation code in EvaluateTreeCT23.");
        }
    }
    default:
        throw runtime_error("Unknown node type in EvaluateTreeCT23.");
    }
}

void TransformTreeCT23(PNodeCT23& Node) {
    if (Node == nullptr) {
        return;
    }

    TransformTreeCT23(Node->Left);
    TransformTreeCT23(Node->Right);

    // Проверяем шаблон A + x
    if (Node->Type == OPERATION && Node->Op == '+' &&
        Node->Right != nullptr && Node->Right->Type == VARIABLE_X) {
        // A - это Node->Left, x - это Node->Right
        // Меняем их местами: x + A
        PNodeCT23 TempA = Node->Left;
        PNodeCT23 TempX = Node->Right;
        Node->Left = TempX;
        Node->Right = TempA;
        // Символ операции остается '+', код операции тоже.
    }
}

void PrintTreeCT23Prefix(PNodeCT23 Node, ostream& Out) {
    if (Node == nullptr) {
        return;
    }
    if (Node->Type == NUMBER) Out << Node->Value << " ";
    else if (Node->Type == VARIABLE_X) Out << "x" << " ";
    else if (Node->Type == OPERATION) Out << Node->Op << " ";

    PrintTreeCT23Prefix(Node->Left, Out);
    PrintTreeCT23Prefix(Node->Right, Out);
}

void PrintTreeCT23Postfix(PNodeCT23 Node, ostream& Out) {
    if (Node == nullptr) {
        return;
    }
    PrintTreeCT23Postfix(Node->Left, Out);
    PrintTreeCT23Postfix(Node->Right, Out);

    if (Node->Type == NUMBER) Out << Node->Value << " ";
    else if (Node->Type == VARIABLE_X) Out << "x" << " ";
    else if (Node->Type == OPERATION) Out << Node->Op << " ";
}

void PrintTreeCT23InfixExcessiveBrackets(PNodeCT23 Node, ostream& Out) {
    if (Node == nullptr) {
        return;
    }
    if (Node->Type == NUMBER) {
        Out << Node->Value;
    }
    else if (Node->Type == VARIABLE_X) {
        Out << "x";
    }
    else if (Node->Type == OPERATION) {
        Out << "(";
        PrintTreeCT23InfixExcessiveBrackets(Node->Left, Out);
        Out << Node->Op; // Печатаем оператор между операндами
        PrintTreeCT23InfixExcessiveBrackets(Node->Right, Out);
        Out << ")";
    }
}

void DeleteTreeCT23(PNodeCT23 Node) {
    if (Node == nullptr) {
        return;
    }
    DeleteTreeCT23(Node->Left);
    DeleteTreeCT23(Node->Right);
    delete Node;
}
