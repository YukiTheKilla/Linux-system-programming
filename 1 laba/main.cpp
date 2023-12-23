#include <iostream>
#include <cstdlib>
#include <ctime>
#include <climits>
#include <cstring>
#include <utility>
#include <vector>

using namespace std;

const int boardSize = 8;

struct BestMoveInfo {
    int value;
    pair<int, int> endCoordinates;
};

void initializeBoard(vector<vector<char>>& board) {
    for (int i = 0; i < boardSize; ++i) {
        for (int j = 0; j < boardSize; ++j) {
            board[i][j] = '_';
        }
    }
}

void printBoard(const vector<vector<char>>& board) {
    cout << "   A B C D E F G H\n";
    for (int i = 0; i < boardSize; ++i) {
        cout << i + 1 << ".|";
        for (int j = 0; j < boardSize; ++j) {
            cout << board[i][j] << '|';
        }
        cout << '\n';
    }
}

void placeRandomFigures(vector<vector<char>>& board, int numKings, int numQueens, int numBishops, int numKnights, int numRooks, int numPawns) {
    srand(static_cast<unsigned>(time(0)));

    // Place White Kings
    for (int i = 0; i < numKings; ++i) {
        int row = rand() % boardSize;
        int col = rand() % boardSize;
        while (board[row][col] != '_') {
            row = rand() % boardSize;
            col = rand() % boardSize;
        }
        board[row][col] = 'K';  // Use a different character for white Kings, e.g., 'K'
    }

    // Place Black Kings
    for (int i = 0; i < numKings; ++i) {
        int row = rand() % boardSize;
        int col = rand() % boardSize;
        while (board[row][col] != '_') {
            row = rand() % boardSize;
            col = rand() % boardSize;
        }
        board[row][col] = 'k';
    }

    // Place White Queens
    for (int i = 0; i < numQueens; ++i) {
        int row = rand() % boardSize;
        int col = rand() % boardSize;
        while (board[row][col] != '_') {
            row = rand() % boardSize;
            col = rand() % boardSize;
        }
        board[row][col] = 'Q';
    }

    // Place Black Queens
    for (int i = 0; i < numQueens; ++i) {
        int row = rand() % boardSize;
        int col = rand() % boardSize;
        while (board[row][col] != '_') {
            row = rand() % boardSize;
            col = rand() % boardSize;
        }
        board[row][col] = 'q';
    }

    // Place White Bishops
    for (int i = 0; i < numBishops; ++i) {
        int row = rand() % boardSize;
        int col = rand() % boardSize;
        while (board[row][col] != '_') {
            row = rand() % boardSize;
            col = rand() % boardSize;
        }
        board[row][col] = 'B';
    }

    // Place Black Bishops
    for (int i = 0; i < numBishops; ++i) {
        int row = rand() % boardSize;
        int col = rand() % boardSize;
        while (board[row][col] != '_') {
            row = rand() % boardSize;
            col = rand() % boardSize;
        }
        board[row][col] = 'b';
    }

    // Place White Knights
    for (int i = 0; i < numKnights; ++i) {
        int row = rand() % boardSize;
        int col = rand() % boardSize;
        while (board[row][col] != '_') {
            row = rand() % boardSize;
            col = rand() % boardSize;
        }
        board[row][col] = 'N';
    }

    // Place Black Knights
    for (int i = 0; i < numKnights; ++i) {
        int row = rand() % boardSize;
        int col = rand() % boardSize;
        while (board[row][col] != '_') {
            row = rand() % boardSize;
            col = rand() % boardSize;
        }
        board[row][col] = 'n';
    }

    // Place White Rooks
    for (int i = 0; i < numRooks; ++i) {
        int row = rand() % boardSize;
        int col = rand() % boardSize;
        while (board[row][col] != '_') {
            row = rand() % boardSize;
            col = rand() % boardSize;
        }
        board[row][col] = 'R';
    }

    // Place Black Rooks
    for (int i = 0; i < numRooks; ++i) {
        int row = rand() % boardSize;
        int col = rand() % boardSize;
        while (board[row][col] != '_') {
            row = rand() % boardSize;
            col = rand() % boardSize;
        }
        board[row][col] = 'r';
    }

    // Place White Pawns
    for (int i = 0; i < numPawns; ++i) {
        int row = rand() % boardSize;
        int col = rand() % boardSize;
        while (board[row][col] != '_') {
            row = rand() % boardSize;
            col = rand() % boardSize;
        }
        board[row][col] = 'P';
    }

    // Place Black Pawns
    for (int i = 0; i < numPawns; ++i) {
        int row = rand() % boardSize;
        int col = rand() % boardSize;
        while (board[row][col] != '_') {
            row = rand() % boardSize;
            col = rand() % boardSize;
        }
        board[row][col] = 'p';
    }
}


