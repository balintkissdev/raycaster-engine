#ifndef RAYCASTER_H 
#define RAYCASTER_H

#include "WallTypes.h"
#include "MathLib.h"
#include "Camera.h"
#include "Texture.h"

#include <vector>

class IRenderer;

class RayCaster
{
    public:
        // TODO: Move draw drawBuffer to renderer
        static uint32_t drawBuffer[768][1024];

        RayCaster(
            const std::vector< std::vector<int> >& map,
            const int width,
            const int height
        );

        bool initialize(IRenderer* renderer);

        void drawTop(IRenderer* renderer);
        void drawBottom(IRenderer* renderer, const Camera& camera);
        void drawWalls(IRenderer* renderer, const Camera& camera);

    private:
        static const uint8_t texture_width;
        static const uint8_t texture_height;

        const std::vector< std::vector<int> >& map_;
        int width_, height_;

        Texture sky_texture_;
        Texture floor_texture_;
        Texture wall_textures_[4];
};

#endif
