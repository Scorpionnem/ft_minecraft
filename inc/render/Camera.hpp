#pragma once

#include "math.hpp"

struct   Camera
{
    mat4f	getViewMatrix()
    {
        return (mat4f::lookAt(vec3f(0.0), vec3f(0.0) + front(), vec3f(0, 1, 0)));
    }
    mat4f	getProjectionMatrix()
    {
        return (mat4f::perspective(fov, aspect, near, far));
    }

    vec3f    front() const
    {
        float   c = std::cos(pitch);

        return (vec3f(
            std::sin(yaw) * c,
            std::sin(pitch),
            -std::cos(yaw) * c));
    }

    vec3d   pos;
    double  yaw = 0;
    double  pitch = 0;
    double  fov = 0;
    double  aspect = 0;
    double  near = 0;
    double  far = 0;
};
