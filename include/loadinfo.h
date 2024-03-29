// load obj and xml and mtl files.
#ifndef LOADINFO_H
#define LOADINFO_H

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "./glm/glm.hpp"
#include "./glm/gtc/matrix_transform.hpp"
#include "tiny_obj_loader.h"
#include "tinyxml2.h"


using namespace std;
using namespace tinyobj;
using namespace glm;

typedef struct camera {  // 存储相机信息
	string type;
	float width, height;
	float fovy;
	vec3 eye;
	vec3 lookat;
	vec3 up;
}camera;

typedef struct light {  // 存储光源信息
	vector<string> lightname;
	vector<vec3> radiance;
}light;

void LoadObjMtl(const string& objfile, const string& mtlpath, attrib_t& attrib, vector<shape_t>& shapes, vector<material_t>& materials);

void LoadXml(const string& xmlfile, camera& camera0, light& light0);

#endif // !LOADINFO_H