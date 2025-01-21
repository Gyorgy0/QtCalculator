#include "calculator.h"
#include "ui_calculator.h"
#include <cfloat>

Calculator::Calculator(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Calculator)
{
    ui->setupUi(this);
    // Szamok gombjai
    connect(ui->pushButton, SIGNAL(clicked()), SLOT(NumberInput()));
    connect(ui->pushButton_2, SIGNAL(clicked()), SLOT(NumberInput()));
    connect(ui->pushButton_3, SIGNAL(clicked()), SLOT(NumberInput()));
    connect(ui->pushButton_4, SIGNAL(clicked()), SLOT(NumberInput()));
    connect(ui->pushButton_5, SIGNAL(clicked()), SLOT(NumberInput()));
    connect(ui->pushButton_6, SIGNAL(clicked()), SLOT(NumberInput()));
    connect(ui->pushButton_7, SIGNAL(clicked()), SLOT(NumberInput()));
    connect(ui->pushButton_8, SIGNAL(clicked()), SLOT(NumberInput()));
    connect(ui->pushButton_9, SIGNAL(clicked()), SLOT(NumberInput()));
    connect(ui->pushButton_10, SIGNAL(clicked()), SLOT(NumberInput()));
    // Muvelet gombok
    connect(ui->pushButton_11, SIGNAL(clicked()), SLOT(Operator()));   // Osztas
    connect(ui->pushButton_12, SIGNAL(clicked()), SLOT(Operator()));   // Szorzas
    connect(ui->pushButton_13, SIGNAL(clicked()), SLOT(Operator()));   // Kivonas
    connect(ui->pushButton_14, SIGNAL(clicked()), SLOT(Operator()));   // Osszeadas
    // Kiszamitas
    connect(ui->pushButton_15, SIGNAL(clicked()), SLOT(Calculate()));
    // Backspace gomb
    connect(ui->pushButton_16, SIGNAL(clicked()), SLOT(Backspace()));
    // CE, C
    connect(ui->pushButton_17, SIGNAL(clicked()), SLOT(ClearEverything()));
    connect(ui->pushButton_18, SIGNAL(clicked()), SLOT(Clear()));
    // Memoria
    connect(ui->pushButton_19, SIGNAL(clicked()), SLOT(MemClear()));
    connect(ui->pushButton_20, SIGNAL(clicked()), SLOT(MemRecall()));
    connect(ui->pushButton_21, SIGNAL(clicked()), SLOT(MemMinus()));
    connect(ui->pushButton_22, SIGNAL(clicked()), SLOT(MemPlus()));
}

Calculator::~Calculator()
{
    delete ui;
}

// Billentyuzet gombjainak lekezelese - lehetove teszi a billentyuvel valo bevitelt
void Calculator::KeyHandling(QKeyEvent *keyPressEvent) {
    // Szamok beirasa
    if ((keyPressEvent->key() - 48) >= 0 && (keyPressEvent->key() - 48) <= 9) {
        NumberInput(keyPressEvent);
        // Szamok kiradirozasa
    } else if (keyPressEvent->key() == Qt::Key_Backspace) {
        Backspace();
        // Muvelet kiszamitasa
    } else if (keyPressEvent->key() == Qt::Key_Enter || keyPressEvent->key() == Qt::Key_Return
               || keyPressEvent->key() == Qt::Key_Equal) {
        Calculate();
        // Muveletek beirasa
    } else if (keyPressEvent->key() == Qt::Key_Minus || keyPressEvent->key() == Qt::Key_Plus
               || keyPressEvent->key() == Qt::Key_Asterisk
               || keyPressEvent->key() == Qt::Key_Slash) {
        Operator(keyPressEvent);
    }
    qDebug() << keyPressEvent;
}

void Calculator::NumberInput(QKeyEvent *keyPressevent)
{
    if ((ui->lcdNumber->intValue() * 10) >= 0) {
        // Overflow kezelese
        if (keyPressevent->text().toInt() > (INT_MAX - (ui->lcdNumber->value() * 10))) {
            return;
        }
    }
    lcdvalue = (QString::number(ui->lcdNumber->intValue()) + keyPressevent->text()).toInt();
    if (QString::number(lcdvalue, 'g', 12).length() > 5) {
        ui->lcdNumber->setDigitCount(QString::number(lcdvalue, 'g', 12).length());
    }
    ui->lcdNumber->display(QString::number(lcdvalue, 'g', 12));
    newinput = true;
}

void Calculator::Operator(QKeyEvent *keyPressEvent)
{
    if (operators.capacity() > 0 && operators.last() != keyPressEvent->text() && !newinput) {
        operators[operators.size() - 1] = keyPressEvent->text();
    } else {
        if (keyPressEvent->text() == "*") {
            operators.push_back("×");
        } else if (keyPressEvent->text() == "/") {
            operators.push_back("÷");
        } else {
            operators.push_back(keyPressEvent->text());
        }
        numbers.push_back(QString::number(lcdvalue, 'g', 12));
    }
    QString str;
    for (int i = 0; i < numbers.size(); ++i) {
        str += numbers[i];
        if (i <= operators.size())
            str += " " + operators[i] + " ";
    }
    ui->textBrowser->setText(str);
    lcdvalue = 0;
    ui->lcdNumber->setDigitCount(5);
    ui->lcdNumber->display(lcdvalue);
}

