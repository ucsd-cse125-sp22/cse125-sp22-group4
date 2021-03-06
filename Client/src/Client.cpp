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
static bool hasPlayedFootstepCat = false;


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

static Model* fakecat;

static Model* mazescene;

static Animation* catidleAnimation;
static Animator* catanimator;
static Animation* catwalkingAnimation;
static Animator* catwalkinganimator;

static Animation* mouseidleAnimation1;
static Animation* mousewalkingAnimation1;
static Animator* mouseanimator1;
static Animator* mousewalkinganimator1;

static Animation* mouseidleAnimation2;
static Animation* mousewalkingAnimation2;
static Animator* mouseanimator2;
static Animator* mousewalkinganimator2;

static Animation* mouseidleAnimation3;
static Animation* mousewalkingAnimation3;
static Animator* mouseanimator3;
static Animator* mousewalkinganimator3;

//Particles
static ParticleSystem* smokeparticles;
static ParticleSystem* flameparticles;
static ParticleSystem* glintparticles;
static ParticleSystem* bloodparticles1;
static ParticleSystem* bloodparticles2;
static ParticleSystem* bloodparticles3;

static ParticleSystem* glintparticlesitem2;
static ParticleSystem* glintparticlesitem3;

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

bool retStartbtn;
int image_width_startbtn = 0;
int image_height_startbtn = 0;
GLuint image_texture_startbtn = 0;

bool retCatHoverIcon;
GLuint image_texture_cat_hover_icon = 0;

bool retMouseIcon;
int image_width_mouse_icon = 0;
int image_height_mouse_icon = 0;
GLuint image_texture_mouse_icon = 0;

bool retMouseHoverIcon;
GLuint image_texture_mouse_hover_icon = 0;



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
bool retCardback;
int image_width_cardback = 0;
int image_height_cardback = 0;
GLuint image_texture_cardback = 0;
bool retCard1;
int image_width_card1 = 0;
int image_height_card1 = 0;
GLuint image_texture_card1 = 0;
bool retCard2;
int image_width_card2 = 0;
int image_height_card2 = 0;
GLuint image_texture_card2 = 0;
bool retCard3;
int image_width_card3 = 0;
int image_height_card3 = 0;
GLuint image_texture_card3 = 0;
bool retBackground;
int image_width_background = 0;
int image_height_background = 0;
GLuint image_texture_background = 0;
bool retTitle;
int image_width_title = 0;
int image_height_title = 0;
GLuint image_texture_title = 0;

// DEBUG COLLISION
//Scene
std::vector<Model*> sceneObjects;
static SceneLoader* scene;

// state variables
static bool gameEnded = 0;
static bool gameStarted = 0;
static bool displayStart = 1;
static bool gameStartPressed = 0;
static bool displayStartPressed = 0;
static bool inMiniGame = false;

static bool playerSelect = true;
static bool playerSelected = false;
static int playerType = NONE;

float bloodTime[PLAYER_NUM] = {1.0f, 1.0f, 1.0f, 1.0f};
bool startBloodCountdown[PLAYER_NUM] = { 0, 0, 0, 0 };
glm::vec3 deathpos[PLAYER_NUM] = {glm::vec3(1), glm::vec3(1), glm::vec3(1), glm::vec3(1) };
// Debounce for sending network packets
static bool playerTypeSent = false;

// Array containing:
// 0 (cat): client_id1
// 1 (m1): client_id2
// etc.
static int playerSelection[PLAYER_NUM];

static bool mouse1Selected = false;
static bool mouse2Selected = false;
static bool mouse3Selected = false;

static bool catHover = false;
static bool mouse1Hover = false;
static bool mouse2Hover = false;
static bool mouse3Hover = false;

static bool catSelected = false;
vector<int> cardChoices = { 1, 1, 2, 2, 3, 3 };
vector<GLuint> cards;
static int possiblePair = -1;
static GLuint possibleImg = -1;
static int pairs1 = 0;
static int pairs2 = 0;
static int numSelected = 0;
static bool cardsSelected[6] = { false, false, false, false, false, false };
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
// 0 for DEBUG COLLISION
static int select = 0;
// 1 for ACTUAL GAME
static bool pause = false;
static bool cursorNormal = false;
static bool showMouse = false;
static bool disableMouse = false;
static bool middlePressed = false;
static bool isThirdPersonCam = false;
static const char* scenes[2] = { "new maze", "Maze"};
static bool movingState[PLAYER_NUM];
static bool aliveState[PLAYER_NUM];

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
static ImFont* BIGcuteFont;
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

    GLFWwindow* window = glfwCreateWindow(width, height, windowTitle.c_str(), glfwGetPrimaryMonitor(), NULL);
    //GLFWwindow* window = glfwCreateWindow(width, height, windowTitle.c_str(), NULL, NULL);
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
    glClearColor(0.99607f, 0.8f, 0.48235f, 1.0f);
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

void Client::loadGUIFonts() {
    ImGuiIO& io = ImGui::GetIO();
    plainFont = io.Fonts->AddFontDefault();
    cuteFont = io.Fonts->AddFontFromFileTTF("../../fonts/Gidole/Gidolinya-Regular.otf", 32.0f);
    BIGcuteFont = io.Fonts->AddFontFromFileTTF("../../fonts/Gidole/Gidolinya-Regular.otf", 34.0f);
    HUGEcuteFont = io.Fonts->AddFontFromFileTTF("../../fonts/Gidole/Gidolinya-Regular.otf", 52.0f);
    MASSIVEcuteFont = io.Fonts->AddFontFromFileTTF("../../fonts/Gidole/Gidolinya-Regular.otf", 70.0f);
}

