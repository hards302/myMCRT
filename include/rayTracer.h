#ifndef RAYTRACER_H
#define RAYTRACER_H

#include <vector>
#include "shape.h"
#include "loadinfo.h"
#include "bvh.h"

using namespace glm;

class rayTracer {
public:
	rayTracer(camera c, light l, BVHNode* n, int Depth) {
		mycamera = c, mylights = l, node = n, Depth = Depth;
	}
	camera mycamera;
	light mylights;
	vector<Triangle*> shapes;
	BVHNode* node;
	int Depth = 1;

	Ray rayCasting(int& x, int& y);

	HitResult shoot(Ray& ray);

	vec3 pathTracing(Ray& ray, int depth);
};

#endif