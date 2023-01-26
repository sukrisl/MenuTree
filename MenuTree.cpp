#include "esp_log.h"

#include "MenuTree.h"

static const char* TAG = "menu_struct";

MenuItem* MenuTree::create(std::string name, MenuCallback_t callback, MenuItem* parent) {
    if (parent != NULL) {
        std::string submenuName = parent->getName() + "/" + name;
        return parent->addSubmenu(submenuName, callback);
    }

    MenuItem* item = new MenuItem(name, callback, NULL);
    list_.push_back(item);
    return item;
}

bool MenuTree::remove(MenuItem* item) {
    if (item == NULL) return false;
    delete item;
    return true;
}

MenuItem* MenuTree::getActive() {
    if (selected_ == NULL) {
        selected_ = *(activeList_->begin());
        menuPos_ = activeList_->begin();
    }
    
    selected_->runApp();
    return selected_; 
}

MenuItem* MenuTree::next() {
    selected_->stopApp();

    std::list<MenuItem*>::iterator nextPos = std::next(menuPos_);
    if (nextPos != activeList_->end()) {
        menuPos_ = nextPos;
        selected_ = *menuPos_;
    }

    return selected_;
}

MenuItem* MenuTree::prev() {
    selected_->stopApp();

    if (menuPos_ != activeList_->begin()) {
        std::list<MenuItem*>::iterator prevPos = std::prev(menuPos_);
        menuPos_ = prevPos;
        selected_ = *menuPos_;
        selected_->runApp();
    }

    return selected_;
}

MenuItem* MenuTree::enter() {
    std::list<MenuItem*>* submenu = selected_->getSubmenu();
    selected_->stopApp();

    if (!submenu->empty()) {
        std::list<MenuItem*>* prevList = activeList_;
        activeList_ = submenu;
        menuPos_ = activeList_->begin();
        selected_ = *menuPos_;
        selected_->setPrevSubmenu(prevList);
        selected_->runApp();
    }
    
    return selected_;
}

MenuItem* MenuTree::back() {
    MenuItem* parent = selected_->getParent();
    selected_->stopApp();

    if (parent != NULL) {
        activeList_ = selected_->getPrevSubmenu();
        menuPos_ = std::find(activeList_->begin(), activeList_->end(), parent);
        selected_ = *menuPos_;
        selected_->runApp();
    }

    return selected_;
}

void MenuTree::printMenu(std::list<MenuItem*>* list, uint8_t depth) {
    std::list<MenuItem*>::iterator i;
    for (i = list->begin(); i != list->end() && (*i); ++i) {
        ESP_LOGI(
            TAG, "%.*s(%d)> %s", depth*3,
            "                              ",
            depth, (*i)->getName().c_str()
        );

        std::list<MenuItem*>* children = (*i)->getSubmenu();
        if (!children->empty()) {
            uint8_t curDepth = depth;
            printMenu(children, ++curDepth);
        }
    }
}

void MenuTree::printAllMenu() {
    ESP_LOGI(TAG, "========================================");
    ESP_LOGI(TAG, "                Menu List               ");
    ESP_LOGI(TAG, "========================================");

    printMenu(&list_, 0);
}