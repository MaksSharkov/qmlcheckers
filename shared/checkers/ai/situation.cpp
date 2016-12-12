#include "situation.h"

#include <QtAlgorithms>

CheckersSituation::CheckersSituation(const CheckersBoard board)
    : m_board(board)
{

}

void CheckersSituation::applyMove(const CheckersMove move)
{

    if(move != CheckersMove()){
        m_board.applyMove(m_board.m_board[m_board.indexOf(move.from().row(),move.from().col())]
                ,m_board.m_board[m_board.indexOf(move.to().row(),move.to().col())]);
    }

}

void CheckersSituation::applyMoves(const QVector<CheckersMove> moves)
{
    foreach(const CheckersMove move,moves)
        applyMove(move);
}

int CheckersSituation::getRate(const QVector<CheckersMove> moves)
{
    const QString player=moves.first().to().man()["whoose"].toString();
    CheckersSituation newSituation= CheckersSituation(m_board);
    newSituation.applyMoves(moves);

    return getRate(newSituation.m_board,player);
}

CheckersBoard CheckersSituation::board()const
{
    return m_board;
}

int CheckersSituation::getRate(const CheckersSituation &newSituation, const QString &player) const
{
    const QString enemy= getEnemyPlayer(player);
    const CheckersBoard newBoard = newSituation.board();

    if(!newBoard.hasMoves(getEnemyPlayer(player)))
        return 100;
    else{
        const int playerDifference = m_board.getPlayersCells(player).size()
                - newBoard.getPlayersCells(player).size();
        const int enemyDifference = m_board.getPlayersCells(enemy).size()
                - newBoard.getPlayersCells(enemy).size();

        return playerDifference - enemyDifference;
    }
}

static CheckersSituation *situationForCompare;
static QString playerForCompare = QString();
int compareByRate(const void* first,const void* second)
{
    CheckersSituation firstSituation = *situationForCompare;
    CheckersSituation secondSituation = *situationForCompare;

    firstSituation.applyMoves(*((const MoveSequence*)first));
    secondSituation.applyMoves(*((const MoveSequence*)second));

    return situationForCompare->getRate(firstSituation,playerForCompare)
            - situationForCompare->getRate(secondSituation,playerForCompare);
}

void CheckersSituation::sortByRate(QVector<MoveSequence> &sequences,const QString player)
{
    situationForCompare=this;
    playerForCompare = player;
    qsort(sequences.data(),sequences.size(),sizeof(MoveSequence),compareByRate);
}