/**
 * Initialize shader program
 *
 * @return  True if success, False otherwise
**/
bool Client::initializeClient() {
    for (int i = 0; i < PLAYER_NUM; ++i)
        playerSelection[i] = NONE;

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
    lightPosn = { {-1, 1, -1, 0}, {1, 1, 1, 0} ,  {-100, 100, -100, 1}, {1, 1, 0, 0}, {0, 1, 1, 0} };
    lightColorn = { {0.85, 0.8, 0.8, 1}, {0.1, 0.1, 0.1, 1}, {0.5, 0.5, 0.5, 1}, {0.1, 0.1, 0.1, 1}, {0.1, 0.1, 0.1, 1} };
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
        1,      //blendMethod
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

    ParticleProperty blood = {
        500,    //amount
        1.0f,   //Life
        glm::vec3(0, 0, 0), //Velocity
        glm::vec3(10, 10, 10), //useRandomVelocity
        glm::vec3(0.3, 0.3, 0.3), //randomPositionRange
        1,      //randomColor
        1.0f,   //colorFade
        0,      //blendMethod
    };



    // DEBUG COLLISION
    scene = new SceneLoader("../../objects/new_maze_collision/scene.txt");
    sceneObjects = scene->load("../../objects/new_maze_collision/");

    mazescene = new Model("../../objects/new_maze_collision/new_maze.obj");

    //initialize particle system
    smokeparticles = new ParticleSystem(particleShader, "../../particles/smoke.png", smoke);
    flameparticles = new ParticleSystem(particleShader, "../../particles/flame.png", flame);

    glintparticles = new ParticleSystem(particleShader, "../../particles/smallglint.png", glint);
    glintparticlesitem2 = new ParticleSystem(particleShader, "../../particles/smallglint_red.png", glint);
    glintparticlesitem3 = new ParticleSystem(particleShader, "../../particles/smallglint_blue.png", glint);

    cattrailparticles = new ParticleSystem(particleShader, "../../particles/dust.png", trail);
    micetrailparticles1 = new ParticleSystem(particleShader, "../../particles/dust.png", micetrail);
    micetrailparticles2 = new ParticleSystem(particleShader, "../../particles/dust.png", micetrail);
    micetrailparticles3 = new ParticleSystem(particleShader, "../../particles/dust.png", micetrail);

    bloodparticles1 = new ParticleSystem(particleShader, "../../particles/bloodfume.png", blood);
    bloodparticles2 = new ParticleSystem(particleShader, "../../particles/bloodfume.png", blood);
    bloodparticles3 = new ParticleSystem(particleShader, "../../particles/bloodfume.png", blood);

    // initialize objects
    ground = new Cube(glm::vec3(-25, -1, -25), glm::vec3(25, 1, 25));
    ground->moveGlobal(glm::vec3(0, -3, 0));

    fakecat = new Model("../../objects/cat/cat.obj");

    cat = new Model("../../objects/cat/new_cat_idle.fbx");
    catidleAnimation = new Animation("../../objects/cat/new_cat_idle.fbx", cat);
    catanimator = new Animator(catidleAnimation);

    catwalkingAnimation = new Animation("../../objects/cat/cat_walk/cat_walking.fbx", cat); 
    catwalkinganimator = new Animator(catwalkingAnimation);

    mouse1 = new Model("../../objects/mouse/mouse_idle.fbx");
    mouse1->moveGlobal(glm::vec3(80, 0, -5));
    mouseidleAnimation1 = new Animation("../../objects/mouse/mouse_idle.fbx", mouse1);
    mouseanimator1 = new Animator(mouseidleAnimation1);
    mousewalkingAnimation1 = new Animation("../../objects/mouse/mouse_walk/mouse_walking.fbx", mouse1);
    mousewalkinganimator1 = new Animator(mousewalkingAnimation1);

    mouse2 = new Model("../../objects/mouse/mouse_idle.fbx");
    mouse2->moveGlobal(glm::vec3(70, 0, -5));
    mouseidleAnimation2 = new Animation("../../objects/mouse/mouse_idle.fbx", mouse2);
    mouseanimator2 = new Animator(mouseidleAnimation2);
    mousewalkingAnimation2 = new Animation("../../objects/mouse/mouse_walk/mouse_walking.fbx", mouse2);
    mousewalkinganimator2 = new Animator(mousewalkingAnimation2);

    mouse3 = new Model("../../objects/mouse/mouse_idle.fbx");
    mouse3->moveGlobal(glm::vec3(60, 0, -5));
    mouseidleAnimation3 = new Animation("../../objects/mouse/mouse_idle.fbx", mouse3);
    mouseanimator3 = new Animator(mouseidleAnimation3);
    mousewalkingAnimation3 = new Animation("../../objects/mouse/mouse_walk/mouse_walking.fbx", mouse3);
    mousewalkinganimator3 = new Animator(mousewalkingAnimation3);

    maze = new Model("../../objects/maze_textured/maze3D.obj");
    maze->moveGlobal(glm::vec3(0, -3, 0));
    bear = new Model("../../objects/bear/bear.obj");
    bear->moveGlobal(glm::vec3(55, -3, -135));
    bear->scale(glm::vec3(0.5));
    fallenstar = new Model("../../objects/fallenstar/fallenstar.obj");
    fallenstar->moveGlobal(glm::vec3(95, 2, -35));
   
    item = new Model("../../objects/diploma/diploma.dae");
   
    item2 = new Model("../../objects/computer/computer.fbx");
    item2->moveGlobal(glm::vec3(70, 0, -5));
    item3 = new Model("../../objects/books/books.fbx");
    item3->moveGlobal(glm::vec3(65, -1, -10));
    item3->scale(glm::vec3(6));
    
    geisel = new Model("../../objects/Geisel/Geisel.fbx");
    
    geisel->moveGlobal(glm::vec3(75, 5, -75));
    geisel->scale(glm::vec3(0.005));

    sungod = new Model("../../objects/sungod/sungod.obj");
    sungod->moveGlobal(glm::vec3(105, 2, -135));
    sungod->spin(90);

    ret = LoadTextureFromFile("../../objects/ImGui/cute_cat.png", &my_image_texture, &my_image_width, &my_image_height);
    retGameOver = LoadTextureFromFile("../../objects/ImGui/explosion.png", &image_texture_game_over, &image_width_game_over, &image_height_game_over);
    retMouseWin = LoadTextureFromFile("../../objects/ImGui/celebration.png", &image_texture_mouse_win, &image_width_mouse_win, &image_height_mouse_win);
    retMouseFlag = LoadTextureFromFile("../../objects/ImGui/cheese.png", &image_texture_mouse_flag, &image_width_mouse_flag, &image_height_mouse_flag);
    retMouseFlagPale = LoadTextureFromFile("../../objects/ImGui/cheese_paler2.png", &image_texture_mouse_flag_pale, &image_width_mouse_flag_pale, &image_height_mouse_flag_pale);
    retMap = LoadTextureFromFile("../../objects/ImGui/minimap.png", &image_texture_map, &image_width_map, &image_height_map);
    retGameStart = LoadTextureFromFile("../../objects/ImGui/game_start_maze2.jpg", &image_texture_game_start, &image_width_game_start, &image_height_game_start);
    //retStartCat = LoadTextureFromFile("../../objects/ImGui/gamelogo.png", &image_texture_start_cat, &image_width_start_cat, &image_height_start_cat);
    retStartCat = LoadTextureFromFile("../../objects/ImGui/mao_cat.png", &image_texture_start_cat, &image_width_start_cat, &image_height_start_cat);

    retStartMouse = LoadTextureFromFile("../../objects/ImGui/mao_mouse.png", &image_texture_start_mouse, &image_width_start_mouse, &image_height_start_mouse);
    retStartCatuate = LoadTextureFromFile("../../objects/ImGui/catuate2.png", &image_texture_start_catuate, &image_width_start_catuate, &image_height_start_catuate);
    
    retCatHoverIcon = LoadTextureFromFile("../../objects/ImGui/cat_icon_onhover.png", &image_texture_cat_hover_icon, &image_width_cat_icon, &image_height_cat_icon);
    retCatIcon = LoadTextureFromFile("../../objects/ImGui/cat_icon.png", &image_texture_cat_icon, &image_width_cat_icon, &image_height_cat_icon);
    
    retMouseHoverIcon = LoadTextureFromFile("../../objects/ImGui/mouse_icon_onhover.png", &image_texture_mouse_hover_icon, &image_width_mouse_icon, &image_height_mouse_icon);
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
    retCardback = LoadTextureFromFile("../../objects/ImGui/cardback.png", &image_texture_cardback, &image_width_cardback, &image_height_cardback);
    retCard1 = LoadTextureFromFile("../../objects/ImGui/card1.png", &image_texture_card1, &image_width_card1, &image_height_card1);
    retCard2 = LoadTextureFromFile("../../objects/ImGui/card2.png", &image_texture_card2, &image_width_card2, &image_height_card2);
    retCard3 = LoadTextureFromFile("../../objects/ImGui/card3.png", &image_texture_card3, &image_width_card3, &image_height_card3);
    retBackground = LoadTextureFromFile("../../objects/ImGui/background.png", &image_texture_background, &image_width_background, &image_height_background);
    //retStartbtn = LoadTextureFromFile("../../objects/ImGui/startbtn.png", &image_texture_startbtn, &image_width_startbtn, &image_height_startbtn);
    retTitle = LoadTextureFromFile("../../objects/ImGui/titlescreen.png", &image_texture_title, &image_width_title, &image_height_title);

    random_shuffle(cardChoices.begin(), cardChoices.end());
    for (int i = 0; i < cardChoices.size(); i++) {
        if (cardChoices[i] == 1)
            cards.push_back(image_texture_card1);
        else if (cardChoices[i] == 2)
            cards.push_back(image_texture_card2);
        else if (cardChoices[i] == 3)
            cards.push_back(image_texture_card3);
    }

    //hard coded for now
   
    players[0] = cat;
    players[1] = mouse1;
    players[2] = mouse2;
    players[3] = mouse3;
   


    skybox = new Skybox();

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
    if (!gameStarted)
        return;

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
    // DEBUG COLLISION
    
    case 0: {
        isThirdPersonCam = true;
        //scene->draw(currCam->viewProjMat, identityMat, shader, sceneObjects);
        //ground->draw(currCam->viewProjMat, identityMat, shader);
        mazescene->draw(currCam->viewProjMat, identityMat, shader);
        //for (auto& wall : sceneObjects) {
        //   drawOBB(wall->getOBB(), currCam->viewProjMat, shader, false);
        //}

        int cat_id = playerSelection[CAT];
        int m1_id = playerSelection[M1];
        int m2_id = playerSelection[M2];
        int m3_id = playerSelection[M3];

        if (!playerSelected) {
            m1_id = 1;
            m2_id = 2;
            m3_id = 3;
        }

        int ids[3] = { m1_id, m2_id, m3_id };

        if (movingState[CAT] == true) {
            calcFinalBoneMatrix(catwalkinganimator);
        }
        else {
            printf("not walk\n");
            calcFinalBoneMatrix(catanimator);
        }
        cat->draw(currCam->viewProjMat, identityMat, shader);

        if (movingState[M1] == true) {
            calcFinalBoneMatrix(mousewalkinganimator1);
        }
        else {
            calcFinalBoneMatrix(mouseanimator1);
        }
        mouse1->draw(currCam->viewProjMat, identityMat, shader);

        if (movingState[M2] == true) {
            calcFinalBoneMatrix(mousewalkinganimator2);
        }
        else {
            calcFinalBoneMatrix(mouseanimator2);
        }
        mouse2->draw(currCam->viewProjMat, identityMat, shader);

        if (movingState[M3] == true) {
            calcFinalBoneMatrix(mousewalkinganimator3);
        }
        else {
            calcFinalBoneMatrix(mouseanimator3);
        }
        mouse3->draw(currCam->viewProjMat, identityMat, shader);

        //drawOBB(CollisionDetector::computeOBB(fakecat->getOBB(), cat->getModel()), currCam->viewProjMat, shader, false);

        cattrailparticles->draw(currCam->viewProjMat, Camera_Right, Camera_Up);
        micetrailparticles1->draw(currCam->viewProjMat, Camera_Right, Camera_Up);
        micetrailparticles2->draw(currCam->viewProjMat, Camera_Right, Camera_Up);
        micetrailparticles3->draw(currCam->viewProjMat, Camera_Right, Camera_Up);

        smokeparticles->draw(currCam->viewProjMat, Camera_Right, Camera_Up);
        flameparticles->draw(currCam->viewProjMat, Camera_Right, Camera_Up);

        item->draw(currCam->viewProjMat, identityMat, shader);
        item2->draw(currCam->viewProjMat, identityMat, shader);
        item3->draw(currCam->viewProjMat, identityMat, shader);

        glintparticles->draw(currCam->viewProjMat, Camera_Right, Camera_Up);
        glintparticlesitem2->draw(currCam->viewProjMat, Camera_Right, Camera_Up);
        glintparticlesitem3->draw(currCam->viewProjMat, Camera_Right, Camera_Up);

        bloodparticles1->draw(currCam->viewProjMat, Camera_Right, Camera_Up);
        bloodparticles2->draw(currCam->viewProjMat, Camera_Right, Camera_Up);
        bloodparticles3->draw(currCam->viewProjMat, Camera_Right, Camera_Up);

        break;
    }
    

    case 1: {
        isThirdPersonCam = true;
        maze->draw(currCam->viewProjMat, identityMat, shader);
        bear->draw(currCam->viewProjMat, identityMat, shader);
       
        geisel->draw(currCam->viewProjMat, identityMat, shader);
        sungod->draw(currCam->viewProjMat, identityMat, shader);
        fallenstar->draw(currCam->viewProjMat, identityMat, shader);

        
        break;
    }
    }
}

