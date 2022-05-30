#include "Client.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

static CAudioEngine* audioEngine;
static bool hasPlayedTimer = false;
static bool hasPlayedTask1 = false;
static bool hasPlayedStationary1 = false;
static bool hasPlayedStationary2 = false;
static bool hasPlayedMouseCatCollision = false;
static bool hasPlayedMouse1CatCollision = false;
static bool hasPlayedMouse2CatCollision = false;
static bool hasPlayedMouse3CatCollision = false;

// shader, camera and light
static GLuint shader;
static GLuint skyboxShader;
static GLuint particleShader;
static Camera* camera;
static ThirdPersonCamera* thirdPersonCamera;
static int lightCount;
static std::vector<glm::vec4> lightPosn;
static std::vector<glm::vec4> lightColorn;

//skybox
static Skybox* skybox;

// objects
static Cube* ground;
static Model* cat;
static Model* mouse1;
static Model* mouse2;
static Model* mouse3;
static Model* player;
//static Model* backpack;
static Model* maze;
static Model* players[PLAYER_NUM];
static Model* bear;
static Model* item;
static Model* geisel;
static Model* sungod;
static Model* stonehenge;
static Model* fallenstar;
static Model* item2;
static Model* item3;
//static Model* demoChar;
//static Model* demoChar2;

static Model* fakecat;

// Animations
//static Animation* demoAnimation;
//static Animator* animator;
//static Animation* demoAnimation2;
//static Animator* animator2;

static Animation* catidleAnimation;
static Animator* catanimator;

static Animation* mouseidleAnimation1;
static Animator* mouseanimator1;
static Animation* mouseidleAnimation2;
static Animator* mouseanimator2;
static Animation* mouseidleAnimation3;
static Animator* mouseanimator3;

//Particles
static ParticleSystem* smokeparticles;
static ParticleSystem* flameparticles;
static ParticleSystem* glintparticles;

static ParticleSystem* cattrailparticles;
static ParticleSystem* micetrailparticles1;
static ParticleSystem* micetrailparticles2;
static ParticleSystem* micetrailparticles3;

// for ImGui Image display
int my_image_width = 0;
int my_image_height = 0;
GLuint my_image_texture = 0;
int window_width;
int window_height;
bool ret;
bool retGameOver;
int image_width_game_over = 0;
int image_height_game_over = 0;
GLuint image_texture_game_over = 0;
bool retMouseWin;
int image_width_mouse_win = 0;
int image_height_mouse_win = 0;
GLuint image_texture_mouse_win = 0;
bool retMouseFlag;
int image_width_mouse_flag = 0;
int image_height_mouse_flag = 0;
GLuint image_texture_mouse_flag = 0;
bool retMouseFlagPale;
int image_width_mouse_flag_pale = 0;
int image_height_mouse_flag_pale = 0;
GLuint image_texture_mouse_flag_pale = 0;
bool retOneThird;
int image_width_one_third = 0;
int image_height_one_third = 0;
GLuint image_texture_one_third = 0;
bool retTwoThirds;
int image_width_two_thirds = 0;
int image_height_two_thirds = 0;
GLuint image_texture_two_thirds = 0;
bool retMap;
int image_width_map = 0;
int image_height_map = 0;
GLuint image_texture_map = 0;
bool retGameStart;
int image_width_game_start = 0;
int image_height_game_start = 0;
GLuint image_texture_game_start = 0;
bool retStartMouse;
int image_width_start_mouse = 0;
int image_height_start_mouse = 0;
GLuint image_texture_start_mouse = 0;
bool retStartCat;
int image_width_start_cat = 0;
int image_height_start_cat = 0;
GLuint image_texture_start_cat = 0;
bool retStartCatuate;
int image_width_start_catuate = 0;
int image_height_start_catuate = 0;
GLuint image_texture_start_catuate = 0;
bool retCatIcon;
int image_width_cat_icon = 0;
int image_height_cat_icon = 0;
GLuint image_texture_cat_icon = 0;
bool retMouseIcon;
int image_width_mouse_icon = 0;
int image_height_mouse_icon = 0;
GLuint image_texture_mouse_icon = 0;
bool retDiploma;
int image_width_diploma = 0;
int image_height_diploma = 0;
GLuint image_texture_diploma = 0;
bool retHourglass;
int image_width_hourglass = 0;
int image_height_hourglass = 0;
GLuint image_texture_hourglass = 0;
bool retPartyIcon;
int image_width_party_icon = 0;
int image_height_party_icon = 0;
GLuint image_texture_party_icon = 0;
bool retZeroesOnes;
int image_width_zeroes_ones = 0;
int image_height_zeroes_ones = 0;
GLuint image_texture_zeroes_ones = 0;
bool retFireplace;
int image_width_fireplace = 0;
int image_height_fireplace = 0;
GLuint image_texture_fireplace = 0;
bool retMouseIconPale;
int image_width_mouse_icon_pale = 0;
int image_height_mouse_icon_pale = 0;
GLuint image_texture_mouse_icon_pale = 0;
bool retCatIconPale;
int image_width_cat_icon_pale = 0;
int image_height_cat_icon_pale = 0;
GLuint image_texture_cat_icon_pale = 0;


std::vector<Model*> sceneObjects;

//Scene
static SceneLoader* scene;

// COLLISION DEBUG
static Cube* wall1;
static Cube* wall2;
static CollisionDetector cDetector;
// COLLITION DEBUG

// state variables
static bool gameEnded = 0;
static bool gameStarted = 0;
static bool playerSelect = false;
static bool gameStartPressed = 0;
static bool mouse1Clicked = false;
static bool mouse2Clicked = false;
static bool mouse3Clicked = false;
static bool catClicked = false;
static bool playerSelected = false;
static int finalDestRotateTime = -1;
static int timeLeftStationaryItem = 0;
static int timeLeftStationaryItem2 = 0;
static bool* holdIdStationary;
static bool* holdIdStationary2;
static bool task1 = false;
static bool stationary1 = false;
static bool stationary2 = false;
static bool catSeesItem = false;
static int cheese = 0;
static int numPlayers = 0;
static int finalTime = 0;
static bool catWon = 0;
unsigned int my_id;
static int currTime = 0;
static double prevXPos;
static double prevYPos;
static int select = 1;
static bool pause = false;
static bool showMouse = false;
static bool middlePressed = false;
static bool isThirdPersonCam = false;
static const char* scenes[4] = { "Animation Demo", "Maze", "Backpack", "Scene import Demo"};
static bool movingState[PLAYER_NUM];

//for particle demo
float x = 0.0f;
float y = 0.0f;
float z = 0.0f;

static glm::mat4 finalDest = glm::mat4(1); //where item is taken to

static bool keys[4];
static bool keyHeld = false;
static bool mouseMoving = false;
static int direction = -1;

static glm::mat4 currRotationUpdate = glm::mat4(1);
static float currRotationDelta = 0;

static char itemhold = PLAYER_NUM + 1;

// fonts
static ImFont* plainFont;
static ImFont* cuteFont;
static ImFont* HUGEcuteFont;
static ImFont* MASSIVEcuteFont;

// callbacks
static void resizeCallback(GLFWwindow* window, int width, int height);
static void cursorCallback(GLFWwindow* window, double xPos, double yPos);
static void scrollCallback(GLFWwindow* window, double xMove, double yMove);
static void mouseCallback(GLFWwindow* window, int button, int action, int mods);
static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

// debug private functions
static void drawOBB(const OBB& obb, const glm::mat4& viewProjMat, GLuint shader, bool fill);

/**
 * Create a GLFW window of given size
 *
 * @param   width       Window width
 * @param   height      Window height
 * @param   windowTitle Window title
 * @return  Pointer to GLFW window
**/
GLFWwindow* Client::createWindow(int width, int height, std::string windowTitle) {
    glfwWindowHint(GLFW_SAMPLES, 4);
    GLFWwindow* window = glfwCreateWindow(width, height, windowTitle.c_str(), NULL, NULL);
    prevXPos = (double) width / 2;
    prevYPos = (double) height / 2;
    window_height = height;
    window_width = width;
    return window;
}

/**
 * Print out glfw error
 *
 * @param   error       Error number
 * @param   description Error description
**/
void Client::errorCallback(int error, const char* description) {
    spdlog::error("GLFW Error {}: {}", error, description);
}

/**
 * Setup OpenGL settings like clear color
**/
void Client::setupGLSettings() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_DEPTH_CLAMP);
    glEnable(GL_MULTISAMPLE);
    glDepthFunc(GL_LEQUAL);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

