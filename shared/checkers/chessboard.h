#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <QAbstractListModel>
#include <QVector>
#include <QJsonArray>
#include <QJsonObject>

#include "checkers/cell.h"

class ChessBoard : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int boardSize READ boardSize NOTIFY boardSizeChanged)
public:
    explicit ChessBoard(int boardSize=8, QObject *parent = 0);
    enum RoleNames {
        RowRole = Qt::UserRole+1
        ,ColRole
        ,ManRole
    };

    Q_INVOKABLE void initialize(QJsonArray board,int boardSize);
    Q_INVOKABLE void initializeWithEmpty(int boardSize);
    Q_INVOKABLE bool isMoveCorrect(const Cell &from,const Cell &to)const;
    Q_INVOKABLE bool isMoveCorrect(const QJsonObject &from, const QJsonObject &to)const;

    int boardSize()const{
        return m_boardSize;
    }

    QJsonArray toJson()const;
    Q_INVOKABLE int indexOf(int row,int col);
    Q_INVOKABLE QJsonObject get(int index);

    virtual int rowCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
signals:
    void manMoved(Cell &from,Cell &to);
    void boardSizeChanged(int);
public slots:
    void addMan(int row, int col, QString rank, QString player);

    void moveMan(Cell &from, Cell &to);
    void moveMan(int rowFrom, int colFrom, int rowTo, int colTo);
    void moveMan(QJsonObject from,QJsonObject to);

    void clearFromMans();
    void clear();
    void append(const QJsonObject& cell);

protected:
    virtual QHash<int, QByteArray> roleNames() const override;
private slots:
    void handleManChanged();
    void setBoardSize(int boardSize);
private:
    Cell *cellAt(int row,int col);
    void initRoles();

private:
    QVector<Cell> m_board;
    int m_boardSize;

    QHash<int, QByteArray> m_roleNames;

};

#endif // CHESSBOARD_H
