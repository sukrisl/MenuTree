#pragma once

#include <algorithm>
#include <iterator>
#include <list>
#include <string>

typedef void (*MenuCallback_t)();

class MenuItem {
 private:
    std::string name_;
    MenuItem* parent_ = NULL;
    std::list<MenuItem*> submenuList_;

    std::list<MenuItem*>* prevSubmenuList_;

    MenuCallback_t callback_ = NULL;

    MenuItem* searchSubmenu(std::string name);

 public:
    MenuItem(std::string name, MenuCallback_t callback, MenuItem* parent);
    ~MenuItem();

    MenuItem* addSubmenu(std::string name, MenuCallback_t callback);
    bool removeSubmenu(std::string name);

    void setPrevSubmenu(std::list<MenuItem*>* submenu) { prevSubmenuList_ = submenu; }

    std::list<MenuItem*>* getSubmenu() { return &submenuList_; }
    std::list<MenuItem*>* getPrevSubmenu() { return prevSubmenuList_; }
    std::string getName() { return name_; }
    MenuItem* getParent() { return parent_; }
};