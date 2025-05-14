#include<bits/stdc++.h>
using namespace std;

class Board;
//creating board;

enum class Symbol{
    EMPTY,
    O,
    X
};

struct Position{
    int row,col;
   
    Position(int r,int c):row(r),col(c){};
};

//using Strategy design pattern 
// Benefits of using the Strategy Pattern:
// • Easily add new player strategies, such as AI or networked players.
// • No need to change the existing codebase.

class PlayerStrategy{
    // Allows different player strategies to be used interchangeably without
    // modifying client code.
    public:
    virtual Position makeMove(Board &board)=0;
    virtual ~PlayerStrategy(){};
};




// we are using state design  pattern for game state management
// The State Pattern is a behavioral design pattern that allows an object to alter its behavior when
//  its internal state changes. In the context of game state management, the GameState enum defines
//  the various states a game can be in, such as IN_PROGRESS, X_WON, O_WON, and DRAW. Each state has
//  an associated boolean value indicating whether the game is over.

class GameContext;

//state interface
class GameState{
    public:
    virtual void next(GameContext *context)=0;
    virtual bool isGameOver()=0;
    virtual ~GameState(){};
};

//context class
class GameContext{
    private:
    GameState *currentState;

    public :
    GameContext(GameState *initialState):currentState(initialState){};

    void setState(GameState* state){
        delete currentState;
        currentState=state;
    }

    void next(){
        currentState->next(this);
    }

    bool isGameOver(){
        return currentState->isGameOver();
    }

    GameState* getCurrentState() {
        return currentState;
    }

    ~GameContext() {
        delete currentState;
    }
};

//concrete state: Xturn State
class XTurnState : public GameState {
public:
    void next(GameContext* context) override;
    bool isGameOver() override {
        return false;
    }
};

// Concrete State: OTurnState
class OTurnState : public GameState {
public:
    void next(GameContext* context) override;
    bool isGameOver() override {
        return false;
    }
};

// Switching logic
void XTurnState::next(GameContext* context) {
    context->setState(new OTurnState());
}

void OTurnState::next(GameContext* context) {
    context->setState(new XTurnState());
}

// Concrete State: XWonState
class XWonState : public GameState {
public:
    void next(GameContext* context) override {
        // Game is over, do nothing
    }
    bool isGameOver() override {
        return true;
    }
};

// Concrete State: OWonState
class OWonState : public GameState {
public:
    void next(GameContext* context) override {
        // Game is over, do nothing
    }
    bool isGameOver() override {
        return true;
    }
};

// The GameState interface defines the contract for different game states, each implementing specific behavior.
//  This encapsulates state-specific logic, providing a clean way to manage game states.




class Board{
    private:
    int rows;
    int cols;
    vector<vector<Symbol>>grid;

    bool isWinningLine(const vector<Symbol>&line){
        Symbol first = line[0];
        for (const Symbol& s : line) {
            if (s != first) return false;
        }
        return true;
    };

    public:
    Board(int r,int c):rows(r),cols(c),grid(r,vector<Symbol>(c,Symbol::EMPTY)){};

    bool isValidMove(const Position& pos) {
        return pos.row >= 0 && pos.row < rows &&
               pos.col >= 0 && pos.col < cols &&
               grid[pos.row][pos.col] == Symbol::EMPTY;
    }

    void makeMove(const Position& pos, Symbol symbol) {
        grid[pos.row][pos.col] = symbol;
    }



    void checkGameState(GameContext* context){
        // Check rows
    for (int i = 0; i < rows; i++) {
        if (grid[i][0] != Symbol::EMPTY) {
            if (isWinningLine(grid[i])) {
                GameState* state = (grid[i][j] == Symbol::X) ? new XWonState() : new OWonState();
                context->setState(state);
                // Switch to win state if needed
                return;
            }
        }
    }

    // Check columns
    for (int col = 0; col < cols; col++) {
        vector<Symbol> column;
        for (int row = 0; row < rows; row++) {
            column.push_back(grid[row][col]);
        }
        if (column[0] != Symbol::EMPTY && isWinningLine(column)) {
            context->next(); // Switch to win state if needed
            return;
        }
    }

    // Check diagonals
    vector<Symbol> diag1, diag2;
    for (int i = 0; i < min(rows, cols); i++) {
        diag1.push_back(grid[i][i]);
        diag2.push_back(grid[i][cols - 1 - i]);
    }

    if (diag1[0] != Symbol::EMPTY && isWinningLine(diag1)) {
        context->next();
        return;
    }

    if (diag2[0] != Symbol::EMPTY && isWinningLine(diag2)) {
        context->next();
        return;
    }

    }