// Function to evaluate the chess board
int evaluateBoard(const vector<vector<char>>& board) {
    // Add your own evaluation function based on piece values
    // For simplicity, we'll use a basic evaluation here
    int evaluation = 0;

    for (size_t i = 0; i < board.size(); ++i) {
        for (size_t j = 0; j < board[i].size(); ++j) {
            switch (board[i][j]) {
                case 'K': 
                    if (i > 0 && board[i - 1][j] == 'K' || 
                        i < board.size() - 1 && board[i + 1][j] == 'K' || 
                        j > 0 && board[i][j - 1] == 'K' || 
                        j < board[i].size() - 1 && board[i][j + 1] == 'K') {
                        break;
                    }
                    evaluation -= 10; 
                    break;
                case 'Q': evaluation += 9; break;
                case 'B': evaluation += 3; break;
                case 'N': evaluation += 3; break;
                case 'R': evaluation += 5; break;
                case 'P': evaluation += 1; break;
                case 'k': 
                    if (i > 0 && board[i - 1][j] == 'k' || 
                        i < board.size() - 1 && board[i + 1][j] == 'k' || 
                        j > 0 && board[i][j - 1] == 'k' || 
                        j < board[i].size() - 1 && board[i][j + 1] == 'k') {
                        break;
                    }
                    evaluation -= 10; 
                    break;
                case 'q': evaluation -= 9; break;
                case 'b': evaluation -= 3; break;
                case 'n': evaluation -= 3; break;
                case 'r': evaluation -= 5; break;
                case 'p': evaluation -= 1; break;
                default: break;
            }
        }
    }

    return evaluation;
}


// Minimax function
int minimax(const vector<vector<char>>& board, int depth, bool maximizingPlayer) {
    if (depth == 0) {
        return evaluateBoard(board);
    }

    int bestValue;
    if (maximizingPlayer) {
        bestValue = INT_MIN;
        // You need to consider all possible moves, not just capturing moves
        for (size_t i = 0; i < board.size(); ++i) {
            for (size_t j = 0; j < board[i].size(); ++j) {
                // Simulate a move
                // For simplicity, this example only considers pawn moves (you should extend this for all pieces)
                if (board[i][j] == 'P') {
                    // Simulate a pawn move
                    vector<vector<char>> tempBoard = board;
                    tempBoard[i][j] = '_';
                    size_t newRow = i + 1;  // For simplicity, let's move the pawn one row forward
                    tempBoard[newRow][j] = 'P';

                    int moveValue = minimax(tempBoard, depth - 1, false);
                    bestValue = max(bestValue, moveValue);
                }
            }
        }
    } else {
        bestValue = INT_MAX;
        // Similarly, you would simulate opponent's moves here
        // For simplicity, we'll skip this in the example
    }

    return bestValue;
}