/**
 * Update objects when idle
**/
void Client::idleCallback(float dt) {
    if (!gameStarted)
        return;

    Camera* currCamera = isThirdPersonCam ? thirdPersonCamera : camera;
    currCamera->update();
    mouseMoving = false;

    if (gameEnded) {
        pause = 1;
    }

    if (!pause) {
        //particles update
        smokeparticles->update(dt, 2, glm::vec3(x,y + 1,z));
        flameparticles->update(dt, 1, glm::vec3(x, y - 2, z));

        glm::mat4 itemModel = item->getModel() * glm::translate(glm::mat4(1), glm::vec3(0, 0, 0));
        glm::vec3 itempos = itemModel[3];
        glintparticles->update(dt, 2, itempos);

        itemModel = item2->getModel() * glm::translate(glm::mat4(1), glm::vec3(0, 0, 0));
        itempos = itemModel[3];
        glintparticlesitem2->update(dt, 2, itempos);

        itemModel = item3->getModel() * glm::translate(glm::mat4(1), glm::vec3(0, 0, 0));
        itempos = itemModel[3];
        glintparticlesitem3->update(dt, 2, itempos);

        int cat_id = playerSelection[CAT];
        int m1_id = playerSelection[M1];
        int m2_id = playerSelection[M2];
        int m3_id = playerSelection[M3];

        if (!playerSelected) {
            m1_id = 1;
            m2_id = 2;
            m3_id = 3;
        }

        int ids[3] = { m1_id, m2_id, m3_id };

        glm::mat4 trailModel = cat->getModel() * glm::translate(glm::mat4(1), glm::vec3(0, 0.3, 0.5));
        glm::vec3 catpos = trailModel[3];
        trailModel = mouse1->getModel() * glm::translate(glm::mat4(1), glm::vec3(0, 0.4, 0));
        glm::vec3 micepos1 = trailModel[3];
        trailModel = mouse2->getModel() * glm::translate(glm::mat4(1), glm::vec3(0, 0.4, 0));
        glm::vec3 micepos2 = trailModel[3];
        trailModel = mouse3->getModel() * glm::translate(glm::mat4(1), glm::vec3(0, 0.4, 0));
        glm::vec3 micepos3 = trailModel[3];

        if (movingState[CAT] == true) {
            cattrailparticles->update(dt, 2, catpos);
            catwalkinganimator->update(dt);
        }
        else {
            printf("not walk\n");
            cattrailparticles->update(dt, 0, catpos);
            catanimator->update(dt);
        }    
        
        if (movingState[M1] == true) {
            micetrailparticles1->update(dt, 2, micepos1);
            mousewalkinganimator1->update(3*dt);
        }
        else {
            micetrailparticles1->update(dt, 0, micepos1);
            mouseanimator1->update(dt);
        }
    
        if (movingState[M2] == true) {
            micetrailparticles2->update(dt, 2, micepos2);
            mousewalkinganimator2->update(3*dt);
        }
        else {
            micetrailparticles2->update(dt, 0, micepos2);
            mouseanimator2->update(dt);
        }

        if (movingState[M3] == true) {
            micetrailparticles3->update(dt, 2, micepos3);
            mousewalkinganimator3->update(3*dt);
        }
        else {
            micetrailparticles3->update(dt, 0, micepos3);
            mouseanimator3->update(dt);
        }

       
        //blood logic
        if (aliveState[m1_id] == false) {
            startBloodCountdown[m1_id] = 1;
        }
        if (aliveState[m2_id] == false) {
            startBloodCountdown[m2_id] = 1;
        }
        if (aliveState[m3_id] == false) {
            startBloodCountdown[m3_id] = 1;
        }


        //for (int i = 1; i < PLAYER_NUM; i ++) {
        for (int i: ids) {
            if (bloodTime[i] <= 0) {
                bloodTime[i] = 1.0f;
                startBloodCountdown[i] = false;
            }
            if (startBloodCountdown[i]) {
                bloodTime[i] -= dt;
            }
        }

        //printf("i is %d = %f\n", 2, bloodTime[2]);

        if (startBloodCountdown[m1_id] == true) {
            printf("KILL 1!!\n");
            bloodparticles1->update(dt, 2, deathpos[m1_id]);
        }
        else {
            bloodparticles1->update(dt, 0, deathpos[m1_id]);
        }
        if (startBloodCountdown[m2_id] == true) {
            printf("KILL 2!!\n");
            bloodparticles2->update(dt, 2, deathpos[m2_id]);
        }
        else {
            bloodparticles2->update(dt, 0, deathpos[m2_id]);
        }
        if (startBloodCountdown[m3_id] == true) {
            printf("KILL 3!!\n");
            bloodparticles3->update(dt, 2, deathpos[m3_id]);
        }
        else {
            bloodparticles3->update(dt, 0, deathpos[m3_id]);
        }

        if (aliveState[m1_id] == true) {
            deathpos[m1_id] = micepos1;
        }
        if (aliveState[m2_id] == true) {
            deathpos[m2_id] = micepos2;
        }
        if (aliveState[m3_id] == true) {
            deathpos[m3_id] = micepos3;
        }

        //bloodparticles1->update(dt, 2, micepos1);
    }

    //spdlog::info("pos = {}", cat->getModel()[3]);
    //printf("x = %f, y = %f, z = %f\n", cat->getModel()[3][0], cat->getModel()[3][1], cat->getModel()[3][2]);

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
    delete maze;
    delete skybox;
    delete bear;
    delete sungod;
    delete fallenstar;
    delete item;
    delete geisel;
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
    if (!gameStarted)
        return;

    ImGuiWindowFlags flags = 0;
   
    flags |= ImGuiWindowFlags_NoBackground;
    flags |= ImGuiWindowFlags_NoTitleBar;
    flags |= ImGuiWindowFlags_NoScrollbar;
    flags |= ImGuiWindowFlags_NoResize;
    double adjustment = 0.32;
    ImGui::SetNextWindowSize(ImVec2(my_image_width * adjustment + 30, my_image_height * adjustment + 50));
    //ImGui::SetNextWindowPos(ImVec2(window_width - (my_image_width / 2) + 170, 0), 0, ImVec2(0, 0));
    ImGui::SetNextWindowPos(ImVec2(window_width-(my_image_width*adjustment)-30, 10), 0, ImVec2(0, 0));
    //printf(" % d image height %d image width\n", my_image_height, my_image_width);
    
    ImGui::Begin("Time GUI", NULL, flags);
    if (gameEnded)
        currTime = finalTime;

    // get game countdown time
    int time = currTime;
    int minute = (time % 3600) / 60;  // Minute component
    int seconds = time % 60;          // Second component 

    if (!hasPlayedTimer && currTime == 60) {
        audioEngine->PlayEvent("event:/panic");
        hasPlayedTimer = true;
    }

    if (time < 0) {
        minute = 0;
        seconds = 0;
    }
     

    ImGui::Image((void*)(intptr_t)my_image_texture, ImVec2(my_image_width * adjustment, my_image_height * adjustment));

    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0, 0.5f, 1.0f, 1.0f));
    ImGui::PushFont(HUGEcuteFont);
    ImGui::SetCursorPosY((my_image_width * adjustment)/2);
    ImGui::SetCursorPosX((my_image_height * adjustment) / 2 + 28);
    ImGui::Text("%d:%02d", minute, seconds);
    ImGui::PopFont();
    ImGui::PopStyleColor();

    ImGui::End();
}

