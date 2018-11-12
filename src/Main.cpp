/* This file is part of imaginaryMyst.
 *
 * imaginaryMyst is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * imaginaryMyst is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with imaginaryMyst.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <GL/glew.h>
#include <iostream>
#include <GL/glu.h>
#include <graphics/imPipeline.h>
#include "imCommon.h"
#include "scene/imSceneDatabase.h"
#include "scene/imSceneIndex.h"
#include "surface/imMipmap.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#ifdef WIN32
#  include <windows.h>
#  include <direct.h>
#endif

static void openglCallbackFunction(
        GLenum source,
        GLenum type,
        GLuint id,
        GLenum severity,
        GLsizei length,
        const GLchar *message,
        const void *userParam
) {
    // ignore non-significant error/warning codes
    if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

    std::cout << "---------------" << std::endl;
    std::cout << "Debug message (" << id << "): " << message << std::endl;

    switch (source) {
        case GL_DEBUG_SOURCE_API:
            std::cout << "Source: API";
            break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
            std::cout << "Source: Window System";
            break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER:
            std::cout << "Source: Shader Compiler";
            break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:
            std::cout << "Source: Third Party";
            break;
        case GL_DEBUG_SOURCE_APPLICATION:
            std::cout << "Source: Application";
            break;
        case GL_DEBUG_SOURCE_OTHER:
            std::cout << "Source: Other";
            break;
    }
    std::cout << std::endl;

    switch (type) {
        case GL_DEBUG_TYPE_ERROR:
            std::cout << "Type: Error";
            break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
            std::cout << "Type: Deprecated Behaviour";
            break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
            std::cout << "Type: Undefined Behaviour";
            break;
        case GL_DEBUG_TYPE_PORTABILITY:
            std::cout << "Type: Portability";
            break;
        case GL_DEBUG_TYPE_PERFORMANCE:
            std::cout << "Type: Performance";
            break;
        case GL_DEBUG_TYPE_MARKER:
            std::cout << "Type: Marker";
            break;
        case GL_DEBUG_TYPE_PUSH_GROUP:
            std::cout << "Type: Push Group";
            break;
        case GL_DEBUG_TYPE_POP_GROUP:
            std::cout << "Type: Pop Group";
            break;
        case GL_DEBUG_TYPE_OTHER:
            std::cout << "Type: Other";
            break;
    }
    std::cout << std::endl;

    switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH:
            std::cout << "Severity: high";
            break;
        case GL_DEBUG_SEVERITY_MEDIUM:
            std::cout << "Severity: medium";
            break;
        case GL_DEBUG_SEVERITY_LOW:
            std::cout << "Severity: low";
            break;
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            std::cout << "Severity: notification";
            break;
    }
    std::cout << std::endl;
    std::cout << std::endl;
}

ST::string s_rootPath;

imVfs s_vfs;
SDL_Window *s_display;
SDL_GLContext s_glContext
FILE *s_logFile = stderr;

PFNGLCOMPRESSEDTEXIMAGE2DARBPROC GLX_CompressedTexImage2D = 0;

void cleanup() {
    SDL_GL_DeleteContext(s_glContext);
    SDL_DestroyWindow(s_display);

    SDL_Quit();
}

#ifdef WIN32
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nCmdShow)
#else

int main(int argc, char *argv[])
#endif
{
    /* Initialize SDL */
    SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(
            SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG
    );

#ifdef WIN32
    s_logFile = fopen("imaginaryMyst.log", "wb");

    char pathbuf[MAX_PATH];
    s_rootPath = _getcwd(pathbuf, MAX_PATH);
#else
    s_rootPath = path_pathname(argv[0]);
    if (s_rootPath.empty())
        s_rootPath = ".";