void Calculator::NumberInput()
{
    QPushButton* senderButton = qobject_cast<QPushButton*>(sender());
    if ((ui->lcdNumber->intValue() * 10) >= 0) {
        // Overflow kezelese
        if (senderButton->text().toInt() > (INT_MAX - (ui->lcdNumber->value() * 10))) {
            return;
        }
    }
    lcdvalue = (QString::number(ui->lcdNumber->intValue()) + senderButton->text()).toInt();
    if (QString::number(lcdvalue, 'g', 12).length() > 5) {
        ui->lcdNumber->setDigitCount(QString::number(lcdvalue, 'g', 12).length());
    }
    ui->lcdNumber->display(QString::number(lcdvalue, 'g', 12));
    newinput = true;
}

void Calculator::Backspace()
{
    if (ui->lcdNumber->value() > 0) {
        if (QString::number(lcdvalue, 'g', 12).removeLast() != "")
            lcdvalue = (QString::number(lcdvalue, 'g', 12).removeLast()).toDouble();
        else {
            lcdvalue = 0;
        }
    }
    if (QString::number(lcdvalue).length() > 5)
    {
        ui->lcdNumber->setDigitCount(QString::number(lcdvalue, 'g', 12).length());
    }
    ui->lcdNumber->display(QString::number(lcdvalue, 'g', 12));
    newinput = true;
}

void Calculator::Calculate(){
    QString str;
    for (int i = 0; i < numbers.size(); ++i)
    {
        str += numbers[i];
        if (i <= operators.size())
            str += " " + operators[i] + " ";
    }
    numbers.push_back(QString::number(lcdvalue, 'g', 12));
    // ~Kiszamolas~
    for (int i = 0; i < operators.size(); i++) {
        // Osztas
        if (operators[i] == "÷") {
            numbers[i] = QString::number(numbers[i].toDouble() / numbers[i + 1].toDouble(), 'g', 12);
            numbers.removeAt(i + 1);
            operators.removeAt(i);
        }
    }
    for (int i = 0; i < operators.size(); i++) {
        // Szorzas
        if (operators[i] == "×") {
            numbers[i] = QString::number(numbers[i].toDouble() * numbers[i + 1].toDouble(), 'g', 12);
            numbers.removeAt(i + 1);
            operators.removeAt(i);
        }
    }

    result = numbers[0].toDouble();
    for (int i = 0; i < operators.size(); i++)
    {
        // Kivonas
        if (operators[i] == "-") {
            result -= numbers[i + 1].toDouble();
        }
        // Osszeadas
        else if (operators[i] == "+") {
            result += numbers[i + 1].toDouble();
        }
    }
    if (QString::number(result, 'g', 12).length() > 5) {
        ui->lcdNumber->setDigitCount(QString::number(result, 'g', 12).length());
    }
    QVector<QString> number = QString::number(result, 'g', 12).split('.');
    if (number.length() == 2 && number[1].length() >= 12
        && result != QString::number(result, 'f', 13).toDouble()) {
        ui->textBrowser->setText(str + QString::number(lcdvalue, 'g', 12) + " ≈ ");
    } else {
        ui->textBrowser->setText(str + QString::number(lcdvalue, 'g', 12) + " = ");
    }
    lcdvalue = result;
    ui->lcdNumber->display(QString::number(result, 'g', 12));
    numbers.clear();
    operators.clear();
}

void Calculator::Operator()
{
    QPushButton* senderButton = qobject_cast<QPushButton*>(sender());
    if (operators.capacity() > 0 && operators.last() != senderButton->text() && !newinput){
        operators[operators.size() - 1] = senderButton->text();
    }
    else
    {
        operators.push_back(senderButton->text());
        numbers.push_back(QString::number(lcdvalue, 'g', 12));
    }
    QString str;
    for (int i = 0; i < numbers.size(); ++i)
    {
        str += numbers[i];
        if (i <= operators.size())
            str += " " + operators[i] + " ";
    }
    ui->textBrowser->setText(str);
    lcdvalue = 0;
    ui->lcdNumber->setDigitCount(5);
    ui->lcdNumber->display(lcdvalue);
}

void Calculator::Clear()
{
    lcdvalue = 0;
    ui->lcdNumber->display(lcdvalue);
    ui->lcdNumber->setDigitCount(5);
}
void Calculator::ClearEverything()
{
    Clear();
    numbers.clear();
    operators.clear();
    ui->textBrowser->setText("");
}

void Calculator::MemClear()
{
    memvalue = 0.0;
}
void Calculator::MemRecall()
{
    lcdvalue = memvalue;
    if (QString::number(lcdvalue, 'g', 12).length() > 5) {
        ui->lcdNumber->setDigitCount(QString::number(lcdvalue, 'g', 12).length());
    }
    ui->lcdNumber->display(QString::number(lcdvalue, 'g', 12));
}
void Calculator::MemMinus()
{
    if (lcdvalue >= 0) {
        if (memvalue > (DBL_MAX - lcdvalue)) {
            return;
        }
    } else {
        if (memvalue < (DBL_MIN - lcdvalue)) {
            return;
        }
    }
    memvalue -= lcdvalue;
}
void Calculator::MemPlus()
{
    if (lcdvalue >= 0) {
        if (memvalue > (DBL_MAX - lcdvalue)) {
            return;
        }
    } else {
        if (memvalue < (DBL_MIN - lcdvalue)) {
            return;
        }
    }
    memvalue += lcdvalue;
}