void Client::LoadingGUI() {
    ImGuiWindowFlags flags = 0;

    flags |= ImGuiWindowFlags_NoBackground;
    flags |= ImGuiWindowFlags_NoTitleBar;
    flags |= ImGuiWindowFlags_NoScrollbar;
    flags |= ImGuiWindowFlags_NoResize;

    int w;
    int h;
    GLuint loading;
    int retLoad = LoadTextureFromFile("../../objects/ImGui/loading.png", &loading, &w, &h);

     
    ImGui::SetNextWindowSize(ImVec2(w, h));
    ImGui::SetNextWindowPos(ImVec2(0, 0), 0, ImVec2(0, 0));

    ImGui::Begin("Load GUI", NULL, flags);

    ImGui::Image((void*)(intptr_t)loading, ImVec2(w, h));

    ImGui::End();
}

void Client::audioUpdate() {
    if (!gameStarted)
        return;

    int cat_id = playerSelection[CAT];
    int m1_id = playerSelection[M1];
    int m2_id = playerSelection[M2];
    int m3_id = playerSelection[M3];

    // Mouse/Cat collision
    if (hasPlayedMouse1CatCollision && my_id == playerSelection[CAT] &&
        aliveState[m1_id]) {
        hasPlayedMouse1CatCollision = false;
    }

    if (hasPlayedMouse2CatCollision && my_id == playerSelection[CAT] &&
        aliveState[m2_id]) {
        hasPlayedMouse2CatCollision = false;
    }

    if (hasPlayedMouse3CatCollision && my_id == playerSelection[CAT] &&
        aliveState[m3_id]) {
        hasPlayedMouse3CatCollision = false;
    }


    if (!hasPlayedMouse1CatCollision && my_id == playerSelection[CAT] &&
        !aliveState[m1_id]) {
        audioEngine->PlayEvent("event:/cat_screech_1");
        hasPlayedMouse1CatCollision = true;
    }

    if (!hasPlayedMouse2CatCollision && my_id == playerSelection[CAT] &&
        !aliveState[m2_id]) {
        audioEngine->PlayEvent("event:/cat_screech_1");
        hasPlayedMouse2CatCollision = true;
    }

    if (!hasPlayedMouse3CatCollision && my_id == playerSelection[CAT] &&
        !aliveState[m3_id]) {
        audioEngine->PlayEvent("event:/cat_screech_1");
        hasPlayedMouse3CatCollision = true;
    }

    if (!hasPlayedMouseCatCollision && my_id != playerSelection[CAT] && 
        !aliveState[my_id]) {
        printf("SQUEEK!\n");
        audioEngine->PlayEvent("event:/mice_shriek_1");
        hasPlayedMouseCatCollision = true;
        hasPlayedTask1 = false; // task1 respawns on collision
    }

    if (hasPlayedMouseCatCollision && my_id != playerSelection[CAT] && 
        aliveState[my_id]) {
        hasPlayedMouseCatCollision = false;
    }

    // Tasks 
    if (hasPlayedTask1 && !task1 && my_id != playerSelection[CAT]) {
        hasPlayedTask1 = false;
    }

    if (hasPlayedStationary1 && !stationary1 && my_id != playerSelection[CAT]) {
        hasPlayedStationary1 = false;
    }

    if (hasPlayedStationary2 && !stationary2 && my_id != playerSelection[CAT]) {
        hasPlayedStationary2 = false;
    }

    if (!hasPlayedTask1 && task1 && my_id != playerSelection[CAT]) {
        audioEngine->PlayEvent("event:/powerup_1");
        hasPlayedTask1 = true;
    }

    if (!hasPlayedStationary1 && stationary1 && my_id != playerSelection[CAT]) {
        audioEngine->PlayEvent("event:/powerup_1");
        hasPlayedStationary1 = true;
    }

    if (!hasPlayedStationary2 && stationary2 && my_id != playerSelection[CAT]) {
        audioEngine->PlayEvent("event:/powerup_1");
        hasPlayedStationary2 = true;
    }

    // Footsteps
    if (hasPlayedFootstepCat && my_id == playerSelection[CAT] && currTime % 5 != 0) {
        hasPlayedFootstepCat = false;
    }

    if (!hasPlayedFootstepCat && my_id == playerSelection[CAT] && currTime % 5 == 0) {
        audioEngine->PlayEvent("event:/footsteps");
        hasPlayedFootstepCat = true;
    }
}

void Client::ItemHoldGUI() {
    if (!gameStarted)
        return;

    double adjustment = 0.28;
    
    ImGuiWindowFlags flags = 0;
    flags |= ImGuiWindowFlags_NoBackground;
    flags |= ImGuiWindowFlags_NoTitleBar;
    flags |= ImGuiWindowFlags_NoScrollbar;
    flags |= ImGuiWindowFlags_NoResize;
    int tasks = 0;

    if (task1) 
        tasks++;
    if (stationary1)
        tasks++;
    if (stationary2)
        tasks++;

    ImGui::SetNextWindowSize(ImVec2(image_width_mouse_flag * adjustment+10, image_height_mouse_flag*adjustment+10));
    ImGui::SetNextWindowPos(ImVec2(530, 15), 0, ImVec2(0, 0));
   
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
    float locX = model[3][0] * .85 + 260;
    float locZ = model[3][2] * .85 + 260;

    if (model[3][1] < 0) { // this is a hack, manually checking if banished height
        return;
    }

    float icon_size = 11.0f;
    

    if (id == CAT) { // display cat
        ImGui::GetWindowDrawList()->AddImage((void*)(intptr_t)image_texture_cat_icon, ImVec2(locX-icon_size, locZ-icon_size), ImVec2(locX+icon_size, locZ+icon_size), ImVec2(0, 0), ImVec2(1, 1));
    }
    else if (id == M1) { // display mice
        ImGui::GetWindowDrawList()->AddImage((void*)(intptr_t)image_texture_mouse_icon, ImVec2(locX - icon_size, locZ - icon_size), ImVec2(locX + icon_size, locZ + icon_size), ImVec2(0, 0), ImVec2(1, 1));
    }
    else {
      /*  if (currTime % 2 == 0)
            ImGui::GetForegroundDrawList()->AddCircle(ImVec2(locZ, locX), 2, IM_COL32(r, g, b, 255), 100, 2.f);*/
    
        if (itemhold == my_id) { // final Dest location revealed only to player with item
            if (currTime % 2 == 0) {
                float locX1 = finalDest[3][0] * .85 + 260;
                float locZ1 = finalDest[3][2] * .85 + 260;
                //ImGui::GetWindowDrawList()->AddImage((void*)(intptr_t)image_texture_diploma, ImVec2(locZ1 - icon_size, locX1 - icon_size), ImVec2(locZ1 + icon_size, locX1 + icon_size), ImVec2(0, 0), ImVec2(1, 1));
                icon_size = 12.0f;
                ImGui::GetWindowDrawList()->AddImage((void*)(intptr_t)image_texture_party_icon, ImVec2(locX1 - icon_size, locZ1 - icon_size), ImVec2(locX1 + icon_size, locZ1 + icon_size), ImVec2(0, 0), ImVec2(1, 1));
            
            }
              
                //ImGui::GetForegroundDrawList()->AddCircle(ImVec2(75*1.15 + 24, 75* 1.15 + 26), 2, IM_COL32(204, 0, 204, 255), 100, 2.f);
        }
        else if (id == 4 && itemhold == PLAYER_NUM + 1) { // if no player holding flag, show location of flag
            icon_size = 9.0f;
            if (currTime % 2 == 0)
                ImGui::GetWindowDrawList()->AddImage((void*)(intptr_t)image_texture_mouse_flag, ImVec2(locX - icon_size, locZ - icon_size), ImVec2(locX + icon_size, locZ + icon_size), ImVec2(0, 0), ImVec2(1, 1));
        }
        else if (id >= 5) {
            icon_size = 9.0f;
            if (currTime % 2 == 0)
                ImGui::GetWindowDrawList()->AddImage((void*)(intptr_t)image_texture_mouse_flag, ImVec2(locX - icon_size, locZ - icon_size), ImVec2(locX + icon_size, locZ + icon_size), ImVec2(0, 0), ImVec2(1, 1));
        }
    }
    
    
}

