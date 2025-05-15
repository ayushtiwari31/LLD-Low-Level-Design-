#include<bits/stdc++.h>
using namespace std;

// Design Challenges :
// 🧑‍💼Interviewer: What design challenges do you anticipate?
// 🧑‍💻Candidate: The key challenges for the Chess game will include:
// • Managing Game State: Ensuring the system accurately reflects the current state of the game, including player turns and board status.
// • Implementing Move Validation: Verifying that each move is legal and within the rules of the game.
// • Tracking Player Turns: Ensuring that players alternate turns correctly.
// • Detecting Game-Ending Conditions: Accurately identifying check, checkmate, and draw scenarios to conclude the game appropriately.

// Point 5: Approach : 
// 🧑‍💼Interviewer: How would you approach these challenges to ensure our game doesn't break?
// 🧑‍💻Candidate: To tackle the design challenges, I propose utilizing design patterns effectively. Here are the strategies which I am considering along with examples:
// 1. Strategy Pattern for Piece Movements : 
// ○ Define Different Movement Strategies: Implement specific movement logic for each type of piece (King, Queen, etc.).
// ○ Encapsulate Movement Logic: Make movement strategies interchangeable and easily extendable.
// 2. Singleton Pattern for Board : 
// ○ Ensure a Single Instance: Guarantee that only one instance of the Board class exists throughout the game.
// ○ Global Access Point: Provide a global point of access to the board instance.
// 3. Factory Pattern for Piece Creation : 
// ○ Create Pieces with a Consistent Interface: Use a factory to instantiate piece objects, ensuring they adhere to the Piece interface.
// ○ Enable Easy Addition of New Pieces: Allow seamless addition of new piece types without modifying existing code.
// 4. Manage Game State:
// Use an Enum to track the game state (e.g., ACTIVE, SAVED, BLACK_WIN, WHITE_WIN, STALEMATE).
class Cell;
class Board;

class Player{
    private:
    string name;
    bool isWhiteSide;

    public:
    Player(string name,bool f):name(name),isWhiteSide(f){};

    string getName(){
        return name;
    }

    bool isWhiteSide(){
        return isWhiteSide;
    }
};

enum class STATUS{
    ACTIVE,
    SAVED,
    BLACK_WIN,
    WHITE_WIN,
    STALEMATE
};

class MovementStrategy{
    public:
    ~MovementStrategy(){};
    virtual bool canMove(Board* board,Cell* startCell,Cell *endCell)=0;
};

class kingMovementStrategy:public MovementStrategy{
    public:
    bool canMove(Board *board,Cell* start,Cell*end)override{
        int startRow=start->getRow();
        int startCol=start->getCol();
        int endRow=end->getRow();
        int endCol=end->getCol();

        int rowDiff = abs(endRow - startRow);
        int colDiff = abs(endCol - startCol);

        return (rowDiff <= 1 && colDiff <= 1);
    }
};

class QueenMovementStrategy : public MovementStrategy {
public:
    bool canMove(Board* board, Cell* startCell, Cell* endCell) override {
        int startRow = startCell->getRow();
        int startCol = startCell->getCol();
        int endRow = endCell->getRow();
        int endCol = endCell->getCol();

        int rowDiff = std::abs(endRow - startRow);
        int colDiff = std::abs(endCol - startCol);

        // Diagonal, vertical, or horizontal move
        return (rowDiff == colDiff || startRow == endRow || startCol == endCol);
    }
};

class KnightMovementStrategy : public MovementStrategy {
public:
    bool canMove(Board* board, Cell* startCell, Cell* endCell) override {
        int startRow = startCell->getRow();
        int startCol = startCell->getCol();
        int endRow = endCell->getRow();
        int endCol = endCell->getCol();

        int rowDiff = std::abs(endRow - startRow);
        int colDiff = std::abs(endCol - startCol);

        // Valid knight move: 2 by 1 or 1 by 2
        return (rowDiff == 2 && colDiff == 1) || (rowDiff == 1 && colDiff == 2);
    }
};



