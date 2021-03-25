#include "calculate.h"

#include <QStack>

// 参数构造函数
Calculate::Calculate(const QString expression)
{
    // 对中缀表达式进行词法分析
    wordAnalysis(words, expression);

    // 判断词法正确性
    if (!wordError) {   // 词法正确

        // 语法分析
        Next();
        expr();

        // 判断语法的正确性
        if (type != 0 || grammerError) {    // type = 0表示式已经扫描完毕

            correct = false;                // 语法错误
        } else {                            // 语法和词法都正确

            correct = true;                 // 表达式语法和词法都正确

            QVector<QPair<QString, int>> p; // 存放后缀表达式
            p = getInfixToSuffix(words);    // 将中缀表达式转换为后缀表达式
            result = exprToCalculate(p);    // 计算后缀表达式的值
        }
    } else {                // 词法错误

        correct = false;    // 表达式错误
    }
}

/*
 * 根据单词的种别判断字符是否为操作符。
 * 如果字符为操作符，返回true；反之，返回false。
 */
bool Calculate::isOperator(int type)
{
    switch (type) {

    case 1:
    case 2:
    case 3:
    case 4:
        return true;
    default:
        return false;
    }
}

/*
 * 比较操作符的优先级。
 * 根据优先级返回一个整数。
 */
int Calculate::compareOperator(int type)
{
    switch (type) {

    case 1:
    case 2:
        return 1;
    case 3:
    case 4:
        return 2;
    default:
        return 0;
    }
}

// 词法分析
void Calculate::wordAnalysis(QVector<QPair<QString, int>> &words, const QString expr)
{
    // 从左到右依次扫描表达式
    for (int i = 0; i < expr.size(); i++) {

        // 提取运算符
        if (expr.at(i) == '-' || expr.at(i) == '+' || expr.at(i) == '*' ||
            expr.at(i) == '/' || expr.at(i) == '(' || expr.at(i) == ')' ||
            expr.at(i) == '%') {

            // 将QChar转换成QString类型存储
            QString strTmp;
            strTmp.push_back(expr.at(i));

            switch (expr.at(i).toLatin1()) {

            case '-':
                if (i == 0 || expr.at(i - 1) == '(')
                    // 匹配@
                    words.push_back(qMakePair(QString("@"), 8));
                else
                    // 匹配-
                    words.push_back(qMakePair(strTmp, 1));
                break;

            case '+':
                if (i == 0 || expr.at(i - 1) == '(')
                    // 匹配#
                    words.push_back(qMakePair(QString("#"), 9));
                else
                    // 匹配+
                    words.push_back(qMakePair(strTmp, 2));
                break;

            case '*':
                // 匹配*
                words.push_back(qMakePair(strTmp, 3));
                break;

            case '/':
                // 匹配/
                words.push_back(qMakePair(strTmp, 4));
                break;

            case '(':
                // 匹配(
                words.push_back(qMakePair(strTmp, 5));
                break;

            case ')':
                // 匹配)
                words.push_back(qMakePair(strTmp, 6));
                break;

            case '%':
                // 匹配%
                words.push_back(qMakePair(strTmp, 10));
                break;

            default:
                break;
            }
        } else if (expr.at(i) >= '0' && expr.at(i) <= '9') {
            // 以数字开头

            // 存放单词
            QString strTmp;

            // 提取连续的数字
            while (expr.at(i) >= '0' && expr.at(i) <= '9') {

                strTmp.push_back(expr.at(i));
                i++;

                // 表达式扫描完毕
                if (i == expr.size()) {
                    words.push_back(qMakePair(strTmp, 7));
                    return;
                }
            }

            // 遇到小数点
            if (expr.at(i) == '.') {

                strTmp.push_back(expr.at(i));
                i++;

                if (i == expr.size()) {

                    // 表达式已小数点结束，词法错误
                    wordError = true;
                    return;

                } else if (expr.at(i) >= '0' && expr.at(i) <= '9') {

                    // 提取小数点后面的数字
                    while (expr.at(i) >= '0' && expr.at(i) <= '9') {
                        strTmp.push_back(expr.at(i));
                        i++;

                        // 表达式扫描完毕
                        if (i == expr.size()) {
                            words.push_back(qMakePair(strTmp, 7));
                            return;
                        }
                    }
                } else {

                     // 小数点后面没有数字，词法错误
                    wordError = true;
                    return;
                }
            }

            words.push_back(qMakePair(strTmp, 7));

            // 上面的while循环多扫描了一个字符
            i--;

        } else {

            // 以.开头，单词错误
            wordError = true;
            return;
        }
    }
}

