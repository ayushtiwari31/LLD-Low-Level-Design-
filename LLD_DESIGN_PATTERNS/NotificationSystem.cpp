#include<bits/stdc++.h>
using namespace std;

//abstract class for Notification sender type
class NotificationSender{
    public:
    virtual void send(const string &to,const string &message)=0;
    virtual ~NotificationSender(){};
};

class Email:public NotificationSender{
    void send(const string &to ,const string &message)override{
        cout<<"Sending message through email "<<to<<endl;
    }
};

class SMS:public NotificationSender{
    void send(const string &to ,const string &message)override{
        cout<<"Sending message through SMS "<<to<<endl;
    }
};

class PushMessage:public NotificationSender{
    void send(const string &to ,const string &message)override{
        cout<<"Sending message through Push Method "<<to<<endl;
    }
};


//abstract class for the type of notification

class Notification{
    protected:
    NotificationSender* sender;

    public:
    Notification(NotificationSender*sender):sender(sender){};
    virtual ~Notification(){};

    virtual void NotifyUser(const string&to,const string &message)=0;
};

class AlertNotification:public Notification{
    
    public:
    AlertNotification(NotificationSender*sender):Notification(sender){}
    void NotifyUser(const string &to,const string&message)override{
        cout<<"Sending Alert Notification"<<endl;
        sender->send(to,message);
    }
};

class ReminderNotification:public Notification{
    
    public:
    ReminderNotification(NotificationSender*sender):Notification(sender){};
    void NotifyUser(const string &to,const string&message)override{
        cout<<"Sending Reminder Notification"<<endl;
        sender->send(to,message);
    }
};


int main(){
    Email email;
    AlertNotification alert(&email);
    alert.NotifyUser("ayush@gmail.com","hello");


    return 0;
}