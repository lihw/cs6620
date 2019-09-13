/**
 * \file main.cpp
 * \author lihw (lihw81@gmail.com)
 * \changelog
 * - 2019/09/03 initial check in
 *
 * The first project.
 */

#include "../common/scene.hpp"
#include "../common/camera.hpp"
#include "../common/view.hpp"

int main(int argc, const char *argv[])
{
    // Load scene.
    cs6620::Scene scene;
    if (!scene.load("../data/project1/scene.xml"))
    {
        return -1;
    }

    scene.prepare();

    // Create the preview view.
    cs6620::View view(scene.camera->width, scene.camera->height);
        
    const u32 N = 16;
    cs6620::NaieSampler sampler(N);
    vec2 *samples = sampler.samples();

    for (u32 i = 0; i < camera.height; ++i)
    for (u32 j = 0; j < camera.width; ++j)
    {
        vec3 ray = vec3(0, 0, 0);
        for (u32 s = 0; s < N; ++s)
        {
            f32 x = (f32)j + samples[s].x;
            f32 y = (f32)i + samples[s].y;

            Ray ray = scene.caemra->unproject(x, y);

            color += scene.shade(ray);
        }

        view.write(sample.position(), color / (f32)N);
    }

    if (!view.dump("../data/project1/result.ppm"))
    {
        return -1;
    }
    else
    {
        LOG(INFO) << "Render succeeds. The result image dumps to data/project1/result.ppm";
    }

    return 0;
}
