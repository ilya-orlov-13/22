#include "header.h"

int main() {
    setlocale(LC_ALL, "Russian");

    // Чтение выражения из файла
    string filename = "input.txt";
    string PostfixExpr1;
    ifstream fin(filename);
    if (!fin) {
        cerr << "Не удалось открыть файл " << filename << endl;
        return 1;
    }
    getline(fin, PostfixExpr1);
    fin.close();

    PNodeCT1 Tree1 = nullptr;
    try {
        Tree1 = BuildTreeFromPostfix(PostfixExpr1);

        PrintSideways(Tree1,0);
        cout << endl;

        TransformTreeCT1(Tree1);

        PrintSideways(Tree1,0);
        cout << endl;

        cout << "Указатель на корень дерева: " << Tree1 << endl;
    }
    catch (const runtime_error& e) {
        cerr << "Ошибка в CalcTree1: " << e.what() << endl;
    }
    DeleteTreeCT1(Tree1);

    return 0;
}