void Client::miniMapGUI() {

    if (gameStarted == 0)
        return;

    double adjustment = 0.5;
    float height_resize = window_height / static_cast<float>(1017);
    float width_resize = window_width / static_cast<float>(1920);
    ImGuiWindowFlags flags = 0;
    flags |= ImGuiWindowFlags_NoBackground;
    flags |= ImGuiWindowFlags_NoTitleBar;
    flags |= ImGuiWindowFlags_NoScrollbar;
    flags |= ImGuiWindowFlags_NoResize;

    ImGui::SetNextWindowSize(ImVec2(image_width_map * adjustment+10, image_height_map * adjustment+10));
    ImGui::SetNextWindowPos(ImVec2(0, 0), 0, ImVec2(0, 0));
    ImGui::Begin("MiniMap GUI", NULL, flags);
    ImGui::Image((void*)(intptr_t)image_texture_map, ImVec2(image_width_map * adjustment, image_height_map * adjustment));

    //printf("%d %d\n", my_id, playerSelection[CAT]);


    //if (catSelected == my_id)
    for (int i = 0; i < PLAYER_NUM; i++) {
        // If cat, see myself
        if (my_id == playerSelection[CAT]) {
            if (i == my_id) {
                displayLocation(players[i]->getModel(), CAT);
            } else {
                displayLocation(players[i]->getModel(), M1);
            }
            continue;
        }
        if (i != playerSelection[CAT])
            displayLocation(players[i]->getModel(), M1);
    }
    
    if (item && (my_id != playerSelection[CAT] || catSeesItem)) {
        displayLocation(item->getModel(), 4);
    }

    if (item2 && (my_id != playerSelection[CAT] || catSeesItem)) {
        displayLocation(item2->getModel(), 5);
    }

    if (item3 && (my_id != playerSelection[CAT] || catSeesItem)) {
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
    //if (gameEnded == 1 || my_id == CAT) { // don't display on game over or if cat
    if (!gameStarted || gameEnded || my_id == playerSelection[CAT]) {
        return;
    }
    ImGuiWindowFlags flags = 0;

    flags |= ImGuiWindowFlags_NoTitleBar;
    flags |= ImGuiWindowFlags_NoScrollbar;
    flags |= ImGuiWindowFlags_NoResize;

    ImGui::SetNextWindowSize(ImVec2(window_width, window_height), 0);
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(24.0f /  255, 68.0f / 255, 62.0f / 255, 1.0f));

    if (player && !aliveState[my_id]) {
        pairs1 = 0;
        pairs2 = 0;
        resetCardArray();
        inMiniGame = false;
    }
   
    if (timeLeftStationaryItem > 0 && holdIdStationary[my_id] && pairs1 != -1) { // for computer stationary item
        ImGui::Begin("StationaryItem GUI", NULL, flags);
        inMiniGame = true;
        displayCards(1);
        //ImGui::Image((void*)(intptr_t)image_texture_zeroes_ones, ImVec2(window_width, window_height));
        ImGui::End();
    }
    else if (timeLeftStationaryItem2 > 0 && holdIdStationary2[my_id] && pairs2 != -1) { // for books stationary item
        ImGui::Begin("StationaryItem2 GUI", NULL, flags);
        inMiniGame = true;
        displayCards(2);
        //ImGui::Image((void*)(intptr_t)image_texture_fireplace, ImVec2(window_width, window_height));
        ImGui::End();
    }
    ImGui::PopStyleColor();
}

void Client::displayCards(int item) {
    float height_resize = window_height / static_cast<float>(1017);
    float width_resize = window_width / static_cast<float>(1920);
   
    //printf("card 1 %d card2 %d\n", cardsSelected[0], cardsSelected[1]);

    ImGui::SetCursorPos(ImVec2((window_width - image_width_cardback) * 1 / 4 - image_width_cardback / 2, (window_height / 2) - image_height_cardback -image_height_cardback/8));

    if (!cardsSelected[0] && possiblePair == -1) {
        ImGui::PushID("possible 1");
        if (ImGui::ImageButton((void*)(intptr_t)image_texture_cardback, ImVec2(image_width_cardback, image_height_cardback), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(24.0f / 255, 68.0f / 255, 62.0f / 255, 1.0f), ImVec4(1, 1, 1, 1))) {
            cardsSelected[0] = true;
            possiblePair = 0;
        }
        ImGui::PopID();
    }
    else if (!cardsSelected[0]) {
        ImGui::PushID("possible 1 again");
        if (ImGui::ImageButton((void*)(intptr_t)image_texture_cardback, ImVec2(image_width_cardback, image_height_cardback), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(24.0f / 255, 68.0f / 255, 62.0f / 255, 1.0f), ImVec4(1, 1, 1, 1))) {
            if (cards[possiblePair] == cards[0]) {
                if (item == 1)
                    pairs1++;
                else
                    pairs2++;
                cardsSelected[0] = true;
                possiblePair = -1;
            }
            else {
                cardsSelected[possiblePair] = false;
                possiblePair = -1;
            }

        }
        ImGui::PopID();
    }
    else {
        ImGui::Image((void*)(intptr_t)cards[0], ImVec2(image_width_cardback, image_height_cardback));
    }
   
    ImGui::SetCursorPos(ImVec2((window_width - image_width_cardback) * 1 / 2 - image_width_cardback / 2, (window_height / 2) - image_height_cardback - image_height_cardback/8));

    if (!cardsSelected[1] && possiblePair == -1) {
        ImGui::PushID("possible 2");
        if (ImGui::ImageButton((void*)(intptr_t)image_texture_cardback, ImVec2(image_width_cardback, image_height_cardback), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(24.0f / 255, 68.0f / 255, 62.0f / 255, 1.0f), ImVec4(1, 1, 1, 1))) {
            cardsSelected[1] = true;
            possiblePair = 1;
        }
        ImGui::PopID();
    }
    else if (!cardsSelected[1]) {
        ImGui::PushID("possible 2 again");
        if (ImGui::ImageButton((void*)(intptr_t)image_texture_cardback, ImVec2(image_width_cardback, image_height_cardback), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(24.0f / 255, 68.0f / 255, 62.0f / 255, 1.0f), ImVec4(1, 1, 1, 1))) {
            //printf("card choices %d %d\n", cardChoices[possiblePair], cardChoices[1]);
            if (cards[possiblePair] == cards[1]) {
                if (item == 1)
                    pairs1++;
                else
                    pairs2++;
                cardsSelected[1] = true;
                possiblePair = -1;
            }
            else {
                cardsSelected[possiblePair] = false;
                possiblePair = -1;
            }
            
        }
        ImGui::PopID();
    }
    else {
        ImGui::Image((void*)(intptr_t)cards[1], ImVec2(image_width_cardback, image_height_cardback));
    }
    
    ImGui::SetCursorPos(ImVec2((window_width - image_width_cardback) * 3 / 4 - image_width_cardback / 2, (window_height / 2) - image_height_cardback-image_height_cardback/8));
    if (!cardsSelected[2] && possiblePair == -1) {
        ImGui::PushID("possible 3");
        if (ImGui::ImageButton((void*)(intptr_t)image_texture_cardback, ImVec2(image_width_cardback, image_height_cardback), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(24.0f / 255, 68.0f / 255, 62.0f / 255, 1.0f), ImVec4(1, 1, 1, 1))) {
            cardsSelected[2] = true;
            possiblePair = 2;
        }
        ImGui::PopID();
    }
    else if (!cardsSelected[2]) {
        ImGui::PushID("possible 3 again");
        if (ImGui::ImageButton((void*)(intptr_t)image_texture_cardback, ImVec2(image_width_cardback, image_height_cardback), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(24.0f / 255, 68.0f / 255, 62.0f / 255, 1.0f), ImVec4(1, 1, 1, 1))) {
            if (cards[possiblePair] == cards[2]) {
                if (item == 1)
                    pairs1++;
                else
                    pairs2++;
                cardsSelected[2] = true;
                possiblePair = -1;
            }
            else {
                cardsSelected[possiblePair] = false;
                possiblePair = -1;
            }

        }
        ImGui::PopID();
    }
    else {
        ImGui::Image((void*)(intptr_t)cards[2], ImVec2(image_width_cardback, image_height_cardback));
    }
    
    ImGui::SetCursorPos(ImVec2((window_width - image_width_cardback) * 1 / 4 - image_width_cardback / 2, (window_height / 2) + image_height_cardback/8));
    
    if (!cardsSelected[3] && possiblePair == -1) {
        ImGui::PushID("possible 4");
        if (ImGui::ImageButton((void*)(intptr_t)image_texture_cardback, ImVec2(image_width_cardback, image_height_cardback), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(24.0f / 255, 68.0f / 255, 62.0f / 255, 1.0f), ImVec4(1, 1, 1, 1))) {
            cardsSelected[3] = true;
            possiblePair = 3;
        }
        ImGui::PopID();
    }
    else if (!cardsSelected[3]) {
        ImGui::PushID("possible 4 again");
        if (ImGui::ImageButton((void*)(intptr_t)image_texture_cardback, ImVec2(image_width_cardback, image_height_cardback), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(24.0f / 255, 68.0f / 255, 62.0f / 255, 1.0f), ImVec4(1, 1, 1, 1))) {
            if (cards[possiblePair] == cards[3]) {
                if (item == 1)
                    pairs1++;
                else
                    pairs2++;
                cardsSelected[3] = true;
                possiblePair = -1;
            }
            else {
                cardsSelected[possiblePair] = false;
                possiblePair = -1;
            }

        }
        ImGui::PopID();
    }
    else {
        ImGui::Image((void*)(intptr_t)cards[3], ImVec2(image_width_cardback, image_height_cardback));
    }
    
    ImGui::SetCursorPos(ImVec2((window_width - image_width_cardback) * 1 / 2 - image_width_cardback / 2, (window_height / 2) + image_height_cardback/8));
    
    if (!cardsSelected[4] && possiblePair == -1) {
        ImGui::PushID("possible 5");
        if (ImGui::ImageButton((void*)(intptr_t)image_texture_cardback, ImVec2(image_width_cardback, image_height_cardback), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(24.0f / 255, 68.0f / 255, 62.0f / 255, 1.0f), ImVec4(1, 1, 1, 1))) {
            cardsSelected[4] = true;
            possiblePair = 4;
        }
        ImGui::PopID();
    }
    else if (!cardsSelected[4]) {
        ImGui::PushID("possible 5 again");
        if (ImGui::ImageButton((void*)(intptr_t)image_texture_cardback, ImVec2(image_width_cardback, image_height_cardback), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(24.0f / 255, 68.0f / 255, 62.0f / 255, 1.0f), ImVec4(1, 1, 1, 1))) {
            if (cards[possiblePair] == cards[4]) {
                if (item == 1)
                    pairs1++;
                else
                    pairs2++;
                cardsSelected[4] = true;
                possiblePair = -1;
            }
            else {
                cardsSelected[possiblePair] = false;
                possiblePair = -1;
            }

        }
        ImGui::PopID();
    }
    else {
        ImGui::Image((void*)(intptr_t)cards[4], ImVec2(image_width_cardback, image_height_cardback));
    }
    
    ImGui::SetCursorPos(ImVec2((window_width - image_width_cardback) * 3 / 4 - image_width_cardback / 2, (window_height / 2) + image_height_cardback/8));
    if (!cardsSelected[5] && possiblePair == -1) {
        ImGui::PushID("possible 6");
        if (ImGui::ImageButton((void*)(intptr_t)image_texture_cardback, ImVec2(image_width_cardback, image_height_cardback), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(24.0f / 255, 68.0f / 255, 62.0f / 255, 1.0f), ImVec4(1, 1, 1, 1))) {
            cardsSelected[5] = true;
            possiblePair = 5;
        }
        ImGui::PopID();
    }
    else if (!cardsSelected[5]) {
        ImGui::PushID("possible 6 again");
        if (ImGui::ImageButton((void*)(intptr_t)image_texture_cardback, ImVec2(image_width_cardback, image_height_cardback), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(24.0f / 255, 68.0f / 255, 62.0f / 255, 1.0f), ImVec4(1, 1, 1, 1))) {
            if (cards[possiblePair] == cards[5]) {
                if (item == 1)
                    pairs1++;
                else
                    pairs2++;
                cardsSelected[5] = true;
                possiblePair = -1;
            }
            else {
                cardsSelected[possiblePair] = false;
                possiblePair = -1;
            }

        }
        ImGui::PopID();
    }
    else {
        ImGui::Image((void*)(intptr_t)cards[5], ImVec2(image_width_cardback, image_height_cardback));
    }
}


