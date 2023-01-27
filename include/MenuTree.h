#pragma once

#include "MenuItem.h"

class MenuTree {
 private:
    MenuItem* root_;

    std::list<MenuItem*>* activeList_;
    std::list<MenuItem*>::iterator menuPos_;
    MenuItem* selected_;

    void printMenu(std::list<MenuItem*>* list, uint8_t depth);

 public:
    MenuTree();
    ~MenuTree() {}

    bool load();

    MenuItem* create(std::string name, MenuCallback_t callback, MenuItem* parent = NULL);
    bool remove(MenuItem* item);

    MenuItem* getActive() { return selected_; }

    MenuItem* next();
    MenuItem* prev();
    MenuItem* enter();
    MenuItem* back();

    void printAllMenu();
};