string chessNotation(int row, int col) {
    return string(1, char('A' + col)) + to_string(row + 1);
}
// Function to check if the king is safe in the given board position
bool isKingSafe(const vector<vector<char>>& board, int kingRow, int kingCol, bool isWhite) {
    // Check threats from enemy bishops and queens along diagonals
    int bishopMoves[4][2] = {
        {-1, -1}, {-1, 1},
        {1, -1}, {1, 1}
    };

    for (int i = 0; i < 4; ++i) {
        for (int j = 1; j < boardSize; ++j) {
            int threatRow = kingRow + j * bishopMoves[i][0];
            int threatCol = kingCol + j * bishopMoves[i][1];

            if (threatRow >= 0 && threatRow < boardSize && threatCol >= 0 && threatCol < boardSize) {
                char threatPiece = board[threatRow][threatCol];
                if ((isWhite && (threatPiece == 'b' || threatPiece == 'q')) ||
                    (!isWhite && (threatPiece == 'B' || threatPiece == 'Q'))) {
                    return false; // King is under threat from a bishop or queen
                }
                if (threatPiece != '_') {
                    break; // Stop checking in this direction if a piece is encountered
                }
            } else {
                break; // Stop checking in this direction if outside the board bounds
            }
        }
    }

    // Check threats from enemy rooks and queens along ranks and files
    int rookMoves[4][2] = {
        {-1, 0}, {1, 0},
        {0, -1}, {0, 1}
    };

    for (int i = 0; i < 4; ++i) {
        int threatRow = kingRow;
        int threatCol = kingCol;

        while (true) {
            threatRow += rookMoves[i][0];
            threatCol += rookMoves[i][1];

            if (threatRow < 0 || threatRow >= boardSize || threatCol < 0 || threatCol >= boardSize) {
                break; // Stop if we reach the board edge
            }

            char threatPiece = board[threatRow][threatCol];
            if ((isWhite && (threatPiece == 'r' || threatPiece == 'q')) ||
                (!isWhite && (threatPiece == 'R' || threatPiece == 'Q'))) {
                return false; // King is under threat from a rook or queen
            }

            if (threatPiece != '_') {
                break; // Stop checking in this direction if a piece is encountered
            }
        }
    }
    

    // Check threats from enemy knights
    int knightMoves[8][2] = {
        {-2, -1}, {-2, 1},
        {-1, -2}, {-1, 2},
        {1, -2}, {1, 2},
        {2, -1}, {2, 1}
    };

    for (int i = 0; i < 8; ++i) {
        int threatRow = kingRow + knightMoves[i][0];
        int threatCol = kingCol + knightMoves[i][1];

        if (threatRow >= 0 && threatRow < boardSize && threatCol >= 0 && threatCol < boardSize) {
            char threatPiece = board[threatRow][threatCol];
            if ((isWhite && threatPiece == 'n') || (!isWhite && threatPiece == 'N')) {
                return false; // King is under threat from a knight
            }
        }
    }

    // Check threats from enemy pawns
    int pawnMoves[2][2] = {
        {-1, -1}, {-1, 1}
    };

    int pawnDirection = (isWhite) ? -1 : 1; // Direction in which pawns move
    for (int i = 0; i < 2; ++i) {
        int threatRow = kingRow + pawnDirection * pawnMoves[i][0];
        int threatCol = kingCol + pawnDirection * pawnMoves[i][1];

        if (threatRow >= 0 && threatRow < boardSize && threatCol >= 0 && threatCol < boardSize) {
            char threatPiece = board[threatRow][threatCol];
            if ((isWhite && threatPiece == 'p') || (!isWhite && threatPiece == 'P')) {
                return false; // King is under threat from a pawn
            }
        }
    }

    // Check threats from enemy kings
    int kingMoves[8][2] = {
        {-1, -1}, {-1, 0}, {-1, 1},
        {0, -1},           {0, 1},
        {1, -1}, {1, 0}, {1, 1}
    };

    for (int i = 0; i < 8; ++i) {
        int threatRow = kingRow + kingMoves[i][0];
        int threatCol = kingCol + kingMoves[i][1];

        if (threatRow >= 0 && threatRow < boardSize && threatCol >= 0 && threatCol < boardSize) {
            char threatPiece = board[threatRow][threatCol];
            if ((isWhite && threatPiece == 'k') || (!isWhite && threatPiece == 'K')) {
                return false; // King is under threat from an enemy king
            }
        }
    }

    // If no threats found, the king is safe
    return true;
}

