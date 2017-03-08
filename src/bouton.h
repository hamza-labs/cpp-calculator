#ifndef BUTTON_H
#define BUTTON_H

#include <QToolButton>

class Bouton : public QToolButton
{
    Q_OBJECT

public:
    explicit Bouton(const QString &text, QWidget *parent = 0);

    QSize sizeHint() const override;
};

#endif
