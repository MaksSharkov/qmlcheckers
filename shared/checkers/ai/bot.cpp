#include "bot.h"

QPair<Cell,Cell> BotUtils::getMove(const QString &player, const ChessBoard &board)
{
    QPair<Cell,Cell> result;
    bool mustEat = board.mustEat(player);

    QVector<Cell> cells=board.getPlayersCells(player);
    QMap<Cell,bool> moves;
        foreach(Cell const& cell,cells){
            moves = board.getAvailableMoves(cell);
            if(moves.values().contains(mustEat)){
                result.first=cell;
                result.second=moves.key(mustEat);
                break;
            }
        }

    return result;
}

QPair<Cell,Cell> BotUtils::getMove(const Cell &cell,const ChessBoard &board)
{
     QMap<Cell,bool> moves=board.getAvailableMoves(cell);
     QPair<Cell,Cell> result;

     result.first = cell;
     result.second = moves.key(true);
     return result;
}
