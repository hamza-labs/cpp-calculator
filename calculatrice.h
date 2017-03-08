
#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QWidget>

class QLineEdit;

class Bouton;

class Calculatrice : public QWidget
{
    Q_OBJECT

public:
    Calculatrice(QWidget *parent = 0);

private slots:
    void chiffreClique();
    void operationAdditionClique();
    void operationMultiplicationClique();
    void egalClique();
    void pointClique();
    void clear();

private:
    Bouton *creerBouton(const QString &text, const char *member);
    void annulerOperation();
    bool calculer(double rightOperand, const QString &pendingOperator);

    double SommeActuelle;
    double facteurActuel;
    QString operationAdditionEnAttente;
    QString operationMultiplicationEnAttente;
    bool attenteOperande;

    QLineEdit *display;

    enum { NbChiffreBoutons = 10 };
    Bouton *boutonChiffre[NbChiffreBoutons];
};

#endif