pair<int, pair<int, int>> findBestPawnMove(vector<vector<char>>& board, int startRow, int startCol) {
    int bestMoveValue = INT_MIN;
    pair<int, int> bestMoveEndCoordinates = {-1, -1};

    // Determine the direction of pawn movement based on its color
    int direction = (islower(board[startRow][startCol])) ? 1 : -1;

    // Check the two possible moves for a pawn: one square forward and two squares forward on its first move
    for (int i = 1; i <= 2; ++i) {
        int endRow = startRow + direction * i;

        // Check if the destination is within the bounds of the board
        if (endRow >= 0 && endRow < static_cast<int>(board.size())) {
            // Check if the destination square is empty
            if (board[endRow][startCol] == '_') {
                // Update the board with the potential move
                vector<vector<char>> tempBoard = board;
                tempBoard[startRow][startCol] = '_';
                tempBoard[endRow][startCol] = board[startRow][startCol];

                // Evaluate the move using the overall board evaluation function
                int moveValue = evaluateBoard(tempBoard);

                // Update the best move if the current move is better
                if (moveValue > bestMoveValue) {
                    bestMoveValue = moveValue;
                    bestMoveEndCoordinates = {endRow, startCol};
                }

                // If the pawn has moved two squares, it cannot move two squares again
                if (i == 2) {
                    break;
                }
            } else {
                break;  // Stop further movement if the square is not empty
            }
        }
    }

    // Check possible captures for the pawn
    vector<vector<int>> pawnCaptureMoves = {{-1, -1}, {-1, 1}};  // Diagonal capture moves for white pawn
    if (direction == -1) {
        // Adjust for black pawn
        pawnCaptureMoves[0][0] = 1;
        pawnCaptureMoves[1][0] = 1;
    }

    for (const auto& captureMove : pawnCaptureMoves) {
        int endRow = startRow + captureMove[0];
        int endCol = startCol + captureMove[1];

        // Check if the capture is within the bounds of the board
        if (endRow >= 0 && endRow < static_cast<int>(board.size()) && endCol >= 0 && endCol < static_cast<int>(board[startRow].size())) {
            // Check if the capture square contains an opponent's piece
            if ((islower(board[startRow][startCol]) && isupper(board[endRow][endCol])) ||
                (isupper(board[startRow][startCol]) && islower(board[endRow][endCol]))) {
                // Update the board with the potential move
                vector<vector<char>> tempBoard = board;
                tempBoard[startRow][startCol] = '_';
                tempBoard[endRow][endCol] = board[startRow][startCol];

                // Evaluate the move using the overall board evaluation function
                int moveValue = evaluateBoard(tempBoard);

                // Update the best move if the current move is better
                if (moveValue > bestMoveValue) {
                    bestMoveValue = moveValue;
                    bestMoveEndCoordinates = {endRow, endCol};
                }
            }
        }
    }

    return {bestMoveValue, bestMoveEndCoordinates};
}

pair<int, pair<int, int>> findBestKnightMove(vector<vector<char>>& board, int startRow, int startCol) {
    int bestMoveValue = INT_MIN;
    pair<int, int> bestMoveEndCoordinates = {-1, -1};

    // Possible knight moves relative to the starting position
    vector<vector<int>> knightMoves = {
        {-2, -1}, {-2, 1},
        {-1, -2}, {-1, 2},
        {1, -2}, {1, 2},
        {2, -1}, {2, 1}
    };

    for (const auto& move : knightMoves) {
        int endRow = startRow + move[0];
        int endCol = startCol + move[1];

        // Check if the destination is within the bounds of the board
        if (endRow >= 0 && endRow < static_cast<int>(board.size()) && endCol >= 0 && endCol < static_cast<int>(board[startRow].size())) {
            // Check if the destination square is either empty or contains an opponent's piece
            if (board[endRow][endCol] == '_' || (islower(board[startRow][startCol]) && isupper(board[endRow][endCol])) ||
                (isupper(board[startRow][startCol]) && islower(board[endRow][endCol]))) {
                // Evaluate the move
                int captureCount = 0;

                // Check captures for the current move
                for (const auto& captureMove : knightMoves) {
                    int captureRow = endRow + captureMove[0];
                    int captureCol = endCol + captureMove[1];

                    // Check if the capture is within the bounds of the board
                    if (captureRow >= 0 && captureRow < static_cast<int>(board.size()) && captureCol >= 0 && captureCol < static_cast<int>(board[startRow].size())) {
                        // Check if the capture square contains an opponent's piece
                        if ((islower(board[endRow][endCol]) && isupper(board[captureRow][captureCol])) ||
                            (isupper(board[endRow][endCol]) && islower(board[captureRow][captureCol]))) {
                            captureCount++;
                        }
                    }
                }

                // Update the best move if the current move is better
                if (captureCount > bestMoveValue) {
                    bestMoveValue = captureCount;
                    bestMoveEndCoordinates = {endRow, endCol};
                }
            }
        }
    }

    return {bestMoveValue, bestMoveEndCoordinates};
}

