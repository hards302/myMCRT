#include "draw.h"
#include "./svpng.inc"
#include "./glm/glm.hpp"

void imshow(double* img, int width, int height, std::string path) {
	unsigned char* image = new unsigned char[width * height * 3];
	unsigned char* p = image;
	double* s = img;
	FILE* fp;
	path = path + ".png";
	fopen_s(&fp, path.c_str(), "wb");

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			*p++ = (unsigned char)glm::clamp(int((*s++) * 255), 0, 255);
			*p++ = (unsigned char)glm::clamp(int((*s++) * 255), 0, 255);
			*p++ = (unsigned char)glm::clamp(int((*s++) * 255), 0, 255);
		}
	}

	svpng(fp, width, height, image, 0);
}
