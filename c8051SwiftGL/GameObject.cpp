#include "GameObject.hpp"

GameObject::GameObject() {
    globalTransform = new Transform();
    localTransform = new Transform();
}