pair<int, pair<int, int>> findBestBishopMove(vector<vector<char>>& board, int startRow, int startCol) {
    int bestMoveValue = INT_MIN;
    pair<int, int> bestMoveEndCoordinates = {-1, -1};

    // Possible bishop moves: diagonally in any direction
    vector<vector<int>> bishopMoves = {
        {-1, -1}, {-1, 1},
        {1, -1}, {1, 1}
    };

    for (const auto& move : bishopMoves) {
        for (int j = 1; j < static_cast<int>(board.size()); ++j) {
            int endRow = startRow + j * move[0];
            int endCol = startCol + j * move[1];

            // Check if the destination is within the bounds of the board
            if (endRow >= 0 && endRow < static_cast<int>(board.size()) && endCol >= 0 && endCol < static_cast<int>(board[startRow].size())) {
                // Check if the destination square is either empty or contains an opponent's piece
                if (board[endRow][endCol] == '_' || (islower(board[startRow][startCol]) && isupper(board[endRow][endCol])) ||
                    (isupper(board[startRow][startCol]) && islower(board[endRow][endCol]))) {
                    // Update the board with the potential move
                    vector<vector<char>> tempBoard = board;
                    tempBoard[startRow][startCol] = '_';
                    tempBoard[endRow][endCol] = board[startRow][startCol];

                    // Evaluate the move using the overall board evaluation function
                    int moveValue = evaluateBoard(tempBoard);

                    // Update the best move if the current move is better
                    if (moveValue > bestMoveValue) {
                        bestMoveValue = moveValue;
                        bestMoveEndCoordinates = {endRow, endCol};
                    }

                    // If the destination is not empty, stop further movement in this direction
                    if (board[endRow][endCol] != '_') {
                        break;
                    }
                } else {
                    // If the destination contains a piece of the same color, stop further movement in this direction
                    break;
                }
            }
        }
    }

    return {bestMoveValue, bestMoveEndCoordinates};
}

pair<int, pair<int, int>> findBestRookMove(vector<vector<char>>& board, int startRow, int startCol) {
    int bestMoveValue = INT_MIN;
    pair<int, int> bestMoveEndCoordinates = {-1, -1};

    // Possible rook moves: horizontally or vertically
    vector<vector<int>> rookMoves = {
        {-1, 0}, {1, 0},
        {0, -1}, {0, 1}
    };

    for (const auto& move : rookMoves) {
        int endRow = startRow;
        int endCol = startCol;

        while (true) {
            endRow += move[0];
            endCol += move[1];

            // Check if the destination is within the bounds of the board
            if (endRow < 0 || endRow >= static_cast<int>(board.size()) || endCol < 0 || endCol >= static_cast<int>(board[startRow].size())) {
                break;  // Stop if we reach the board edge
            }

            // Check if the destination square is either empty or contains an opponent's piece
            if (board[endRow][endCol] == '_' || (islower(board[startRow][startCol]) && isupper(board[endRow][endCol])) ||
                (isupper(board[startRow][startCol]) && islower(board[endRow][endCol]))) {
                // Update the board with the potential move
                vector<vector<char>> tempBoard = board;
                tempBoard[startRow][startCol] = '_';
                tempBoard[endRow][endCol] = board[startRow][startCol];

                // Evaluate the move using the overall board evaluation function
                int moveValue = evaluateBoard(tempBoard);

                // Update the best move if the current move is better
                if (moveValue > bestMoveValue) {
                    bestMoveValue = moveValue;
                    bestMoveEndCoordinates = {endRow, endCol};
                }

                // If the destination contains a piece, stop further movement in this direction
                if (board[endRow][endCol] != '_') {
                    break;
                }
            } else {
                // If the destination contains a piece of the same color, stop further movement in this direction
                break;
            }
        }
    }

    return {bestMoveValue, bestMoveEndCoordinates};
}

