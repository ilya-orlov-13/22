#include "header.h"

// Функция для построения дерева из инфиксного выражения
PNodeCT23 BuildTreeFromInfix(const string& infixExpr);

// Функция для вычисления значения дерева при заданном x
int EvaluateTreeCT23(PNodeCT23 tree, int x);

// Функция для трансформации дерева
void TransformTreeCT23(PNodeCT23& tree);



int main() {
    setlocale(LC_ALL, "Russian");

    // CalcTree1
    cout << "CalcTree1" << endl;
    string PostfixExpr1 = "12+3*45+-"; // ((1+2)*3) + (4-5) = 3*3 + (-1) = 9 - 1 = 8
    PNodeCT1 Tree1 = nullptr;
    try {
        Tree1 = BuildTreeFromPostfix(PostfixExpr1);
        cout << "Дерево для '" << PostfixExpr1 << "' построено." << endl;

        cout << "Префиксный обход до трансформации: ";
        PrintTreeCT1Prefix(Tree1, cout);
        cout << endl;

        cout << "Значение поддерева с корнем (2+1)*3: " << EvaluateSubtreeCT1(Tree1->Left) << endl; // (1+2)*3=9
        cout << "Значение поддерева с корнем 4-5: " << EvaluateSubtreeCT1(Tree1->Right) << endl; // 4-5 = -1
        cout << "Значение всего выражения до трансформации: " << EvaluateSubtreeCT1(Tree1) << endl;

        TransformTreeCT1(Tree1);
        cout << "Дерево трансформировано (удалено вычитание)." << endl;

        cout << "Префиксный обход после трансформации: ";
        PrintTreeCT1Prefix(Tree1, cout);
        cout << endl;
        cout << "Значение всего выражения после трансформации (должно быть то же): " << EvaluateSubtreeCT1(Tree1) << endl;

    }
    catch (const runtime_error& e) {
        cerr << "Ошибка в CalcTree1: " << e.what() << endl;
    }
    DeleteTreeCT1(Tree1);


    // CalcTree23
    cout << "CalcTree23" << endl;
    string Fn1_ct23 = "input_ct23.txt";
    string Fn2_ct23 = "output_ct23.txt";
    ofstream OutFileCT23(Fn2_ct23);

    if (!OutFileCT23) {
        cerr << "Не удалось открыть выходной файл " << Fn2_ct23 << endl;
        return 1;
    }

    // Чтение инфиксного выражения из файла
    string infixExpr;
    ifstream InFileCT23(Fn1_ct23);
    if (!InFileCT23) {
        cerr << "Не удалось открыть входной файл " << Fn1_ct23 << endl;
        OutFileCT23 << "Не удалось открыть входной файл." << endl;
        return 1;
    }
    getline(InFileCT23, infixExpr);
    InFileCT23.close();

    PNodeCT23 Tree23 = nullptr;
    try {
        Tree23 = BuildTreeFromInfix(infixExpr);
    }
    catch (const runtime_error& e) {
        cerr << "Ошибка парсинга инфиксного выражения: " << e.what() << endl;
        OutFileCT23 << "Ошибка парсинга инфиксного выражения: " << e.what() << endl;
        OutFileCT23.close();
        return 1;
    }

    if (Tree23) {
        cout << "Дерево для CalcTree23 построено из инфиксного выражения." << endl;
        int XTestValue = 3;
        cout << "Вычисление значения выражения при x = " << XTestValue << ":" << endl;
        try {
            int ResultCT23 = EvaluateTreeCT23(Tree23, XTestValue);
            cout << "  Результат: " << ResultCT23 << endl;
            OutFileCT23 << "Результат выражения при x = " << XTestValue << ": " << ResultCT23 << endl;
        }
        catch (const runtime_error& e) {
            cerr << "  Ошибка вычисления: " << e.what() << endl;
            OutFileCT23 << "Ошибка вычисления: " << e.what() << endl;
        }

        cout << "Трансформация дерева (A+x -> x+A)..." << endl;
        TransformTreeCT23(Tree23);
        cout << "Дерево трансформировано." << endl;

        OutFileCT23 << "\nДерево после преобразования:" << endl;
        OutFileCT23 << "Префиксная форма: ";
        PrintTreeCT23Prefix(Tree23, OutFileCT23);
        OutFileCT23 << endl;
        cout << "Префиксная форма записана в " << Fn2_ct23 << endl;

        OutFileCT23 << "Постфиксная форма: ";
        PrintTreeCT23Postfix(Tree23, OutFileCT23);
        OutFileCT23 << endl;
        cout << "Постфиксная форма записана в " << Fn2_ct23 << endl;

        OutFileCT23 << "Инфиксная форма (с избыт. скобками): ";
        PrintTreeCT23InfixExcessiveBrackets(Tree23, OutFileCT23);
        OutFileCT23 << endl;
        cout << "Инфиксная форма записана в " << Fn2_ct23 << endl;

        // Вычислим еще раз после трансформации, результат должен быть тот же
        cout << "Повторное вычисление значения выражения при x = " << XTestValue << " после трансформации:" << endl;
        try {
            int ResultCT23_after = EvaluateTreeCT23(Tree23, XTestValue);
            cout << "  Результат: " << ResultCT23_after << endl;
            OutFileCT23 << "Результат выражения при x = " << XTestValue << " после трансформации: " << ResultCT23_after << endl;
        }
        catch (const runtime_error& e) {
            cerr << "  Ошибка вычисления: " << e.what() << endl;
            OutFileCT23 << "Ошибка вычисления после трансформации: " << e.what() << endl;
        }
    }
    else {
        cout << "Не удалось создать дерево для CalcTree23." << endl;
        OutFileCT23 << "Не удалось создать дерево." << endl;
    }

    DeleteTreeCT23(Tree23);
    OutFileCT23.close();

    return 0;
}