// Simple helper function to load an image into a OpenGL texture with common settings
bool Client::LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height)
{
    // Load from file
    int image_width = 0;
    int image_height = 0;
    unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
    if (image_data == NULL)
        return false;

    // Create a OpenGL texture identifier
    GLuint image_texture;
    glGenTextures(1, &image_texture);
    glBindTexture(GL_TEXTURE_2D, image_texture);

    // Setup filtering parameters for display
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same

    // Upload pixels into texture
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    stbi_image_free(image_data);

    *out_texture = image_texture;
    *out_width = image_width;
    *out_height = image_height;

    return true;
}

/**
 * Initialize shader program
 *
 * @return  True if success, False otherwise
**/
bool Client::initializeClient() {
    // initialize shader
    shader = Shader::loadShaders("../../shaders/shader.vert", "../../shaders/shader.frag");
    skyboxShader = Shader::loadShaders("../../shaders/skyboxShader.vert", "../../shaders/skyboxShader.frag");
    particleShader = Shader::loadShaders("../../shaders/particleShader.vert", "../../shaders/particleShader.frag");

    if (!shader) {
        spdlog::error("Failed to initialize main shader programs.");
        return false;
    }
    if (!skyboxShader) {
        spdlog::error("Failed to initialize skyboxShader programs.");
        return false;
    }
    if (!particleShader) {
        spdlog::error("Failed to initialize particleShader programs.");
        return false;
    }

    // initialize camera
    camera = new Camera();

    // initialize light sources
    lightPosn = { {0, 5, -10, 1}, {0, 5, 10, 1}, {1, 1, 1, 0} };
    lightColorn = { {0.9, 0.6, 0.5, 1}, {0.5, 0.6, 0.9, 1}, {0.8, 0.8, 0.8, 1} };
    lightCount = lightPosn.size();

    // particle properties
    ParticleProperty smoke = {
        700,    //amount
        3.0f,   //Life
        glm::vec3(0, -3, 0), //Velocity
        glm::vec3(1, 0, 1), //useRandomVelocity
        glm::vec3(2, 2, 2), //randomPositionRange
        1,      //randomColor
        0.3f,   //colorFade
        0,      //blendMethod
    };

    ParticleProperty flame = {
        200,    //amount
        0.5f,   //Life
        glm::vec3(0, -5, 0), //Velocity
        glm::vec3(0, 3, 0), //useRandomVelocity
        glm::vec3(2, 2, 2), //randomPositionRange
        1,      //randomColor
        1.0f,   //colorFade
        1,      //blendMethod
    };

    ParticleProperty glint = {
        150,    //amount
        1.0f,   //Life
        glm::vec3(0, 2, 0), //Velocity
        glm::vec3(5, 1, 5), //useRandomVelocity
        glm::vec3(2, 2, 2),      //randomPositionRange
        0,      //randomColor
        0.7f,   //colorFade
        0,      //blendMethod
    };

    ParticleProperty trail = {
        200,    //amount
        0.7f,   //Life
        glm::vec3(0, -2, 0), //Velocity
        glm::vec3(1, 2, 1), //useRandomVelocity
        glm::vec3(1, 0, 1.5), //randomPositionRange
        1,      //randomColor
        1.5f,   //colorFade
        0,      //blendMethod
    };

    ParticleProperty micetrail = {
        200,    //amount
        0.6f,   //Life
        glm::vec3(0, -2, 0), //Velocity
        glm::vec3(1, 2, 1), //useRandomVelocity
        glm::vec3(0.5, 0, 1), //randomPositionRange
        1,      //randomColor
        1.7f,   //colorFade
        0,      //blendMethod
    };

    //initialize particle system
    smokeparticles = new ParticleSystem(particleShader, "../../particles/smoke.png", smoke);
    flameparticles = new ParticleSystem(particleShader, "../../particles/flame.png", flame);
    glintparticles = new ParticleSystem(particleShader, "../../particles/glint.png", glint);
    cattrailparticles = new ParticleSystem(particleShader, "../../particles/dust.png", trail);

    micetrailparticles1 = new ParticleSystem(particleShader, "../../particles/dust.png", micetrail);
    micetrailparticles2 = new ParticleSystem(particleShader, "../../particles/dust.png", micetrail);
    micetrailparticles3 = new ParticleSystem(particleShader, "../../particles/dust.png", micetrail);

    // initialize objects
    ground = new Cube(glm::vec3(-10, -1, -10), glm::vec3(10, 1, 10));
    ground->moveGlobal(glm::vec3(0, -3, 0));

    cat = new Model("../../objects/cat/cat_idle.fbx");
    catidleAnimation = new Animation("../../objects/cat/cat_idle.fbx", cat);
    catanimator = new Animator(catidleAnimation);

    fakecat = new Model("../../objects/mouse/mouse.obj");

    mouse1 = new Model("../../objects/mouse/mouse_idle.fbx");
    mouseidleAnimation1 = new Animation("../../objects/mouse/mouse_idle.fbx", mouse1);
    mouseanimator1 = new Animator(mouseidleAnimation1);

    mouse2 = new Model("../../objects/mouse/mouse_idle.fbx");
    mouseidleAnimation2 = new Animation("../../objects/mouse/mouse_idle.fbx", mouse2);
    mouseanimator2 = new Animator(mouseidleAnimation2);

    mouse3 = new Model("../../objects/mouse/mouse_idle.fbx");
    mouseidleAnimation3 = new Animation("../../objects/mouse/mouse_idle.fbx", mouse3);
    mouseanimator3 = new Animator(mouseidleAnimation3);

    //backpack = new Model("../../objects/backpack/backpack.obj");
    maze = new Model("../../objects/maze_textured/maze3D.obj");
    maze->moveGlobal(glm::vec3(0, -3, 0));
    bear = new Model("../../objects/bear/bear.obj");
    bear->moveGlobal(glm::vec3(55, -3, -135));
    bear->scale(glm::vec3(0.5));
    fallenstar = new Model("../../objects/fallenstar/fallenstar.obj");
    fallenstar->moveGlobal(glm::vec3(95, 2, -35));
   
    item = new Model("../../objects/diploma/diploma.dae");
   
    item2 = new Model("../../objects/computer/computer.obj");
    item2->moveGlobal(glm::vec3(70, 0, -5));
    item2->scale(glm::vec3(5));
    item3 = new Model("../../objects/books/books.fbx");
    item3->moveGlobal(glm::vec3(65, -1, -10));
    item3->scale(glm::vec3(6));
    
    //item = new Model("../../objects/backpack/backpack.obj");

    //demoChar = new Model("../../objects/Kachujin/jog.fbx");
    //demoChar->scale(glm::vec3(0.3));
    //demoChar->moveGlobal(glm::vec3(9, -2, -2));

    //demoAnimation = new Animation("../../objects/Kachujin/jog.fbx", demoChar);
    //animator = new Animator(demoAnimation);

    geisel = new Model("../../objects/Geisel/Geisel.fbx");
    
    geisel->moveGlobal(glm::vec3(75, 5, -75));
    geisel->scale(glm::vec3(0.005));

    sungod = new Model("../../objects/sungod/sungod.obj");
    sungod->moveGlobal(glm::vec3(105, 2, -135));
    sungod->spin(90);
    //sungod->scale(glm::vec3(2));

    //demoChar2 = new Model("../../objects/morak/morak_samba_small.fbx");
    //demoAnimation2 = new Animation("../../objects/morak/morak_samba_small.fbx", demoChar2);
    //animator2 = new Animator(demoAnimation2);
    //demoChar2->scale(glm::vec3(0.6));
    //demoChar2->moveGlobal(glm::vec3(0, -2, 0));


    ret = LoadTextureFromFile("../../objects/ImGui/cute_cat.png", &my_image_texture, &my_image_width, &my_image_height);
    retGameOver = LoadTextureFromFile("../../objects/ImGui/explosion.png", &image_texture_game_over, &image_width_game_over, &image_height_game_over);
    retMouseWin = LoadTextureFromFile("../../objects/ImGui/celebration.png", &image_texture_mouse_win, &image_width_mouse_win, &image_height_mouse_win);
    retMouseFlag = LoadTextureFromFile("../../objects/ImGui/cheese.png", &image_texture_mouse_flag, &image_width_mouse_flag, &image_height_mouse_flag);
    retMouseFlagPale = LoadTextureFromFile("../../objects/ImGui/cheese_paler2.png", &image_texture_mouse_flag_pale, &image_width_mouse_flag_pale, &image_height_mouse_flag_pale);
    retMap = LoadTextureFromFile("../../objects/ImGui/minimap.png", &image_texture_map, &image_width_map, &image_height_map);
    retGameStart = LoadTextureFromFile("../../objects/ImGui/game_start_maze2.jpg", &image_texture_game_start, &image_width_game_start, &image_height_game_start);
    retStartCat = LoadTextureFromFile("../../objects/ImGui/mao_cat.png", &image_texture_start_cat, &image_width_start_cat, &image_height_start_cat);
    retStartMouse = LoadTextureFromFile("../../objects/ImGui/mao_mouse.png", &image_texture_start_mouse, &image_width_start_mouse, &image_height_start_mouse);
    retStartCatuate = LoadTextureFromFile("../../objects/ImGui/catuate2.png", &image_texture_start_catuate, &image_width_start_catuate, &image_height_start_catuate);
    retCatIcon = LoadTextureFromFile("../../objects/ImGui/cat_icon.png", &image_texture_cat_icon, &image_width_cat_icon, &image_height_cat_icon);
    retMouseIcon = LoadTextureFromFile("../../objects/ImGui/mouse_icon.png", &image_texture_mouse_icon, &image_width_mouse_icon, &image_height_mouse_icon);
    retDiploma = LoadTextureFromFile("../../objects/ImGui/diploma.png", &image_texture_diploma, &image_width_diploma, &image_height_diploma);
    retHourglass = LoadTextureFromFile("../../objects/ImGui/hourglass.png", &image_texture_hourglass, &image_width_hourglass, &image_height_hourglass);
    retPartyIcon = LoadTextureFromFile("../../objects/ImGui/party_icon.png", &image_texture_party_icon, &image_width_party_icon, &image_height_party_icon);
    retZeroesOnes = LoadTextureFromFile("../../objects/ImGui/nyan.png", &image_texture_zeroes_ones, &image_width_zeroes_ones, &image_height_zeroes_ones);
    retFireplace = LoadTextureFromFile("../../objects/ImGui/fireplace2.png", &image_texture_fireplace, &image_width_fireplace, &image_height_fireplace);
    retOneThird = LoadTextureFromFile("../../objects/ImGui/one_third.png", &image_texture_one_third, &image_width_one_third, &image_height_one_third);
    retTwoThirds = LoadTextureFromFile("../../objects/ImGui/two_thirds.png", &image_texture_two_thirds, &image_width_two_thirds, &image_height_two_thirds);
    retMouseIconPale = LoadTextureFromFile("../../objects/ImGui/mouse_icon_pale.png", &image_texture_mouse_icon_pale, &image_width_mouse_icon_pale, &image_height_mouse_icon_pale);
    retCatIconPale = LoadTextureFromFile("../../objects/ImGui/cat_icon_pale.png", &image_texture_cat_icon_pale, &image_width_cat_icon_pale, &image_height_cat_icon_pale);

    // COLLISION DEBUG
    wall1 = new Cube(glm::vec3(-2, -5, -1), glm::vec3(2, 5, 1));
    wall1->moveGlobal(glm::vec3(8, 0, -8));
    wall2 = new Cube(glm::vec3(-2, -5, -1), glm::vec3(2, 5, 1));
    wall2->moveGlobal(glm::vec3(8, 0, 8));
    cDetector.insert(wall1->getOBB());
    cDetector.insert(wall2->getOBB());
    cDetector.insert(cat->getOBB());
    // COLLITION DEBUG

    //Load the scene
    scene = new SceneLoader("../../scripts/scene.txt");
    sceneObjects = scene->load();

    //hard coded for now
    players[0] = cat;
    players[1] = mouse1;
    players[2] = mouse2;
    players[3] = mouse3;

    skybox = new Skybox();

    ImGuiIO& io = ImGui::GetIO();
    plainFont = io.Fonts->AddFontDefault();
    cuteFont = io.Fonts->AddFontFromFileTTF("../../fonts/Gidole/Gidolinya-Regular.otf", 32.0f);
    HUGEcuteFont = io.Fonts->AddFontFromFileTTF("../../fonts/Gidole/Gidolinya-Regular.otf", 52.0f);
    MASSIVEcuteFont = io.Fonts->AddFontFromFileTTF("../../fonts/Gidole/Gidolinya-Regular.otf", 70.0f);


    //initialize audio
    audioEngine->LoadBank("Master.bank", FMOD_STUDIO_LOAD_BANK_NORMAL);
    audioEngine->LoadBank("Master.strings.bank", FMOD_STUDIO_LOAD_BANK_NORMAL);

    audioEngine->PlayEvent("event:/music1");

    return true;
}

