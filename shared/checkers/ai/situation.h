#include "checkers/chessboard.h"

#include "move.h"

class CheckersSituation
{
public:
    CheckersSituation(const CheckersBoard board = CheckersBoard());

    void applyMove(const CheckersMove move);
    void applyMoves(const QVector<CheckersMove> moves);
    int getRate(const QVector<CheckersMove> moves);
    int getRate(const CheckersSituation &newSituation, const QString &player) const;

    void sortByRate(QVector<MoveSequence> &sequences, const QString player);

    CheckersBoard board()const;
private:
    CheckersBoard m_board;


};