#endif

    // Load the game settings
    int winWidth = 640, winHeight = 480;
    imFileStream settings;
    if (settings.open(s_rootPath + "/imaginaryMyst.cfg", "rt")) {
        while (!settings.eof()) {
            ST::string ln = settings.readLine();
            ST_ssize_t commentPos = ln.find('#');
            if (commentPos != -1)
                ln = ln.left(commentPos);
            ln = ln.trim();
            if (ln.empty()) {
                continue;
            } else if (ln == "NoDXT") {
                imMipmap::s_noDXTCompression = true;
            } else if (ln.left(5) == "Width") {
                ln = ln.substr(6).trim();
                winWidth = ln.to_int();
            } else if (ln.left(6) == "Height") {
                ln = ln.substr(7).trim();
                winHeight = ln.to_int();
            } else {
                ST_ssize_t cmdPos = ln.find(' ');
                if (cmdPos != -1)
                    ln = ln.left(cmdPos);
                imLog("Unrecognized option in config: {}", ln);
            }
        }
    } else {
        imLog("WARN: No imaginaryMyst.cfg found, using default settings!");
    }

    // Load the .dni files and local filesystem into the VFS
    imLog("Indexing Local Resources... ");

    if (!file_exists(s_rootPath + "/sho.dni")) {
        imLog("Fatal: Cannot read sho.dni!");
        return 1;
    }
    s_vfs.addDniFile(s_rootPath + "/sho.dni");

    if (!file_exists(s_rootPath + "/mara.dni")) {
        imLog("Fatal: Cannot read mara.dni!");
        return 1;
    }
    s_vfs.addDniFile(s_rootPath + "/mara.dni");

    if (!file_exists(s_rootPath + "/pera.dni")) {
        imLog("Fatal: Cannot read pera.dni!");
        return 1;
    }
    s_vfs.addDniFile(s_rootPath + "/pera.dni");

    // Add the game dir last, so duplicates use the physical copy
    s_vfs.addPhysicalPath(s_rootPath);

    imLog("Reading Scene Database...");
    imSceneDatabase sdb;
    imStream *stream = s_vfs.open("/scn/myst.idx");
    if (stream == 0) {
        imLog("Fatal: Cannot open Scene Database!");
        return 1;
    }
    if (!sdb.readIndex(stream)) {
        delete stream;
        imLog("Fatal: Error reading Scene Database!");
        return 1;
    }
    delete stream;

    std::vector<unsigned int> roomKeys = sdb.room("open");
    std::vector<unsigned int>::iterator k;
    for (k = roomKeys.begin(); k != roomKeys.end(); ++k) {
        stream = s_vfs.openSdb(*k);
        if (stream == 0) {
            imLog("Error opening SDB {}", *k);
            continue;
        }
        imLog("DEBUG: Reading SDB {}...", *k);
        imSceneIndex scn;
        if (!scn.read(stream)) {
            imLog("Error reading SDB {}", *k);
            imLog("But we got to {08X}", stream->tell());
        }
        delete stream;
    }


    // Create a window for the game
<<<<<<< HEAD
    s_display = SDL_CreateWindow("imaginaryMyst Alpha", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, winWidth,
                                 winHeight, SDL_WINDOW_OPENGL);


    SDL_GLContext glContext = SDL_GL_CreateContext(s_display);
=======
    s_display = SDL_CreateWindow("imaginaryMyst Alpha",
                                 SDL_WINDOWPOS_CENTERED,
                                 SDL_WINDOWPOS_CENTERED,
                                 winWidth, winHeight,
                                 SDL_WINDOW_OPENGL);
    s_glContext = SDL_GL_CreateContext(s_display);

>>>>>>> CHeck for opengl version, minimum require 3, and only enable debugging when supported
    glewExperimental = true;
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return -1;
    }
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

    int glMajorVersion;
    int glMinorVersion;
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &glMajorVersion);
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &glMinorVersion);

    if (glMajorVersion < 3) {
        fprintf(stderr,
                "ERROR OoenGL version too low, only OpenGL 3 or higher are supported. Opengl version detected %d.%d.",
                glMajorVersion, glMinorVersion);
        cleanup();
        return 1;
    }
    if (glMajorVersion > 4 || (glMajorVersion == 4 && glMinorVersion >= 3)) {
        glDebugMessageCallback(openglCallbackFunction, nullptr);
        glDebugMessageControl(
                GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, true
        );
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glewExperimental = true;
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return -1;
    }
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(openglCallbackFunction, nullptr);
    glDebugMessageControl(
            GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, true
    );

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    GLX_CompressedTexImage2D = (PFNGLCOMPRESSEDTEXIMAGE2DARBPROC)
            SDL_GL_GetProcAddress("glCompressedTexImage2DARB");
    if (GLX_CompressedTexImage2D == 0)
        imMipmap::s_noDXTCompression = true;

    imLog("DEBUG: Reading CreditsScene03Top.hsm...");
    imMipmap img;
    stream = s_vfs.open("/scn/maps/CreditsScene03Top.hsm");
    if (stream != 0) {
        img.read(stream);
        delete stream;
    } else {
        imLog("Error reading HSM file");
    }
    img.TEST_ExportDDS("test1.dds");
    imPipeline pipeline;
    img.prepare();
    pipeline.initialize();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glTranslatef(0.0f, 0.0f, -4.0f);
    img.bind();
    pipeline.render();


    SDL_GL_SwapWindow(s_display);
    SDL_Delay(3000);
cleanup();
    return 0;
}