/**
 * Display objects
**/
void Client::displayCallback() {

    Camera* currCam = isThirdPersonCam ? thirdPersonCamera : camera;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // activate the shader program and send some values
    glUseProgram(shader);
    glUniform3fv(glGetUniformLocation(shader, "eyePos"), 1, glm::value_ptr(currCam->pos));
    glUniform1i(glGetUniformLocation(shader, "lightCount"), lightCount);
    glUniform4fv(glGetUniformLocation(shader, "lightPosn"), lightCount, (float*)lightPosn.data());
    glUniform4fv(glGetUniformLocation(shader, "lightColorn"), lightCount, (float*)lightColorn.data());
    glUseProgram(0);

    glUseProgram(particleShader);
    glUniform1i(glGetUniformLocation(particleShader, "sprite"), 0);
    glUseProgram(0);

    glm::vec3 Camera_Right = glm::vec3(currCam->view[0][0], currCam->view[1][0], currCam->view[2][0]);
    glm::vec3 Camera_Up = glm::vec3(currCam->view[0][1], currCam->view[1][1], currCam->view[2][1]);

    glm::mat4 identityMat = glm::mat4(1);
    isThirdPersonCam = false;

    //performance tradeoff: drawing skybox first
    //to enable alpha blending for particle system
    glUseProgram(skyboxShader);
    glUniform1i(glGetUniformLocation(skyboxShader, "skybox"), 0);
    glUseProgram(0);
    //drop right column
    glm::mat4 viewNoTranslate = glm::mat4(glm::mat3(currCam->view));
    skybox->draw(currCam->projection * viewNoTranslate, skyboxShader);

    switch (select) {
    case 0: {

        for (auto character : players) {
            character->draw(currCam->viewProjMat, identityMat, shader);
        }

        //calcFinalBoneMatrix(animator);
        //demoChar->draw(currCam->viewProjMat, identityMat, shader);

        //calcFinalBoneMatrix(animator2);
        //demoChar2->draw(currCam->viewProjMat, identityMat, shader);
       
        ground->draw(currCam->viewProjMat, identityMat, shader);

        smokeparticles->draw(currCam->viewProjMat, Camera_Right, Camera_Up);
        flameparticles->draw(currCam->viewProjMat, Camera_Right, Camera_Up);
        glintparticles->draw(currCam->viewProjMat, Camera_Right, Camera_Up);

        // COLLITION DEBUG
        /*
        wall1->draw(currCam->viewProjMat, identityMat, shader);
        wall2->draw(currCam->viewProjMat, identityMat, shader);
        drawOBB(wall1->getOBB(), currCam->viewProjMat, shader, false);
        drawOBB(wall2->getOBB(), currCam->viewProjMat, shader, false);


        int collideID = cDetector.check(2);
        if (collideID != -1) {
            spdlog::info("Collide with wall {}", collideID + 1);
            drawOBB(tyra->getOBB(), currCam->viewProjMat, shader, true);
        }
        else {
            drawOBB(tyra->getOBB(), currCam->viewProjMat, shader, false);
        }
        */
        // COLLITION DEBUG

        //item->draw(currCam->viewProjMat, identityMat, shader);

        break;
    }

    case 1: {
        isThirdPersonCam = true;
        maze->draw(currCam->viewProjMat, identityMat, shader);
        bear->draw(currCam->viewProjMat, identityMat, shader);
       
        geisel->draw(currCam->viewProjMat, identityMat, shader);
        sungod->draw(currCam->viewProjMat, identityMat, shader);
        fallenstar->draw(currCam->viewProjMat, identityMat, shader);
        
        calcFinalBoneMatrix(catanimator);
        cat->draw(currCam->viewProjMat, identityMat, shader);
        calcFinalBoneMatrix(mouseanimator1);
        mouse1->draw(currCam->viewProjMat, identityMat, shader);
        calcFinalBoneMatrix(mouseanimator2);
        mouse2->draw(currCam->viewProjMat, identityMat, shader);
        calcFinalBoneMatrix(mouseanimator3);
        mouse3->draw(currCam->viewProjMat, identityMat, shader);
        item->draw(currCam->viewProjMat, identityMat, shader);
        item2->draw(currCam->viewProjMat, identityMat, shader);
        item3->draw(currCam->viewProjMat, identityMat, shader);

        cattrailparticles->draw(currCam->viewProjMat, Camera_Right, Camera_Up);
        micetrailparticles1->draw(currCam->viewProjMat, Camera_Right, Camera_Up);
        micetrailparticles2->draw(currCam->viewProjMat, Camera_Right, Camera_Up);
        micetrailparticles3->draw(currCam->viewProjMat, Camera_Right, Camera_Up);
        
        break;
    }

    case 2: {
        //backpack->draw(currCam->viewProjMat, identityMat, shader);
        //drawOBB(backpack->getOBB(), currCam->viewProjMat, shader, false);
        break;
    }
    case 3: {
        scene->draw(currCam->viewProjMat, identityMat, shader, sceneObjects);
        break;
    }
    }


}

