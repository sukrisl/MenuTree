#pragma once

#include <algorithm>
#include <iterator>
#include <list>
#include <string>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

typedef enum {
    MENU_TYPE_CONTAINER = 0,
    MENU_TYPE_APP,
} MenuType_t;

typedef void (*MenuCallback_t)(int8_t inc);

class MenuItem {
 private:
    MenuType_t type_;
    std::string name_;
    MenuItem* parent_ = NULL;
    std::list<MenuItem*> submenuList_;
    std::list<MenuItem*>* prevSubmenuList_;

    TaskHandle_t taskHandle = NULL;
    QueueHandle_t queueHandle = NULL;

    static void menuTask(void* arg);

    MenuItem* searchSubmenu(std::string name);

 public:
    MenuItem(MenuType_t type, std::string name, MenuCallback_t callback, MenuItem* parent);
    ~MenuItem();

    MenuCallback_t callback_ = NULL;

    MenuItem* addSubmenu(MenuType_t type, std::string name, MenuCallback_t callback);
    bool removeSubmenu(std::string name);
    void setPrevSubmenu(std::list<MenuItem*>* submenu) { prevSubmenuList_ = submenu; }
    std::list<MenuItem*>* getPrevSubmenu() { return prevSubmenuList_; }

    std::list<MenuItem*>* getSubmenu() { return &submenuList_; }
    std::string getName() { return name_; }
    MenuItem* getParent() { return parent_; }
    MenuType_t getType() { return type_; }

    void increment();
    void decrement();

    bool runApp();
    bool stopApp();
};