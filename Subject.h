#ifndef _SUBJECT_
#define _SUBJECT_
#include <set>
class Observer;

class Subject {
public:
    void subscribe(Observer*);
    void unsubscribe(Observer*);

protected:
    void notify();

private:
    typedef std::set< Observer* > Observers;
    Observers observers_;
}; // Subject


#endif