/**
 * Update objects when idle
**/
void Client::idleCallback(float dt) {
    Camera* currCamera = isThirdPersonCam ? thirdPersonCamera : camera;
    currCamera->update();
    mouseMoving = false;

    if (gameEnded) {
        pause = 1;
    }

    if (!pause) {
        //backpack->update();

        // COLLITION DEBUG
        wall2->update();
        cDetector.update(wall2->getOBB(), 1);
        cDetector.update(cat->getOBB(), 2);
        // COLLITION DEBUG

        //animation update
        //animator->update(dt);
        //animator2->update(dt);

        catanimator->update(dt);

        mouseanimator1->update(dt);
        mouseanimator2->update(dt);
        mouseanimator3->update(dt);


        //particles update
        smokeparticles->update(dt, 2, glm::vec3(x,y + 1,z));
        flameparticles->update(dt, 1, glm::vec3(x, y - 2, z));
        glintparticles->update(dt, 2, glm::vec3(-7, 4, 0));

        glm::mat4 trailModel = cat->getModel() * glm::translate(glm::mat4(1), glm::vec3(0, 0.3, 0.5));
        glm::vec3 catpos = trailModel[3];
        trailModel = mouse1->getModel() * glm::translate(glm::mat4(1), glm::vec3(0, 0.4, 0));
        glm::vec3 micepos1 = trailModel[3];
        trailModel = mouse2->getModel() * glm::translate(glm::mat4(1), glm::vec3(0, 0.4, 0));
        glm::vec3 micepos2 = trailModel[3];
        trailModel = mouse3->getModel() * glm::translate(glm::mat4(1), glm::vec3(0, 0.4, 0));
        glm::vec3 micepos3 = trailModel[3];

        if (movingState[0] == true) {
            cattrailparticles->update(dt, 2, catpos);
        }
        else {
            cattrailparticles->update(dt, 0, catpos);
        }

        if (movingState[1] == true) {
            micetrailparticles1->update(dt, 2, micepos1);
        }
        else {
            micetrailparticles1->update(dt, 0, micepos1);
        }

        if (movingState[2] == true) {
            micetrailparticles2->update(dt, 2, micepos2);
        }
        else {
            micetrailparticles2->update(dt, 0, micepos2);
        }

        if (movingState[3] == true) {
            micetrailparticles3->update(dt, 2, micepos3);
        }
        else {
            micetrailparticles3->update(dt, 0, micepos3);
        }
    }

    if (!isThirdPersonCam && keyHeld) {
        switch (direction) {
        case FORWARD:
            camera->move({ 0, 0, -0.5 });
            break;

        case BACK:
            camera->move({ 0, 0, 0.5 });
            break;

        case LEFT:
            camera->move({ -0.5, 0, 0 });
            break;

        case RIGHT:
            camera->move({ 0.5, 0, 0 });
            break;
        }
    }
}

/**
 * Cleanup the client, free all memories
**/
void Client::cleanup() {
    glDeleteProgram(shader);
    glDeleteProgram(skyboxShader);
    delete camera;
    delete thirdPersonCamera;
    delete mouse1;
    delete mouse2;
    delete cat;
    delete ground;
    //delete backpack;
    delete maze;
    delete skybox;
    delete bear;
    delete sungod;
    delete fallenstar;
    delete item;
    delete geisel;
    //delete demoChar;
    //delete demoChar2;
    //delete animator;
    //delete animator2;
    //delete demoAnimation;
    //delete demoAnimation2;

    // COLLISION DEBUG
    delete wall1;
    delete wall2;

    delete scene;
}

/**
 * Client GUI
**/
void Client::GUI() {
    ImGui::Begin("Client GUI");
    ImGui::Checkbox("Pause", &pause);
    ImGui::Checkbox("Third Person Camera", &isThirdPersonCam);
    ImGui::Separator();
    ImGui::Text("Press F to toggle show/hide mouse");
    ImGui::ListBox("Scene Selection", &select, scenes, IM_ARRAYSIZE(scenes), IM_ARRAYSIZE(scenes));
    ImGui::Separator();
    ImGui::Text("particle demo pos");
    ImGui::SliderFloat("particle x", &x, 0.0f, 5.0f);
    ImGui::SliderFloat("particle y", &y, 0.0f, 5.0f);
    ImGui::SliderFloat("particle z", &z, 0.0f, 5.0f);
    ImGui::End();
}

void TextCentered(std::string text) {
    auto windowWidth = ImGui::GetWindowSize().x;
    auto textWidth = ImGui::CalcTextSize(text.c_str()).x;

    ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
    ImGui::Text(text.c_str());
}

void TextYCentered(std::string text) {
    auto windowWidth = ImGui::GetWindowSize().y;
    auto textWidth = ImGui::CalcTextSize(text.c_str()).y;

    ImGui::SetCursorPosY((windowWidth - textWidth) * 0.5f);
    ImGui::Text(text.c_str());
}

void Client::timeGUI() {
    ImGuiWindowFlags flags = 0;
   
    flags |= ImGuiWindowFlags_NoBackground;
    flags |= ImGuiWindowFlags_NoTitleBar;
    flags |= ImGuiWindowFlags_NoScrollbar;
    flags |= ImGuiWindowFlags_NoResize;
    double adjustment = 0.27;
    ImGui::SetNextWindowSize(ImVec2(my_image_width * adjustment + 30, my_image_height * adjustment + 50));
    //ImGui::SetNextWindowPos(ImVec2(window_width - (my_image_width / 2) + 170, 0), 0, ImVec2(0, 0));
    ImGui::SetNextWindowPos(ImVec2(window_width-(my_image_width*adjustment)-30, 10), 0, ImVec2(0, 0));
    //printf(" % d image height %d image width\n", my_image_height, my_image_width);
    
    ImGui::Begin("Time GUI", NULL, flags);

    if (gameEnded)
        currTime = finalTime;

    // get game countdown time
    int time = 180 - currTime;
    int minute = (time % 3600) / 60;  // Minute component
    int seconds = time % 60;          // Second component 

    if (!hasPlayedTimer && minute == 0) {
        audioEngine->PlayEvent("event:/panic"); //TODO Audio, play timer
        hasPlayedTimer = true;
    }

    if (time < 0) {
        minute = 0;
        seconds = 0;
    }
     

    ImGui::Image((void*)(intptr_t)my_image_texture, ImVec2(my_image_width * adjustment, my_image_height * adjustment));

    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0, 0.5f, 1.0f, 1.0f));
    ImGui::PushFont(cuteFont);
    ImGui::SetCursorPosY((my_image_width * adjustment)/2);
    ImGui::SetCursorPosX((my_image_height * adjustment) / 2 + 32);
    ImGui::Text("%d:%02d", minute, seconds);
    ImGui::PopFont();
    ImGui::PopStyleColor();

    ImGui::End();
}

