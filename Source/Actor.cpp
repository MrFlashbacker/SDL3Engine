#include "Actor.hpp"
#include "Scene.hpp"

void PhysicsPlanet::OnStart() {
        Circle::OnStart();
        netForce = 0;
}
void PhysicsPlanet::Update(float deltaTime){
    Circle::Update(deltaTime);
    netForce = 0;
    for (auto& actor : scene->GetActors()) {
        PhysicsPlanet* planet = dynamic_cast<PhysicsPlanet*>(actor.get());
        if (!planet || planet == this) continue;

        Vector2 direction = planet->Position - Position;
        float distanceSquared = LengthSquared(direction);
        if (distanceSquared == 0.0f) continue;

        netForce += Normalize(direction) * ((mass * planet->mass)/distanceSquared) * 400;
    }
    acceleration = netForce/mass;
    velocity += acceleration * deltaTime;
    
}
void PhysicsPlanet::LateUpdate(float deltaTime) {
    Position += velocity * deltaTime;
}