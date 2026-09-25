#include "Scene.hpp"

Actor& Scene::Add(std::unique_ptr<Actor> actor) {
    actor->scene = this;
    Actor& reference = *actor;
    actors.push_back(std::move(actor));
    reference.OnStart();
    return reference;
}
Actor& Scene::Add(std::unique_ptr<Actor> actor, bool callOnStart) {
    actor->scene = this;
    Actor& reference = *actor;
    actors.push_back(std::move(actor));
    if (callOnStart) reference.OnStart();
    return reference;
}

void Scene::Start() {
    for (auto& actor : actors) actor->OnStart();
}
void Scene::Update(float deltaTime) {
    for (auto& actor : actors) actor->Update(deltaTime);
}
void Scene::LateUpdate(float deltaTime) {
    for (auto& actor : actors) actor->LateUpdate(deltaTime);
}
void Scene::Stop() {
    for (auto& actor : actors) actor->OnStop();
}

std::span<std::unique_ptr<Actor>> Scene::GetActors()
{
    return actors;
}