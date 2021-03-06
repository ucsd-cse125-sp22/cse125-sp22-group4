#include "ObjectLoader.h"

ObjectLoader::ObjectLoader(std::string filename) {
	phongMat = color::GREEN;

	FILE* fp;
	float x, y, z;
	int fx, fy, fz, ignore;
	int c1, c2;
	float minY = INFINITY, minZ = INFINITY;
	float maxY = -INFINITY, maxZ = -INFINITY;

	fp = fopen(filename.c_str(), "rb");

	if (fp == NULL) {
		printf("Error loading file! %s", filename.c_str());
		exit(-1);
	}

	while (!feof(fp)) {
		c1 = fgetc(fp);
		while (!(c1 == 'v' || c1 == 'f')) {
			c1 = fgetc(fp);
			if (feof(fp))
				break;
		}
		c2 = fgetc(fp);

		if ((c1 == 'v') && (c2 == ' ')) {
			fscanf(fp, "%f %f %f", &x, &y, &z);
			points.push_back(glm::vec3(x, y, z));
			if (y < minY) minY = y;
			if (z < minZ) minZ = z;
			if (y > maxY) maxY = y;
			if (z > maxZ) maxZ = z;
		}
		else if ((c1 == 'v') && (c2 == 'n')) {
			fscanf(fp, "%f %f %f", &x, &y, &z);
			// Ignore the normals in mytest2, as we use a solid color for the teapot.
			normals.push_back(glm::normalize(glm::vec3(x, y, z)));
		}
		else if ((c1 == 'f'))
		{
			fscanf(fp, "%d//%d %d//%d %d//%d", &fx, &ignore, &fy, &ignore, &fz, &ignore);
			indices.push_back(fx - 1);
			indices.push_back(fy - 1);
			indices.push_back(fz - 1);
		}
	}

	fclose(fp); // Finished parsing

	//recenter
	float avgY = (minY + maxY) / 2.0f - 0.02f;
	float avgZ = (minZ + maxZ) / 2.0f;
	for (unsigned int i = 0; i < points.size(); ++i) {
		glm::vec3 shiftedVertex = (points[i] - glm::vec3(0.0f, avgY, avgZ)) * glm::vec3(1.58f, 1.58f, 1.58f);
		points[i] = shiftedVertex;
	}

    generateBuffers();
    bindBuffers();
}

