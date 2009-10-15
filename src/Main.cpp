#include <SDL.h>
#include "imCommon.h"
#include "scene/imSceneDatabase.h"

#ifdef WIN32
#  include <windows.h>
#  include <direct.h>
#endif

imString s_rootPath;
imVfs s_vfs;

#ifdef WIN32
  int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                     LPSTR lpCmdLine, int nCmdShow)
#else
  int main(int argc, char* argv[])
#endif
{
    /* Initialize SDL */
    SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO);

#ifdef WIN32
    s_logFile = fopen("imaginaryMyst.log", "wb");

    char pathbuf[MAX_PATH];
    s_rootPath = _getcwd(pathbuf, MAX_PATH);
#else
    s_logFile = stderr;
    s_rootPath = path_pathname(argv[0]);
#endif

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
    imStream* stream = s_vfs.open("/scn/myst.idx");
    if (stream == 0) {
        imLog("Fatal: Cannot open Scene Database!");
        return 1;
    }
    sdb.readIndex(stream);
    delete stream;

    // Create a window for the game
    SDL_Surface* window = SDL_SetVideoMode(640, 480, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    SDL_Delay(1000);
    SDL_FreeSurface(window);

    SDL_Quit();

    return 0;
}
