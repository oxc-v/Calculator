/*
 * 这个类提供一个计算算术表达式的功能。
 * 该类通过构造函数接收使用者传递的算术表达式，进一步对表达式进行词法分析和语法分析，
 * 最后进行表达式求值。
 * ---------------------------
 * 如果表达式的词法和语法正确，则correct将被赋值为true,反之赋值为false;
 * 表达式的结果保存在共有成员变量result中。
 * ----------------------------
 * 用于词法分析的单词种类
 * |单词  种别|
 * |-      1 |
 * |+      2 |
 * |*      3 |
 * |/      4 |
 * |(      5 |
 * |)      6 |
 * |digit  7 |
 * |@(负号) 8 |
 * |#(正号) 9 |
 * |%(百分号) 10 |
 * |lg     11|
 * |^      12|
 * |blank  0 |
 */

#ifndef CALCULATE_H
#define CALCULATE_H

#include <QCoreApplication>

class Calculate
{

public:

    explicit Calculate(const QString expression);

private:

    void Next();                        // 扫描下一个字符
    bool isOperator(int type);          // 判断字符是否是操作符
    int compareOperator(int type);      // 比较操作符的优先级

    // 从栈中连续弹出两个操作数
    void popTwoNumbers(QStack<double> &s, double &first, double &second);

    // 词法分析器
    void wordAnalysis(QVector<QPair<QString, int>> &words, const QString expr);

    /* 语法分析
     * -------
     * 算术表达式的文法表示
     * expr → term | @term | #term {+term | -term | ^term}
     * term → factor {*factor | /factor}
     * factor → (expr) | lg expr | digit {%}
     */
    void expr();
    void term();
    void factor();

    // 表达式求值

    // 将中缀表达式转换为后缀表达式
    QVector<QPair<QString, int>> getInfixToSuffix (QVector<QPair<QString, int>> &expr);

    // 计算后缀表达式
    double exprToCalculate(const QVector<QPair<QString, int>> &suffixExpr);    // 计算后缀表达式的值

public:

    bool correct = false;       // 用于判断表达式的正确性
    double result = 0;          // 存放表达式的结果

private:

    int type;                               // 单词的种别
    int index = 0;                          // 算术表达式的下标索引
    bool wordError = false;                 // 判断词法是否正确
    bool grammerError = false;              // 判断语法是否正确
    QVector<QPair<QString, int>> words;     // 存放单词的集合

};

#endif // CALCULATE_H