// Factory Design Pattern for piece creation 

class Piece{
    private:
    bool isWhitePiece;
    bool killed=false;
    MovementStrategy* movementStrategy;

    public:
    Piece(bool f,MovementStrategy* movementStrategy):isWhitePiece(f),movementStrategy(movementStrategy){};

    ~Piece(){};

    bool isWhite(){
        return isWhitePiece;
    }

    bool isKilled(){
        return killed;
    }

    // Modifies the killed status
    void setKilled(bool status) {
        killed = status;
    }

    bool canMove(Board *board,Cell* start,Cell*end){
        return movementStrategy->canMove(board,start,end);
    }

    virtual char getType()=0; //to make Piece an abstract class 
};



class Bishop:public Piece{
    public:
    Bishop(bool isWhitePiece):Piece(isWhitePiece){};
    char getType()override{
        return 'B';
    }
};

class Knight : public Piece {
public:
    Knight(bool isWhitePiece) : Piece(isWhitePiece,new KnightMovementStrategy()) {}
    char getType() override { return 'N'; }
};

class Rook : public Piece {
public:
    Rook(bool isWhitePiece) : Piece(isWhitePiece) {}
    char getType() override { return 'R'; }
};

class Pawn : public Piece {
public:
    Pawn(bool isWhitePiece) : Piece(isWhitePiece) {}
    char getType() override { return 'P'; }
};

class Queen : public Piece {
public:
    Queen(bool isWhitePiece) : Piece(isWhitePiece,new QueenMovementStrategy()) {}
    char getType() override { return 'Q'; }
};

class King : public Piece {
public:
    King(bool isWhitePiece) : Piece(isWhitePiece,new kingMovementStrategy()) {}
    char getType() override { return 'K'; }
};


class PieceFactory{
    private:
    static string toLower(string s){
        string lower;
        for (char c : s) {
            lower += tolower(c);
        }
        return lower;
    }

    public:
    static Piece* createPiece(string pieceType,bool isWhitePiece){
        string type=toLower(pieceType);

        if(type=="king")return new King(isWhitePiece);
        if (type == "queen") return new Queen(isWhitePiece);
        if (type == "bishop") return new Bishop(isWhitePiece);
        if (type == "knight") return new Knight(isWhitePiece);
        if (type == "rook") return new Rook(isWhitePiece);
        if (type == "pawn") return new Pawn(isWhitePiece);

        throw invalid_argument("Unknown piece type: " + string(pieceType));

    }
};

// Singleton Pattern for Board:
// Before diving into the Board class, we will first focus on the Cell class, as the Cell class forms the foundation of our board. 
// Each cell on the board is represented by an instance of the Cell class, encapsulating 
// important attributes such as position and the piece occupying the cell.

class Cell{
    private:
    int row;
    int col;
    Piece*piece;
    string label;

    public:
    Cell(int row, int col, Piece* piece) : row(row), col(col), piece(piece) {}

    // Getter for the piece
    Piece* getPiece() {
        return piece;
    }

    // Setter for the piece
    void setPiece(Piece* newPiece) {
        piece = newPiece;
    }

    // Optional: you can also add row/col getters if needed
    int getRow() { return row; }
    int getCol() { return col; }

};

class Board{
    private:
    static Board * instance;
    vector<vector<Cell*>>board;

    Board(int rows){
        initializeBoard(rows);
    }

    void initializeBoard(int rows){
        board.resize(rows,vector<Cell*>(rows,nullptr));
        setPieceRow(0, true);              // White major pieces
        setPawnRow(1, rows, true);         // White pawns
        setPieceRow(rows - 1, false);      // Black major pieces
        setPawnRow(rows - 2, rows, false); // Black pawns

        // Empty cells
        for (int row = 2; row < rows - 2; ++row) {
            for (int col = 0; col < rows; ++col) {
                board[row][col] = new Cell(row, col, nullptr);
            }
        }
    }

