#pragma once

#include "MenuItem.h"

class MenuTree {
 private:
    std::list<MenuItem*> list_;
    std::list<MenuItem*>* activeList_ = &list_;
    MenuItem* selected_ = NULL;
    std::list<MenuItem*>::iterator menuPos_;

    void printMenu(std::list<MenuItem*>* list, uint8_t depth);

 public:
    MenuTree() {}
    ~MenuTree() {}

    MenuItem* create(std::string name, MenuCallback_t callback, MenuItem* parent = NULL);
    bool remove(MenuItem* item);

    MenuItem* getActive();

    MenuItem* next();
    MenuItem* prev();
    MenuItem* enter();
    MenuItem* back();

    void printAllMenu();
};