#pragma once

#include <algorithm>
#include <iterator>
#include <list>
#include <string>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

typedef void (*MenuCallback_t)();

class MenuItem {
 private:
    std::string name_;
    MenuItem* parent_ = NULL;
    std::list<MenuItem*> submenuList_;
    std::list<MenuItem*>* prevSubmenuList_;

    TaskHandle_t taskHandle = NULL;

    static void menuTask(void* arg);

    MenuItem* searchSubmenu(std::string name);

 public:
    MenuItem(std::string name, MenuCallback_t callback, MenuItem* parent);
    ~MenuItem();

    MenuCallback_t callback_ = NULL;

    MenuItem* addSubmenu(std::string name, MenuCallback_t callback);
    bool removeSubmenu(std::string name);
    void setPrevSubmenu(std::list<MenuItem*>* submenu) { prevSubmenuList_ = submenu; }
    std::list<MenuItem*>* getPrevSubmenu() { return prevSubmenuList_; }

    std::list<MenuItem*>* getSubmenu() { return &submenuList_; }
    std::string getName() { return name_; }
    MenuItem* getParent() { return parent_; }

    bool runApp();
    bool stopApp();
};