    void setPieceRow(int row,bool isWhite){
        board[row][0]=new Cell(row,0,PieceFactory::createPiece("rook",isWhite));
        board[row][1]=new Cell(row,1,PieceFactory::createPiece("knight",isWhite));
        board[row][2] = new Cell(row, 2, PieceFactory::createPiece("bishop", isWhite));
        board[row][3] = new Cell(row, 3, PieceFactory::createPiece("queen", isWhite));
        board[row][4] = new Cell(row, 4, PieceFactory::createPiece("king", isWhite));
        board[row][5] = new Cell(row, 5, PieceFactory::createPiece("bishop", isWhite));
        board[row][6] = new Cell(row, 6, PieceFactory::createPiece("knight", isWhite));
        board[row][7] = new Cell(row, 7, PieceFactory::createPiece("rook", isWhite));
    }

    void setPawnRow(int row, int cols, bool isWhite) {
        for (int j = 0; j < cols; ++j) {
            board[row][j] = new Cell(row, j, PieceFactory::createPiece("pawn", isWhite));
        }
    }


    public:

    Board(const Board&)=delete;
    Board& operator=(const Board&) = delete;

    static Board * getInstance(int rows){
        if(instance==nullptr){
            instance= new Board(rows);
        }
        return instance;
    }

    Cell* getCell(int row, int col) const {
        return board[row][col];
    }

    // Destructor (to avoid memory leaks — optional if smart pointers are used)
    ~Board() {
        for (auto& row : board) {
            for (Cell* cell : row) {
                delete cell;
            }
        }
    }
};

Board*Board::instance=nullptr;



class Move {
private:
    Cell* startCell;
    Cell* endCell;

public:
    Move(Cell* start, Cell* end): startCell(start), endCell(end) {}

    bool isValid() {
        if (!startCell || !endCell || !startCell->getPiece() || !endCell->getPiece()) {
            return false;
        }
        return (startCell->getPiece()->isWhite()) != (endCell->getPiece()->isWhite());
    }

    Cell* getStartCell()  {
        return startCell;
    }

    Cell* getEndCell()  {
        return endCell;
    }
};

class Game{
    private:
    Board* board;
    Player* player1;
    Player* player2;
    bool isWhiteTurn;
    STATUS status;
    vector<Move> gameLog;
    

    public:
    Game(Player* p1, Player* p2): player1(p1), player2(p2), isWhiteTurn(true), status(STATUS::ACTIVE) {
        board = Board::getInstance(8); // Assuming 8x8 board
    }

    void start(Cell* startCell, Cell* endCell) {
        while (status == STATUS::ACTIVE) {
            Move move(startCell,endCell);
            if (isWhiteTurn) {
                makeMove(move, player1);
            } else {
                makeMove(move, player2);
            }
        }
    }

    void makeMove(Move& move, Player* player) {
        if (move.isValid()) {
            Piece* sourcePiece = move.getStartCell()->getPiece();
            if (sourcePiece->canMove(board, move.getStartCell(), move.getEndCell())) {
                Piece* destinationPiece = move.getEndCell()->getPiece();

                if (destinationPiece != nullptr) {
                    if (dynamic_cast<King*>(destinationPiece) != nullptr) {
                        status = isWhiteTurn ? STATUS::WHITE_WIN : STATUS::BLACK_WIN;
                        return;
                    }
                    destinationPiece->setKilled(true);
                }

                gameLog.push_back(move);
                move.getEndCell()->setPiece(sourcePiece);
                move.getStartCell()->setPiece(nullptr);
                isWhiteTurn = !isWhiteTurn;
            }
        }
    }

    STATUS getStatus() {
        return status;
    }

};



int main(){
    Player* player1 = new Player("Player1",true); // White
    Player* player2 = new Player("Player2",false); // Black

    // Initialize game
    Game chessGame(player1, player2);

    // You would normally pass actual Cell* objects here based on game state.
    // For now, passing nullptr as placeholders for startCell and endCell
    chessGame.start(nullptr, nullptr); // Replace nullptr with actual Cell* pointers as needed

    // Clean up
    delete player1;
    delete player2;
    return 0;
}