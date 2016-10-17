#include "checkersgame.h"

#include "checkers/ai/bot.h"

CheckersGame::CheckersGame(QObject *parent)
    : QObject(parent)
    , m_board(8,this)
    , m_isBottomPlayerTurnNow(false)
{
    connect(&m_board,SIGNAL(manMoved(QString,Cell&,Cell&,bool))
            ,this,SLOT(onManMoved(QString,Cell&,Cell&,bool)));
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

void CheckersGame::onReplyReceived(QWebSocket *client, QJsonObject reply, QString username)
{
    Q_UNUSED(client)
    if(reply["type"]=="moveMan"){
        QJsonObject from=reply["from"].toObject();
        QJsonObject to=reply["to"].toObject();

        QString player="";
        if(!m_isBottomPlayerTurnNow && (username == topPlayer()))
            player="topPlayer";
        else if(m_isBottomPlayerTurnNow && (username == bottomPlayer()))
            player="bottomPlayer";
        else
            return;

        if(!player.isEmpty())
            m_board.moveMan(player,from,to);

    }else if(reply["type"]=="requestBot"){
        if(username == bottomPlayer() && topPlayer().isEmpty()){
            setTopPlayer("bot");
            connect(&m_board,SIGNAL(manMoved(QString,Cell&,Cell&,bool))
                       ,this,SLOT(handleBotsTurn(QString,Cell&,Cell&,bool)));

            connect(this,SIGNAL(gameEnded(QString)),this,SLOT(removeBot()));
            initializeGame();
        }

    }
}

void CheckersGame::onManMoved(QString player, Cell &from, Cell &to, bool giveTurnToNext)
{
    if(giveTurnToNext)
        m_isBottomPlayerTurnNow=!m_isBottomPlayerTurnNow;

    QJsonObject message;
    message["type"]="moveMan";
    message["from"]=from.toJson();
    message["to"]=to.toJson();
    message["player"]=player;
    message["switchTurn"]=giveTurnToNext;

    qDebug()<<"Man moved :"<<from.name()<<" - "<<to.name()<<endl;
    emit notifyAbout(message);
}

QJsonObject CheckersGame::getBoardInfo()
{
    QJsonObject message;
    message["type"]="gameInit";
    message["board"]=m_board.toJson();
    message["boardSize"]=m_board.boardSize();
    message["topPlayer"]=topPlayerUsername;
    message["bottomPlayer"]=bottomPlayerUsername;

    return message;
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

    QJsonObject message = getBoardInfo();
    m_isBottomPlayerTurnNow=true;
    notifyAbout(message);
    qDebug()<<"GameInitialized";

}

void CheckersGame::onClientAdded(QWebSocket* client,QString username)
{
    Q_UNUSED(client)
    if(bottomPlayer().isEmpty()){
        setBottomPlayer(username);
        return;
    }else if(topPlayer().isEmpty()){
        setTopPlayer(username);
        initializeGame();
    }else{
        //tell observer about game state
        QJsonObject message = getBoardInfo();
        emit sendReply(client,message);
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

    QJsonObject message;
    message["type"]="gameEnded";
    message["winner"]=winnersUsername;
    notifyAbout(message);
    emit gameEnded(winnersUsername);
}

void CheckersGame::handleBotsTurn(QString player,Cell &from,Cell &to,bool switchTurn)
{
    Q_UNUSED(from)
    Q_UNUSED(to)
    if(player!= "topPlayer" && switchTurn){
        qDebug()<<"Bot's turn now."<<endl;

        QPair<Cell,Cell> move = BotUtils::getMove("topPlayer",m_board);
        m_board.moveMan("topPlayer",move.first.toJson(),move.second.toJson());
    }else if(player=="topPlayer" && !switchTurn){
        //continious move
        QPair<Cell,Cell> move = BotUtils::getEatMove(to,m_board);
        m_board.moveMan("topPlayer",move.first.toJson(),move.second.toJson());
    }

}

void CheckersGame::removeBot()
{
    if(topPlayer() == "bot"){
        disconnect(&m_board,SIGNAL(manMoved(QString,Cell&,Cell&,bool))
                   ,this,SLOT(handleBotsTurn(QString,Cell&,Cell&,bool)));

        disconnect(this,SIGNAL(gameEnded(QString)),this,SLOT(removeBot()));
    }
}

