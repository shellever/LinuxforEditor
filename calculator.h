#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QLineEdit;
QT_END_NAMESPACE
class Button;

class Calculator : public QWidget
{
    Q_OBJECT

public:
    Calculator(QWidget *parent = 0);
    ~Calculator();
    enum ErrorCode { DivisionByZero,  SqrtOfNegativeNumber, Error };

private slots:
    void digitClicked();
    void unaryOperatorClicked();
    void additiveOperatorClicked();
    void multiplicativeOperatorClicked();
    void equalClicked();
    void pointClicked();
    void changeSignClicked();
    void backspaceClicked();
    void clear();
    void clearAll();
    void clearMemory();
    void readMemory();
    void setMemory();
    void addToMemory();

private:
    Button *createButton(const QString &text, const char *member);
    void abortOperation(ErrorCode err);
    bool calculate(double rightOperand, const QString &pendingOperator);

    double sumInMemory;     //contains the value stored in the calculator's memory
    double sumSoFar;        //stores the value accumulated so far
    double factorSoFar;     //stores a temporary value when doing multiplications and divisions
    QString pendingAdditiveOperator;        //stores the last additive operator clicked by the user
    QString pendingMultiplicativeOperator;  //stores the last multiplicative operator clicked by the user
    bool waitingForOperand;                 //be true when the calculator is expecting the user to start typing an operand

    QLineEdit *display;

    enum { NumDigitButtons = 10 };
    Button *digitButtons[NumDigitButtons];
};

#endif // CALCULATOR_H
