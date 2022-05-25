#include "Client.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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
static Model* teapot;
static Model* bunny;
static Model* tyra;
static Model* tyra2;
static Model* player;
static Model* backpack;
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
static Model* demoChar;
static Model* demoChar2;

// Animations
static Animation* demoAnimation;
static Animator* animator;
static Animation* demoAnimation2;
static Animator* animator2;

//Particles
static ParticleSystem* particlesystem;

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
static bool gameStartPressed = 0;
static bool catSeesItem = false;
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
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
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

    //initialize particle system
    particlesystem = new ParticleSystem(particleShader, "../../particles/particle.png", 500, glm::mat4(1));

    // initialize objects
    ground = new Cube(glm::vec3(-10, -1, -10), glm::vec3(10, 1, 10));
    ground->moveGlobal(glm::vec3(0, -3, 0));
    teapot = new Model("../../objects/teapot/teapot.obj");
    teapot->scale(glm::vec3(2));
    teapot->moveGlobal(glm::vec3(-5, -2, -5));
    bunny = new Model("../../objects/bunny/bunny.obj");
    bunny->scale(glm::vec3(2));
    bunny->moveGlobal(glm::vec3(5, -2, -5));
    tyra = new Model("../../objects/tyra/tyra.obj");
    tyra->scale(glm::vec3(1.5));
    tyra->moveGlobal(glm::vec3(0, -0.1, 0));
    tyra2 = new Model("../../objects/tyra/tyra.obj");
    tyra2->moveGlobal(glm::vec3(2, -0.1, 0));
    backpack = new Model("../../objects/backpack/backpack.obj");
    maze = new Model("../../objects/maze_textured/maze3D.obj");
    maze->moveGlobal(glm::vec3(0, -3, 0));
    bear = new Model("../../objects/bear/bear.obj");
    bear->moveGlobal(glm::vec3(55, -3, -135));
    bear->scale(glm::vec3(0.5));
    fallenstar = new Model("../../objects/fallenstar/fallenstar.obj");
    fallenstar->moveGlobal(glm::vec3(95, 2, -35));
   
    item = new Model("../../objects/DiplomaFrame/DiplomaFrame.fbx");
    item2 = new Model("../../objects/computer/computer.obj");
    item2->moveGlobal(glm::vec3(70, 0, -5));
    item2->scale(glm::vec3(5));
    item3 = new Model("../../objects/backpack2/backpack2.obj");
    item3->moveGlobal(glm::vec3(65, -5, -10));
    item3->scale(glm::vec3(.1));
    //item = new Model("../../objects/backpack/backpack.obj");

    demoChar = new Model("../../objects/Kachujin/jog.fbx");
    demoChar->scale(glm::vec3(0.3));
    demoChar->moveGlobal(glm::vec3(9, -2, -2));

    demoAnimation = new Animation("../../objects/Kachujin/jog.fbx", demoChar);
    animator = new Animator(demoAnimation);

    geisel = new Model("../../objects/Geisel/Geisel.fbx");
    
    geisel->moveGlobal(glm::vec3(75, 5, -75));
    geisel->scale(glm::vec3(0.005));

    sungod = new Model("../../objects/sungod/sungod.obj");
    sungod->moveGlobal(glm::vec3(105, 2, -135));
    sungod->spin(90);
    //sungod->scale(glm::vec3(2));

    demoChar2 = new Model("../../objects/morak/morak_samba_small.fbx");
    demoAnimation2 = new Animation("../../objects/morak/morak_samba_small.fbx", demoChar2);
    animator2 = new Animator(demoAnimation2);
    demoChar2->scale(glm::vec3(0.6));
    demoChar2->moveGlobal(glm::vec3(0, -2, 0));


    ret = LoadTextureFromFile("../../objects/ImGui/cute_cat.png", &my_image_texture, &my_image_width, &my_image_height);
    retGameOver = LoadTextureFromFile("../../objects/ImGui/explosion.png", &image_texture_game_over, &image_width_game_over, &image_height_game_over);
    retMouseWin = LoadTextureFromFile("../../objects/ImGui/celebration.png", &image_texture_mouse_win, &image_width_mouse_win, &image_height_mouse_win);
    retMouseFlag = LoadTextureFromFile("../../objects/ImGui/cheese.png", &image_texture_mouse_flag, &image_width_mouse_flag, &image_height_mouse_flag);
    retMouseFlagPale = LoadTextureFromFile("../../objects/ImGui/cheese_paler.png", &image_texture_mouse_flag_pale, &image_width_mouse_flag_pale, &image_height_mouse_flag_pale);
    retMap = LoadTextureFromFile("../../objects/ImGui/minimap.png", &image_texture_map, &image_width_map, &image_height_map);
    retGameStart = LoadTextureFromFile("../../objects/ImGui/game_start_maze2.jpg", &image_texture_game_start, &image_width_game_start, &image_height_game_start);
    retStartCat = LoadTextureFromFile("../../objects/ImGui/mao_cat.png", &image_texture_start_cat, &image_width_start_cat, &image_height_start_cat);
    retStartMouse = LoadTextureFromFile("../../objects/ImGui/mao_mouse.png", &image_texture_start_mouse, &image_width_start_mouse, &image_height_start_mouse);
    retStartCatuate = LoadTextureFromFile("../../objects/ImGui/catuate2.png", &image_texture_start_catuate, &image_width_start_catuate, &image_height_start_catuate);
    retCatIcon = LoadTextureFromFile("../../objects/ImGui/cat_icon.png", &image_texture_cat_icon, &image_width_cat_icon, &image_height_cat_icon);
    retMouseIcon = LoadTextureFromFile("../../objects/ImGui/mouse_icon.png", &image_texture_mouse_icon, &image_width_mouse_icon, &image_height_mouse_icon);
    retDiploma = LoadTextureFromFile("../../objects/ImGui/diploma.png", &image_texture_diploma, &image_width_diploma, &image_height_diploma);

    // COLLISION DEBUG
    wall1 = new Cube(glm::vec3(-2, -5, -1), glm::vec3(2, 5, 1));
    wall1->moveGlobal(glm::vec3(8, 0, -8));
    wall2 = new Cube(glm::vec3(-2, -5, -1), glm::vec3(2, 5, 1));
    wall2->moveGlobal(glm::vec3(8, 0, 8));
    cDetector.insert(wall1->getOBB());
    cDetector.insert(wall2->getOBB());
    cDetector.insert(tyra->getOBB());
    // COLLITION DEBUG

    //Load the scene
    scene = new SceneLoader("../../scripts/scene.txt");
    sceneObjects = scene->load();



    //hard coded for now
    players[0] = tyra;
    players[1] = teapot;
    players[2] = bunny;
    players[3] = tyra2;

    skybox = new Skybox();

    ImGuiIO& io = ImGui::GetIO();
    plainFont = io.Fonts->AddFontDefault();
    cuteFont = io.Fonts->AddFontFromFileTTF("../../fonts/Gidole/Gidolinya-Regular.otf", 32.0f);
    HUGEcuteFont = io.Fonts->AddFontFromFileTTF("../../fonts/Gidole/Gidolinya-Regular.otf", 52.0f);
    MASSIVEcuteFont = io.Fonts->AddFontFromFileTTF("../../fonts/Gidole/Gidolinya-Regular.otf", 70.0f);

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

    glm::mat4 identityMat = glm::mat4(1);
    isThirdPersonCam = false;
    switch (select) {
    case 0: {

        for (auto character : players) {
            character->draw(currCam->viewProjMat, identityMat, shader);
        }

        calcFinalBoneMatrix(animator);
        demoChar->draw(currCam->viewProjMat, identityMat, shader);

        calcFinalBoneMatrix(animator2);
        demoChar2->draw(currCam->viewProjMat, identityMat, shader);
       
        ground->draw(currCam->viewProjMat, identityMat, shader);

        particlesystem->draw(currCam->viewProjMat);

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
        //isThirdPersonCam = true;
        maze->draw(currCam->viewProjMat, identityMat, shader);
        tyra->draw(currCam->viewProjMat, identityMat, shader);
        bear->draw(currCam->viewProjMat, identityMat, shader);
       
        geisel->draw(currCam->viewProjMat, identityMat, shader);
        sungod->draw(currCam->viewProjMat, identityMat, shader);
        fallenstar->draw(currCam->viewProjMat, identityMat, shader);
        
        teapot->draw(currCam->viewProjMat, identityMat, shader);
        bunny->draw(currCam->viewProjMat, identityMat, shader);
        tyra2->draw(currCam->viewProjMat, identityMat, shader);
        item->draw(currCam->viewProjMat, identityMat, shader);
        item2->draw(currCam->viewProjMat, identityMat, shader);
        item3->draw(currCam->viewProjMat, identityMat, shader);
        
        break;
    }

    case 2: {
        backpack->draw(currCam->viewProjMat, identityMat, shader);
        drawOBB(backpack->getOBB(), currCam->viewProjMat, shader, false);
        break;
    }
    case 3: {
        scene->draw(currCam->viewProjMat, identityMat, shader, sceneObjects);
        break;
    }
    }

    //drawOBB skybox last for efficiency
    glUseProgram(skyboxShader);
    glUniform1i(glGetUniformLocation(skyboxShader, "skybox"), 0);
    glUseProgram(0);
    //drop right column
    glm::mat4 viewNoTranslate = glm::mat4(glm::mat3(currCam->view));
    skybox->draw(currCam->projection* viewNoTranslate, skyboxShader);
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
        backpack->update();

        // COLLITION DEBUG
        wall2->update();
        cDetector.update(wall2->getOBB(), 1);
        cDetector.update(tyra->getOBB(), 2);
        // COLLITION DEBUG

        animator->update(dt);
        animator2->update(dt);

        particlesystem->update(dt, *demoChar2, 2, glm::vec3(x,y,z));
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
    delete teapot;
    delete bunny;
    delete tyra;
    delete ground;
    delete backpack;
    delete maze;
    delete skybox;
    delete bear;
    delete sungod;
    delete fallenstar;
    delete item;
    delete geisel;
    delete demoChar;
    delete demoChar2;
    delete animator;
    delete animator2;
    delete demoAnimation;
    delete demoAnimation2;

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

