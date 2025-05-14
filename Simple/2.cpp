#include<bits/stdc++.h>
using namespace std;


class Observer;

class WeatherForecast{

    private:
    float temp;
    vector<Observer*>list;

    public:

    void addObserver(Observer* node){
        list.push_back(node);
    }

    void removeObserver(Observer* node){
        list.erase(find(list.begin(),list.end(),node));
    }

    void notifyObserver();

    void setTemperature(int temp){
        this->temp=temp;
        notifyObserver();
    }

    float getTemperature(){
        return temp;
    }
};

class Observer{
    public:
    virtual void update(float temp)=0;

    virtual ~Observer(){}
};


class TVDisplay:public Observer{
    public:
    void update(float temp)override{
        cout<<"TVDisplay Temperature is : "<<temp<<endl;
    }
};

void WeatherForecast::notifyObserver(){
    for(Observer * obj:list){
        obj->update(temp);
    }
}

int main (){

    WeatherForecast forecast;
    TVDisplay tv;

    forecast.addObserver(&tv);
    forecast.setTemperature(10);

   return 0; 
}