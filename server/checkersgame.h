#ifndef CHECKERSCOORDINATOR_H
#define CHECKERSCOORDINATOR_H

#include "checkers/chessboard.h"

#include <QObject>
#include <QWebSocket>
#include <QJsonObject>

class CheckersGame : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString topPlayer READ topPlayer WRITE setTopPlayer NOTIFY topPlayerChanged)
    Q_PROPERTY(QString bottomPlayer READ bottomPlayer WRITE setBottomPlayer NOTIFY bottomPlayerChanged)

public:
    CheckersGame(QObject *parent=0);

    QString topPlayer(){
        return topPlayerUsername;
    }
    QString bottomPlayer(){
        return bottomPlayerUsername;
    }

signals:
    void notifyAbout(QJsonObject message);
    void topPlayerChanged(QString player);
    void bottomPlayerChanged(QString player);
    void gameEnded(QString winnersUsername);

public slots:
    void onReplyReceived(QWebSocket *client,QJsonObject reply);
    void onClientAdded(QWebSocket* client,QString username);
    void onClientRemoved(QWebSocket* client,QString username);

    void setTopPlayer(QString username);
    void setBottomPlayer(QString username);

    void initializeGame();
private slots:
    void onManMoved(Cell &from, Cell &to);
    void endGame(QString winnersUsername);
private:
    ChessBoard m_board;

    QString topPlayerUsername;
    QString bottomPlayerUsername;
};

#endif // CHECKERSCOORDINATOR_H
