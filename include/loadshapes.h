#ifndef LOADSHAPES_H
#define LOADSHAPES_H
#include <vector>
#include <string>
#include "shape.h"
#include "tiny_obj_loader.h"

using namespace tinyobj;
using namespace std;

vector<Triangle*> loadshapes(attrib_t attrib, vector<shape_t> objshapes, vector<material_t> materials, string texpath);

#endif