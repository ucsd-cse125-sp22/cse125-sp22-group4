#include "Shader.h"

enum class ShaderType { vertex, fragment };

/**
 * Load a single shader file of given shader type
 *
 * @param   shaderFilePath    Path to the shader file
 * @param   type              Type of the shader file
 * @return  Shader ID
**/
static GLuint loadSingleShader(const char* shaderFilePath, ShaderType type) {
    // create a shader id.
    GLuint shaderID = 0;
    if (type == ShaderType::vertex) {
        shaderID = glCreateShader(GL_VERTEX_SHADER);
    } else if (type == ShaderType::fragment) {
        shaderID = glCreateShader(GL_FRAGMENT_SHADER);
    }

    // try to read shader codes from the shader file.
    std::string shaderCode;
    std::ifstream shaderStream(shaderFilePath, std::ios::in);
    if (shaderStream.is_open()) {
        std::string Line = "";
        while (getline(shaderStream, Line)) {
            shaderCode += "\n" + Line;
        }
        shaderStream.close();
    } else {
        spdlog::error("Cannot open {}!", shaderFilePath);
        return 0;
    }

    GLint Result = GL_FALSE;
    int InfoLogLength;

    // compile Shader.
    spdlog::info("Compiling {}...", shaderFilePath);
    char const * sourcePointer = shaderCode.c_str();
    glShaderSource(shaderID, 1, &sourcePointer, NULL);
    glCompileShader(shaderID);

    // check Shader.
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);

    // log success or failure
    if (InfoLogLength > 0) {
        std::vector<char> shaderErrorMessage(InfoLogLength + 1);
        glGetShaderInfoLog(shaderID, InfoLogLength, NULL, shaderErrorMessage.data());
        std::string msg(shaderErrorMessage.begin(), shaderErrorMessage.end());
        spdlog::error("Cannot compile {}!", shaderFilePath);
        spdlog::error(msg);
        return 0;
    } else {
        if (type == ShaderType::vertex) {
            spdlog::info("Successfully compiled vertex shader.");
        }
        else if (type == ShaderType::fragment) {
            spdlog::info("Successfully compiled fragment shader.");
        }
    }
    return shaderID;
}

/**
 * Load vertex and fragment shaders
 *
 * @param   vertexFilePath    Path of vertex shader
 * @param   fragmentFilePath  Path of fragment shader
 * @return  Shader program ID
**/
GLuint Shader::loadShaders(const char* vertexFilePath, const char* fragmentFilePath)
{
    spdlog::info("Loading shaders...");
    // create the vertex shader and fragment shader.
    GLuint vertexShaderID = loadSingleShader(vertexFilePath, ShaderType::vertex);
    GLuint fragmentShaderID = loadSingleShader(fragmentFilePath, ShaderType::fragment);

    // check both shaders.
    if (vertexShaderID == 0 || fragmentShaderID == 0) {
        return 0;
    }

    GLint Result = GL_FALSE;
    int InfoLogLength;

    // link the program.
    spdlog::info("Linking the program...");
    GLuint programID = glCreateProgram();
    glAttachShader(programID, vertexShaderID);
    glAttachShader(programID, fragmentShaderID);
    glLinkProgram(programID);

    // check the program.
    glGetProgramiv(programID, GL_LINK_STATUS, &Result);
    glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0) {
        std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
        glGetProgramInfoLog(programID, InfoLogLength, NULL, ProgramErrorMessage.data());
        std::string msg(ProgramErrorMessage.begin(), ProgramErrorMessage.end());
        std::cerr << msg << std::endl;
        spdlog::error("Cannot link the program!");
        spdlog::error(msg);
        glDeleteProgram(programID);
        return 0;
    } else {
        spdlog::info("Successfully linked the program.");
    }

    // detach and delete the shaders as they are no longer needed.
    glDetachShader(programID, vertexShaderID);
    glDetachShader(programID, fragmentShaderID);
    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);

    return programID;
}
