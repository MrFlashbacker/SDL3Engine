#include "Math.hpp"
#include <iostream>
#include "Color.hpp"
#include "Game.hpp"

Game::Game(int width, int height) 
    : state(SDLState{nullptr, nullptr, nullptr})
    , screenWidth(width), screenHeight(height)
    , running(false)
    , mouseX(0), mouseY(0)
{
    pixels.resize(screenWidth * screenHeight, 0);
}

Game::~Game() {
    ShutdownSDL();
}

bool Game::InitializeSDL() {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "SDL initialization failed: " << SDL_GetError() << std::endl;
        return false;
    }

    // Create a window
    state.window = SDL_CreateWindow(
        "GameEngine",
        screenWidth, screenHeight,
        SDL_WINDOW_RESIZABLE
    );

    if (!state.window) {
        std::cerr << "Window creation failed: " << SDL_GetError() << std::endl;
        return false;
    }
    state.renderer = SDL_CreateRenderer(state.window, NULL);
    if (!state.renderer) {
        std::cerr << "Renderer creation failed: " << SDL_GetError() << std::endl;
        return false;
    }

    state.texture = SDL_CreateTexture(state.renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING,
        screenWidth, screenHeight);
    if (!state.texture) {
        std::cerr << "Texture creation failed: " << SDL_GetError() << std::endl;
        return false;
    }

    state.gpuDevice = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV | SDL_GPU_SHADERFORMAT_DXIL | SDL_GPU_SHADERFORMAT_MSL, true, nullptr);
    if (!state.gpuDevice) {
        std::cerr << "GPU Device creation failed: " << SDL_GetError() << std::endl;
        return false;
    }

    return true;
}
void Game::ShutdownSDL() {
    if (state.texture) {
        SDL_DestroyTexture(state.texture);
        state.texture = nullptr;
    }
    if (state.renderer) {
        SDL_DestroyRenderer(state.renderer);
        state.renderer = nullptr;
    }
    if (state.window) {
        SDL_DestroyWindow(state.window);
        state.window = nullptr;
    }
    if(state.gpuDevice) {
        SDL_DestroyGPUDevice(state.gpuDevice);
        state.gpuDevice = nullptr;
    }
    SDL_Quit();
}
void Game::Run() {
    if (!InitializeSDL()) {
        return;
    }

    running = true;
    SDL_Delay(300);
    
    OnStart();
    while(running) {
        Uint32 currentTime = SDL_GetTicks();
        deltaTime = (currentTime - lastTime) / 1000.0f;  // Seconds
        lastTime = currentTime;

        ProcessInput();
        scene.Update(deltaTime);
        scene.LateUpdate(deltaTime);
        Render();
        SDL_Delay(6);
    }
    scene.Stop();
    ShutdownSDL();
    std::cout << "Game exited successfully" << std::endl;
}
void Game::OnStart() {
    lastTime = SDL_GetTicks();
    scene.Start();
    /*Color randomColor{(Uint8)Math::RandomInt(150, 255), (Uint8)Math::RandomInt(150, 255), (Uint8)Math::RandomInt(150, 255)};         
    scene.Add(std::make_unique<PhysicsPlanet>(-RightVector2 * 200.0f, 40, 0, randomColor, 32000, UpVector2 * 300));
    randomColor = Color{(Uint8)Math::RandomInt(150, 255), (Uint8)Math::RandomInt(150, 255), (Uint8)Math::RandomInt(150, 255)};
    scene.Add(std::make_unique<PhysicsPlanet>(RightVector2 * 200.0f, 40, 0, randomColor, 32000, -UpVector2 * 300));
    randomColor = Color{(Uint8)Math::RandomInt(150, 255), (Uint8)Math::RandomInt(150, 255), (Uint8)Math::RandomInt(150, 255)};
    scene.Add(std::make_unique<PhysicsPlanet>(ZeroVector2, 40, 0, randomColor, 32000));*/
}
void Game::ProcessInput() {
    SDL_Event event;
    SDL_GetMouseState(&mouseX, &mouseY);

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT) {
            running = false;
            break;
        }
        if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_ESCAPE) {
            running = false;
            break;
        }
        if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN && event.button.button == SDL_BUTTON_LEFT) {
            Color randomColor{(Uint8)Math::RandomInt(0, 255), (Uint8)Math::RandomInt(0, 255), (Uint8)Math::RandomInt(0, 255)};
            Vector2 randomVelocity = RandomDirection() * 60;
            scene.Add(std::make_unique<PhysicsPlanet>(GetMouseFromCenter(), 10, 0, randomColor, 1, randomVelocity));
        }
        if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN && event.button.button == SDL_BUTTON_RIGHT) {
            Color randomColor{(Uint8)Math::RandomInt(0, 255), (Uint8)Math::RandomInt(0, 255), (Uint8)Math::RandomInt(0, 255)};
            Vector2 randomVelocity = RandomDirection() * 40;
            scene.Add(std::make_unique<PhysicsPlanet>(GetMouseFromCenter(), 80, 0, randomColor, 512, randomVelocity));
        }
        if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_1) {
            Color randomColor{(Uint8)Math::RandomInt(0, 255), (Uint8)Math::RandomInt(0, 255), (Uint8)Math::RandomInt(0, 255)};
            
            scene.Add(std::make_unique<PhysicsPlanet>(GetMouseFromCenter(), 150, 0, randomColor, 3375));
        }
        if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_2) {
            Color randomColor{(Uint8)Math::RandomInt(150, 255), (Uint8)Math::RandomInt(150, 255), (Uint8)Math::RandomInt(150, 255)};
            
            scene.Add(std::make_unique<PhysicsPlanet>(GetMouseFromCenter(), 40, 0, randomColor, 32000));
        }
        if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_3) {
            scene.Add(std::make_unique<PhysicsPlanet>(GetMouseFromCenter(), 80, 0, Colors::Red, 9000000));
        }
    }
}
void Game::Render() {
    ClearScreen(Colors::Black);
    for (const auto& actorPtr : scene.GetActors()) {
        const PhysicsPlanet* circle = dynamic_cast<const PhysicsPlanet*>(actorPtr.get());
        if (circle) {
            if(circle->GetMass() >= 10000) {
                DrawCircleOutline(*circle);
            }
            else {
                DrawCircle(*circle);
            } 
        }
    }
    SDL_UpdateTexture(state.texture, nullptr, pixels.data(), screenWidth * sizeof(uint32_t));
    SDL_RenderClear(state.renderer);
    SDL_RenderTexture(state.renderer, state.texture, NULL, NULL);
    SDL_RenderPresent(state.renderer);
}
void Game::SetPixel(int x, int y, Color color) {
    pixels[y * screenWidth + x] = ToUint32(color);
}
void Game::SetPixel(const Vector2& position, Color color) {
    int x = static_cast<int>(position.X + 0.5f); 
    int y = static_cast<int>(position.Y + 0.5f);
    SetPixel(x, y, color);
}
void Game::DrawShape(Vector2 position, float scale, std::span<const Vector2> vertices, Color color) {
    int size = vertices.size();
    for(int i = 0; i < size - 2; i++) {
        DrawTriangle(vertices[0] * scale + position, vertices[i + 1] * scale + position, vertices[i+2] * scale + position, color);
    }
}
void Game::DrawSquare(const Square& sq) {
    float halfSize = sq.Scale;
    Vector2 v0{sq.Position.X - halfSize + 4, sq.Position.Y + halfSize};
    Vector2 v1{sq.Position.X + halfSize + 4, sq.Position.Y + halfSize};
    Vector2 v2{sq.Position.X + halfSize, sq.Position.Y - halfSize};
    Vector2 v3{sq.Position.X - halfSize, sq.Position.Y - halfSize};
    DrawTriangle(v0, v1, v2, sq.color);
    DrawTriangle(v0, v2, v3, sq.color);
}
void Game::DrawTriangle(const Vector2& a, const Vector2& b, const Vector2& c, const Color& color) {
    uint32_t color32 = ToUint32(color);

    float ax = screenWidth  * 0.5f + a.X;
    float ay = screenHeight * 0.5f - a.Y;
    float bx = screenWidth  * 0.5f + b.X;
    float by = screenHeight * 0.5f - b.Y;
    float cx = screenWidth  * 0.5f + c.X;
    float cy = screenHeight * 0.5f - c.Y;

    int minX = static_cast<int>(std::floor(std::min(ax, std::min(bx, cx))));
    int maxX = static_cast<int>(std::ceil(std::max(ax, std::max(bx, cx))));
    int minY = static_cast<int>(std::floor(std::min(ay, std::min(by, cy))));
    int maxY = static_cast<int>(std::ceil(std::max(ay, std::max(by, cy))));

    minX = std::max(minX, 0);
    maxX = std::min(maxX, screenWidth);
    minY = std::max(minY, 0);
    maxY = std::min(maxY, screenHeight);

    Vector2 ab{ bx - ax, by - ay };
    Vector2 bc{ cx - bx, cy - by };
    Vector2 ca{ ax - cx, ay - cy };

    for (int y = minY; y < maxY; ++y) {
        int rowOffset = y * screenWidth;
        for (int x = minX; x < maxX; ++x) {
            Vector2 p{x + 0.5f, y + 0.5f};
            float w0 = Cross(ab, p - Vector2{ ax, ay });
            float w1 = Cross(bc, p - Vector2{ bx, by });
            float w2 = Cross(ca, p - Vector2{ cx, cy });

            bool inside = (w0 >= 0 && w1 >= 0 && w2 >= 0) ||
                          (w0 <= 0 && w1 <= 0 && w2 <= 0);

            if (inside) {
                pixels[rowOffset+ x] = color32;
            }
        }
    }
}
void Game::DrawCircle(const Circle& circle)
{
    const int cx = static_cast<int>(screenWidth * 0.5f + circle.Position.X + 0.5f);
    const int cy = static_cast<int>(screenHeight * 0.5f - circle.Position.Y + 0.5f);
    const int r = static_cast<int>(circle.Scale + 0.5f);

    const uint32_t pixel = ToUint32(circle.color);
    const int radiusSquared = r*r;

    const int minX = std::max(0, cx - r);
    const int maxX = std::min(screenWidth - 1, cx + r);
    const int minY = std::max(0, cy - r);
    const int maxY = std::min(screenHeight - 1, cy + r);

    for (int y = minY; y <= maxY; ++y)
    {
        const int rowOffset = y * screenWidth;
        for (int x = minX; x <= maxX; ++x)
        {
            const int dx = x - cx;
            const int dy = y - cy;

            if (dx * dx + dy * dy <= radiusSquared)
            {
                pixels[rowOffset + x] = pixel;
            }
        }
    }
}
void Game::DrawCircleOutline(const Circle& circle)
{
    const int cx = static_cast<int>(screenWidth * 0.5f + circle.Position.X + 0.5f);
    const int cy = static_cast<int>(screenHeight * 0.5f - circle.Position.Y + 0.5f);
    const int r = static_cast<int>(circle.Scale + 0.5f);

    uint32_t pixel = ToUint32(circle.color);
    uint32_t pixel2 = ToUint32(circle.color*0.6f);
    const int radiusSquared = r*r;

    const int minX = std::max(0, cx - r);
    const int maxX = std::min(screenWidth - 1, cx + r);
    const int minY = std::max(0, cy - r);
    const int maxY = std::min(screenHeight - 1, cy + r);

    for (int y = minY; y <= maxY; ++y)
    {
        const int rowOffset = y * screenWidth;
        for (int x = minX; x <= maxX; ++x)
        {
            const int dx = x - cx;
            const int dy = y - cy;
            const int squareProd = dx * dx + dy * dy;

            if (squareProd <= radiusSquared && squareProd >= radiusSquared * 0.8f)
            {
                pixels[rowOffset + x] = pixel;
            }
            else if (squareProd < radiusSquared* 0.8f && squareProd >= radiusSquared * 0.6f) {
                pixels[rowOffset + x] = pixel2;
            }
        }
    }
}
void Game::ClearScreen(Color color) {
    const uint32_t color32 = ToUint32(color);
    const size_t totalPixels = screenHeight * screenWidth;

    std::fill_n(pixels.data(), totalPixels, color32);
}

Vector2 Game::GetMouseFromCenter() {
    //return Vector2{screenWidth*0.5f + mouseX, screenHeight*0.5f - mouseY};
    return Vector2{
        mouseX - screenWidth * 0.5f,
        screenHeight * 0.5f - mouseY
    };
}