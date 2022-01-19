#include "Thallium/Core/Base.hpp"
#include "Thallium/Scene/Scene.hpp"
#include "Thallium/Scene/Entity.hpp"

#include <iostream>

using namespace Thallium;

int main(int, char**) {
    Ref<Scene> scene = Scene::Create();

    for (size_t i = 0; i < 10; i++) {
        Entity entity(scene);
        entity.AddComponent<float>();
        if (i % 2 == 0) {
            entity.AddComponent<int>();
        }
        if (i % 3 == 0) {
            entity.AddComponent<double>();
        }
    }

    std::cout << "-------------" << std::endl;

    scene->IterateComponent<float>([&](EntityID id, float& value) -> void {
        std::cout << "Float Component!" << std::endl;
    });

    std::cout << "-------------" << std::endl;

    scene->IterateComponents(
        std::function<void(EntityID, float&, int&, double&)>([&](EntityID id, float&, int&, double&) -> void {
            std::cout << "Float, Int and Double Components!" << std::endl;
        }));

    std::cout << "-------------" << std::endl;

    return 0;
}