void Client::ItemHoldGUI() {
    double adjustment = 0.18;
    
    ImGuiWindowFlags flags = 0;
    flags |= ImGuiWindowFlags_NoBackground;
    flags |= ImGuiWindowFlags_NoTitleBar;
    flags |= ImGuiWindowFlags_NoScrollbar;
    flags |= ImGuiWindowFlags_NoResize;

    ImGui::SetNextWindowSize(ImVec2(image_width_mouse_flag * adjustment+10, image_height_mouse_flag*adjustment+10));
   // ImGui::SetNextWindowPos(ImVec2((window_width - 533)/2, window_height/30), 0, ImVec2(0, 0));
    ImGui::SetNextWindowPos(ImVec2(280, 15), 0, ImVec2(0, 0));
   
    ImGui::Begin("ItemHold GUI", NULL, flags);

   

    if (itemhold != PLAYER_NUM + 1) {
        ImGui::Image((void*)(intptr_t)image_texture_mouse_flag, ImVec2(image_width_mouse_flag * adjustment, image_height_mouse_flag * adjustment));

    /*    if (currTime % 2 == 0) {
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 0.95f));
        }
        else {
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 1.0f, 0.0f, 0.95f));
        }
        ImGui::PushFont(HUGEcuteFont);
        ImGui::Text("Player %d is holding the Flag!!", itemhold);
        ImGui::PopFont();
        ImGui::PopStyleColor();*/

       
    }
    else {
        ImGui::Image((void*)(intptr_t)image_texture_mouse_flag_pale, ImVec2(image_width_mouse_flag_pale * adjustment, image_height_mouse_flag_pale * adjustment));
    }

    ImGui::End();
}