// 扫描下一个字符
void Calculate::Next()
{
    if (index < words.size())   // 表达式还没有扫描完毕
        // 获取单词种别
        type = words.at(index++).second;
    else                        // 表达式已扫描完毕
        type = 0;
}

// expr → term | @term | #term {+term | -term}
void Calculate::expr()
{
    if (type == 8 || type == 9) {
        Next();
        term();
    } else {
        term();
    }
    while (type == 1 || type == 2) {
        Next();
        term();
    }
}

// term → factor {*factor | /factor}
void Calculate::term()
{
    factor();
    while (type == 3 || type == 4) {
        Next();
        factor();
    }
}

// factor → (expr) | digit {%}
void Calculate::factor()
{
    if (type == 7) {

        Next();

    } else if (type == 5) {

        Next();
        expr();

        if (type == 6) {

            Next();

        } else {

            grammerError = true;    // 表达式的语法有误  
        }
    } else {

            grammerError = true;    // 表达式的语法有误
    }

    while (type == 10)
        Next();
}

// 从栈中弹出连续的两个操作数
void Calculate::popTwoNumbers(QStack<double> &s, double &first, double &second)
{
    first = s.pop();
    second = s.pop();
}

//将中缀表达式转换为后缀表达式
QVector<QPair<QString, int>> Calculate::getInfixToSuffix(QVector<QPair<QString, int>> &expr)
{
    QStack<QPair<QString, int>> s;   // 存放运算符
    QVector<QPair<QString, int>> v;  // 存放中间结果

    // 从左到右依次扫描表达式
    for (int i = 0; i < expr.size(); i++) {

        // 处理@
        if (expr.at(i).second == 8) {

            // 将@替换成 （0 - 数字） 形式
            expr[i] = qMakePair(QString("-"), 1);
            expr.insert(i, qMakePair(QString("0"), 7));

        } else if (expr.at(i).second == 9) {
            // 处理#

            // 将#替换成 （0 + 数字） 形式
            expr[i] = qMakePair(QString("+"), 2);
            expr.insert(i, qMakePair(QString("0"), 7));
        }
    }

    // 从左到右扫描表达式
    for (int i = 0; i < expr.size(); i++) {

        // 提取单词
        QPair<QString, int> word = expr.at(i);

        // 如果word是操作符，则需比较操作符的优先级
        if (isOperator(word.second)) {

            // 如何word的操作符的优先级小于或等于s中的操作符,
            // 需要将s中的操作符弹出并压入v中，
            while (!s.isEmpty() && isOperator(s.top().second) &&
                   (compareOperator(word.second) <= compareOperator(s.top().second)))
                v.push_back(s.pop());

            // 将运算符压入s中
            s.push(word);

        } else if (word.second == 5) {

            // 如果是 ( ，则直接压入s
            s.push(word);

        } else if (word.second == 6) {

            // 如果是 ) ，则将s的操作符压入v，直到遇到 (
            while (s.top().second != 5)
                v.push_back(s.pop());

            // 弹出s中的 (
            s.pop();

        } else {

            // 如果是操作数，直接压入v
            v.push_back(word);
        }
    }

    // 将s中剩余的操作符全部压入v中
    while (!s.isEmpty())
        v.push_back(s.pop());

    // 此时v就是后缀表达式
    return v;
}


// 计算后缀表达式的值
double Calculate::exprToCalculate(const QVector<QPair<QString, int>> &suffixExpr)
{
    QStack<double> output;   // 存放表达式计算过程的中间值
    double first, second;    // 用于存放output栈弹出的两个值

    // 遍历后缀表达式
    for (int i = 0; i < suffixExpr.size(); i++) {

        // 提取单词
        QPair<QString, int> p = suffixExpr.at(i);

        switch (p.second) {

        case 1:
            popTwoNumbers(output, first, second);
            output.push(second - first);
            break;

        case 2:
            popTwoNumbers(output, first, second);
            output.push(second + first);
            break;

        case 3:
            popTwoNumbers(output, first, second);
            output.push(second * first);
            break;

        case 4:
            popTwoNumbers(output, first, second);
            output.push(second / first);
            break;

        case 10:
            // 处理多个%
            while (suffixExpr.at(i).second == 10) {

                output.push_back(output.pop() / 100);
                i++;

                // 表达式扫描完毕
                if (i == suffixExpr.size())
                    break;
            }
            // while循环多扫描了一个字符
            i--;
            break;

        default:
            // 将操作数压入output中
            output.push_back(p.first.toDouble());
            break;
        }
    }

    // output的顶部操作数就是表达式的值
    return output.pop();
}
