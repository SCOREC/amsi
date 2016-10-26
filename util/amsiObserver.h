#ifndef AMSI_OBSERVER_H_
#define AMSI_OBSERVER_H_
#include <set>
namespace amsi
{
  enum Event
  {
    DELETED = 0,
    SYNCHRONIZED = 1
  };
  class Subject;
  class Observer
  {
  public:
    virtual void onNotify(Subject*, Event) = 0;
  };
  class Subject
  {
  private:
    std::set<Observer*> observers;
  protected:
    Subject() : observers() {}
    ~Subject()
    {
      notify(DELETED);
    }
    void notify(Event event)
    {
      for(std::set<Observer*>::iterator it = observers.begin();
          it != observers.end();
          it++)
        (*it)->onNotify(this, event);
    }
  public:
    void addObserver(Observer * o)
    {
      observers.insert(o);
    }
    void removeObserver(Observer * observer)
    {
      std::set<Observer*>::iterator it = observers.find(observer);
      if(it != observers.end())
        observers.erase(it);
    }
  };
}
#endif
