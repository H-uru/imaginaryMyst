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

#include <iostream>
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

ST::string s_rootPath;
imVfs s_vfs;
SDL_Window *s_display;

FILE *s_logFile = stderr;

PFNGLCOMPRESSEDTEXIMAGE2DARBPROC GLX_CompressedTexImage2D = 0;

#ifdef WIN32
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nCmdShow)
#else
int main(int argc, char *argv[])
#endif
{
    /* Initialize SDL */
    SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO);

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
    s_display = SDL_CreateWindow("imaginaryMyst Alpha",
                          SDL_WINDOWPOS_CENTERED,
                          SDL_WINDOWPOS_CENTERED,
                          winWidth, winHeight,
                          SDL_WINDOW_OPENGL);
    SDL_GLContext glContext = SDL_GL_CreateContext(s_display);
    glm::mat4 projection = glm::perspective(
      45.0f,                                // FoV
      (float)winWidth / (float)winHeight,   // Aspect Ratio
      0.1f,                                 // Near Plane
      10000.0f);                            // Far Plane

    GLX_CompressedTexImage2D = (PFNGLCOMPRESSEDTEXIMAGE2DARBPROC)
            SDL_GL_GetProcAddress("glCompressedTexImage2DARB");
    if (GLX_CompressedTexImage2D == 0)
        imMipmap::s_noDXTCompression = true;

    glShadeModel(GL_SMOOTH);
    glClearDepth(1.0f);
    glClearColor(0.0f, 0.5f, 0.5f, 1.0f);
    glViewport(0, 0, winWidth, winHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glLoadMatrixf(glm::value_ptr(projection));
    glEnable(GL_TEXTURE_2D);
    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);


    imLog("DEBUG: Reading CreditsScene03Top.hsm...");
    imMipmap img;
    stream = s_vfs.open("/scn/maps/CreditsScene03Top.hsm");
    if (stream != 0) {
        img.read(stream);
        delete stream;
        img.prepare();
    } else {
        imLog("Error reading HSM file");
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glTranslatef(0.0f, 0.0f, -4.0f);
    img.bind();

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-1.0f, 1.0f, 0.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(1.0f, 1.0f, 0.0f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 0.0f);
    glEnd();

    SDL_GL_SwapWindow(s_display);
    SDL_Delay(3000);
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(s_display);

    SDL_Quit();
    return 0;
}
