#include "Math.hpp"
#include "Color.hpp"

struct Actor {
    Vector2 Position;
    float Scale;
    float Rotation;

    Actor(Vector2 pos, float s, float rotation) : Position(pos) , Scale(s), Rotation(rotation) {}
    virtual ~Actor() {}

    virtual void OnStart() {}
    virtual void Update(float deltaTime) {}
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
struct RandomHexagon : public Actor {
    Color color;
    Vector2 vertices[6] = {
        { 2, -3 },
        { 7, -1 },
        { 8, 4 },
        { 3, 7 },
        { -2, 5 },
        { -4, 0 }
    };
    RandomHexagon(Vector2 pos = Vector2(0, 0), float s = 1, float r = 0, Color c = Color{255, 255, 255}) : Actor(pos, s, r), color(c) {}
    
    void OnStart() override {
        velocity = RandomDirection() * Math::RandomF(5, 8);
    }
    void Update(float deltaTime) override {
        Position += velocity * deltaTime;
    }
private:
    Vector2 velocity;
};