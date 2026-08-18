#pragma once

#include "math.hpp"

struct   Camera
{
    mat4f	getViewMatrix()
    {
        return (mat4f::lookAt(pos, pos + front(), vec3f(0, 1, 0)));
    }
    mat4f	getProjectionMatrix()
    {
        return (mat4f::perspective(fov, aspect, near, far));
    }

    vec3f    front() const
    {
        float   c = std::cos(radians(pitch));

        return (vec3f(
            std::sin(radians(yaw)) * c,
            std::sin(radians(pitch)),
            -std::cos(radians(yaw)) * c));
    }

    vec3f   pos;
    float  yaw = 0;
    float  pitch = 0;
    float  fov = 0;
    float  aspect = 0;
    float  near = 0;
    float  far = 0;
};
