// The Snake & Food game is a classic arcade game where the player
// controls a snake moving on a grid, attempting to eat food items
// while avoiding collisions. The game is simple in concept but
// requires strategic movement to maximize the snake's growth without
//  running into walls or itself.

#include<bits/stdc++.h>
using namespace std;


//we are using singleton design pattern for designing game board
class GameBoard{
    private:
    int height;
    int width;
    GameBoard(int h,int w):height(h),width(w){};
    static GameBoard* board;
    public:

    GameBoard(const GameBoard&)=delete;
    GameBoard& operator=(const GameBoard&)=delete;
    
    static GameBoard* getInstance(int h,int w){
        if(board==nullptr)
        board= new GameBoard(h,w);
        return board;
    }

    int getWidth(){
        return width;
    }

    int getHeight(){
        return height;
    }

};


GameBoard* GameBoard::board = nullptr;


// we are using Factory Design pattern for creating different type of food;
class FoodItem{
    protected:
    int row;
    int col;
    int points;
    public:
    // Constructor

    FoodItem(int row, int column) : row(row), col(column) {}

    // Pure virtual destructor (makes this an abstract class)
    virtual ~FoodItem() = default;

     // Pure virtual function to enforce abstraction if needed
    virtual int getPoints() = 0;

    // Getter methods
    int getRow() { return row; }
    int getColumn() { return col; }
};

// Concrete class for normal food with 1 point
class NormalFood : public FoodItem {
public:
    NormalFood(int row, int column) : FoodItem(row, column) {
        points = 1;
    }

    int getPoints() override {
        return points;
    }
};

// Concrete class for bonus food with 3 points
class BonusFood : public FoodItem {
public:
    BonusFood(int row, int column) : FoodItem(row, column) {
        points = 3;
    }

    int getPoints() override {
        return points;
    }
};

class Snake{
    private:
    deque<pair<int,int>>body;
    map<pair<int,int>,bool>positionMap;
    public:
    Snake(){
        body.push_back({0,0});
        positionMap[{0,0}]=true;
    }
};

class FoodFactory{
    public:
    static FoodItem* createFood(int position[2],string type){
        if (type == "bonus") {
            return new BonusFood(position[0], position[1]);
        }
        return new NormalFood(position[0], position[1]); 
    }
};


// strategic design pattern for snake movement 

class MovementStrategy{
    public:
    virtual pair<int,int> getNextPosition(pair<int,int>currHead,string dir)=0;
    virtual ~MovementStrategy(){};
};

class HumanMovementStrategy : public MovementStrategy {
public:
    pair<int, int> getNextPosition(pair<int, int> currentHead, string direction) override {
        int row = currentHead.first;
        int col = currentHead.second;

        if (direction == "U") return {row - 1, col};
        if (direction == "D") return {row + 1, col};
        if (direction == "L") return {row, col - 1};
        if (direction == "R") return {row, col + 1};

        return currentHead; // No movement on invalid direction
    }
};

class AIMovementStrategy : public MovementStrategy {
public:
    pair<int, int> getNextPosition(pair<int, int> currentHead, string direction) override {
        // Simple placeholder AI: always moves right
        return {currentHead.first, currentHead.second + 1};
    }
};


// SnakeGame class
class SnakeGame {
private:
    GameBoard* board;
    deque<pair<int, int>> snake;
    map<pair<int, int>, bool> snakeMap;
    vector<vector<int>> food;
    int foodIndex;
    unique_ptr<MovementStrategy> movementStrategy;

public:
    SnakeGame(int width, int height, const vector<vector<int>>& food) 
        : food(food), foodIndex(0) {
        board = GameBoard::getInstance(width, height);
        pair<int, int> initialPos = {0, 0};
        snake.push_front(initialPos);
        snakeMap[initialPos] = true;
        movementStrategy = make_unique<HumanMovementStrategy>();
    }

    void setMovementStrategy(MovementStrategy* strategy) {
        movementStrategy.reset(strategy);
    }

    int move( string direction) {
        pair<int, int> currentHead = snake.front();
        pair<int, int> newHead = movementStrategy->getNextPosition(currentHead, direction);
        int newHeadRow = newHead.first;
        int newHeadCol = newHead.second;

        bool crossesBoundary = newHeadRow < 0 || newHeadRow >= board->getHeight() ||
                               newHeadCol < 0 || newHeadCol >= board->getWidth();

        pair<int, int> currentTail = snake.back();
        bool bitesItself = snakeMap.count(newHead) && newHead != currentTail;

        if (crossesBoundary || bitesItself) {
            return -1;
        }

        bool ateFood = (foodIndex < food.size()) &&
                       (food[foodIndex][0] == newHeadRow) &&
                       (food[foodIndex][1] == newHeadCol);

        if (ateFood) {
            foodIndex++;
        } else {
            snakeMap.erase(currentTail);
            snake.pop_back();
        }

        snake.push_front(newHead);
        snakeMap[newHead] = true;

        return static_cast<int>(snake.size()) - 1;
    }
};

int main(){
    return 0;
}