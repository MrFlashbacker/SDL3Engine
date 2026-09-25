#pragma once
#include "Math.hpp"
#include "Color.hpp"

class Scene;

struct Actor {
    Vector2 Position;
    float Scale;
    float Rotation;
    Scene* scene;

    Actor(Vector2 pos, float s, float rotation) : Position(pos) , Scale(s), Rotation(rotation) {}
    virtual ~Actor() = default;

    virtual void OnStart() {}
    virtual void Update(float deltaTime) {}
    virtual void LateUpdate(float deltaTime) {}
    virtual void OnStop() {}
};
struct Square : public Actor {
public:
    Color color;
    Square(Vector2 pos = Vector2(0, 0), float s = 1, float r = 0, Color c = Color{255, 255, 255}) : Actor(pos, s, r), color(c) {}
    
    void OnStart() override {
        velocity = RandomDirection() * Math::RandomF(5, 8);
    }
    void Update(float deltaTime) override {
        Position += velocity * deltaTime;
    }
private:
    Vector2 velocity;
};
struct Circle : public Actor {
    Color color;
    Circle(Vector2 pos = Vector2(0, 0), float s = 1, float r = 0, Color c = Color{255, 255, 255}) : Actor(pos, s, r), color(c) {}
    void OnStart() override {
        intendedScale = Scale;
        Scale = 0.0f;
    }
    void Update(float deltaTime) override {
        float scaleToAdd = intendedScale / animationDuration * deltaTime;
        if (Scale < intendedScale) {
            if(Scale + scaleToAdd >= intendedScale) {
                Scale = intendedScale;
                return;
            }
            Scale += scaleToAdd;
        }
    }
private:
    float animationDuration = 0.1f;
    float intendedScale = 1.0f;
};
struct PhysicsPlanet : public Circle {
    Vector2 velocity;
    Vector2 acceleration;
    Vector2 netForce;
    float mass = 10;

    PhysicsPlanet(
        Vector2 pos = Vector2(0, 0),
        float s = 1, float r = 0,
        Color c = Color{255, 255, 255},
        float planetmass = 1, Vector2 startV = ZeroVector2
    ) : Circle(pos, s, r, c), mass(planetmass), velocity(startV) {}
    void OnStart() override;
    void Update(float deltaTime) override;
    void LateUpdate(float deltaTime) override;
    const float GetMass() const {
        return mass;
    }
};