void Client::audioUpdate() {

    // Mouse/Cat collision
    if (hasPlayedMouse1CatCollision && my_id == 0 &&
        players[1]->getModel()[3][1] <= 10) {
        hasPlayedMouse1CatCollision = false;
    }

    if (hasPlayedMouse2CatCollision && my_id == 0 &&
        players[2]->getModel()[3][1] <= 10) {
        hasPlayedMouse2CatCollision = false;
    }

    if (hasPlayedMouse3CatCollision && my_id == 0 &&
        players[3]->getModel()[3][1] <= 10) {
        hasPlayedMouse3CatCollision = false;
    }

    if (!hasPlayedMouse1CatCollision && my_id == 0 &&
        players[1]->getModel()[3][1] > 10) {
        audioEngine->PlayEvent("event:/cat_screech_1");
        hasPlayedMouse1CatCollision = true;
    }

    if (!hasPlayedMouse2CatCollision && my_id == 0 &&
        players[2]->getModel()[3][1] > 10) {
        audioEngine->PlayEvent("event:/cat_screech_1");
        hasPlayedMouse2CatCollision = true;
    }

    if (!hasPlayedMouse3CatCollision && my_id == 0 &&
        players[3]->getModel()[3][1] > 10) {
        audioEngine->PlayEvent("event:/cat_screech_1");
        hasPlayedMouse3CatCollision = true;
    }

    if (!hasPlayedMouseCatCollision && my_id != 0 && player->getModel()[3][1] > 10) {
        audioEngine->PlayEvent("event:/mice_shriek_1");
        hasPlayedMouseCatCollision = true;
        hasPlayedTask1 = false; // task1 respawns on collision
    }

    if (!hasPlayedMouseCatCollision && my_id != 0 && player->getModel()[3][1] > 10) {
        hasPlayedMouseCatCollision = false;
    }



    // Tasks 
    if (hasPlayedTask1 && !task1 && my_id != 0) {
        hasPlayedTask1 = false;
    }

    if (hasPlayedStationary1 && !stationary1 && my_id != 0) {
        hasPlayedStationary1 = false;
    }

    if (hasPlayedStationary2 && !stationary2 && my_id != 0) {
        hasPlayedStationary2 = false;
    }

    if (!hasPlayedTask1 && task1 && my_id != 0) {
        audioEngine->PlayEvent("event:/powerup_1"); //TODO Audio, play task1 pickup
        hasPlayedTask1 = true;
    }

    if (!hasPlayedStationary1 && stationary1 && my_id != 0) {
        audioEngine->PlayEvent("event:/powerup_1"); //TODO Audio, play stationary1 pickup
        hasPlayedStationary1 = true;
    }

    if (!hasPlayedStationary2 && stationary2 && my_id != 0) {
        audioEngine->PlayEvent("event:/powerup_1"); //TODO Audio, play stationary2 pickup
        hasPlayedStationary2 = true;
    }
}

void Client::ItemHoldGUI() {
    double adjustment = 0.18;
    
    ImGuiWindowFlags flags = 0;
    flags |= ImGuiWindowFlags_NoBackground;
    flags |= ImGuiWindowFlags_NoTitleBar;
    flags |= ImGuiWindowFlags_NoScrollbar;
    flags |= ImGuiWindowFlags_NoResize;
    int tasks = 0;

    if (itemhold != PLAYER_NUM + 1 || task1) 
        tasks++;
    if (stationary1)
        tasks++;
    if (stationary2)
        tasks++;

    ImGui::SetNextWindowSize(ImVec2(image_width_mouse_flag * adjustment+10, image_height_mouse_flag*adjustment+10));
    ImGui::SetNextWindowPos(ImVec2(280, 15), 0, ImVec2(0, 0));
   
    ImGui::Begin("ItemHold GUI", NULL, flags);

    
    if (tasks == 0) {
        ImGui::Image((void*)(intptr_t)image_texture_mouse_flag_pale, ImVec2(image_width_mouse_flag_pale * adjustment, image_height_mouse_flag_pale * adjustment));
    }
    else if (tasks == 1) {
        ImGui::Image((void*)(intptr_t)image_texture_one_third, ImVec2(image_width_one_third * adjustment, image_height_one_third * adjustment));
    }
    else if (tasks == 2) {
        ImGui::Image((void*)(intptr_t)image_texture_two_thirds, ImVec2(image_width_two_thirds * adjustment, image_height_two_thirds * adjustment));
    }
    else {
        ImGui::Image((void*)(intptr_t)image_texture_mouse_flag, ImVec2(image_width_mouse_flag * adjustment, image_height_mouse_flag * adjustment));
    }

    ImGui::End();
}

void displayLocation(glm::mat4 model, int id) {
    float locX = model[3][0] * 1.55 + 25;
    float locZ = abs(model[3][2]) * 1.55 + 25;

    if (model[3][1] > 10) { // this is a hack, manually checking if banished height
        return;
    }

    float icon_size = 11.0f;
    

    if (id == 0) { // display cat
        ImGui::GetWindowDrawList()->AddImage((void*)(intptr_t)image_texture_cat_icon, ImVec2(locZ-icon_size, locX-icon_size), ImVec2(locZ+icon_size, locX+icon_size), ImVec2(0, 0), ImVec2(1, 1));
    }
    else if (id < 4) { // display mice
        ImGui::GetWindowDrawList()->AddImage((void*)(intptr_t)image_texture_mouse_icon, ImVec2(locZ - icon_size, locX - icon_size), ImVec2(locZ + icon_size, locX + icon_size), ImVec2(0, 0), ImVec2(1, 1));
    }
    else {
      /*  if (currTime % 2 == 0)
            ImGui::GetForegroundDrawList()->AddCircle(ImVec2(locZ, locX), 2, IM_COL32(r, g, b, 255), 100, 2.f);*/
    
        if (itemhold == my_id) { // final Dest location revealed only to player with item
            if (currTime % 2 == 0) {
                float locX1 = finalDest[3][0] * 1.55 + 25;
                float locZ1 = abs(finalDest[3][2]) * 1.55 + 25;
                //ImGui::GetWindowDrawList()->AddImage((void*)(intptr_t)image_texture_diploma, ImVec2(locZ1 - icon_size, locX1 - icon_size), ImVec2(locZ1 + icon_size, locX1 + icon_size), ImVec2(0, 0), ImVec2(1, 1));
                icon_size = 12.0f;
                ImGui::GetWindowDrawList()->AddImage((void*)(intptr_t)image_texture_party_icon, ImVec2(locZ1 - icon_size, locX1 - icon_size), ImVec2(locZ1 + icon_size, locX1 + icon_size), ImVec2(0, 0), ImVec2(1, 1));
            
            }
              
                //ImGui::GetForegroundDrawList()->AddCircle(ImVec2(75*1.15 + 24, 75* 1.15 + 26), 2, IM_COL32(204, 0, 204, 255), 100, 2.f);
        }
        else if (id == 4 && itemhold == PLAYER_NUM + 1) { // if no player holding flag, show location of flag
            icon_size = 9.0f;
            if (currTime % 2 == 0)
                ImGui::GetWindowDrawList()->AddImage((void*)(intptr_t)image_texture_mouse_flag, ImVec2(locZ - icon_size, locX - icon_size), ImVec2(locZ + icon_size, locX + icon_size), ImVec2(0, 0), ImVec2(1, 1));
        }
        else if (id >= 5) {
            icon_size = 9.0f;
            if (currTime % 2 == 0)
                ImGui::GetWindowDrawList()->AddImage((void*)(intptr_t)image_texture_mouse_flag, ImVec2(locZ - icon_size, locX - icon_size), ImVec2(locZ + icon_size, locX + icon_size), ImVec2(0, 0), ImVec2(1, 1));
        }
    }
    
    
}

void Client::miniMapGUI() {

    if (gameStarted == 0)
        return;

    double adjustment = 0.4;
    float height_resize = window_height / static_cast<float>(1017);
    float width_resize = window_width / static_cast<float>(1920);
    ImGuiWindowFlags flags = 0;
    flags |= ImGuiWindowFlags_NoBackground;
    flags |= ImGuiWindowFlags_NoTitleBar;
    flags |= ImGuiWindowFlags_NoScrollbar;
    flags |= ImGuiWindowFlags_NoResize;

    ImGui::SetNextWindowSize(ImVec2(image_width_map * adjustment+15, image_height_map * adjustment+15));
    ImGui::SetNextWindowPos(ImVec2(15, 15), 0, ImVec2(0, 0));
    ImGui::Begin("MiniMap GUI", NULL, flags);
    ImGui::Image((void*)(intptr_t)image_texture_map, ImVec2(image_width_map * adjustment, image_height_map * adjustment));
    
    if (players[0] && my_id == 0) {
        displayLocation(players[0]->getModel(), 0);
    }
    if (players[1]) {
        displayLocation(players[1]->getModel(), 1);
    }

    if (players[2]) {
        displayLocation(players[2]->getModel(), 2);
    }

    if (players[3]) {
        displayLocation(players[3]->getModel(), 3);
    }

    
    if (item && (my_id != 0 || catSeesItem)) {
        displayLocation(item->getModel(), 4);
    }

    if (item2 && (my_id != 0 || catSeesItem)) {
        displayLocation(item2->getModel(), 5);
    }

    if (item3 && (my_id != 0 || catSeesItem)) {
        displayLocation(item3->getModel(), 6);
    }

    ImGui::End();
}

