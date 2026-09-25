#pragma once

#include "Actor.hpp"
#include <memory>
#include <span>
#include <vector>

class Scene
{
public:
    Actor& Add(std::unique_ptr<Actor> actor, bool callOnStart);
    Actor& Add(std::unique_ptr<Actor> actor);

    void Start();
    void Update(float deltaTime);
    void LateUpdate(float deltaTime);
    void Stop();

    std::span<std::unique_ptr<Actor>> GetActors();

private:
    std::vector<std::unique_ptr<Actor>> actors;
};