pair<int, pair<int, int>> findBestKingMove(vector<vector<char>>& board, int startRow, int startCol) {
    int bestMoveValue = INT_MIN;
    pair<int, int> bestMoveEndCoordinates = {-1, -1};

    // Possible king moves: one square in any direction
    vector<vector<int>> kingMoves = {
        {-1, -1}, {-1, 0}, {-1, 1},
        {0, -1},           {0, 1},
        {1, -1}, {1, 0}, {1, 1}
    };

    for (const auto& move : kingMoves) {
        int endRow = startRow + move[0];
        int endCol = startCol + move[1];

        // Check if the destination is within the bounds of the board
        if (endRow >= 0 && endRow < static_cast<int>(board.size()) && endCol >= 0 && endCol < static_cast<int>(board[startRow].size())) {
            // Check if the destination square is either empty or contains an opponent's piece
            if (board[endRow][endCol] == '_' || (islower(board[startRow][startCol]) && isupper(board[endRow][endCol])) ||
                (isupper(board[startRow][startCol]) && islower(board[endRow][endCol]))) {
                // Update the board with the potential move
                vector<vector<char>> tempBoard = board;
                tempBoard[startRow][startCol] = '_';
                tempBoard[endRow][endCol] = board[startRow][startCol];

                // Check if the king is safe after the move
                if (isKingSafe(tempBoard, endRow, endCol, isupper(board[startRow][startCol]))) {
                    // Evaluate the move using the overall board evaluation function
                    int moveValue = evaluateBoard(tempBoard);

                    // Update the best move if the current move is better
                    if (moveValue > bestMoveValue) {
                        bestMoveValue = moveValue;
                        bestMoveEndCoordinates = {endRow, endCol};
                    }
                }
            }
        }
    }

    return {bestMoveValue, bestMoveEndCoordinates};
}

// Function to move a pawn on the board
void makePawnMove(vector<vector<char>>& board, int startRow, int startCol, int endRow, int endCol) {
    char piece = board[startRow][startCol];
    board[startRow][startCol] = '_';
    board[endRow][endCol] = piece;
}

// Function to move a knight on the board
void makeKnightMove(vector<vector<char>>& board, int startRow, int startCol, int endRow, int endCol) {
    char piece = board[startRow][startCol];
    board[startRow][startCol] = '_';
    board[endRow][endCol] = piece;
}

// Function to move a bishop on the board
void makeBishopMove(vector<vector<char>>& board, int startRow, int startCol, int endRow, int endCol) {
    char piece = board[startRow][startCol];
    board[startRow][startCol] = '_';
    board[endRow][endCol] = piece;
}

// Function to move a rook on the board
void makeRookMove(vector<vector<char>>& board, int startRow, int startCol, int endRow, int endCol) {
    char piece = board[startRow][startCol];
    board[startRow][startCol] = '_';
    board[endRow][endCol] = piece;
}

// Function to move a king on the board
void makeKingMove(vector<vector<char>>& board, int startRow, int startCol, int endRow, int endCol) {
    char piece = board[startRow][startCol];
    board[startRow][startCol] = '_';
    board[endRow][endCol] = piece;
}


