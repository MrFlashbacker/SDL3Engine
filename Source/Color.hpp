#pragma once
#include <cstdint>
#include <ostream>

struct Color {
    uint8_t R, G, B, A;
    
    constexpr Color(uint8_t r = 0, uint8_t g = 0, uint8_t b = 0, uint8_t a = 255) noexcept
        : R(r), G(g), B(b), A(a) {}
};

namespace Colors {
    inline constexpr Color Black{ 0, 0, 0 };
    inline constexpr Color White{ 255, 255, 255 };
    inline constexpr Color Red{ 255, 0, 0 };
    inline constexpr Color Green{ 0, 255, 0 };
    inline constexpr Color Blue{ 0, 0, 255 };
    inline constexpr Color Yellow{ 255, 255, 0 };
    inline constexpr Color Cyan{ 0, 255, 255 };
    inline constexpr Color Magenta{ 255, 0, 255 };
    inline constexpr Color Gray{ 128, 128, 128 };
    inline constexpr Color DarkGray{ 64, 64, 64 };
    inline constexpr Color LightGray{ 192, 192, 192 };
    inline constexpr Color Orange{ 255, 165, 0 };
    inline constexpr Color Purple{ 128, 0, 128 };
    inline constexpr Color Transparent{ 0, 0, 0, 0 };
}

inline constexpr uint32_t ToUint32(const Color& c) noexcept {
    return (static_cast<uint32_t>(c.R) << 24) |
           (static_cast<uint32_t>(c.G) << 16) |
           (static_cast<uint32_t>(c.B) << 8) |
           static_cast<uint32_t>(c.A);
}
inline constexpr Color FromUint32(uint32_t rgba) noexcept {
    return Color{
        static_cast<uint8_t>((rgba >> 24) & 0xFF),
        static_cast<uint8_t>((rgba >> 16) & 0xFF),
        static_cast<uint8_t>((rgba >> 8) & 0xFF),
        static_cast<uint8_t>(rgba & 0xFF)
    };
}

inline constexpr bool operator==(const Color& a, const Color& b) noexcept {
    return a.R == b.R && a.G == b.G && a.B == b.B && a.A == b.A;
}
inline constexpr bool operator!=(const Color& a, const Color& b) noexcept {
    return !(a == b);
}

inline std::ostream& operator<<(std::ostream& os, const Color& c) {
    os << "Color(" << static_cast<int>(c.R) << ", " 
                   << static_cast<int>(c.G) << ", " 
                   << static_cast<int>(c.B) << ", " 
                   << static_cast<int>(c.A) << ")";
    return os;
}