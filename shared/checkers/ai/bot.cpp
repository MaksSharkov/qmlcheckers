#include "bot.h"

#include <QMap>
#include <functional>
#include <QtAlgorithms>
#include <QDebug>

QVector<Move> BotUtils::getAllPlayersMoves(const QString &player, const ChessBoard &board)
{
    QVector<Move> result;
    bool mustEat = board.mustEat(player);

    QVector<Cell> cells=board.getPlayersCells(player);
    QMap<Cell,bool> moves;
    foreach(Cell const& cell,cells){
        moves = board.getAvailableMoves(cell);
        result += mapToVector(cell,moves,mustEat);
    }

    ChessBoard testBoard;
    Cell testTo;
    Cell testFrom;
    foreach(auto move,result){
        if(move.isEating()){
            testBoard=board;
            testBoard.applyMove(move);
            testTo=move.to();
            testFrom=move.from();
            testTo.swapMans(testFrom);
            move.setMustReadFuther(testBoard.canEat(testTo));
        }
    }
    return result;
}

Move BotUtils::getMoveFor(const Cell &from,const QString player,const ChessBoard board)
{
    QVector<QVector<MoveSequence>> tree(analyzeDepth);

    MoveSequence initialMoves;
    if(from == Cell()){
        initialMoves=getAllPlayersMoves(player,board);
    }else{
        initialMoves=mapToVector(from,board.getAvailableMoves(from),board.mustEat(player));
    }
    foreach(const Move move,initialMoves){
        MoveSequence newSequence;
        newSequence.append(move);
        tree[0].append(newSequence);
    }

    if(initialMoves.size() == 1)
        return initialMoves.first();

    QString currentPlayer = getEnemyPlayer(player);
    for(int depth = 1; depth<analyzeDepth; depth++){
        foreach(const MoveSequence previousSequence, tree.at(depth-1)){
            currentPlayer = (needSwitchTurn(previousSequence)) ?
                        player
                      : getEnemyPlayer(player) ;
            ChessBoard currentBoard = ChessBoard(board);
            currentBoard.applyMoves(previousSequence);
            MoveVariants currentVariants=getAllPlayersMoves(currentPlayer,currentBoard);
            foreach(const Move variant,currentVariants){
                MoveSequence currentSequence = MoveSequence(previousSequence);
                currentSequence.append(variant);
                tree[depth].append(currentSequence);
            }
        }
    }

    sortByRate(tree.last(),board);

    for(int depth=0;depth<analyzeDepth;depth++){
        qDebug("Bot Tier %d variants of move:",depth);
        foreach(const MoveSequence sequence,tree[depth]){
            qDebug()<<toString(sequence);
        }
    }

    return tree.last().first().first();
}

QPair<Cell,Cell> BotUtils::getMove(const QString &player, const ChessBoard &board)
{
    return getMoveFor(Cell(),player,board).toPair();
}

QPair<Cell,Cell> BotUtils::getMove(const Cell &cell,const ChessBoard &board)
{
    return getMoveFor(cell,cell.man()["whoose"].toString(),board).toPair();
}


QVector<Move> BotUtils::mapToVector(const Cell from,const QMap<Cell,bool> moves,const bool eatMovesOnly)
{
    QVector<Move> result;
    foreach(const Cell to,moves.keys()){
        result.append(Move(from,to,moves.value(to)));
    }

    if(eatMovesOnly){
        foreach(const Move move,result){
            if(!move.isEating())
                result.removeOne(move);
        }
    }

    return result;
}

bool BotUtils::needSwitchTurn(const MoveSequence sequence)
{
    bool result=true;
    foreach(const Move move,sequence){
        if(!move.mustEatFuther())
            result=!result;
    }
    return result;
}

QString BotUtils::getEnemyPlayer(const QString player)
{
    const QString enemyPlayer = (player == "topPlayer") ? "bottomPlayer" : "topPlayer";

    return enemyPlayer;
}

int BotUtils::getRate(const MoveSequence moves, const ChessBoard board)
{
    const QString player=moves.first().to().man()["whoose"].toString();
    ChessBoard newBoard=board;
    newBoard.applyMoves(moves);

    return getRate(newBoard,board,player);
}

int BotUtils::getRate(const ChessBoard board,const ChessBoard oldBoard,const QString player)
{
    Q_UNUSED(oldBoard)
    if(!board.hasMoves(getEnemyPlayer(player)))
        return 100;
    else
        return 0;
}

static ChessBoard boardForCompare;
int compareByRate(const void* first,const void* second)
{
    return BotUtils::getRate(*((const BotUtils::MoveSequence*)first),boardForCompare)
            - BotUtils::getRate(*((const BotUtils::MoveSequence*)second),boardForCompare);
}

void BotUtils::sortByRate(QVector<MoveSequence> &sequences,const ChessBoard board)
{
    boardForCompare=board;
    qsort(sequences.data(),sequences.size(),sizeof(MoveSequence),compareByRate);
}

QString BotUtils::toString(const MoveSequence sequence)
{
    QString result;
    foreach(const Move move,sequence)
        result +=move.toString();

    return result;
}
