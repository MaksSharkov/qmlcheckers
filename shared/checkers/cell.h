#ifndef CELL_H
#define CELL_H

#include <QObject>
#include <QJsonObject>

class Cell : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int row READ row WRITE setRow NOTIFY rowChanged)
    Q_PROPERTY(int col READ col WRITE setCol NOTIFY colChanged)
public:
    explicit Cell(int row=0, int col=0, QObject *parent = 0);
    Cell(const Cell &other);

    bool isBlack();
    static bool isBlack(int row,int col);

    int row() const{
        return m_row;
    }

    int col() const{
        return m_col;
    }

    bool isEmpty()const;
    bool belongsTo(QString player)const;

    Cell& operator=(const Cell &second);

    QJsonObject toJson();
signals:
    void rowChanged(uint);
    void colChanged(uint);

public slots:
    void setRow(int row);
    void setCol(int col);

    void swapMans(Cell &with);

    void putMan(QString rank, QString whoose);
    void purgeMan();

private:
    int m_row;
    int m_col;
    QJsonObject man;

};

#endif // CELL_H
