#ifndef ENGINE_H
#define ENGINE_H

#include <QChar>
#include <QSet>
#include "cbasicmatrix.h"

//Решение представлено в виде набора функций и классов, объединённых в единое пространство имён
namespace Engine
{
//Матрица символов (интернациональных, в соответствиии с заданием
typedef CBasicMatrix<QChar> CharsetMatrix;
typedef QSet<QChar> CharSet;
typedef CBasicMatrix<CharSet> AllowedSets;

//Проверка матрицы символов на соответствие правилам заполнения
bool valid(const CharsetMatrix& m);

//проверка матрицы на заполненость (содержет-ли незаполненые поля)
bool solved(const CharsetMatrix& m);

//Найти для незаполненых полей множества символов, которые могут быть расположены в соответствующих полях
AllowedSets findAllowedSet(const CharsetMatrix& m, const CharSet& fullSet);

//точках входа в алгоритм
CharsetMatrix findSolve(const CharsetMatrix& m);

}

#endif // ENGINE_H
