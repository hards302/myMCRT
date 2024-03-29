#include "loadinfo.h"

void LoadObjMtl(const string& objfile, const string& mtlpath, attrib_t& attrib, vector<shape_t>& shapes, vector<material_t>& materials) {
	string warn;
	string err;
	// load obj file
	bool ret = LoadObj(&attrib, &shapes, &materials, &warn, &err, objfile.c_str(), mtlpath.c_str());

	if (!warn.empty()) cout << "Warn: " << warn << endl;

	if (!err.empty()) cout << "Err: " << err << endl;

	if (!ret) exit(1);

	//cout << "objshapes number: " << shapes.size() << endl;
	//cout << "materials number: " << materials.size() << endl;
}

void LoadXml(const string& xmlfile, camera& camera0, light& light0) {
	using namespace tinyxml2;
	XMLDocument doc;
	XMLError err = doc.LoadFile(xmlfile.c_str());

	if (err != XML_SUCCESS) {
		cerr << "Failed to load file: " << xmlfile << endl;
		exit(1);
	}
	XMLElement* root = doc.FirstChildElement("root");
	if (root) {
		XMLElement* cameras = root->FirstChildElement("camera");
		if (cameras) {
			camera0.type = cameras->Attribute("type");
			cameras->QueryFloatAttribute("width", &camera0.width);
			cameras->QueryFloatAttribute("height", &camera0.height);
			cameras->QueryFloatAttribute("fovy", &camera0.fovy);

			XMLElement* eye = cameras->FirstChildElement("eye");
			if (eye) {
				eye->QueryFloatAttribute("x", &camera0.eye.x);
				eye->QueryFloatAttribute("y", &camera0.eye.y);
				eye->QueryFloatAttribute("z", &camera0.eye.z);
			}

			XMLElement* lookat = cameras->FirstChildElement("lookat");
			if (lookat) {
				lookat->QueryFloatAttribute("x", &camera0.lookat.x);
				lookat->QueryFloatAttribute("y", &camera0.lookat.y);
				lookat->QueryFloatAttribute("z", &camera0.lookat.z);
			}

			XMLElement* up = cameras->FirstChildElement("up");
			if (up) {
				up->QueryFloatAttribute("x", &camera0.up.x);
				up->QueryFloatAttribute("y", &camera0.up.y);
				up->QueryFloatAttribute("z", &camera0.up.z);
			}
		}

		XMLElement* lights = root->FirstChildElement("light");
		while (lights) {
			light0.lightname.push_back(lights->Attribute("mtlname"));
			string rad = lights->Attribute("radiance");
			stringstream ss(rad);
			string token;
			vec3 tmp;
			int index = 0;
			while (getline(ss, token, ',')) {
				tmp[index] = stoi(token);
				index++;
			}
			light0.radiance.push_back(tmp);
			lights = lights->NextSiblingElement("light");
		}
	}

	if (root == nullptr) {
		cerr << "Failed to find TagName: root." << endl;
		exit(1);
	}

}

