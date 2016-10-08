#ifndef QCARD_H
#define QCARD_H

#include <QObject>

class QCard : public QObject
{
    Q_OBJECT
public:
    explicit QCard(QObject *parent = 0);

signals:

public slots:
};

#endif // QCARD_H