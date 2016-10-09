#include "checkersgame.h"

CheckersGame::CheckersGame(QObject *parent)
    : QObject(parent)
    , m_board(8,this)
{
    connect(&m_board,SIGNAL(manMoved(Cell&,Cell&))
            ,this,SLOT(onManMoved(Cell&,Cell&)));
}

void CheckersGame::setTopPlayer(QString username)
{
    if(topPlayerUsername != username)
    {
        topPlayerUsername = username;
        emit topPlayerChanged(topPlayerUsername);
    }
}

void CheckersGame::setBottomPlayer(QString username)
{
    if(bottomPlayerUsername != username)
    {
        bottomPlayerUsername = username;
        emit bottomPlayerChanged(bottomPlayerUsername);
    }
}

void CheckersGame::onReplyReceived(QWebSocket *client, QJsonObject reply)
{
    Q_UNUSED(client)
    if(reply["type"]=="moveMan"){
        QJsonObject from=reply["from"].toObject();
        QJsonObject to=reply["to"].toObject();
        int rowFrom=from["row"].toInt();
        int colFrom=from["col"].toInt();
        int rowTo=to["row"].toInt();
        int colTo=to["col"].toInt();
        m_board.moveMan(rowFrom,colFrom
                ,rowTo,colTo);
    }
}

void CheckersGame::onManMoved(Cell &from, Cell &to)
{
    QJsonObject message;
    message["type"]="moveMan";
    message["from"]=from.toJson();
    message["to"]=to.toJson();

    qDebug()<<"Man moved from"<<message["from"]<<" to "<<message["to"];
    emit notifyAbout(message);
}

void CheckersGame::initializeGame()
{
    const int initRowCount=3;
    const int boardSize=m_board.boardSize();

    m_board.clearFromMans();

    int col;
    for(int row=boardSize-1;row>boardSize-1-initRowCount;row--)
        for(col=row%2;col<boardSize;col+=2)
            m_board.addMan(row,col,"man","topPlayer");

    for(int row=0;row<initRowCount;row++)
        for(col=row%2;col<boardSize;col+=2)
            m_board.addMan(row,col,"man","bottomPlayer");

    QJsonObject message;
    message["type"]="gameInit";
    message["board"]=m_board.toJson();
    message["boardSize"]=m_board.boardSize();

    qDebug()<<"GameInitialized";
    notifyAbout(message);
}

void CheckersGame::onClientAdded(QWebSocket* client,QString username)
{
    Q_UNUSED(client)
    if(topPlayerUsername.isEmpty()){
        setTopPlayer(username);
        initializeGame();//DEBUGGING ONLY!!!
    }else if(bottomPlayer().isEmpty()){
        setBottomPlayer(username);
        initializeGame();
    }
}

void CheckersGame::onClientRemoved(QWebSocket *client, QString username)
{
    Q_UNUSED(client)
    if(topPlayer() == username){
        setTopPlayer("");
        endGame(bottomPlayer());
    }else if(bottomPlayer() == username){
        setBottomPlayer("");
        endGame(topPlayer());
    }
}

void CheckersGame::endGame(QString winnersUsername)
{
    qDebug()<<"Game ended. Winner:"+winnersUsername;
    m_board.clearFromMans();
    emit gameEnded(winnersUsername);
}
