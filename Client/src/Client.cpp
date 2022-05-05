#include "Client.h"

// shader, camera and light
static GLuint shader;
static GLuint skyboxShader;
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

// COLLISION DEBUG
static Cube* wall1;
static Cube* wall2;
static CollisionDetector cDetector;
// COLLITION DEBUG

// state variables
unsigned int my_id;
static int oldTime = 0;
static double prevXPos;
static double prevYPos;
static int select = 0;
static bool pause = false;
static bool showMouse = false;
static bool middlePressed = false;
static bool isThirdPersonCam = false;
static const char* scenes[3] = { "3rd Person Tyra", "Baby Maze", "Backpack"};

static bool keys[4];
static bool keyHeld = false;
static int direction = -1;

static glm::mat4 currRotationUpdate = glm::mat4(1);

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

/**
 * Initialize shader program
 *
 * @return  True if success, False otherwise
**/
bool Client::initializeClient() {
    // initialize shader
    shader = Shader::loadShaders("../../shaders/shader.vert", "../../shaders/shader.frag");
    skyboxShader = Shader::loadShaders("../../shaders/skyboxShader.vert", "../../shaders/skyboxShader.frag");
    if (!shader) {
        spdlog::error("Failed to initialize shader programs.");
        return false;
    }
    if (!skyboxShader) {
        spdlog::error("Failed to initialize skyboxShader programs.");
        return false;
    }

    // initialize camera
    camera = new Camera();

    // initialize light sources
    lightPosn = { {0, 5, -10, 1}, {0, 5, 10, 1}, {1, 1, 1, 0} };
    lightColorn = { {0.9, 0.6, 0.5, 1}, {0.5, 0.6, 0.9, 1}, {1, 1, 1, 1} };
    lightCount = lightPosn.size();

    // initialize objects
    ground = new Cube(glm::vec3(-10, -1, -10), glm::vec3(10, 1, 10));
    ground->moveGlobal(glm::vec3(0, -3, 0));
    teapot = new Model("../../objects/teapot.obj");
    teapot->scale(glm::vec3(2));
    teapot->moveGlobal(glm::vec3(-5, -2, -5));
    bunny = new Model("../../objects/bunny.obj");
    bunny->scale(glm::vec3(2));
    bunny->moveGlobal(glm::vec3(5, -2, -5));
    tyra = new Model("../../objects/tyra.obj");
    tyra->scale(glm::vec3(1.5));
    tyra->moveGlobal(glm::vec3(0, -0.1, 0));
    tyra2 = new Model("../../objects/tyra.obj");
    tyra2->moveGlobal(glm::vec3(2, -0.1, 0));
    backpack = new Model("../../objects/backpack/backpack.obj");
    maze = new Model("../../objects/maze_textured/mazeTextured.obj");
    maze->moveGlobal(glm::vec3(0, -3, 0));
    bear = new Model("../../objects/bear.obj");
    bear->moveGlobal(glm::vec3(75, -3, -75));
    item = new Model("../../objects/teapot.obj");

    // COLLISION DEBUG
    wall1 = new Cube(glm::vec3(-2, -5, -1), glm::vec3(2, 5, 1));
    wall1->moveGlobal(glm::vec3(8, 0, -8));
    wall2 = new Cube(glm::vec3(-2, -5, -1), glm::vec3(2, 5, 1));
    wall2->moveGlobal(glm::vec3(8, 0, 8));
    cDetector.insert(wall1->getOBB());
    cDetector.insert(wall2->getOBB());
    cDetector.insert(tyra->getOBB());
    // COLLITION DEBUG



    //hard coded for now
    players[0] = tyra;
    players[1] = teapot;
    players[2] = bunny;
    players[3] = tyra2;

    skybox = new Skybox();

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

    glm::mat4 identityMat = glm::mat4(1);
    switch (select) {
    case 0: {
        for (auto character : players) {
            character->draw(currCam->viewProjMat, identityMat, shader);
        }
       
        ground->draw(currCam->viewProjMat, identityMat, shader);

        // COLLITION DEBUG
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
        // COLLITION DEBUG

        item->draw(currCam->viewProjMat, identityMat, shader);

        break;
    }

    case 1: {
        maze->draw(currCam->viewProjMat, identityMat, shader);
        tyra->draw(currCam->viewProjMat, identityMat, shader);
        bear->draw(currCam->viewProjMat, identityMat, shader);
        item->draw(currCam->viewProjMat, identityMat, shader);
        break;
    }

    case 2: {
        backpack->draw(currCam->viewProjMat, identityMat, shader);
        drawOBB(backpack->getOBB(), currCam->viewProjMat, shader, false);
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
void Client::idleCallback() {
    Camera* currCamera = isThirdPersonCam ? thirdPersonCamera : camera;
    currCamera->update();

    if (!pause) {
        backpack->update();

        // COLLITION DEBUG
        wall2->update();
        cDetector.update(wall2->getOBB(), 1);
        cDetector.update(tyra->getOBB(), 2);
        // COLLITION DEBUG
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
    delete item;

    // COLLISION DEBUG
    delete wall1;
    delete wall2;
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
    ImGui::End();
}

void Client::timeGUI() {
    ImGuiWindowFlags flags = 0;
    flags |= ImGuiWindowFlags_NoBackground;
    flags |= ImGuiWindowFlags_NoTitleBar;
    //ImGui::Begin("Time GUI", NULL, flags);
    ImVec4* colors = ImGui::GetStyle().Colors;

    ImGui::Begin("Time GUI");
    
    //ImGui::Image(0, )
    //colors[ImGuiCol_Text] = ImVec4(0.5f, 0.0f, 0.5f, 0.95f);
    //ImGui::StyleColorsDark();
    ImGui::Text("Game time counter: %d", oldTime);
    ImGui::End();
    //ImGui::SetNextWindowPos
}

MovementState Client::getMovementState() {
    return MovementState{
        direction,
        keyHeld
    };
}

RotationState Client::getRotationState() {
    return RotationState{
        currRotationUpdate,
    };
}

Model** Client::getPlayers() {
    return players;
}

void Client::resetRotUpdate() {
    currRotationUpdate = glm::mat4(1);
}

void Client::setPlayerfromID(unsigned int id) {
    my_id = id;
    player = players[my_id];
    thirdPersonCamera = new ThirdPersonCamera(player);
}

void Client::updateItemLocation(glm::mat4 location) {
    item->setModel(location);
}

void Client::updateTime(int t) {
      oldTime = t;
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
        if (abs(xPos - prevXPos) > 0.00001 || abs(yPos - prevYPos) > 0.0001) {
            double yawAngle = -0.5 * (xPos - prevXPos);
            double pitchAngle = -0.5 * (yPos - prevYPos);
           
            float playerSpinDegree = 0.5 * (float)(thirdPersonCamera->upVec.y > 0 ? yawAngle : -yawAngle);
            currRotationUpdate = glm::rotate(glm::radians(playerSpinDegree), glm::vec3(0.0f, 1.0f, 0.0f));
            thirdPersonCamera->pitch((float)pitchAngle);

            prevXPos = xPos;
            prevYPos = yPos;
        }
    } else {
        if (middlePressed) {
            if (abs(xPos - prevXPos) > 0.00001 || abs(yPos - prevYPos) > 0.0001) {
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
            currRotationUpdate = glm::rotate(glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            thirdPersonCamera->reset();
            //camera->move(glm::vec3(0.5, 0, 0));
            break;

        case GLFW_KEY_LEFT:
            currRotationUpdate = glm::rotate(glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            thirdPersonCamera->reset();
            //camera->move(glm::vec3(-0.5, 0, 0));
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
