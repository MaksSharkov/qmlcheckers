#include "bot.h"

#include <QMap>
#include <functional>
#include <QDebug>


QVector<CheckersMove> BotUtils::getAllPlayersMoves(const QString &player, const CheckersSituation &situation)
{
    QVector<CheckersMove> result;
    CheckersBoard board = situation.board();
    bool mustEat = board.mustEat(player);

    QVector<Cell> cells=board.getPlayersCells(player);
    QMap<Cell,bool> moves;
    foreach(Cell const& cell,cells){
        moves = board.getAvailableMoves(cell);
        result += mapToVector(cell,moves,mustEat);
    }


    Cell testTo;
    Cell testFrom;
    for(int i = 0 ;i < result.size() ; i++){
        if(result[i].isEating()){
            CheckersSituation newSituation(board);
            newSituation.applyMove(result[i]);
            testTo=result[i].to();
            testFrom=result[i].from();
            testTo.swapMans(testFrom);
            result[i].setMustReadFuther(newSituation.board().canEat(testTo));
        }
    }
    return result;
}

CheckersMove BotUtils::getMoveFor(const Cell &from,const QString player,const CheckersBoard board)
{
    QVector<QVector<MoveSequence>> tree(analyzeDepth);

    MoveSequence initialMoves;
    if(from == Cell()){
        initialMoves=getAllPlayersMoves(player,board);
    }else{
        initialMoves=mapToVector(from,board.getAvailableMoves(from),board.mustEat(player));
    }
    foreach(const CheckersMove move,initialMoves){
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
            CheckersSituation currentSituation = CheckersSituation(board);
            currentSituation.applyMoves(previousSequence);
            MoveVariants currentVariants=getAllPlayersMoves(currentPlayer,currentSituation);
            foreach(const CheckersMove variant,currentVariants){
                MoveSequence currentSequence = MoveSequence(previousSequence);
                currentSequence.append(variant);
                tree[depth].append(currentSequence);
            }
        }
    }

    CheckersSituation initialSituation = CheckersSituation(board);
    initialSituation.sortByRate(tree.last(),player);

    for(int depth=0;depth<analyzeDepth;depth++){
        qDebug("Bot Tier %d variants of move:",depth);
        foreach(const MoveSequence sequence,tree[depth]){
            qDebug()<<toString(sequence);
        }
    }

    return tree.last().first().first();
}

QPair<Cell,Cell> BotUtils::getMove(const QString &player, const CheckersBoard &board)
{
    return getMoveFor(Cell(),player,board).toPair();
}

QPair<Cell,Cell> BotUtils::getMove(const Cell &cell,const CheckersBoard &board)
{
    return getMoveFor(cell,cell.man()["whoose"].toString(),board).toPair();
}


QVector<CheckersMove> BotUtils::mapToVector(const Cell from,const QMap<Cell,bool> moves,const bool eatMovesOnly)
{
    QVector<CheckersMove> result;
    foreach(const Cell to,moves.keys()){
        result.append(CheckersMove(from,to,moves.value(to)));
    }

    if(eatMovesOnly){
        foreach(const CheckersMove move,result){
            if(!move.isEating())
                result.removeOne(move);
        }
    }

    return result;
}

bool BotUtils::needSwitchTurn(const MoveSequence sequence)
{
    bool result=true;
    foreach(const CheckersMove move,sequence){
        if(!move.mustEatFuther())
            result=!result;
    }
    return result;
}
