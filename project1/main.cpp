/**
 * \file main.cpp
 * \author lihw (lihw81@gmail.com)
 * \changelog
 * - 2019/09/03 initial check in
 *
 * The first project.
 */

#include "common/scene.hpp"
#include "common/camera.hpp"
#include "common/view.hpp"

int main(int argc, const char *argv[])
{
    // Load scene.
    cs6620::Scene scene;
    if (!scene.load("data/project1/scene.xml"))
    {
        return -1;
    }

    // Create the preview view.
    cs6620::View view(scene.camera.width, scene.camera.height);

    cs6620::Camera::RayIterator ri, rb = scene.camera.beginRay(), re = scene.camera.endRay();
    for (ri = rb; ri != re; ++ri)
    {
        view.write(ri->sample, scene.shade(*ri));
    }

    if (!view.dump("data/project1/result.ppm"))
    {
        return -1;
    }
    else
    {
        LOG(INFO) << "Render succeeds. The result image dumps to data/project1/result.ppm";
    }

    return 0;
}
