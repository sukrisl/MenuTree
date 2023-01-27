#include "esp_log.h"

#include "MenuTree.h"

static const char* TAG = "menu_struct";

MenuTree::MenuTree() {
    root_ = new MenuItem("root", NULL, NULL);
    selected_ = root_;
}

bool MenuTree::load() {
    activeList_ = root_->getSubmenu();
    if (activeList_->empty()) {
        ESP_LOGW(TAG, "Menu empty, create a menu first.");
        return false;
    }

    menuPos_ = activeList_->begin();
    selected_->setPrevSubmenu(activeList_);
    selected_ = *menuPos_;
    selected_->runApp();
    return true;
}

MenuItem* MenuTree::create(std::string name, MenuCallback_t callback, MenuItem* parent) {
    if (parent == NULL) return root_->addSubmenu(name, callback);

    std::string submenuName = parent->getName() + "/" + name;
    return parent->addSubmenu(submenuName, callback);
}

bool MenuTree::remove(MenuItem* item) {
    if (item == NULL) return false;
    delete item;
    return true;
}

MenuItem* MenuTree::next() {
    std::list<MenuItem*>::iterator nextPos = std::next(menuPos_);
    if (nextPos != activeList_->end()) {
        selected_->stopApp();
        selected_->setPrevSubmenu(activeList_);
        menuPos_ = nextPos;
        selected_ = *menuPos_;
        selected_->runApp();
    }

    return selected_;
}

MenuItem* MenuTree::prev() {
    if (menuPos_ != activeList_->begin()) {
        selected_->stopApp();
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
        selected_->setPrevSubmenu(activeList_);
        activeList_ = submenu;
        menuPos_ = activeList_->begin();
        selected_ = *menuPos_;
        selected_->runApp();
    }
    
    return selected_;
}

MenuItem* MenuTree::back() {
    MenuItem* parent = selected_->getParent();

    if (parent != NULL) {
        selected_->stopApp();
        selected_ = parent;
        activeList_ = selected_->getPrevSubmenu();
        menuPos_ = std::find(activeList_->begin(), activeList_->end(), selected_);
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

    std::list<MenuItem*>* list = root_->getSubmenu();
    printMenu(list, 0);
}