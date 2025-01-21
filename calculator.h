#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QMainWindow>
#include <QKeyEvent>

QT_BEGIN_NAMESPACE
namespace Ui {
class Calculator;
}
QT_END_NAMESPACE

class Calculator : public QMainWindow
{
    Q_OBJECT

public:
    Calculator(QWidget *parent = nullptr);
    ~Calculator();
    double result = 0.0;
    bool newinput = false;
    QVector<QString> numbers;
    QVector<QString> operators;
    double lcdvalue = 0.0;
    double memvalue = 0.0;

public slots:
    void KeyHandling(QKeyEvent *keyPressEvent);
    void NumberInput();
    void NumberInput(QKeyEvent *keyPressEvent);
    void Backspace();
    void Operator();
    void Operator(QKeyEvent *keyPressEvent);
    void Calculate();
    void Clear();
    void ClearEverything();
    void MemClear();
    void MemRecall();
    void MemMinus();
    void MemPlus();

protected:
    void keyPressEvent(QKeyEvent *keyPressEvent) override { KeyHandling(keyPressEvent); }

private:
    Ui::Calculator *ui;
};

#endif // CALCULATOR_H
