#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "shape.h"

vec3 Triangle::getTexColor(const unsigned char* data, int width, int height, vec2 uv) {
	int x = static_cast<int>(uv.x * width);
	int y = static_cast<int>((1.0f - uv.y) * height);
	int pixelIndex = (y * width + x) * 4; // 假设每个像素颜色4字节RBGA
	vec3 color;
	color.x = data[pixelIndex] / 255.0f;
	color.y = data[pixelIndex + 1] / 255.0f;
	color.z = data[pixelIndex + 2] / 255.0f;
	return color;
}

void Triangle::hitTexColor(vec3 hitpoint) {
	int width, height, channels;
	unsigned char* data = stbi_load(texpath.c_str(), &width, &height, &channels, 4);
	if (data == nullptr) return;
	vec3 c1 = getTexColor(data, width, height, t1);
	vec3 c2 = getTexColor(data, width, height, t2);
	vec3 c3 = getTexColor(data, width, height, t3);

	// 重心坐标计算hitpoint点的颜色
	vec3 v0 = p2 - p1, v1 = p3 - p1, v2 = hitpoint - p1;
	float d00 = dot(v0, v0), d01 = dot(v0, v1), d11 = dot(v1, v1), d20 = dot(v2, v0), d21 = dot(v2, v1);
	float denom = d00 * d11 - d01 * d01;
	float v = (d11 * d20 - d01 * d21) / denom;
	float w = (d00 * d21 - d01 * d20) / denom;
	float u = 1.0f - v - w;
	color = u * c1 + v * c2 + w * c3;
	stbi_image_free(data);
}

HitResult Triangle::intersect(Ray &ray) {
	HitResult res;

	vec3 s = ray.startpos;
	vec3 d = ray.direction;
	vec3 n = triangleNormal(p1, p2, p3);
	/*
	// AABB
	vec3 min = glm::min(p1, glm::min(p2, p3));
	vec3 max = glm::max(p1, glm::max(p2, p3));
	float tmin = (min.x - s.x) / d.x;
	float tmax = (max.x - s.x) / d.x;
	if (tmin > tmax) swap(tmin, tmax);
	float tymin = (min.y - s.y) / d.y;
	float tymax = (max.y - s.y) / d.y;
	if (tymin > tymax) swap(tymin, tymax);
	if ((tmin > tymax) || (tymin > tmax)) return res;
	if (tymin > tmin) tmin = tymin;
	if (tymax < tmax) tmax = tymax;
	float tzmin = (min.z - s.z) / d.z;
	float tzmax = (max.z - s.z) / d.z;
	if (tzmin > tzmax) swap(tzmin, tzmax);
	if ((tmin > tzmax) || (tzmin > tmax)) return res;
	// AABB end
	*/
	vec3 tmpn = n;
	if (dot(tmpn, d) > 0.0f) tmpn = -n;
	if (fabs(dot(tmpn, d)) < 0.00001f) return res;
	float t = (dot(tmpn, p1) - dot(s, tmpn)) / dot(d, tmpn);
	if (t < 0.0005f) return res;

	vec3 p = s + d * t;
	vec3 c1 = cross(p2 - p1, p - p1);
	vec3 c2 = cross(p3 - p2, p - p2);
	vec3 c3 = cross(p1 - p3, p - p3);
	if (dot(c1, n) < 0 || dot(c2, n) < 0 || dot(c3, n) < 0) return res;

	hitTexColor(p);
	res.isHit = true;
	res.distance = t;
	res.hitPoint = p;
	res.texcolor = color;
	res.normal = n;
	res.material = material;
	return res;
}

HitResult Sphere::intersect(Ray &ray) {
	HitResult res;

	vec3 s = ray.startpos;
	vec3 d = ray.direction;

	float os = length(o - s);
	float sh = dot(o - s, d);
	float oh = sqrt(pow(os, 2) - pow(sh, 2));

	if (oh > r) return res;    // not intersect

	float ph = sqrt(pow(r, 2) - pow(oh, 2));
	float t1 = length(sh) - ph;
	float t2 = length(sh) + ph;
	float t = (t1 < 0) ? t2 : t1;
	vec3 p = s + t * d;

	if (fabs(t1) < 0.0005f || fabs(t2) < 0.0005f) return res;

	res.isHit = true;
	res.distance = t;
	res.hitPoint = p;
	res.texcolor = texcolor;
	res.material = material;
	return res;
}