#include<bits/stdc++.h>
using namespace std;

class LogLevel{
    public:
    virtual string getName()=0;
    virtual int getLevel()=0;
    virtual ~LogLevel(){};
};


class InfoLevel:public LogLevel{
    public:
    string getName() override{
        return "Info";
    }

    int getLevel()override{
        return 1;
    }
};


class ErrorLevel:public LogLevel{
    public:
    string getName() override{
        return "Error";
    }

    int getLevel()override{
        return 2;
    }
};


class DebugLevel:public LogLevel{
    public:
    string getName() override{
        return "Debug";
    }

    int getLevel()override{
        return 3;
    }
};



class LogHandler{
    protected:
    LogHandler* nextHandler=nullptr;

    public:
    void setNext(LogHandler* next){
        nextHandler=next;
    }

    virtual void Handler(LogLevel* level,string &message){
        if(nextHandler){
            nextHandler->Handler(level,message);
        }
    }
    virtual ~LogHandler(){}
};


class consoleLogHandler:public LogHandler{
    int level=2;

    public:

    void Handler(LogLevel* level,string& message)override{
        if(level->getLevel()>=this->level)
        cout<<message<<endl;
        if (nextHandler) {
            nextHandler->Handler(level, message);
        }
    }
};


class FileLogHandler:public LogHandler{
    ofstream file;

    public:

    FileLogHandler(string filename){
        file.open(filename);

        if(!file.is_open()){
            cerr<<"Unable to open file: "<<filename<<endl;
        }
    }

    void Handler(LogLevel* level,string &message){
        if(file.is_open()){
            file<<message<<endl;
        }

        if(nextHandler){
            nextHandler->Handler(level,message);
        }
    }

    ~FileLogHandler(){
        if(file.is_open()){
            file.close();
        }
    }
};



class Logger{

    private:
    LogHandler* chainHead=nullptr;
    Logger(){};

    public:

    static Logger& getinstance(){
        static Logger instance;
        return instance;
    }

    // prevent copying 

    Logger(const Logger&)=delete;
    Logger& operator=(const Logger&)=delete;

    // set chain head 
    void setChain(LogHandler* head) {
        chainHead = head;
    }

    void log(LogLevel* level,string message) {
        if (chainHead)
        {
            chainHead->Handler(level, message);
        }
    }

};


int main()
{

    consoleLogHandler console;
    FileLogHandler file("Strange");
    console.setNext(&file);
    

    Logger::getinstance().setChain(&console);


    DebugLevel debug;
    InfoLevel info;
    ErrorLevel error;

    Logger::getinstance().log(&debug,"This is simple debug message");
    Logger::getinstance().log(&info,    "This is a simple info message.");

    return 0;
}