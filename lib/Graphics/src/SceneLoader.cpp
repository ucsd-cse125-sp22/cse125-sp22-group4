#include "SceneLoader.h"

SceneLoader::SceneLoader(std::string filename) {
	scenefilename = filename;
}

SceneLoader::~SceneLoader() {
}

std::vector<Model*> SceneLoader::load() {

	std::vector<Model*> sceneObjects;

	Tokenizer* tok = new Tokenizer();
	if (tok->Open(scenefilename.data())) {
		spdlog::info("Reading scene file {}", scenefilename);
	}
	else {
		spdlog::error("Cannot open scene file: {}!", scenefilename);
	}

	char buff[50];
	tok->GetToken(buff);

	float t;
	glm::vec3 translation = glm::vec3(0);
	float r;
	glm::vec3 rotation = glm::vec3(0);

	Model* tempModel;

	while (!strcmp(buff, "Object:")) {
		tok->GetToken(buff);
		spdlog::info("Putting object {} in scene", buff);
		std::string objName(buff);
		std::string modelPath = "../../objects/" + objName + "/" + objName + ".obj";
		tempModel = new Model(modelPath);

		//set translation
		t = tok->GetFloat();
		translation.x = t;

		tok->GetChar();
		t = tok->GetFloat();
		translation.z = -t;

		tok->GetChar();
		t = tok->GetFloat();
		translation.y = t;

		tempModel->moveGlobal(translation);

		//set rotation
		r = tok->GetFloat();
		r = r - glm::radians(90.0f);
		rotation.x = r;

		tok->GetChar();
		r = tok->GetFloat();
		rotation.z = -r;

		tok->GetChar();
		r = tok->GetFloat();
		rotation.y = r;

		tempModel->rotate(rotation);

		sceneObjects.push_back(tempModel);

		tok->GetToken(buff);
	}

	tok->Close();
	return sceneObjects;
}

void SceneLoader::draw(const glm::mat4& viewProjMat,
	const glm::mat4& transform,
	GLuint shader,
	std::vector<Model*> sceneObjects) {
	for (auto obj : sceneObjects) {
		obj->draw(viewProjMat, transform, shader);
	}
}
