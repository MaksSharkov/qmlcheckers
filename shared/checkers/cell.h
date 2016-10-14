#ifndef CELL_H
#define CELL_H

#include <QObject>
#include <QJsonObject>

class Cell : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int row READ row WRITE setRow NOTIFY rowChanged)
    Q_PROPERTY(int col READ col WRITE setCol NOTIFY colChanged)
    Q_PROPERTY(QJsonObject man READ man WRITE setMan NOTIFY manChanged)
    Q_PROPERTY(QString name READ name() NOTIFY nameChanged)
    Q_PROPERTY(bool isBlack READ isBlack NOTIFY isBlackChanged)
    Q_PROPERTY(bool isEmpty READ isEmpty NOTIFY isEmptyChanged)
public:
    explicit Cell(int row=0, int col=0, QObject *parent = 0);
    Cell(const Cell &other);

    Q_INVOKABLE bool isBlack()const;
    static bool isBlack(int row,int col);

    int row() const{
        return m_row;
    }

    int col() const{
        return m_col;
    }

    QJsonObject man()const{
        return m_man;
    }

    QString name()const;
    Q_INVOKABLE bool isEmpty()const;
    Q_INVOKABLE bool belongsTo(QString player)const;

    Cell& operator=(const Cell &second);
    bool operator==(const Cell &other)const;
    bool operator!=(const Cell &other)const;
    bool operator<(const Cell &other)const;
    bool isNear(const Cell &other) const;

    Q_INVOKABLE bool containsMan()const{
        return m_man["rank"]=="man";
    }

    Q_INVOKABLE bool containsKing()const{
        return m_man["rank"]=="king";
    }

    QJsonObject toJson()const;
signals:
    void rowChanged(uint);
    void colChanged(uint);
    void manChanged(const QJsonObject&);
    void nameChanged();
    void isBlackChanged();
    void isEmptyChanged();

public slots:
    void setRow(int row);
    void setCol(int col);
    void setMan(const QJsonObject &m_man);

    void swapMans(Cell &with);

    void putMan(QString rank, QString whoose);
    void purgeMan();

private:
    int m_row;
    int m_col;
    QJsonObject m_man;

};

#endif // CELL_H