void Client::finalDestGUI() {
    if (finalDestRotateTime < 0 || gameEnded == 1 || itemhold != my_id)
        return;

    ImGuiWindowFlags flags = 0;
    float adjustment = 0.15f;
    int time = 180 - currTime;
    flags |= ImGuiWindowFlags_NoBackground;
    flags |= ImGuiWindowFlags_NoTitleBar;
    flags |= ImGuiWindowFlags_NoScrollbar;
    flags |= ImGuiWindowFlags_NoResize;

    if (finalDestRotateTime < time)
        time = finalDestRotateTime;


    ImGui::SetNextWindowSize(ImVec2(180, 200), 0);
    ImGui::SetNextWindowPos(ImVec2(window_width - 450, 16));
    ImGui::Begin("FinalDest GUI", NULL, flags);
    ImGui::Image((void*)(intptr_t)image_texture_hourglass, ImVec2(image_width_hourglass * adjustment, image_height_hourglass * adjustment));
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0, 110 / 255.0f, 51 / 255.0f, 1.0f));
    ImGui::PushFont(cuteFont);
    //ImGui::SetCursorPos(ImVec2(window_width - 450 + (image_width_hourglass*adjustment)/2, 15 + (image_height_hourglass*adjustment) / 2));
    ImGui::SetCursorPosY(80 + image_width_hourglass*adjustment/2);
    auto windowWidth = ImGui::GetWindowSize().x;
    ImGui::SetCursorPosX((windowWidth - image_width_hourglass*adjustment) * 0.5f + 6);
    ImGui::Text("0:%02d", time);
    ImGui::PopFont();
    ImGui::PopStyleColor();
    ImGui::End();
}

void Client::stationaryItemGUI() {
    if (gameEnded == 1 || my_id == 0) // don't display on game over or if cat
        return;
 
    ImGuiWindowFlags flags = 0;

    flags |= ImGuiWindowFlags_NoTitleBar;
    flags |= ImGuiWindowFlags_NoScrollbar;
    flags |= ImGuiWindowFlags_NoResize;

    ImGui::SetNextWindowSize(ImVec2(window_width, window_height), 0);
    ImGui::SetNextWindowPos(ImVec2(0, 0));
   
    if (timeLeftStationaryItem > 0 && holdIdStationary[my_id]) { // for computer stationary item
        ImGui::Begin("StationaryItem GUI", NULL, flags);
        ImGui::Image((void*)(intptr_t)image_texture_zeroes_ones, ImVec2(window_width, window_height));
        ImGui::End();
    }
    else if (timeLeftStationaryItem2 > 0 && holdIdStationary2[my_id]) { // for books stationary item
        ImGui::Begin("StationaryItem2 GUI", NULL, flags);
        ImGui::Image((void*)(intptr_t)image_texture_fireplace, ImVec2(window_width, window_height));
        ImGui::End();
    }
  
}

void Client::restore() {
    gameStarted = 0;
    gameEnded = 0;
    catWon = 0;
    currTime = 0;
    pause = 0;
}

void Client::playerSelectGUI() {
    
    if (!playerSelect)
        return;

    double adjust_cat = 0.6f;
    double adjust_mouse = 0.3f;
    ImGuiWindowFlags flags = 0;
    flags |= ImGuiWindowFlags_NoTitleBar;
    flags |= ImGuiWindowFlags_NoScrollbar;
    flags |= ImGuiWindowFlags_NoResize;
    float height_resize = window_height / static_cast<float>(1017);
    float width_resize = window_width / static_cast<float>(1920);
    //printf("height %d width %d\n", (window_height / 1017), (window_width / 1920));

    ImGui::SetNextWindowSize(ImVec2(window_width, window_height), 0);
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(214.0f / 255, 232.0f / 255, 101.0f / 255, 1.0f));
    ImGui::Begin("PlayerSelect GUI", NULL, flags);
    float catLoc = (window_width - image_width_cat_icon * adjust_cat * width_resize) / 5 - 100;
    ImGui::SetCursorPos(ImVec2(catLoc, (window_height - image_height_start_cat * adjust_cat * height_resize) / 2 + 100));
    
    if (!catClicked) {
        if (ImGui::ImageButton((void*)(intptr_t)image_texture_cat_icon, ImVec2(image_width_cat_icon * adjust_cat * width_resize, image_height_cat_icon * adjust_cat * height_resize), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(214.0f / 255, 232.0f / 255, 101.0f / 255, 1.0f), ImVec4(1, 1, 1, 1))) {
            if (!playerSelected) {
                playerSelected = true;
                catClicked = true;
            }          
        }
    }
    else {
        if (!playerSelected) {
            ImGui::SetCursorPos(ImVec2(catLoc, (window_height - image_height_start_cat * adjust_cat * height_resize) / 2 + 100));
            ImGui::Image((void*)(intptr_t)image_texture_cat_icon_pale, ImVec2(image_width_cat_icon * adjust_cat * width_resize, image_height_cat_icon * adjust_cat * height_resize));
        }
       
    }

    
    //ImGui::Image((void*)(intptr_t)image_texture_cat_icon, ImVec2(image_width_cat_icon * adjust_cat * width_resize, image_height_cat_icon * adjust_cat * height_resize));
    
    float mouseLocX = (3 * window_width) / 4 - (image_width_mouse_icon * adjust_mouse * width_resize);
    float mouseLocY = (window_height / 3) - (image_height_mouse_icon * adjust_mouse * height_resize) / 2;
    ImGui::SetCursorPos(ImVec2(mouseLocX, mouseLocY));
    if (!mouse1Clicked) {
        if (ImGui::ImageButton((void*)(intptr_t)image_texture_mouse_icon, ImVec2(image_width_mouse_icon * adjust_mouse * width_resize, image_height_mouse_icon * adjust_mouse * height_resize), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(214.0f / 255, 232.0f / 255, 101.0f / 255, 1.0f), ImVec4(1, 1, 1, 1))) {
            mouse1Clicked = true;
            
        }
    }
    else {
        ImGui::SetCursorPos(ImVec2(mouseLocX, mouseLocY));
        ImGui::Image((void*)(intptr_t)image_texture_mouse_icon_pale, ImVec2(image_width_mouse_icon * adjust_mouse * width_resize, image_height_mouse_icon * adjust_mouse * height_resize));
    }
       
    mouseLocX = mouseLocX - (image_width_mouse_icon * adjust_mouse * width_resize) / 2;
    mouseLocY = mouseLocY + (image_height_mouse_icon * adjust_mouse * height_resize);
    ImGui::SetCursorPos(ImVec2(mouseLocX, mouseLocY));
    
    if (!mouse2Clicked) {
        if (ImGui::ImageButton((void*)(intptr_t)image_texture_mouse_icon, ImVec2(image_width_mouse_icon * adjust_mouse * width_resize, image_height_mouse_icon * adjust_mouse * height_resize), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(214.0f / 255, 232.0f / 255, 101.0f / 255, 1.0f), ImVec4(1, 1, 1, 1))) {
            mouse2Clicked = true;

        }
    }
    else {
        ImGui::SetCursorPos(ImVec2(mouseLocX, mouseLocY));
        ImGui::Image((void*)(intptr_t)image_texture_mouse_icon_pale, ImVec2(image_width_mouse_icon * adjust_mouse * width_resize, image_height_mouse_icon * adjust_mouse * height_resize));
    }

    //ImGui::Image((void*)(intptr_t)image_texture_mouse_icon, ImVec2(image_width_mouse_icon * adjust_mouse * width_resize, image_height_mouse_icon * adjust_mouse * height_resize));
    mouseLocX = mouseLocX + (image_width_mouse_icon * adjust_mouse * width_resize);
    //mouseLocY = mouseLocY + image_height_mouse_icon * adjust_mouse * height_resize;
    ImGui::SetCursorPos(ImVec2(mouseLocX, mouseLocY));

    if (!mouse3Clicked) {
        if (ImGui::ImageButton((void*)(intptr_t)image_texture_mouse_icon, ImVec2(image_width_mouse_icon * adjust_mouse * width_resize, image_height_mouse_icon * adjust_mouse * height_resize), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(214.0f / 255, 232.0f / 255, 101.0f / 255, 1.0f), ImVec4(1, 1, 1, 1))) {
            mouse3Clicked = true;

        }
    }
    else {
        ImGui::SetCursorPos(ImVec2(mouseLocX, mouseLocY));
        ImGui::Image((void*)(intptr_t)image_texture_mouse_icon_pale, ImVec2(image_width_mouse_icon * adjust_mouse * width_resize, image_height_mouse_icon * adjust_mouse * height_resize));
    }
    //ImGui::Image((void*)(intptr_t)image_texture_mouse_icon, ImVec2(image_width_mouse_icon * adjust_mouse * width_resize, image_height_mouse_icon * adjust_mouse * height_resize));


    
    ImGui::PopStyleColor();
    ImGui::End();
}

