#include<bits/stdc++.h>
using namespace std;


class Pizza{
    public:
    virtual void getDescription()=0;
    virtual double getCost()=0;
    virtual ~Pizza(){};
};

class PlainPizza:public Pizza{
    public:
    void getDescription()override{
        cout<<"Your order is plain Pizza"<<endl;
    }

    double getCost()override{
        return 100;
    }
};

class CheesePizza:public Pizza{
    public:
    void getDescription()override{
        cout<<"Your order is Cheese Pizza"<<endl;
    }

    double getCost()override{
        return 100;
    }
};


class Topping_a:public Pizza{
    private:
    Pizza* pizza;

    public:

    Topping_a(Pizza* a):pizza(a){}

    void getDescription()override{
        pizza->getDescription();
        cout<<"Additionaly you have added toppings_a"<<endl;
    }

    double getCost()override{
        return pizza->getCost()+20;
    }

};

int main (){

    PlainPizza pizza;

    pizza.getDescription();
    cout<<pizza.getCost()<<endl;


    Topping_a a(&pizza);

    cout<<a.getCost()<<endl;
    a.getDescription();

   

   return 0; 
}