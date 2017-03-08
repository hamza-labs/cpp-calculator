#include <QtWidgets>

#include <cmath>

#include "bouton.h"
#include "calculatrice.h"

Calculatrice::Calculatrice(QWidget *parent)
    : QWidget(parent)
{
    SommeActuelle = 0.0;
    facteurActuel = 0.0;
    attenteOperande = true;

    // Creation de l'affichage
    display = new QLineEdit("0");
    display->setReadOnly(true);
    display->setAlignment(Qt::AlignRight);
    display->setMaxLength(15);

    QFont font = display->font();
    font.setPointSize(font.pointSize() + 8);
    display->setFont(font);

    // On crée les bouton à utilisé boutton pour chaque nombre et on les connectes au slots associés
    for (int i = 0; i < NbChiffreBoutons; ++i) {
        boutonChiffre[i] = creerBouton(QString::number(i), SLOT(chiffreClique()));
    }

    Bouton *pointButton = creerBouton(tr("."), SLOT(pointClique()));

    Bouton *clearButton = creerBouton(tr("Clear"), SLOT(clear()));

    Bouton *divisionButton = creerBouton(tr("\303\267"), SLOT(operationMultiplicationClique()));
    Bouton *timesButton = creerBouton(tr("\303\227"), SLOT(operationMultiplicationClique()));
    Bouton *minusButton = creerBouton(tr("-"), SLOT(operationAdditionClique()));
    Bouton *plusButton = creerBouton(tr("+"), SLOT(operationAdditionClique()));

    Bouton *equalButton = creerBouton(tr("="), SLOT(egalClique()));

    // On crée une grille fixe et on y mets nos bouton
    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->setSizeConstraint(QLayout::SetFixedSize);
    mainLayout->addWidget(display, 0, 0, 1, 6);
    mainLayout->addWidget(clearButton, 1, 2, 1, 2);

    for (int i = 1; i < NbChiffreBoutons; ++i) {
        int row = ((9 - i) / 3) + 2;
        int column = ((i - 1) % 3) + 1;
        mainLayout->addWidget(boutonChiffre[i], row, column);
    }

    mainLayout->addWidget(boutonChiffre[0], 5, 1);
    mainLayout->addWidget(pointButton, 5, 2);

    mainLayout->addWidget(divisionButton, 2, 4);
    mainLayout->addWidget(timesButton, 3, 4);
    mainLayout->addWidget(minusButton, 4, 4);
    mainLayout->addWidget(plusButton, 5, 4);

    mainLayout->addWidget(equalButton, 5, 5);
    setLayout(mainLayout);

    setWindowTitle(tr("Calculator"));
}

void Calculatrice::chiffreClique()
{
    Bouton *boutonClique = qobject_cast<Bouton *>(sender());
    int valeurNumerique = boutonClique->text().toInt();
    if (display->text() == "0" && valeurNumerique == 0.0)
        return;

    if (attenteOperande) {
        display->clear();
        attenteOperande = false;
    }
    display->setText(display->text() + QString::number(valeurNumerique));
}

void Calculatrice::operationAdditionClique()
{
    Bouton *boutonClique = qobject_cast<Bouton *>(sender());
    QString operandeClique = boutonClique->text();
    double operand = display->text().toDouble();

    if (!operationMultiplicationEnAttente.isEmpty()) {
        if (!calculer(operand, operationMultiplicationEnAttente)) {
            annulerOperation();
            return;
        }
        display->setText(QString::number(facteurActuel));
        operand = facteurActuel;
        facteurActuel = 0.0;
        operationMultiplicationEnAttente.clear();
    }

    if (!operationAdditionEnAttente.isEmpty()) {
        if (!calculer(operand, operationAdditionEnAttente)) {
            annulerOperation();
            return;
        }
        display->setText(QString::number(SommeActuelle));
    } else {
        SommeActuelle = operand;
    }

    operationAdditionEnAttente = operandeClique;
    attenteOperande = true;
}

void Calculatrice::operationMultiplicationClique()
{
    Bouton *boutonClique = qobject_cast<Bouton *>(sender());
    QString operandeClique = boutonClique->text();
    double operand = display->text().toDouble();

    if (!operationMultiplicationEnAttente.isEmpty()) {
        if (!calculer(operand, operationMultiplicationEnAttente)) {
            annulerOperation();
            return;
        }
        display->setText(QString::number(facteurActuel));
    } else {
        facteurActuel = operand;
    }

    operationMultiplicationEnAttente = operandeClique;
    attenteOperande = true;
}

void Calculatrice::egalClique()
{
    double operand = display->text().toDouble();

    if (!operationMultiplicationEnAttente.isEmpty()) {
        if (!calculer(operand, operationMultiplicationEnAttente)) {
            annulerOperation();
            return;
        }
        operand = facteurActuel;
        facteurActuel = 0.0;
        operationMultiplicationEnAttente.clear();
    }
    if (!operationAdditionEnAttente.isEmpty()) {
        if (!calculer(operand, operationAdditionEnAttente)) {
            annulerOperation();
            return;
        }
        operationAdditionEnAttente.clear();
    } else {
        SommeActuelle = operand;
    }

    display->setText(QString::number(SommeActuelle));
    SommeActuelle = 0.0;
    attenteOperande = true;
}

void Calculatrice::pointClique()
{
    if (attenteOperande)
        display->setText("0");
    if (!display->text().contains('.'))
        display->setText(display->text() + tr("."));
    attenteOperande = false;
}

void Calculatrice::clear()
{
    if (attenteOperande)
        return;

    display->setText("0");
    attenteOperande = true;
}

Bouton *Calculatrice::creerBouton(const QString &text, const char *member)
{
    Bouton *bouton = new Bouton(text);
    connect(bouton, SIGNAL(clicked()), this, member);
    return bouton;
}

void Calculatrice::annulerOperation()
{
    clear();
    display->setText(tr("####"));
}

bool Calculatrice::calculer(double rightOperand, const QString &operandeSuivant)
{
    if (operandeSuivant == tr("+")) {
        SommeActuelle += rightOperand;
    } else if (operandeSuivant == tr("-")) {
        SommeActuelle -= rightOperand;
    } else if (operandeSuivant == tr("\303\227")) {
        facteurActuel *= rightOperand;
    } else if (operandeSuivant == tr("\303\267")) {
        if (rightOperand == 0.0)
            return false;
        facteurActuel /= rightOperand;
    }
    return true;
}
