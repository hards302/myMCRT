#include "rayTracer.h"
#include <random>

// genrate random number between (0,1)
std::uniform_real_distribution<> dis(0.0, 1.0);
random_device rd;
mt19937 gen(rd());
double randf() {
	return dis(gen);
}

vec3 randomDir(vec3 n) {
	vec3 d;
	do {
		d = 2.0f * vec3(randf(), randf(), randf()) - vec3(1, 1, 1);
	} while (dot(d, d) > 1.0);
	return normalize(normalize(d) + n);
}

Ray rayTracer::rayCasting(int& x, int& y) {
	float aspectRatio = mycamera.width / mycamera.height;
	mat4 projection = perspective(radians(mycamera.fovy), aspectRatio, 0.1f, 100.0f);
	mat4 view = lookAt(mycamera.eye, mycamera.lookat, mycamera.up);
	mat4 screen2world = inverse(projection * view);

	float ndcX = (2.0f * x) / mycamera.width - 1.0f;
	float ndcY = 1.0f - (2.0f * y) / mycamera.height;

	vec4 raystartworld = screen2world * vec4(ndcX, ndcY, -1.0f, 1.0f);
	raystartworld /= raystartworld.w;

	Ray ray;
	ray.direction = normalize(vec3(raystartworld) - mycamera.eye);
	ray.startpos = mycamera.eye;

	return ray;
}

HitResult rayTracer::shoot(Ray& ray) {
	HitResult res, r;
	res.distance = FLT_MAX;

	for (auto& shape : shapes) {
		r = shape->intersect(ray);
		if (r.isHit && r.distance < res.distance) res = r;
	}

	return res;
}

vec3 rayTracer::pathTracing(Ray& ray, int depth) {
	vec3 color = vec3(0);
	if (depth > Depth) return color;

	//HitResult res = shoot(ray);
	HitResult res = BVHintersect(node, ray);
	if (!res.isHit) return color;
	string name = res.material.name;
	if (name.length() >= 5 && name.substr(0, 5) == "light") {
		for (int i = 0; i < mylights.lightname.size(); i++) {
			if (mylights.lightname[i] == name) return mylights.radiance[i] *
				vec3(res.material.diffuse[0], res.material.diffuse[1], res.material.diffuse[2]);   // hit the light
		}
	}
	double r = randf();
	float P = 0.8;
	if (r > P) return color;     // not hit the light

	// get random direct of ray and cosine
	Ray randomRay;
	randomRay.startpos = res.hitPoint;
	randomRay.direction = randomDir(res.normal);
	float cosine = fabs(dot(-ray.direction, res.normal));

	// get Kd, Ks, Tr
	vec3 Kd = vec3(res.material.diffuse[0], res.material.diffuse[1], res.material.diffuse[2]);
	vec3 Ks = vec3(res.material.specular[0], res.material.specular[1], res.material.specular[2]);
	vec3 Tr = vec3(res.material.transmittance[0], res.material.transmittance[1], res.material.transmittance[2]);

	if (Kd != vec3(0)) {     // ¬˛∑¥…‰
		vec3 diffuseColor = pathTracing(randomRay, depth + 1) * cosine;
		color = diffuseColor * Kd * res.texcolor;
	}
	if (Ks != vec3(0)) {     // æµ√Ê∑¥…‰
		vec3 ref = normalize(reflect(ray.direction, res.normal));
		//randomRay.direction = mix(ref, randomRay.direction, res.material.shininess);
		randomRay.direction = ref;
		color = pathTracing(randomRay, depth + 1) * cosine * Ks;
	}
	if (Tr != vec3(0)) {                    // ’€…‰
		vec3 ref = normalize(refract(ray.direction, res.normal, res.material.ior));
		//randomRay.direction = mix(ref, -randomRay.direction, 0.0f);
		randomRay.direction = ref;
		color = pathTracing(randomRay, depth + 1) * cosine * Tr;
	}
	return color / P;
}