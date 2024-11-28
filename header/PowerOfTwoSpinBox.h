#ifndef POWEROFTWOSPINBOX_H
#define POWEROFTWOSPINBOX_H
#include <QDoubleSpinBox>
#include <QtMath> // pour qPow()

class PowerOfTwoSpinBox : public QDoubleSpinBox {
    Q_OBJECT

public:
    explicit PowerOfTwoSpinBox(QWidget *parent = nullptr) : QDoubleSpinBox(parent) {
        setRange(0, 9); // Exposants de 2^0 à 2^9
        setDecimals(0); // Pas de décimales
        setSingleStep(1); // Incrément/décrément de 1
        setValue(7); // Valeur initiale (2^7 = 128)
        setFixedWidth(50);
    }

protected:
    // Convertit l'exposant interne en puissance de 2 pour l'affichage
    QString textFromValue(double value) const override {
        int exponent = static_cast<int>(value);
        double result = qPow(2, exponent);
        return QString::number(static_cast<int>(result));
    }

    // Convertit une entrée utilisateur (puissance de 2) en exposant interne
    double valueFromText(const QString &text) const override {
        bool ok;
        int inputValue = text.toInt(&ok);
        if (ok && inputValue > 0) {
            return qLn(inputValue) / qLn(2); // log2 de la valeur entrée
        }
        return 0; // Si l'entrée est invalide
    }

    // Valide l'entrée utilisateur
    QValidator::State validate(QString &input, int &pos) const override {
        bool ok;
        int inputValue = input.toInt(&ok);
        if (!ok || inputValue <= 0) {
            return QValidator::Invalid;
        }

        // Vérifie si l'entrée est une puissance de 2
        if ((inputValue & (inputValue - 1)) == 0) {
            return QValidator::Acceptable;
        }

        return QValidator::Intermediate;
    }
};


#endif // POWEROFTWOSPINBOX_H
