// define shapes like triangle and sphere, etc.
#ifndef SHAPE_H
#define SHAPE_H

#include <string>
#include "./glm/glm.hpp"
#include "tiny_obj_loader.h"
#include "./glm/gtx/normal.hpp"

using namespace glm;
using namespace std;

// ray
typedef struct Ray {
	vec3 startpos = vec3(0, 0, 0);
	vec3 direction = vec3(0, 0, 0);
}Ray;

// result that ray hits shape
typedef struct HitResult {
	bool isHit = false;      // is hit or not
	double distance = 0.0f;  // distance to hit point from camera
	vec3 hitPoint = vec3(0, 0, 0); // hit point
	vec3 texcolor = vec3(1, 1, 1);  // texcolor of the hit point
	vec3 normal = vec3(0, 0, 0); //normal of the hit point
	tinyobj::material_t material;  //material of the hit point
}HitResult;

class Shape {

public:
	Shape() {}
	virtual HitResult intersect(Ray &ray) {
		return HitResult();
	}
};

class Triangle : public Shape {

public:
	Triangle() {}
	Triangle(vec3 P1, vec3 P2, vec3 P3, vec3 N1, vec3 N2, vec3 N3, vec2 T1, vec2 T2, vec2 T3, string tpath, tinyobj::material_t Material) {
		p1 = P1, p2 = P2, p3 = P3, n1 = N1, n2 = N2, n3 = N3, t1 = T1, t2 = T2, t3 = T3, texpath = tpath, material = Material;
	}
	vec3 p1, p2, p3;    // 3 points of the triangle
	vec3 n1, n2, n3;    // normal of the triangle
	vec2 t1, t2, t3;    // tex coord of the 3 points of the triangle
	string texpath;
	vec3 color = vec3(1);         // color of the triangle
	tinyobj::material_t material;

	// get texcolor of one point
	vec3 getTexColor(const unsigned char* data, int width, int height, vec2 uv);

	// get hitpoint texcolor
	void hitTexColor(vec3 hitpoint);

	// intersect with ray
	HitResult intersect(Ray &ray) override;
};

class Sphere : public Shape {

public:
	Sphere() {}
	Sphere(vec3 O, double R, vec3 Color, tinyobj::material_t Material) {
		o = O, r = R, texcolor = Color, material = Material;
	}
	vec3 o;     // Ô²ÐÄ
	double r;   // °ë¾¶
	vec3 texcolor; // ÑÕÉ«
	tinyobj::material_t material;

	HitResult intersect(Ray &ray) override;
};

#endif