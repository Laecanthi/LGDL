#include <LGDL/Glyphs.h>

#include <fstream>
#include <nlohmann/json.hpp>

#include <LGDL/LGDLCore.h>

using json = nlohmann::json;


namespace LGDL
{
    Font CreateFont(const std::string& relativePath, const Texture& tex)
    {
        Font font;

        const std::string& path = ResolvePath(relativePath);

        std::ifstream file(path);

        if(!file.is_open())
        {
            throw std::runtime_error(
                "Failed to open font JSON: " + path
            );
        }

        json data;

        file >> data;

        font.atlas = tex;

        font.lineHeight = data["common"]["lineHeight"].get<float>();

        font.atlasSize.x = data["common"]["scaleW"].get<float>();
        font.atlasSize.y = data["common"]["scaleH"].get<float>();

        font.defaultFontSize = data["info"]["size"].get<int>();


        for(const auto& character : data["chars"])
        {
            Glyph glyph;

            glyph.size =
            {
                character["width"].get<float>(),
                character["height"].get<float>()
            };

            glyph.offset =
            {
                character["xoffset"].get<float>(),
                character["yoffset"].get<float>()
            };

            glyph.pos =
            {
                character["x"].get<float>(),
                character["y"].get<float>()
            };

            glyph.uvMin =
            {
                glyph.pos.x / font.atlasSize.x,
                (font.atlasSize.y - glyph.pos.y) / font.atlasSize.y
            };

            glyph.uvMax =
            {
                (glyph.pos.x + glyph.size.x) / font.atlasSize.x,
                ((font.atlasSize.y - glyph.pos.y) - glyph.size.y) / font.atlasSize.y
            };

            glyph.advance =
                character["xadvance"].get<float>();

            font.glyphs[
                character["id"].get<int>()
            ] = glyph;
        }

        return font;
    }

    Glyph& GetGlyph(Font& font, int codepoint)
    {
        auto it = font.glyphs.find(codepoint);

        if(it == font.glyphs.end())
        {
            return font.glyphs['?'];
        }

        return it->second;
    }
}