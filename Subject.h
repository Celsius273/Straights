#ifndef _SUBJECT_
#define _SUBJECT_
#include <set>
#include "Observer.h"
class Observer;

class Subject {
public:
    void subscribe(Observer*);
    void unsubscribe(Observer*);

protected:
    void notify(Notification);

private:
    typedef std::set< Observer* > Observers;
    Observers observers_;
}; // Subject


#endif