void displayLocation(glm::mat4 model, int id, double adjustment, float height_resize, float width_resize) {
    int r, g, b;
    //float multX = 1.15 + 26;
    //float multZ = 1.15 + 24;
    float locX = model[3][0] * 1.55 + 25;
    float locZ = abs(model[3][2]) * 1.55 + 25;
    float side = 3.2f;
    //printf("x %d z %d\n", locX, locZ);
    if (id == 0) {
        r = 255;
        g = 0;
        b = 0;
    }
    else if (id == 4) {
        r = 255;
        g = 240;
        b = 31;
    }
    else {
        r = 177;
        g = 156;
        b = 217;
    }

    //double adjust_icon = 0.01f;
    float icon_size = 8.0f;
    

    if (id == 0) {
        //ImGui::SetCursorPos(ImVec2(locZ, locX));
        ImGui::GetWindowDrawList()->AddImage((void*)(intptr_t)image_texture_cat_icon, ImVec2(locZ-icon_size, locX-icon_size), ImVec2(locZ+icon_size, locX+icon_size), ImVec2(0, 0), ImVec2(1, 1));
        //ImGui::Image((void*)(intptr_t)image_texture_cat_icon, ImVec2(image_width_cat_icon * adjust_icon, image_height_cat_icon * adjust_icon));
        //ImGui::GetForegroundDrawList()->AddTriangleFilled(ImVec2(locZ+side, locX), ImVec2(locZ-side, locX+side), ImVec2(locZ-side, locX-side), IM_COL32(r, g, b, 255));
    }
    else if (id != 4) {
        ImGui::GetWindowDrawList()->AddImage((void*)(intptr_t)image_texture_mouse_icon, ImVec2(locZ - icon_size, locX - icon_size), ImVec2(locZ + icon_size, locX + icon_size), ImVec2(0, 0), ImVec2(1, 1));
        //ImGui::GetForegroundDrawList()->AddCircle(ImVec2(locZ, locX), 2, IM_COL32(r, g, b, 255), 100, 2.f);
    }
    else {
      /*  if (currTime % 2 == 0)
            ImGui::GetForegroundDrawList()->AddCircle(ImVec2(locZ, locX), 2, IM_COL32(r, g, b, 255), 100, 2.f);*/
    
        if (itemhold != PLAYER_NUM + 1) { // bearl location hard coded TODO fix
            if (currTime % 2 == 0) {
                float locX1 = finalDest[3][0] * 1.15 + 25;
                float locZ1 = abs(finalDest[3][2]) * 1.15 + 25;
                ImGui::GetWindowDrawList()->AddImage((void*)(intptr_t)image_texture_diploma, ImVec2(locZ1 - icon_size, locX1 - icon_size), ImVec2(locZ1 + icon_size, locX1 + icon_size), ImVec2(0, 0), ImVec2(1, 1));
            }
              
                //ImGui::GetForegroundDrawList()->AddCircle(ImVec2(75*1.15 + 24, 75* 1.15 + 26), 2, IM_COL32(204, 0, 204, 255), 100, 2.f);
        }
        else {
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
        displayLocation(players[0]->getModel(), 0, adjustment, height_resize, width_resize);
    }
    if (players[1]) {
        displayLocation(players[1]->getModel(), 1, adjustment, height_resize, width_resize);
    }

    if (players[2]) {
        displayLocation(players[2]->getModel(), 2, adjustment, height_resize, width_resize);
    }

    if (players[3]) {
        displayLocation(players[3]->getModel(), 3, adjustment, height_resize, width_resize);
    }

    
    if (item && (my_id != 0 || catSeesItem)) {
        displayLocation(item->getModel(), 4, adjustment, height_resize, width_resize);
    }
    

    ImGui::End();
}

void Client::GameStartGUI() {

    if (gameStarted == 1)
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
    //float mouseLoc = window_width - image_width_start_mouse * adjust_mouse;
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
            gameStartPressed = true;
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
    
    if (gameEnded == 1 && catWon == 0) {
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
    else if (gameEnded == 1) {
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

void Client::setNumPlayers(int p) {
    numPlayers = p;
}

void Client::setCatViewItem(bool c) {
    catSeesItem = c;
}

void Client::updateTime(int t) {
    currTime = t;
}

void Client::setItemHold(char h) {
    itemhold = h;
}

void Client::setFinalDest(glm::mat4 location) {
    finalDest = location;
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