void Client::GameStartGUI() {

    if (gameStarted)
        return;

    double adjustment = 3.5f;
    double adjust_cat = 0.6f;
    double adjust_mouse = 0.4f;
    double adjust_catuate = 2.0f;
    ImGuiWindowFlags flags = 0;
    float height_resize = window_height / static_cast<float>(1017);
    float width_resize = window_width / static_cast<float>(1920);
    //flags |= ImGuiWindowFlags_NoBackground;
    flags |= ImGuiWindowFlags_NoTitleBar;
    flags |= ImGuiWindowFlags_NoScrollbar;
    flags |= ImGuiWindowFlags_NoResize;
    //printf("height %d width %d\n", (window_height / 1017), (window_width / 1920));

    ImGui::SetNextWindowSize(ImVec2(window_width, window_height), 0);
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    //ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.8f, 0.8f, 0.0f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(214.0f / 255, 232.0f / 255, 101.0f / 255, 1.0f));
    ImGui::Begin("GameStart GUI", NULL, flags);
    ImGui::PopStyleColor();
    //ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(1.0, 1.0f, 1.0f, 1.0f));
    ImGui::SetCursorPos(ImVec2((window_width-image_width_game_start*adjustment*width_resize)/2,(window_height-image_height_game_start*adjustment*height_resize)/2));
    //ImGui::Image((void*)(intptr_t)image_texture_game_start, ImVec2(image_width_game_start * adjustment, image_height_game_start * adjustment));
    ImGui::Image((void*)(intptr_t)image_texture_game_start, ImVec2(image_width_game_start * adjustment * width_resize, image_height_game_start * adjustment * height_resize));
    float catLoc = ((window_width - image_width_game_start * adjustment * width_resize) / 2 - image_width_start_cat * adjust_cat*width_resize) / 2;
    ImGui::SetCursorPos(ImVec2(catLoc, (window_height - image_height_start_cat * adjust_cat *height_resize) / 2));
    
    ImGui::Image((void*)(intptr_t)image_texture_start_cat, ImVec2(image_width_start_cat * adjust_cat*width_resize, image_height_start_cat * adjust_cat*height_resize));
    float mouseLoc = (window_width - image_width_game_start * adjustment*width_resize) / 2 + ((window_width - image_width_game_start * adjustment*width_resize) / 2 - image_width_start_mouse * adjust_mouse*width_resize) / 2;
    ImGui::SetCursorPos(ImVec2(mouseLoc, (window_height - image_height_start_mouse * adjust_mouse*height_resize) / 2 + 20));
    ImGui::Image((void*)(intptr_t)image_texture_start_mouse, ImVec2(image_width_start_mouse * adjust_mouse*width_resize, image_height_start_mouse * adjust_mouse*height_resize));
    ImGui::SetCursorPos(ImVec2((window_width - image_width_start_catuate* adjust_catuate*width_resize) / 2, 40));
    ImGui::Image((void*)(intptr_t)image_texture_start_catuate, ImVec2(image_width_start_catuate*adjust_catuate*width_resize, image_height_start_catuate*adjust_catuate*height_resize));
    //ImGui::PopStyleColor();
    float buttonLoc = 3 * window_width / 4;
    ImGui::SetCursorPos(ImVec2(buttonLoc, window_height / 2));
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 0.0f, 0.0f, 1.0f));
    ImGui::PushFont(MASSIVEcuteFont);
    
    if (my_id == 0) {
        if (ImGui::Button("Start Game"))
        {
            //playerSelect = true;
            gameStartPressed = true;
            audioEngine->StopEvent("event:/music1");
            audioEngine->PlayEvent("event:/music_placeholder");
        }
    }
    else
        ImGui::Button("Join Party"); // this actually doesn't do anything right now
   
    ImGui::PopFont();
    ImGui::PushFont(cuteFont);
    ImGui::SetCursorPos(ImVec2(buttonLoc, window_height / 3));
    if (numPlayers == 1)
        ImGui::Text("%d player has joined", numPlayers);
    else
        ImGui::Text("%d players have joined", numPlayers);
    ImGui::PopStyleColor();
    ImGui::PopFont();
    ImGui::End();
}


void Client::GameOverGUI() {
    double adjustment; 
    ImGuiWindowFlags flags = 0;
    flags |= ImGuiWindowFlags_NoBackground;
    flags |= ImGuiWindowFlags_NoTitleBar;
    flags |= ImGuiWindowFlags_NoScrollbar;
    flags |= ImGuiWindowFlags_NoResize;
    
    if (gameEnded && !catWon) {
        adjustment = 0.3;
        ImGui::SetNextWindowSize(ImVec2(image_width_mouse_win * adjustment,image_height_mouse_win * adjustment + 400 ));
        //ImGui::SetNextWindowPos(ImVec2((window_width-image_width_mouse_win*adjustment)/2, (window_height - image_height_mouse_win * adjustment) / 2), 0, ImVec2(0, 0));
        ImGui::SetNextWindowPos(ImVec2((window_width - image_width_mouse_win * adjustment) / 2, 50), 0, ImVec2(0, 0));
        ImGui::Begin("GameOver GUI", NULL, flags);      
        ImGui::Image((void*)(intptr_t)image_texture_mouse_win, ImVec2(image_width_mouse_win * adjustment, image_height_mouse_win * adjustment));
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 0.0f, 0.95f));
        ImGui::PushFont(HUGEcuteFont);
        ImGui::SetCursorPosY(0);
        TextCentered("Game Over");
        TextCentered("Mice won!!");
        ImGui::PopFont();
        ImGui::PopStyleColor();
        ImGui::End();
    }
    else if (gameEnded) {
        adjustment = 0.9;
        ImGui::SetNextWindowSize(ImVec2(image_width_game_over * adjustment, window_height));
        ImGui::SetNextWindowPos(ImVec2((window_width - image_width_game_over * adjustment) / 2, 50), 0, ImVec2(0, 0));
        ImGui::Begin("GameOver GUI", NULL, flags);
        ImGui::SetCursorPosY(60);
        ImGui::Image((void*)(intptr_t)image_texture_game_over, ImVec2(image_width_game_over * adjustment, image_height_game_over * adjustment));
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 0.95f));
        ImGui::PushFont(HUGEcuteFont);
        ImGui::SetCursorPosY(0);
        TextCentered("Game Over");
        TextCentered("Cat won!!");
        ImGui::PopFont();
        ImGui::PopStyleColor();
        ImGui::End();
    }
}



MovementState Client::getMovementState() {
    MovementState ms;
    memcpy(ms.dir, keys, sizeof(ms.dir));
    ms.held = keyHeld;
    return ms;
}

RotationState Client::getRotationState() {
    return RotationState{
        currRotationDelta,
        mouseMoving,
    };
}

Model** Client::getPlayers() {
    return players;
}

void Client::resetRotUpdate() {
    currRotationDelta = 0;
}

void Client::setPlayerfromID(unsigned int id) {
    my_id = id;
    player = players[my_id];
    thirdPersonCamera = new ThirdPersonCamera(player);
}

void Client::updateItemLocation(glm::mat4 location) {
    item->setModel(location);
}

void Client::updateItem2Location(glm::mat4 location) {
    item2->setModel(location);
}

void Client::updateItem3Location(glm::mat4 location) {
    item3->setModel(location);
}

void Client::setStationaryItemCountdown(float t, bool* holdId, bool s) {
    timeLeftStationaryItem = (int)t;
    holdIdStationary = holdId;
    stationary1 = s;
}

void Client::setStationaryItem2Countdown(float t, bool* holdId, bool s) {
    timeLeftStationaryItem2 = (int)t;
    holdIdStationary2 = holdId;
    stationary2 = s;
}

void Client::setNumPlayers(int p) {
    numPlayers = p;
}

