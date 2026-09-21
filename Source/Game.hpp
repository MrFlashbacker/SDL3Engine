#pragma once
#include "Math.hpp"
#include <vector>
#include "Actor.hpp"
#include <Color.hpp>
#include <SDL3/SDL.h>
#include <memory>
#include <span>

struct SDLState {
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    SDL_GPUDevice* gpuDevice;
};
class Game {
public:
    Game(int width, int height);
    ~Game();

    void Run();
private:
    float deltaTime;
    int screenWidth, screenHeight;
    float mouseX, mouseY;
    Uint32 lastTime;

    bool running;
    SDLState state;
    std::vector<unsigned int> pixels;
    std::vector<std::unique_ptr<Actor>> Scene;

    bool InitializeSDL();
    void ShutdownSDL();

    void OnStart();
    void ProcessInput();
    void OnUpdate();
    void Render();
    void OnStop();

    void SetPixel(int x, int y, Color color);
    void SetPixel(const Vector2& v, Color color);
    void DrawShape(Vector2 position, float scale, std::span<const Vector2> vertices, Color);
    void DrawSquare(const Square& sq);
    void DrawTriangle(const Vector2& a, const Vector2& b, const Vector2& c, const Color& color);
    void ClearScreen(Color color);
};

