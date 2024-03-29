#define _USE_MATH_DEFINES

#include "../include/loadinfo.h"
#include "../include/loadshapes.h"
#include "../include/shape.h"
#include "../include/bvh.h"
#include "../include/draw.h"
#include "../include/rayTracer.h"
#include <cmath>
#include <omp.h>
#include <chrono>

using namespace std;



int main() {
	string filepath = "../scenes/";
	string objFile = "veach-mis";
	attrib_t attrib;
	vector<shape_t> objshapes;
	vector<material_t> materials;
	camera cameras;
	light lights;
	LoadObjMtl(filepath + objFile + "/" + objFile + ".obj", filepath + objFile + "/", attrib, objshapes, materials);
	LoadXml(filepath + objFile + "/" + objFile + ".xml", cameras, lights);

	int height = cameras.height;
	int width = cameras.width;
	// vector<Shape*> shapes;
	//shapes = loadshapes(attrib, objshapes, materials, filepath+objFile);
	vector<Triangle*> shapes;
	shapes = loadshapes(attrib, objshapes, materials, filepath + objFile);
	cout << "shapes: " << shapes.size() << endl;
	BVHNode* node = buildBVH(shapes, 0);
	int SAMPLE = 1;
	int Depth = 0;
	const double BRIGHTNESS = (2.0f * 3.1415926f) * (1.0f / double(SAMPLE));
	rayTracer* RT = new rayTracer(cameras, lights, node, Depth);

	double* image = new double[width * height * 3];
	memset(image, 0.0, sizeof(double) * width * height * 3);
	auto start = chrono::high_resolution_clock::now();
	double* p = image;
	omp_set_num_threads(2);
	#pragma omp parallel for
	for (int y = 0; y < height; y++) {
		cout << "y = " << y << endl;
		for (int x = 0; x < width; x++) {
			Ray myray = RT->rayCasting(x, y);
			for (int k = 0; k < SAMPLE; k++) {
				vec3 color = RT->pathTracing(myray, 0);
				color *= BRIGHTNESS;
				int index = (y * width + x) * 3;
				*(p + index + 0) += color.x;
				*(p + index + 1) += color.y;
				*(p + index + 2) += color.z;
			}
		}
	}
	auto end = chrono::high_resolution_clock::now();
	chrono::duration<double> runtime = end - start;
	std::cout << "Run Time: " << runtime.count() << "s" << endl;
	string pngname = "../result/" + objFile + to_string(SAMPLE) + "-" + to_string(Depth);
	imshow(image, width, height, pngname);
	std::cout << "Í¼Æ¬" << pngname << ".png" << "ÒÑ±£´æ" << endl;
	//cout << "shapes num: " << shapes.size() << endl;
	/* test example
  	cameras.eye = vec3(0, 0, -4);
	cameras.lookat = vec3(0, 0, -3);
	shapes.push_back(new Triangle(vec3(-1, -1, 0), vec3(0, 1, 0), vec3(1, -1, 0), vec3(0, 0, -1), vec3(1, 0.5, 0.5), materials[0]));
	shapes.push_back(new Triangle(vec3(-1, 0, -1), vec3(0, 1, -1), vec3(0, -1, -1), vec3(0, 0, -1), vec3(0.5, 1, 0.5), materials[0]));
	
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			Ray ray = RayCasting(cameras, x, y);
			HitResult res = shoot(shapes, ray);
			*p = res.color.x; p++;
			*p = res.color.y; p++;
			*p = res.color.z; p++;
		}
	}
	
	imshow(image, width, height);
	*/
	return 0;
}