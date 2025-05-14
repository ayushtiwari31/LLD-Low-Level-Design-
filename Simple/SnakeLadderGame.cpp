#include<bits/stdc++.h>
using namespace std;


// player class 

class Player{
    public:
    string name;
    int position;

    public:
    Player(string name):name(name),position(0){};

    void move(int steps){
        position+=steps;
    }

    void setNewPosition(int newPosition){
        position=newPosition;
    }
};

class Board{
    const int BoardSize=100;
    map<int,int>snakes;
    map<int,int>ladders;

    public:
    Board(){
         // Initialize ladders (start -> end)
        ladders[2]  = 38;
        ladders[7]  = 14;
        ladders[8]  = 31;
        ladders[15] = 26;
        ladders[21] = 42;
        ladders[28] = 84;
        ladders[36] = 44;
        ladders[51] = 67;
        ladders[71] = 91;
        ladders[78] = 98;
        ladders[87] = 94;

        // Initialize snakes (start -> end)
        snakes[16] = 6;
        snakes[46] = 25;
        snakes[49] = 11;
        snakes[62] = 19;
        snakes[64] = 60;
        snakes[74] = 53;
        snakes[89] = 68;
        snakes[92] = 88;
        snakes[95] = 75;
        snakes[99] = 80;
    }

    int getBoardSize(){
        return BoardSize;
    }

    int getTransition(int pos){

        if(snakes.count(pos)) return snakes[pos];
        if(ladders.count(pos)) return ladders[pos];

        return pos;
    }

};


class Game{
    Board board;
    vector<Player*>players;
    int currPlayerIndex;
    bool gameOver;

    int rollDice() {
        return (rand() % 6) + 1;
    }

    public:

    Game():currPlayerIndex(0),gameOver(false){};

    void addPlayer(Player* player){
        players.push_back(player);
    }

    void play(){
        if(players.size()==0){
            cout<<"No Player was added in the game"<<endl;
            return;
        }

        while(!gameOver){
            Player* player=players[currPlayerIndex];
            cout << "\n" << player->name << "'s turn. Currently at position " << player->position <<endl;
            cout << "Press enter to roll the dice...";
            cin.ignore();

            // Roll the dice.
            int diceValue = rollDice();
            std::cout << player->name << " rolled a " << diceValue << endl;

             // Calculate the intended new position.
            int intendedPos = player->position + diceValue;

            cout << player->name << " moves from " << player->position << " to " << intendedPos << std::endl;
                // Check for snake or ladder transition
                int finalPos = board.getTransition(intendedPos);
                player->setNewPosition(finalPos);

                std::cout << player->name << " is now at position " << player->position << std::endl;

                // Check win condition.
                if(player->position >= board.getBoardSize()) {
                    std::cout << "\n" << player->name << " wins the game!" << std::endl;
                    gameOver = true;
                    break;
                }

                  // Move to the next player.
            currPlayerIndex =    ( currPlayerIndex + 1) % players.size();
        }

    }

};

int main(){

    Game game;

    
    int numPlayers;
    cout << "Enter number of players: ";
    cin >> numPlayers;
    
    // Clear the newline left in the stream
    cin.ignore();

    for (int i = 0; i < numPlayers; ++i) {
        std::string playerName;
        std::cout << "Enter name for player " << (i + 1) << ": ";
        std::getline(std::cin, playerName);
        game.addPlayer(&Player(playerName));
    }

    // Begin the game.
    game.play();

    return 0;
}