void makeBestMove(vector<vector<char>>& board) {
    int bestOverallMoveValue = INT_MIN;
    int bestOverallMoveStartRow = -1;
    int bestOverallMoveStartCol = -1;
    int bestOverallMoveEndRow = -1;
    int bestOverallMoveEndCol = -1;

    for (size_t startRow = 0; startRow < board.size(); ++startRow) {
        for (size_t startCol = 0; startCol < board[startRow].size(); ++startCol) {
            char piece = board[startRow][startCol];
            if (piece != '_') {
                pair<int, pair<int, int>> currentMoveInfo = {0, {-1, -1}};  // Initialize to a default value

                switch (piece) {
                    case 'P':
                        currentMoveInfo = findBestPawnMove(board, startRow, startCol);
                        break;
                    case 'N':
                        currentMoveInfo = findBestKnightMove(board, startRow, startCol);
                        break;
                    case 'B':
                        currentMoveInfo = findBestBishopMove(board, startRow, startCol);
                        break;
                    case 'R':
                        currentMoveInfo = findBestRookMove(board, startRow, startCol);
                        break;
                    case 'K':
                        currentMoveInfo = findBestKingMove(board, startRow, startCol);
                        break;
                    default:
                        break;
                }

                // Update the best overall move if the current move is better
                if (currentMoveInfo.first > bestOverallMoveValue) {
                    bestOverallMoveValue = currentMoveInfo.first;
                    bestOverallMoveStartRow = static_cast<int>(startRow);
                    bestOverallMoveStartCol = static_cast<int>(startCol);
                    bestOverallMoveEndRow = currentMoveInfo.second.first;
                    bestOverallMoveEndCol = currentMoveInfo.second.second;
                }
            }
        }
    }

    // Make the best overall move
    if (bestOverallMoveStartRow != -1 && bestOverallMoveStartCol != -1 &&
        bestOverallMoveEndRow != -1 && bestOverallMoveEndCol != -1) {
        // Call the appropriate move function based on the piece type
        char piece = board[bestOverallMoveStartRow][bestOverallMoveStartCol];
        switch (piece) {
            case 'P':
                makePawnMove(board, bestOverallMoveStartRow, bestOverallMoveStartCol, bestOverallMoveEndRow, bestOverallMoveEndCol);
                break;
            case 'N':
                makeKnightMove(board, bestOverallMoveStartRow, bestOverallMoveStartCol, bestOverallMoveEndRow, bestOverallMoveEndCol);
                break;
            case 'B':
                makeBishopMove(board, bestOverallMoveStartRow, bestOverallMoveStartCol, bestOverallMoveEndRow, bestOverallMoveEndCol);
                break;
            case 'R':
                makeRookMove(board, bestOverallMoveStartRow, bestOverallMoveStartCol, bestOverallMoveEndRow, bestOverallMoveEndCol);
                break;
            case 'K':
                makeKingMove(board, bestOverallMoveStartRow, bestOverallMoveStartCol, bestOverallMoveEndRow, bestOverallMoveEndCol);
                break;
            default:
                break;
        }

        // Output the best overall move
        cout << "Made the Best Overall Move: " << chessNotation(bestOverallMoveStartRow, bestOverallMoveStartCol)
                  << " -> " << chessNotation(bestOverallMoveEndRow, bestOverallMoveEndCol) << endl;
    } else {
        cout << "No valid move found." << endl;
    }
}


int main() {
    vector<vector<char>> chessBoard(boardSize, vector<char>(boardSize));
    initializeBoard(chessBoard);

    int numKings, numQueens, numBishops, numKnights, numRooks, numPawns;
    numKings = 1;
    numQueens = 0;
    numBishops = 2;
    numKnights = 2;
    numRooks = 2;
    numPawns = 8;

    placeRandomFigures(chessBoard, numKings, numQueens, numBishops, numKnights, numRooks, numPawns);

    cout << "Initial Chess Board:\n";
    printBoard(chessBoard);

    int numberOfMoves = 3;
    for (int move = 0; move < numberOfMoves; ++move) {
        makeBestMove(chessBoard);
    }
    printBoard(chessBoard);

    return 0;
}
