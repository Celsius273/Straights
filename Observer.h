#ifndef _OBSERVER_
#define _OBSERVER_

class Subject;

enum Notification {
    NEWSTART, NEWROUND, WINNER, NONE //notifications for pop up dialogs
};

class Observer {
public:
    virtual void update(Notification n) = 0;
};

#endif