void Client::restore() {
    for (int i = 0; i < PLAYER_NUM; ++i)
        playerSelection[i] = NONE;

    displayStart = 1;
    pairs1 = 0;
    pairs2 = 0;
    resetCardArray();
    disableMouse = false;
    gameStarted = 0;
    gameEnded = 0;
    catWon = 0;
    currTime = 0;
    pause = 0;
    playerType = NONE;
}

// Used to pass data from clientGame --> main --> here
void Client::updatePlayerSelection(std::array<int, PLAYER_NUM> selection)
{
    //if (gameStarted)
    //    return;

    for (int i = 0; i < PLAYER_NUM; ++i) {
        playerSelection[i] = selection[i];
        int playerIndex = playerSelection[i];
        if (i == CAT) {
            players[playerIndex] = cat;
        } else if (i == M1) {
            players[playerIndex] = mouse1;
        } else if (i == M2) {
            players[playerIndex] = mouse2;
        } else if (i == M3) {
            players[playerIndex] = mouse3;
        }

        if (playerIndex == my_id) {
            player = players[playerIndex];
            printf("I FOUND MYSELF %d\n", i);
            if (playerIndex == playerSelection[CAT])
                printf("IM THE cat\n");
        }
            
    }
}

void Client::playerSelectGUI() {
    
    // If game started, clear this screen
    if (displayStart || !playerSelect || gameStarted)
        return;

    double adjust_cat = 0.6f;
    double adjust_mouse = 0.3f;
    ImGuiWindowFlags flags = 0;
    flags |= ImGuiWindowFlags_NoTitleBar;
    flags |= ImGuiWindowFlags_NoScrollbar;
    flags |= ImGuiWindowFlags_NoResize;
    float height_resize = window_height / static_cast<float>(1080);
    float width_resize = window_width / static_cast<float>(1920);
    //printf("height %d width %d\n", (window_height / 1017), (window_width / 1920));

    ImGui::SetNextWindowSize(ImVec2(window_width, window_height), 0);
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0));
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.f, 0.f, 0.f, 0.f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.f, 0.f, 0.f, 0.f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.f, 0.f, 0.f, 0.f));

    ImGui::Begin("PlayerSelect GUI", NULL, flags);
    ImGui::SetCursorPosY(10);
    ImGui::PushFont(MASSIVEcuteFont);
    TextCentered("Select a Player");
    ImGui::PopFont();
    ImGui::Image((void*)(intptr_t)image_texture_background, ImVec2(window_width, window_height));
    float catLoc = (window_width - image_width_cat_icon * adjust_cat * width_resize) / 5 - 100;
    ImGui::SetCursorPos(ImVec2(catLoc, (window_height - image_height_start_cat * adjust_cat * height_resize) / 2 + 100));
    
    int catSelected = playerSelection[0];
    int mouse1Selected = playerSelection[1];
    int mouse2Selected = playerSelection[2];
    int mouse3Selected = playerSelection[3];

    auto selectedTextColor = IM_COL32(245, 25, 25, 255);
    ImVec4 selectedRgba = ImVec4(128.0f / 255, 128.0f / 255, 128.0f / 255, 0.5f);


    if (catSelected != NONE) {
        // If cat is already selected.
        if (catSelected == my_id) {
            // Player has cat selected.
            ImGui::PushID("cat icon");
            if (ImGui::ImageButton((void*)(intptr_t)image_texture_cat_icon, ImVec2(image_width_cat_icon * adjust_cat * width_resize, image_height_cat_icon * adjust_cat * height_resize), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0, 0, 0, 0), selectedRgba)) {
                // Player is deselecting!
                playerSelected = true;
                playerTypeSent = false;
                playerType = NONE;
            }
            ImGui::PushFont(cuteFont);
            ImGui::PushStyleColor(ImGuiCol_Text, selectedTextColor);
            ImGui::SetCursorPos(ImVec2(catLoc, (window_height - image_height_start_cat * adjust_cat * height_resize) / 2 + 100));
            //ImGui::Text("-->[P%d]<--", catSelected);
            ImGui::PopStyleColor();
            ImGui::PopFont();
            ImGui::PopID();
        }
        else {
            // Someone else has cat selected.
            ImGui::SetCursorPos(ImVec2(catLoc, (window_height - image_height_start_cat * adjust_cat * height_resize) / 2 + 100));
            ImGui::Image((void*)(intptr_t)image_texture_cat_icon_pale, ImVec2(image_width_cat_icon * adjust_cat * width_resize, image_height_cat_icon * adjust_cat * height_resize));
            
            // TODO: Make font pop out more
            ImGui::PushFont(cuteFont);
            ImGui::SetCursorPos(ImVec2(catLoc, (window_height - image_height_start_cat * adjust_cat * height_resize) / 2 + 100));
            //ImGui::Text("[P%d]", catSelected);
            ImGui::PopFont();
        }
    } else {
        // No one has selected cat
        GLuint icon = image_texture_cat_icon; 
        if (!catHover)
            icon = image_texture_cat_hover_icon;

        ImGui::PushID("cat icon");
        if (ImGui::ImageButton((void*)(intptr_t)icon, ImVec2(image_width_cat_icon * adjust_cat * width_resize, image_height_cat_icon * adjust_cat * height_resize), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0, 0, 0, 0), ImVec4(1, 1, 1, 1))) {
            if (playerType != NONE) {
                return;
            }
            
            // Player is selecting cat!
            playerSelected = true;
            playerTypeSent = false;
            playerType = CAT;
        }
        catHover = ImGui::IsItemHovered();
        ImGui::PopID();
    }

    float mouseLocX = (3 * window_width) / 4 - (image_width_mouse_icon * adjust_mouse * width_resize);
    float mouseLocY = (window_height / 3) - (image_height_mouse_icon * adjust_mouse * height_resize) / 2;

    ImGui::SetCursorPos(ImVec2(mouseLocX, mouseLocY));

    if (mouse1Selected != NONE) {
        // If cat is already selected.
        if (mouse1Selected == my_id) {
            // Player has cat selected.
            ImGui::PushID("mouse 1");
            if (ImGui::ImageButton((void*)(intptr_t)image_texture_mouse_icon, ImVec2(image_width_mouse_icon * adjust_mouse * width_resize, image_height_mouse_icon * adjust_mouse * height_resize), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0, 0, 0, 0), selectedRgba)) {
                // Player is deselecting!
                playerSelected = true;
                playerTypeSent = false;
                playerType = NONE;
            }
            ImGui::PushFont(cuteFont);
            ImGui::PushStyleColor(ImGuiCol_Text, selectedTextColor);
            ImGui::SetCursorPos(ImVec2(mouseLocX, mouseLocY));
            //ImGui::Text("-->[P%d]<--", mouse1Selected);
            ImGui::PopStyleColor();
            ImGui::PopFont();
            ImGui::PopID();
        }
        else {
            // Someone else has cat selected.
            ImGui::SetCursorPos(ImVec2(mouseLocX, mouseLocY));
            ImGui::Image((void*)(intptr_t)image_texture_mouse_icon_pale, ImVec2(image_width_mouse_icon * adjust_mouse * width_resize, image_height_mouse_icon * adjust_mouse * height_resize));

            // TODO: Make font pop out more
            ImGui::PushFont(cuteFont);
            ImGui::SetCursorPos(ImVec2(mouseLocX, mouseLocY));
            //ImGui::Text("[P%d]", mouse1Selected);
            ImGui::PopFont();
        }
    }
    else {
        // No one has selected cat
        GLuint icon = image_texture_mouse_icon;
        if (!mouse1Hover)
            icon = image_texture_mouse_hover_icon;

        ImGui::PushID("mouse 1");
        if (ImGui::ImageButton((void*)(intptr_t)icon, ImVec2(image_width_mouse_icon * adjust_mouse * width_resize, image_height_mouse_icon * adjust_mouse * height_resize), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0, 0, 0, 0), ImVec4(1, 1, 1, 1))) {
            if (playerType != NONE) {
                return;
            }

            // Player is selecting mouse!
            playerSelected = true;
            playerTypeSent = false;
            playerType = M1;
        }
        mouse1Hover = ImGui::IsItemHovered();
        ImGui::PopID();
    }

    mouseLocX = mouseLocX - (image_width_mouse_icon * adjust_mouse * width_resize) / 2;
    mouseLocY = mouseLocY + (image_height_mouse_icon * adjust_mouse * height_resize);
    ImGui::SetCursorPos(ImVec2(mouseLocX, mouseLocY));

    if (mouse2Selected != NONE) {
        // If cat is already selected.
        if (mouse2Selected == my_id) {
            // Player has cat selected.
            ImGui::PushID("mouse 2");
            if (ImGui::ImageButton((void*)(intptr_t)image_texture_mouse_icon, ImVec2(image_width_mouse_icon * adjust_mouse * width_resize, image_height_mouse_icon * adjust_mouse * height_resize), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0, 0, 0, 0), selectedRgba)) {
                // Player is deselecting!
                playerSelected = true;
                playerTypeSent = false;
                playerType = NONE;
            }
            ImGui::PushFont(cuteFont);
            ImGui::PushStyleColor(ImGuiCol_Text, selectedTextColor);
            ImGui::SetCursorPos(ImVec2(mouseLocX, mouseLocY));
            //ImGui::Text("-->[P%d]<--", mouse2Selected);
            ImGui::PopStyleColor();
            ImGui::PopFont();
            ImGui::PopID();
        }
        else {
            // Someone else has cat selected.
            ImGui::SetCursorPos(ImVec2(mouseLocX, mouseLocY));
            ImGui::Image((void*)(intptr_t)image_texture_mouse_icon_pale, ImVec2(image_width_mouse_icon * adjust_mouse * width_resize, image_height_mouse_icon * adjust_mouse * height_resize));

            // TODO: Make font pop out more
            ImGui::PushFont(cuteFont);
            ImGui::SetCursorPos(ImVec2(mouseLocX, mouseLocY));
            //ImGui::Text("[P%d]", mouse2Selected);
            ImGui::PopFont();
        }
    }
    else {
        // No one has selected cat
        GLuint icon = image_texture_mouse_icon;
        if (!mouse2Hover)
            icon = image_texture_mouse_hover_icon;

        ImGui::PushID("mouse 1");
        if (ImGui::ImageButton((void*)(intptr_t)icon, ImVec2(image_width_mouse_icon * adjust_mouse * width_resize, image_height_mouse_icon * adjust_mouse * height_resize), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0, 0, 0, 0), ImVec4(1, 1, 1, 1))) {
            if (playerType != NONE) {
                return;
            }

            // Player is selecting mouse!
            playerSelected = true;
            playerTypeSent = false;
            playerType = M2;
        }
        mouse2Hover = ImGui::IsItemHovered();
        ImGui::PopID();
    }
    

    //ImGui::Image((void*)(intptr_t)image_texture_mouse_icon, ImVec2(image_width_mouse_icon * adjust_mouse * width_resize, image_height_mouse_icon * adjust_mouse * height_resize));
    mouseLocX = mouseLocX + (image_width_mouse_icon * adjust_mouse * width_resize);
    //mouseLocY = mouseLocY + image_height_mouse_icon * adjust_mouse * height_resize;
    ImGui::SetCursorPos(ImVec2(mouseLocX, mouseLocY));

    if (mouse3Selected != NONE) {
        // If cat is already selected.
        if (mouse3Selected == my_id) {
            // Player has cat selected.
            ImGui::PushID("mouse 3");
            if (ImGui::ImageButton((void*)(intptr_t)image_texture_mouse_icon, ImVec2(image_width_mouse_icon * adjust_mouse * width_resize, image_height_mouse_icon * adjust_mouse * height_resize), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0, 0, 0, 0), selectedRgba)) {
                // Player is deselecting!
                playerSelected = true;
                playerTypeSent = false;
                playerType = NONE;
            }
            ImGui::PushFont(cuteFont);
            ImGui::PushStyleColor(ImGuiCol_Text, selectedTextColor);
            ImGui::SetCursorPos(ImVec2(mouseLocX, mouseLocY));
            //ImGui::Text("-->[P%d]<--", mouse3Selected);
            ImGui::PopStyleColor();
            ImGui::PopFont();
            ImGui::PopID();
        }
        else {
            // Someone else has cat selected.
            ImGui::SetCursorPos(ImVec2(mouseLocX, mouseLocY));
            ImGui::Image((void*)(intptr_t)image_texture_mouse_icon_pale, ImVec2(image_width_mouse_icon * adjust_mouse * width_resize, image_height_mouse_icon * adjust_mouse * height_resize));

            // TODO: Make font pop out more
            ImGui::PushFont(cuteFont);
            ImGui::SetCursorPos(ImVec2(mouseLocX, mouseLocY));
            //ImGui::Text("[P%d]", mouse3Selected);
            ImGui::PopFont();
        }
    }
    else {
        // No one has selected cat
        GLuint icon = image_texture_mouse_icon;
        if (!mouse3Hover)
            icon = image_texture_mouse_hover_icon;

        ImGui::PushID("mouse 3");
        if (ImGui::ImageButton((void*)(intptr_t)icon, ImVec2(image_width_mouse_icon * adjust_mouse * width_resize, image_height_mouse_icon * adjust_mouse * height_resize), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0, 0, 0, 0), ImVec4(1, 1, 1, 1))) {
            if (playerType != NONE) {
                return;
            }

            // Player is selecting mouse!
            playerSelected = true;
            playerTypeSent = false;
            playerType = M3;
        }
        mouse3Hover = ImGui::IsItemHovered();
        ImGui::PopID();
    }

    
    ImGui::PopStyleColor();
    ImGui::PopStyleColor(3);
    ImGui::End();
}

