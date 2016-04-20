#ifndef GUI_H
#define GUI_H

#include <QMainWindow>
#include <QGenericMatrix>
#include <QLineEdit>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QRegularExpressionValidator>
#include <QChar>
#include <QTextCodec>
#include <QMessageBox>

#include "kernel/cbasicmatrix.h"
#include "kernel/engine.h"

class GUI : public QWidget
{
    Q_OBJECT

    static const int SIZE = 5;

    //Внутренняя структура, управляющая полями ввода. Сразу при создании структуры, заполним её полями ввода
    struct Table : public QVector<QVector<QPair<QPoint, QLineEdit*> > > {
        Table(QWidget* parent = 0) : QVector<QVector<QPair<QPoint, QLineEdit*> > >(SIZE) {
            parent->setLayout(new QGridLayout);
            //Тут ещё добавим проверочку, чтобы в поля можно было вводить только русские буквы или пробел или вообще уж ничего не вводить
            const QValidator* valid(new QRegularExpressionValidator(QRegularExpression("[а-яА-Я ]?"), parent));
            for (auto& i : *this) {
                i.resize(SIZE);
                for (QPair<QPoint, QLineEdit*>& j : i) {
                    j.second = new QLineEdit(parent);
                    j.first = QPoint(&i - this->data(), &j - i.data());
                    j.second->setValidator(valid);
                    j.second->setAlignment(Qt::AlignCenter);
                    static_cast<QGridLayout*>(parent->layout())->addWidget(j.second, j.first.x(), j.first.y() + 1);
                }
            }
        }
    };

    Table t;

public:
    GUI(QWidget *parent = 0)
        : QWidget(parent)
        , t(this)
    {
        //Теперь поля ввода уже созданы. Осталось их раскидать по виджету
        for (int i(0); i != SIZE; ++i) {
            static_cast<QGridLayout*>(layout())->addWidget(new QLabel(QString::number(SIZE - i)), i, 0);
            static_cast<QGridLayout*>(layout())->addWidget(new QLabel(QString('a' + i)), SIZE + 1, i + 1, 1, 1, Qt::AlignCenter);
        }
        //Ещё нужно расположить кнопочки
        QPushButton* const clear(new QPushButton("Почистить", this));
        QPushButton* const think(new QPushButton("Подумать", this));
        static_cast<QGridLayout*>(layout())->addWidget(clear, SIZE + 2, 0, 1, (SIZE + 1) >> 1);
        static_cast<QGridLayout*>(layout())->addWidget(think, SIZE + 2, (SIZE + 1) >> 1, 1, (SIZE + 1) >> 1);

        //Теперь подпишемся на нажатие кнопки очистки. Она будет очищать все поля ввода
        for (int i(0); i != this->layout()->count(); ++i) {
            QWidget* const w(this->layout()->itemAt(i)->widget());
            if (dynamic_cast<QLineEdit*>(w)) {
                connect(clear, &QPushButton::clicked, static_cast<QLineEdit*>(w), &QLineEdit::clear);
            }
        }

        //тут собственно происходит вызов алгоритма
        connect(think, QPushButton::clicked, [this]() {
            CBasicMatrix<QChar> data(SIZE, SIZE);
            //Сначала выписываем значения полей ввод в матрицу
            for (auto& i : this->t) {
                for (QPair<QPoint, QLineEdit*>& j : i) {
                    data[j.first.x()][j.first.y()] = j.second->text().size() ? j.second->text()[0] : QChar(' ');
                }
            }
            //ставим результат алгоритма как значение
            try {
                setData(Engine::findSolve(data));
            } catch(...) {
                //Если данные противоречивы, скажем пользователю об этом
                QMessageBox::warning(this, "Solve exception", "It's impossible to find any solution");
            }
        });

        //ещё хорошо-бы заполнить сразу поля ввода какими-нибудь входными значениями. Возьмём в их качестве приведнные в задании.
        CBasicMatrix<QChar> initial(SIZE, SIZE);
        initial.fill(' ');
        initial[0][0] = QString("С")[0];
        initial[0][1] = QString("Л")[0];
        initial[0][2] = QString("Е")[0];
        initial[0][3] = QString("З")[0];
        initial[0][4] = QString("А")[0];
        initial[2][2] = QString("Л")[0];
        initial[2][3] = QString("Е")[0];
        initial[2][4] = QString("С")[0];
        setData(initial);

    }

    void setData(const CBasicMatrix<QChar>& v) {
        //просто извлекает из матрицы символов значения и устанавливает в поля ввода
        for (int i(0); i != SIZE; ++i) {
            for (int j(0); j != SIZE; ++j) {
                t[i][j].second->setText(QString(v[i][j]));
            }
        }
    }

    ~GUI() {}
};

#endif // GUI_H
