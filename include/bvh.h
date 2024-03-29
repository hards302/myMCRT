#ifndef BVH_H
#define BVH_H

#include "shape.h"
#include <vector>

struct BVHNode {
	BVHNode *left = nullptr, *right = nullptr;
	vec3 minBounds, maxBounds;    //AABB
	vector<Triangle*> triangles;

	~BVHNode() {
		delete left;
		delete right;
	}
};

void computeBounds(const vector<Triangle*>& triangles, vec3& outMin, vec3& outMax);

BVHNode* buildBVH(vector<Triangle*>& triangles, int depth = 0);

bool intersectAABB(const Ray& ray, const vec3& minBounds, const vec3& maxBounds);

HitResult BVHintersect(BVHNode* node, Ray& ray);

#endif