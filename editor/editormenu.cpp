#include "editormenu.hpp"

EditorMenuItem::~EditorMenuItem() {
}

void EditorMenuItem::activate() {
    clicked(_id);
}

void EditorMenuItem::update_mousepos(const Position & pos) {
    UI::Element::update_mousepos(pos);
    sprite.hovering.show(activated == true or hovering == true);
}

bool EditorMenuItem::contains(const Position & pos) {
    return rect.contains(pos);
}

void EditorMenuItem::click(const Position & pos) {
    UI::Element::click(pos);
    sprite.activated.show(activated == true);
    sprite.hovering.show(activated == true or hovering == true);
}

/////////////////////////////////////////////////////////////////////

EditorMenu::~EditorMenu() {
}
