#include "bvh.h"
#include <algorithm>

void computeBounds(const vector<Triangle*>& triangles, vec3& outMin, vec3& outMax) {
	if (triangles.empty()) return;
	outMin = vec3(FLT_MAX);
	outMax = vec3(-FLT_MAX);
	for (const auto& tri : triangles) {
		vec3 triMin, triMax;
		triMin = glm::min(glm::min(tri->n1, tri->n2), tri->n3);
		triMax = glm::max(glm::max(tri->n1, tri->n2), tri->n3);
		outMin = glm::min(outMin, triMin);
		outMax = glm::max(outMax, triMax);
	}
}

BVHNode* buildBVH(vector<Triangle*>& triangles, int depth) {
	if (triangles.empty()) return nullptr;

	BVHNode* node = new BVHNode();
	node->triangles = triangles;
	computeBounds(triangles, node->minBounds, node->maxBounds);

	if (triangles.size() <= 2 || depth > 16) { // 基准条件
		return node;
	}

	glm::vec3 midPoint(0);
	for (auto* triangle : triangles) {
		midPoint += (triangle->n1 + triangle->n2 + triangle->n3) / 3.0f;
	}
	midPoint /= triangles.size();

	std::vector<Triangle*> leftTriangles;
	std::vector<Triangle*> rightTriangles;

	int longestAxis = 0; // 假设0为x轴，1为y轴，2为z轴
	glm::vec3 diff = node->maxBounds - node->minBounds;
	if (diff.y > diff.x && diff.y > diff.z) longestAxis = 1;
	else if (diff.z > diff.x && diff.z > diff.y) longestAxis = 2;

	for (auto* triangle : triangles) {
		glm::vec3 centroid = (triangle->n1 + triangle->n2 + triangle->n3) / 3.0f;
		if (centroid[longestAxis] < midPoint[longestAxis]) {
			leftTriangles.push_back(triangle);
		}
		else {
			rightTriangles.push_back(triangle);
		}
	}

	if (leftTriangles.size() == triangles.size() || rightTriangles.size() == triangles.size()) {
		return node; // 防止无限递归
	}

	node->left = buildBVH(leftTriangles, depth + 1);
	node->right = buildBVH(rightTriangles, depth + 1);
	node->triangles.clear(); // 清除当前节点的三角形列表，因为它们已经被分配到子节点中

	return node;
}

bool intersectAABB(const Ray& ray, const vec3& minBounds, const vec3& maxBounds) {
	vec3 invDir = 1.0f / ray.direction;
	vec3 t0s = (minBounds - ray.startpos) * invDir;
	vec3 t1s = (maxBounds - ray.startpos) * invDir;

	vec3 tMinVec = glm::min(t0s, t1s);
	vec3 tMaxVec = glm::max(t0s, t1s);
	float tMin = std::max({ tMinVec.x, tMinVec.y, tMinVec.z, 0.0f });
	float tMax = std::max({ tMaxVec.x, tMaxVec.y, tMaxVec.z });

	return tMax >= tMin;
}

HitResult BVHintersect(BVHNode* node, Ray& ray) {
	HitResult res;
	res.distance = FLT_MAX;
	if (!node) return res;
	if (!intersectAABB(ray, node->minBounds, node->maxBounds)) {
		return res;
	}
	if (node->left || node->right) {
		HitResult leftResult = BVHintersect(node->left, ray);
		HitResult rightReuslt = BVHintersect(node->right, ray);

		if (leftResult.isHit && rightReuslt.isHit) return leftResult.distance < rightReuslt.distance ? leftResult : rightReuslt;
		else if (!leftResult.isHit) return rightReuslt;
		else if (!rightReuslt.isHit) return leftResult;
	}
	else {
		for (const auto& tri : node->triangles) {
			HitResult r = tri->intersect(ray);
			if (r.isHit && r.distance < res.distance) res = r;
		}
	}
	return res;
}
