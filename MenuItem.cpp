#include "esp_log.h"

#include "MenuItem.h"

static const char* TAG = "menu_item";

MenuItem::MenuItem(MenuType_t type, std::string name, MenuCallback_t callback, MenuItem* parent) {
    type_ = type;
    name_ = name;
    callback_ = callback;
    parent_ = parent;

    if (type_ == MENU_TYPE_APP) {
        queueHandle = xQueueCreate(10, sizeof(int8_t));
    }
}

MenuItem::~MenuItem() {
    // TODO: Delete list of childs
}

void MenuItem::menuTask(void* arg) {
    MenuItem* self = static_cast<MenuItem*>(arg);
    int8_t inc = 0;
    while (true) {
        if (self->getType() == MENU_TYPE_APP) {
            self->callback_(inc);
            xQueueReceive(self->queueHandle, &inc, portMAX_DELAY);
        } else {
            self->callback_(0);
            vTaskDelay(portMAX_DELAY);
        }
    }
}

MenuItem* MenuItem::searchSubmenu(std::string name) {
    std::list<MenuItem*>::iterator i;
    for (i = submenuList_.begin(); i != submenuList_.end() && (*i); ++i) {
        if ((*i)->name_ == name) return *i;
    }
    return NULL;
}

MenuItem* MenuItem::addSubmenu(MenuType_t type, std::string name, MenuCallback_t callback) {
    bool childNameDuplicate = searchSubmenu(name);

    if (childNameDuplicate) {
        ESP_LOGW(TAG, "Failed to add submenu %s, found duplicate, remove first!", name.c_str());
        return NULL;
    }

    MenuItem* child = new MenuItem(type, name, callback, this);
    submenuList_.push_back(child);
    return child;
}

bool MenuItem::removeSubmenu(std::string name) {
    MenuItem* submenu = searchSubmenu(name);
    bool found = submenu;

    if (!found) {
        ESP_LOGW(TAG, "Failed to remove submenu %s, item not found", name.c_str());
        return false;
    }

    submenuList_.erase(std::remove(submenuList_.begin(), submenuList_.end(), submenu), submenuList_.end());
    delete submenu;

    return true;
}

bool MenuItem::runApp() {
    if (callback_ == NULL) return false;
    if (taskHandle == NULL) stopApp();
    xTaskCreate(menuTask, name_.c_str(), 2048, (void*) this, 1, &this->taskHandle);
    return true;
}

bool MenuItem::stopApp() {
    if (this->taskHandle == NULL) return false;
    vTaskDelete(this->taskHandle);
    this->taskHandle = NULL;
    return true;
}

void MenuItem::increment() {
    int8_t inc = 1;
    xQueueSend(queueHandle, &inc, portMAX_DELAY);
}

void MenuItem::decrement() {
    int8_t inc = -1;
    xQueueSend(queueHandle, &inc, portMAX_DELAY);
}