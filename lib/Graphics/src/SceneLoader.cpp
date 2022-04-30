#include "SceneLoader.h"

SceneLoader::SceneLoader(std::string filename) {
	load(filename);
}

SceneLoader::~SceneLoader() {

}

void SceneLoader::load(std::string filename) {
	float a;

	Tokenizer* tok = new Tokenizer();
	tok->Open(filename.data());

	char buff[50];
	tok->GetToken(buff);
	printf("curr buff: %s\n", buff);
	tok->GetToken(buff);
	printf("curr buff: %s\n", buff);

	a = tok->GetFloat();
	printf("curr floa: %f\n", a);
	tok->GetChar();
	a = tok->GetFloat();
	printf("curr floa: %f\n", a);
	tok->GetChar();
	a = tok->GetFloat();
	printf("curr floa: %f\n", a);

	a = tok->GetFloat();
	printf("curr rott: %f\n", a);
	tok->GetChar();
	a = tok->GetFloat();
	printf("curr rott: %f\n", a);
	tok->GetChar();
	a = tok->GetFloat();
	printf("curr rott: %f\n", a);



	tok->Close();
}