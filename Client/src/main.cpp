#include "main.h"


ClientGame* client;

int main(int argc, char** argv) {
    // Setup audio
    CAudioEngine::Init();

    // Setup glfw
    glfwSetErrorCallback(Client::errorCallback);
    if (!glfwInit()) {
        exit(EXIT_FAILURE);
    }

    // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    const char* glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    // GL 3.2 + GLSL 150
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

    // Create window with graphics context
    GLFWwindow* window = Client::createWindow(mode->width, mode->height, "Client");
    if (window == nullptr) {
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Initialize glew
    bool err = glewInit() != GLEW_OK;
    if (err) {
        spdlog::error("Failed to initialize glew loader!");
        exit(EXIT_FAILURE);
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    //ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != NULL);

    // Our state
    bool show_demo_window = false;
    client = new ClientGame();

    Client::setupGLSettings();
    Client::setupCallbacks(window);

    Client::loadGUIFonts();

    
    //////////////////////load gui//////////////////////
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    Client::LoadingGUI(); 

    ImGui::Render();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
    //////////////////////load gui//////////////////////
    
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    if (!Client::initializeClient()) {
        spdlog::error("Client initialization failed!");
        exit(EXIT_FAILURE);
    }

    client->setPlayers(Client::getPlayers());

    float currentFrame = 0;
    float deltaTime = 0;
    float lastFrame = 0;

    // Main loop
    while (!glfwWindowShouldClose(window))
    {

        currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        //printf("currentFrame: %f, lastFrame: %f, deltaTime: %f \n", currentFrame, lastFrame, deltaTime);

        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        glfwPollEvents();
        //glfwWaitEventsTimeout(0.007);

        /////////////////////////////////IMGUI STUFF IGNORE//////////////////////////////////////////////////////////
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        //if (show_demo_window) {
        //    ImGui::ShowDemoWindow(&show_demo_window);
        //}

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
        //{
        //    ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.
        //    //ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
        //    ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state

        //    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
        //        1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        //    ImGui::End();
        //}
        //Client::GUI();
        Client::timeGUI();
        Client::ItemHoldGUI();
        Client::miniMapGUI();
        //Client::finalDestGUI();
        Client::stationaryItemGUI();
        Client::playerSelectGUI();
        Client::GameStartGUI();
        Client::GameOverGUI();
        Client::audioUpdate();

        /////////////////////////////////IMGUI STUFF IGNORE//////////////////////////////////////////////////////////
        if (Client::checkGameStart()) {
            //client->
            client->sendGameStart();
            printf("Sending start game!\n");
        }
        if (Client::checkHideStartScreen()) {
            client->sendHideStart();
        }

        // Replicate player selection changes to client
        if (client->checkPlayerSelection())
            Client::updatePlayerSelection(client->getPlayerSelection());

        // Send out-bound network requests for player selection
        int playerChoice = Client::checkPlayerSelect();
        if (playerChoice >= 0) {
            printf("Sending player choice update %d\n", playerChoice);
            client->sendPlayerSelect(playerChoice);
        }
        int pair1 = Client::checkPair1Count();
        int pair2 = Client::checkPair2Count();
        if (pair1 == 3 || pair2 == 3) {
            client->sendPairCount(pair1, pair2);
        }
        if (pair1 == 3)
            Client::resetPair1();
        if (pair2 == 3)
            Client::resetPair2();
            
        client->update(Client::getMovementState(), Client::getRotationState());

        // Rendering
        ImGui::Render();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Client::idleCallback(deltaTime);
        Client::displayCallback();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);

        // Audio
        CAudioEngine::Update();
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    Client::cleanup();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