    void printBoard() const {
    cout << "\nCurrent Board:\n\n";

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            char displayChar;
            switch (grid[i][j]) {
                case Symbol::X: displayChar = 'X'; break;
                case Symbol::O: displayChar = 'O'; break;
                case Symbol::EMPTY: displayChar = ' '; break;
            }

            cout << " " << displayChar << " ";
            if (j < cols - 1)
            cout << "|";
        }
        cout << "\n";
        if (i < rows - 1) {
            for (int j = 0; j < cols; ++j) {
                cout << "---";
                if (j < cols - 1)
                    cout << "+";
            }
            cout << "\n";
        }
    }

    cout << "\n";
}

};



class HumanPlayerStrategy:public PlayerStrategy{
    private:
    string playerName;

    public:
    HumanPlayerStrategy(string name):playerName(name){};

    Position makeMove(Board&board) override{
        while(true){
            cout<<playerName<<" Enter Your Move (row [0-2] and column [0-2]): ";
            int row,col;
            if(cin>>row>>col){
                Position move(row,col);
                if(board.isValidMove(move)){
                    return move;
                }else{
                    cout<<"Invalid Move. Please try again";
                }
            }else{
                cout << "Invalid input. Please enter numbers.\n";
                cin.clear();               // clear error flag
                cin.ignore(1000, '\n');    // discard invalid input
            }
        }
    }
};




class Player{
    private:
    Symbol symbol;
    PlayerStrategy *strategy;

    public:
    Player(Symbol sym,PlayerStrategy *str):symbol(sym),strategy(str){};

    Position makeMove(Board& board) {
        return strategy->makeMove(board);
    }

    Symbol getSymbol() const {
        return symbol;
    }
};

class TicTacToe{
  private:
  Board board;
  Player *playerx;
  Player *playero;
  Player *currentPlayer;
  GameContext gameContext;

  public:
  TicTacToe(PlayerStrategy *x,PlayerStrategy *o,int rows,int cols):
  board(rows,cols),gameContext(new XTurnState()){
    playero=new Player(Symbol::O,o);
    playerx=new Player(Symbol::X,x);
    currentPlayer=playerx;
  }

  ~TicTacToe(){
    delete playerx;
    delete playero;
  }

  void play(){
    do{
        board.printBoard();
        Position move=currentPlayer->makeMove(board);
        board.makeMove(move,currentPlayer->getSymbol());
        board.checkGameState(&gameContext);
        switchPlayer();
    }while(!gameContext.isGameOver());

    announceResult();
  }

  private:
  void switchPlayer(){
    currentPlayer=(currentPlayer == playerx) ? playero : playerx;
  }

  void announceResult(){

// dynamic_cast is used to safely downcast a base class pointer (GameState*) to a derived class pointer (XWonState* or OWonState*).
// If the cast is invalid (i.e. state is not actually pointing to an XWonState), it returns nullptr.
    
    GameState* state=gameContext.getCurrentState();
    if (dynamic_cast<XWonState*>(state)) {
    cout << "Player X wins!" << std::endl;
    } else if (dynamic_cast<OWonState*>(state)) {
    cout << "Player O wins!" << std::endl;
    } else {
    cout << "It's a draw!" << std::endl;
    }
  }

};

int main(){
    PlayerStrategy* playerXStrategy = new HumanPlayerStrategy("Player X");
    PlayerStrategy* playerOStrategy = new HumanPlayerStrategy("Player O");

    TicTacToe game(playerXStrategy, playerOStrategy,3,3);
    game.play();

    delete playerXStrategy;
    delete playerOStrategy;
    return 0;
}

// This low-level design for Tic-Tac-Toe showcases a well-structured and scalable architecture,
//  emphasizing modularity and extensibility. By supporting various enhancements such as custom board sizes and multiple players, 
// this design ensures maintainability and flexibility. In an interview setting, presenting this
//  design would demonstrate your ability to create robust and adaptable solutions, highlighting your proficiency in applying design
//  patterns and best practices.