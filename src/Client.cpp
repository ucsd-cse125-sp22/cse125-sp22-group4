#include "Client.h"
#include "Model.h"

// shader, camera and light
static GLuint shader;
static Camera* camera;
static int lightCount;
static std::vector<glm::vec4> lightPosn;
static std::vector<glm::vec4> lightColorn;

// objects
static Cube* cube;
static ObjectLoader* teapot;
static ObjectLoader* bunny;
static ObjectLoader* tyra;
static ObjectLoader* suzanne;
static Model* ourModel;

// state variables
static bool pause = false;
static bool middlePressed = false;
static double prevXPos;
static double prevYPos;

// callbacks
static void resizeCallback(GLFWwindow* window, int width, int height);
static void cursorCallback(GLFWwindow* window, double xPos, double yPos);
static void scrollCallback(GLFWwindow* window, double xMove, double yMove);
static void mouseCallback(GLFWwindow* window, int button, int action, int mods);
static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

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
    //shader = Shader::loadShaders("shaders/basicShader.vert", "shaders/basicShader.frag");
    //shader = Shader::loadShaders("shaders/normalShader.vert", "shaders/normalShader.frag");
    shader = Shader::loadShaders("shaders/shader.vert", "shaders/shader.frag");
    if (!shader) {
        spdlog::error("Failed to initialize shader programs.");
        return false;
    }

    // initialize camera
    camera = new Camera();
    // initialize light sources
    lightCount = 2;
    lightPosn = { {0, 5, -10, 1}, {0, 5, 10, 1} };
    lightColorn = { {0.9, 0.6, 0, 1}, {0, 0.6, 0.9, 1} };

    // initialize objects
    cube = new Cube();
    teapot = new ObjectLoader("objects/teapot.obj");
    bunny = new ObjectLoader("objects/bunny.obj");
    tyra = new ObjectLoader("objects/tyra.obj");
    suzanne = new ObjectLoader("objects/suzanne.obj");
    ourModel = new Model("objects/backpack.obj");

    return true;
}

/**
 * Display objects
**/
void Client::displayCallback() {
    //glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // activate the shader program and send some values
    glUseProgram(shader);
    glUniform3fv(glGetUniformLocation(shader, "eyePos"), 1, glm::value_ptr(camera->pos));
    glUniform1i(glGetUniformLocation(shader, "lightCount"), lightCount);
    glUniform4fv(glGetUniformLocation(shader, "lightPosn"), lightCount, (float*)lightPosn.data());
    glUniform4fv(glGetUniformLocation(shader, "lightColorn"), lightCount, (float*)lightColorn.data());
   // glUseProgram(0);

    glm::mat4 model = glm::mat4(1.0f);
    glUniformMatrix4fv(glGetUniformLocation(shader, "viewProj"), 1, false, glm::value_ptr(camera->viewProjMat));
    glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, false, glm::value_ptr(model));
    //cube->draw(camera->viewProjMat, shader);
    //teapot->draw(camera->viewProjMat, shader);
    //tyra->draw(camera->viewProjMat, shader);
    //bunny->draw(camera->viewProjMat, shader);
    ourModel->Draw(shader);
}

/**
 * Update objects when idle
**/
void Client::idleCallback() {
    camera->update();
    if (!pause) {
        //cube->update();
        //teapot->update();
       // tyra->update();
        //bunny->update();
    }
}

/**
 * Cleanup the client, free all memories
**/
void Client::cleanup() {
    glDeleteProgram(shader);
    delete cube;
    delete camera;
    delete teapot;
    delete bunny;
    delete tyra;
    delete suzanne;
    delete ourModel;
}

/**
 * Client GUI
**/
void Client::GUI() {
    ImGui::Begin("Client GUI");
    ImGui::Checkbox("Pause", &pause);
    ImGui::End();
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
    if (middlePressed) {
        if (abs(xPos - prevXPos) > 0.00001 || abs(yPos - prevYPos) > 0.0001) {
            double yawAngle = -0.005 * (xPos - prevXPos);
            double pitchAngle = -0.005 * (yPos - prevYPos);
            camera->yaw((float)yawAngle);
            camera->pitch((float)pitchAngle);
        }
        prevXPos = xPos;
        prevYPos = yPos;
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
    if (yMove > 0) {
        camera->zoom(0.9f);
    }
    else if (yMove < 0) {
        camera->zoom(1.1f);
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
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        switch (key) {
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window, GL_TRUE);
            break;

        case GLFW_KEY_SPACE:
            pause = !pause;
            break;

        case GLFW_KEY_R:
            camera->reset();
            break;

        case GLFW_KEY_W:
            camera->move(glm::vec3(0, 0, -0.5));
            break;

        case GLFW_KEY_S:
            camera->move(glm::vec3(0, 0, 0.5));
            break;

        case GLFW_KEY_A:
            camera->move(glm::vec3(-0.5, 0, 0));
            break;

        case GLFW_KEY_D:
            camera->move(glm::vec3(0.5, 0, 0));
            break;

        case GLFW_KEY_LEFT_SHIFT:
            camera->move(glm::vec3(0, 0.5, 0));
            break;

        case GLFW_KEY_RIGHT_SHIFT:
            camera->move(glm::vec3(0, 0.5, 0));
            break;

        case GLFW_KEY_LEFT_CONTROL:
            camera->move(glm::vec3(0, -0.5, 0));
            break;

        case GLFW_KEY_RIGHT_CONTROL:
            camera->move(glm::vec3(0, -0.5, 0));
            break;
        }
    }
}

