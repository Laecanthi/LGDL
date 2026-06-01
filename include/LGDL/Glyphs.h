#pragma once

#include <LGDL/Types.h>
#include <LGDL/Graphics.h>
#include <LGDL/Texture.h>

#include <unordered_map>


namespace LGDL
{
    struct Glyph
    {
        Vec2 pos;

        Vec2 uvMin;
        Vec2 uvMax;

        Vec2 size;

        Vec2 offset;

        float advance;
    };

    struct Font
    {
        Texture atlas;

        std::unordered_map<int, Glyph> glyphs;

        float lineHeight;

        Vec2 atlasSize;

        int defaultFontSize;
    };

    Font CreateFont(const std::string& relativePath, const Texture& tex);

    Glyph& GetGlyph(Font& font, int codepoint);
}