void Client::GameStartGUI() {
    if (!displayStart)
        return;

    double adjustment = 3.5f;
    double adjust_cat = 0.6f;
    double adjust_mouse = 0.4f;
    double adjust_catuate = 2.0f;
    ImGuiWindowFlags flags = 0;
    float height_resize = window_height / static_cast<float>(1080);
    float width_resize = window_width / static_cast<float>(1920);
    //flags |= ImGuiWindowFlags_NoBackground;
    flags |= ImGuiWindowFlags_NoTitleBar;
    flags |= ImGuiWindowFlags_NoScrollbar;
    flags |= ImGuiWindowFlags_NoResize;
    ImGui::GetStyle().Colors[ImGuiCol_Button] = ImVec4(1, 1, 1, 0);
    ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered] = ImVec4(0.1, 0.6, 0.1, 0.5);
    ImGui::GetStyle().Colors[ImGuiCol_ButtonActive] = ImVec4(0, 0.7, 0, 1);

    ImGui::SetNextWindowSize(ImVec2(window_width, window_height), 0);
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    //ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.8f, 0.8f, 0.0f, 1.0f));
    //ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(214.0f / 255, 232.0f / 255, 101.0f / 255, 1.0f));
    ImGui::Begin("GameStart GUI", NULL, flags);

    ImGui::Image((void*)(intptr_t)image_texture_title, ImVec2(image_width_title, image_height_title));
    //ImGui::PopStyleColor();
    //ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(1.0, 1.0f, 1.0f, 1.0f));
    //ImGui::SetCursorPos(ImVec2((window_width - image_width_game_start * adjustment * width_resize) / 2, (window_height - image_height_game_start * adjustment * height_resize) / 2));
    //ImGui::Image((void*)(intptr_t)image_texture_game_start, ImVec2(image_width_game_start * adjustment, image_height_game_start * adjustment));
    //ImGui::Image((void*)(intptr_t)image_texture_game_start, ImVec2(image_width_game_start * adjustment * width_resize, image_height_game_start * adjustment * height_resize));
    //float catLoc = ((window_width - image_width_game_start * adjustment * width_resize) / 2 - image_width_start_cat * adjust_cat * width_resize) / 2;
    //ImGui::SetCursorPos(ImVec2(catLoc, (window_height - image_height_start_cat * adjust_cat * height_resize) / 2));

    //ImGui::Image((void*)(intptr_t)image_texture_start_cat, ImVec2(image_width_start_cat * adjust_cat * width_resize, image_height_start_cat * adjust_cat * height_resize));
    //float mouseLoc = (window_width - image_width_game_start * adjustment * width_resize) / 2 + ((window_width - image_width_game_start * adjustment * width_resize) / 2 - image_width_start_mouse * adjust_mouse * width_resize) / 2;
    //ImGui::SetCursorPos(ImVec2(mouseLoc, (window_height - image_height_start_mouse * adjust_mouse * height_resize) / 2 + 20));
    //ImGui::Image((void*)(intptr_t)image_texture_start_mouse, ImVec2(image_width_start_mouse * adjust_mouse * width_resize, image_height_start_mouse * adjust_mouse * height_resize));
    //ImGui::SetCursorPos(ImVec2((window_width - image_width_start_catuate * adjust_catuate * width_resize) / 2, 40));
    //ImGui::Image((void*)(intptr_t)image_texture_start_catuate, ImVec2(image_width_start_catuate * adjust_catuate * width_resize, image_height_start_catuate * adjust_catuate * height_resize));
    //ImGui::PopStyleColor();
    float buttonLoc = 3 * window_width / 4;
    ImGui::SetCursorPos(ImVec2(buttonLoc, 3 * window_height / 4));
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
    ImGui::PushFont(MASSIVEcuteFont);

    if (my_id == 0) {
        if (ImGui::Button("Start Game"))
        {
            //playerSelect = true;
            //gameStartPressed = true;
            displayStartPressed = true;
            if (!playerSelect)
                gameStartPressed = true;
            /*audioEngine->StopEvent("event:/music1");
            audioEngine->PlayEvent("event:/music_placeholder");*/
        }
    }
    /*else
        ImGui::Button("Join Party"); */// this actually doesn't do anything right now

    ImGui::PopFont();
    ImGui::PushFont(cuteFont);
    ImGui::SetCursorPos(ImVec2(buttonLoc, 5 * window_height / 6));
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
    //player = players[my_id];
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

