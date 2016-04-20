#include "engine.h"
#include <QPoint>

namespace Engine
{

bool valid(const CharsetMatrix& m)
{
    for (int i(0); i != m.height(); ++i) {
        for (int j(0); j != m.width(); ++j) {
            if (m[i][j] != QChar(' ')) {
                for (int k(0); k != m.width(); ++k) {
                    //Если значение заполнено и совпадает с любым в этой строке или столбце...
                    if ((j != k && m[i][j] == m[i][k]) ||
                        (i != k && m[i][j] == m[k][j])) {
                        //то оно не валидно
                        return false;
                    }
                }
            }
        }
    }
    return true;
}

bool solved(const CharsetMatrix& m)
{
    for (int i(0); i != m.height(); ++i) {
        for (int j(0); j != m.width(); ++j) {
            if (m[i][j] == ' ') {
                //Если есть незаполненые значения то ещё есть над чем поработать
                return false;
            }
        }
    }
    return true;
}

AllowedSets findAllowedSet(const CharsetMatrix& m, const CharSet& fullSet)
{
    AllowedSets rezult(m.width(), m.height(), fullSet);
    for (int i(0); i != m.height(); ++i) {
        for (int j(0); j != m.width(); ++j) if (m[i][j] == QChar(' ')) {
            CharSet& cur(rezult[i][j]);
            for (int k(0); k != m.width(); ++k) {
                cur.remove(m[i][k]);
                cur.remove(m[k][j]);
            }
        }
    }
    return rezult;
}

namespace {

//рекурентная часть алгоритма.
bool findSolve(CharsetMatrix& m, const CharSet& fullSet, AllowedSets& allowed)
{
    //Сначала найдём самое маленькое множество
    QPoint min(-1,-1);
    for (int i(0); i != m.height(); ++i) {
        for (int j(0); j != m.width(); ++j) if (m[i][j] == QChar(' ')) {
            if (min == QPoint(-1,-1) || allowed[min.x()][min.y()].size() > allowed[i][j].size()) {
                min = QPoint(i, j);
            }
        }
    }
    //Если такое не встречено то пришла уже заполненая матрица, вызов не должен был иметь место. Прерываем работу програмы
    Q_ASSERT(min != QPoint(-1, -1));
    //берём обнаруженое множество
    const CharSet& cur(allowed[min.x()][min.y()]);
    //Попробуем поставить поочерёдно все возможные варианты из обнаруженного множества
    for (const QChar& i : cur) {
        //ПОСТАВИли
        m[min.x()][min.y()] = i;
        //Обновили множество доступных символов
        allowed = findAllowedSet(m, fullSet);
        //Если при этом не найдено решение, рекурсивно вызываем поиск
        if (solved(m) || findSolve(m, fullSet, allowed)) {
            //Если найдено решение (либо в ходе подстановки, либо в ходе рекурсивного вызова), то прерываем алгоритм
            return true;
        }
    }
    //Если решение не обнаружено, то нужно искать другую подстановку выше по стеку вызовов
    return false;
}

} //unnamed namespace

CharsetMatrix findSolve(const CharsetMatrix& m)
{
    if (solved(m)) {
        return m;
    }
    //Точка входа в алгоритм. Делаем копию входной матрицы
    CharsetMatrix rez(m);
    //Находим все, отличающиеся от пробела символы в матрице
    const CharSet fullSet(([&m]() {
        CharSet s;
        for (int i(0); i != m.height(); ++i) {
            for (int j(0); j != m.width(); ++j) if (m[i][j] != QChar(' ')) {
                s.insert(m[i][j]);
            }
        }
        return s;
    })());
    if (fullSet.size() < m.width() || !valid(m)) {
        throw std::exception();
    }
    AllowedSets a(findAllowedSet(m, fullSet));

    //Запускаем рекурентную часть алгоритма
    if (findSolve(rez, fullSet, a)) {
        //Если обнаружено решение, высылаем его
        return rez;
    } else {
        //иначе входные данные противоречивы, вводим программу в ошибочное состояние
        throw std::exception();
    }
}

} //namespace Engine