void Client::setCatViewItem(bool c) {
    catSeesItem = c;
}

void Client::updateTime(int t) {
    currTime = t;
}

void Client::setItemHold(char h, bool s) {
    itemhold = h;
    task1 = s;
}

void Client::setFinalDest(glm::mat4 location, int f) {
    finalDest = location;
    finalDestRotateTime = f;
    //printf("%d finalDest \n", finalDestRotateTime);
}

void Client::setGameStart() {
    gameStarted = 1;
}

bool Client::checkGameStart() {
    if (gameStartPressed) {
        gameStartPressed = false;
        return true;
    }
    return false;
}

void Client::setGameOver(int g, int w) {
    gameEnded = g;
    catWon = w;
    finalTime = currTime;
    //printf("%d gameOver %d\n", g, w);
}

void Client::setMovingState(PlayerState* playerstate) {
    for (int i = 0; i < PLAYER_NUM; i++) {
        movingState[i] = playerstate[i].moving;
        //printf("moving state %d is %d\n", i, movingState[i]);
    }
}

void Client::resetMovingState() {
    for (int i = 0; i < PLAYER_NUM; i++) {
        movingState[i] = false;
    }
}

void Client::calcFinalBoneMatrix(Animator* animator) {
    glUseProgram(shader);
    auto transforms = animator->GetFinalBoneMatrices();
    for (int i = 0; i < transforms.size(); ++i) {
        std::string name = "finalBonesMatrices[" + std::to_string(i) + "]";
        glUniformMatrix4fv(glGetUniformLocation(shader, name.c_str()), 1, GL_FALSE, &transforms[i][0][0]);
    }
    glUseProgram(0);
}

/**
 * Setup all the callbacks for GLFW window
 *
 * @param   window  Pointer to GLFW window
**/
void Client::setupCallbacks(GLFWwindow* window) {
    glfwSetWindowSizeCallback(window, resizeCallback);
    glfwSetCursorPosCallback(window, cursorCallback);
    glfwSetScrollCallback(window, scrollCallback);
    glfwSetMouseButtonCallback(window, mouseCallback);
    glfwSetKeyCallback(window, keyCallback);
}

/**
 * Set viewport and camera correctly for resizing
 *
 * @param   window  Pointer to GLFW window
 * @param   width   New window width
 * @param   height  New window height
**/
static void resizeCallback(GLFWwindow* window, int width, int height) {
#ifdef __APPLE__
    // In case your Mac has a retina display.
    glfwGetFramebufferSize(window, &width, &height);
#endif
    glViewport(0, 0, width, height);
    camera->aspectRatio = float(width) / float(height);
    thirdPersonCamera->aspectRatio = float(width) / float(height);
    window_height = height;
    window_width = width;
}

/**
 * Mouse control
 *
 * @param   window  Pointer to GLFW window
 * @param   xPos    Current x position
 * @param   yPos    Current y position
**/
static void cursorCallback(GLFWwindow* window, double xPos, double yPos) {
    ImGui_ImplGlfw_CursorPosCallback(window, xPos, yPos);


    if (isThirdPersonCam && !pause) {
        if (abs(xPos - prevXPos) > MOUSE_THRESH || abs(yPos - prevYPos) > MOUSE_THRESH) {
            mouseMoving = true;
            double yawAngle = -0.5 * (xPos - prevXPos);
            double pitchAngle = -0.5 * (yPos - prevYPos);
           
            float playerSpinDegree = 0.5 * (float)(thirdPersonCamera->upVec.y > 0 ? yawAngle : -yawAngle);
            currRotationDelta = glm::radians(playerSpinDegree);
            thirdPersonCamera->pitch((float)pitchAngle);

            prevXPos = xPos;
            prevYPos = yPos;
        }
    } else {
        if (middlePressed) {
            if (abs(xPos - prevXPos) > MOUSE_THRESH || abs(yPos - prevYPos) > MOUSE_THRESH) {
                mouseMoving = true;
                double yawAngle = -0.5 * (xPos - prevXPos);
                double pitchAngle = -0.5 * (yPos - prevYPos);
                camera->yaw((float)yawAngle);
                camera->pitch((float)pitchAngle);
            }
            prevXPos = xPos;
            prevYPos = yPos;
        }
    }
}

/**
 * Scroll zooming control
 *
 * @param   window  Pointer to GLFW window
 * @param   xMove   X movement
 * @param   yMove   Y movement
**/
static void scrollCallback(GLFWwindow* window, double xMove, double yMove) {
    ImGui_ImplGlfw_ScrollCallback(window, xMove, yMove);
    Camera* currCam = isThirdPersonCam ? thirdPersonCamera : camera;
    if (yMove > 0) {
        currCam->zoom(0.9f);
    }
    else if (yMove < 0) {
        currCam->zoom(1.1f);
    }
}

/**
 * Record mouse button status
 *
 * @param   window  Pointer to GLFW window
 * @param   button  Button ID
 * @param   action  Action ID
 * @param   mods    Mods
**/
static void mouseCallback(GLFWwindow* window, int button, int action, int mods) {
    ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);
    if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS) {
        middlePressed = true;
        glfwGetCursorPos(window, &prevXPos, &prevYPos);
    }
    else {
        middlePressed = false;
    }
}

/**
 * Keyboard control
 *
 * @param   window      Pointer to GLFW window
 * @param   key         Key ID
 * @param   scancode    Scancode
 * @param   action      Action ID
 * @param   mods        Mods
**/
static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);
    Camera* currCamera = isThirdPersonCam ? thirdPersonCamera : camera;

    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        switch (key) {
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window, GL_TRUE);
            break;

        case GLFW_KEY_SPACE:
            pause = !pause;
            break;

        case GLFW_KEY_R:
            currCamera->reset();
            break;

        case GLFW_KEY_W:
            keys[0] = 1;
            direction = FORWARD;
            break;

        case GLFW_KEY_S:
            keys[1] = 1;
            direction = BACK;
            break;

        case GLFW_KEY_A:
            direction = LEFT;
            keys[2] = 1;
            break;

        case GLFW_KEY_D:
            direction = RIGHT;
            keys[3] = 1;
            break;

        case GLFW_KEY_F:
            showMouse = !showMouse;
            if (showMouse) {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            }
            else {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            }
            break;

        case GLFW_KEY_LEFT_SHIFT:
            camera->move(glm::vec3(0, 0.5, 0));
            break;

        case GLFW_KEY_RIGHT_SHIFT:
            camera->move(glm::vec3(0, -0.5, 0));
            break;

        case GLFW_KEY_LEFT_CONTROL:
            camera->move(glm::vec3(-0.5, 0, 0));
            break;

        case GLFW_KEY_RIGHT_CONTROL:
            camera->move(glm::vec3(0.5, 0, 0));
            break;

        case GLFW_KEY_RIGHT:
            camera->move(glm::vec3(0.5, 0, 0));
            break;

        case GLFW_KEY_LEFT:
            camera->move(glm::vec3(-0.5, 0, 0));
            break;

        case GLFW_KEY_UP:
            camera->move(glm::vec3(0, 0, -0.5));
            break;

        case GLFW_KEY_DOWN:
            camera->move(glm::vec3(0, 0, 0.5));
            break;
        }
    }
    else if (action == GLFW_RELEASE) {
        switch (key) {
            case GLFW_KEY_W:
                keys[0] = 0;
                break;
            case GLFW_KEY_S:
                keys[1] = 0;
                break;

            case GLFW_KEY_A:
                keys[2] = 0;
                break;

            case GLFW_KEY_D:
                keys[3] = 0;
                break;
        }
    }
    keyHeld = keys[0] || keys[1] || keys[2] || keys[3];
}

static void drawOBB(const OBB& obb, const glm::mat4& viewProjMat, GLuint shader, bool fill) {
    glm::mat4 model(1);
    glUseProgram(shader);
    if (fill) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    glUniformMatrix4fv(glGetUniformLocation(shader, "viewProj"), 1, false, glm::value_ptr(viewProjMat));
    glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, false, glm::value_ptr(model));
    glBegin(GL_QUADS);
    glVertex3f(obb.p1.x + 0.1, 0, obb.p1.y + 0.1);
    glVertex3f(obb.p2.x - 0.1, 0, obb.p2.y + 0.1);
    glVertex3f(obb.p3.x - 0.1, 0, obb.p3.y - 0.1);
    glVertex3f(obb.p4.x + 0.1, 0, obb.p4.y - 0.1);
    glEnd();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glUseProgram(0);
}