void Client::resetPair1() {
    pairs1 = -1;
}

void Client::resetPair2() {
    pairs2 = -1;
}

void Client::resetCardArray() {
    for (int i = 0; i < 6; i++)
        cardsSelected[i] = false;
    inMiniGame = false;
    possiblePair = -1;
    cards.clear();
    random_shuffle(cardChoices.begin(), cardChoices.end());
    for (int i = 0; i < cardChoices.size(); i++) {
        if (cardChoices[i] == 1)
            cards.push_back(image_texture_card1);
        else if (cardChoices[i] == 2)
            cards.push_back(image_texture_card2);
        else if (cardChoices[i] == 3)
            cards.push_back(image_texture_card3);
    }
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

void Client::hideStartScreen() {
    displayStart = 0;

    // If player select is disabled, mark game as started.
    if (!playerSelect)
        gameStarted = 1;
}


bool Client::checkHideStartScreen() {
    if (displayStartPressed) {
        displayStartPressed = false;
        return true;
    }
    return false;
}

void Client::setGameStart() {


    // Figure out what which player client belongs to 
    for (int i = 0; i < PLAYER_NUM; ++i) {
        int playerIndex = playerSelection[i];
        if (playerIndex == my_id)
            player = players[playerIndex];
    }

    // If no player select, just go by id
    if (!playerSelect)
        player = players[my_id];


    thirdPersonCamera = new ThirdPersonCamera(player);
    displayStart = 0;
    gameStarted = 1;
}

bool Client::checkGameStart() {
    if (gameStartPressed) {
        gameStartPressed = false;
        return true;
    }
    return false;
}

int Client::checkPlayerSelect() {
    if (playerSelected && !playerTypeSent) {
        playerTypeSent = true;
        return playerType;
    }
    return -1;
}

int Client::checkPair1Count() {
    return pairs1;
}

int Client::checkPair2Count() {
    return pairs2;
}

void Client::setGameOver(int g, int w) {
    gameEnded = g;
    catWon = w;
    finalTime = currTime;

    for (int i = 0; i < PLAYER_NUM; ++i)
        playerSelection[i] = NONE;

    //printf("%d gameOver %d\n", g, w);
}

void Client::setMovingState(PlayerState* playerstate) {
    movingState[CAT] = playerstate[playerSelection[CAT]].moving;
    movingState[M1] = playerstate[playerSelection[M1]].moving;
    movingState[M2] = playerstate[playerSelection[M2]].moving;
    movingState[M3] = playerstate[playerSelection[M3]].moving;
}

void Client::setAliveStatus(PlayerState* playerstate) {
    for (int i = 0; i < PLAYER_NUM; i++) {
        aliveState[i] = playerstate[i].alive;
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
    if (thirdPersonCamera != NULL)
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

    if (!disableMouse) {
        if (inMiniGame || !gameStarted) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            cursorNormal = true;
        }
        else {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            cursorNormal = false;
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
            //showMouse = !showMouse;
            disableMouse = true;
            
            if (cursorNormal) {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                cursorNormal = false;
            }
            else {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                cursorNormal = true;
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
    //keyHeld = keys[0] || keys[1] || keys[2] || keys[3];
    keyHeld = true;
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
