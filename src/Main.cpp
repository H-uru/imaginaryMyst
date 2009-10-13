#include "vfs/imVfs.h"
#include <cstdio>

void hex_dump(imStream* S)
{
    int count = 0;
    while (!S->eof() && count < 64) {
        unsigned char ch = S->readByte();
        printf("%02X ", ch);
        if ((++count % 16) == 0)
            printf("\n");
        else if ((count % 8) == 0)
            printf(" ");
    }
    printf("\n");
}

int main(int argc, char* argv[])
{
    imVfs vfs;
    fprintf(stderr, "Indexing Local Resources... ");
    vfs.addPhysicalPath("/media/Shared/Games/realMYST");
    vfs.addDniFile("/media/Shared/Games/realMYST/sho.dni");
    vfs.addDniFile("/media/Shared/Games/realMYST/mara.dni");
    vfs.addDniFile("/media/Shared/Games/realMYST/pera.dni");
    fprintf(stderr, "Done!\n");

    imStream* S = vfs.open("/SHO.dni");
    if (S != 0) {
        printf("[sho.dni]\n");
        hex_dump(S);
        delete S;
    }

    S = vfs.open("/wav/atrus ee sound.wav");
    if (S != 0) {
        printf("[Atrus EE Sound.wav]\n");
        hex_dump(S);
        delete S;
    }

    S = vfs.open("/BEH/fish.beh");
    if (S != 0) {
        printf("[fish.beh]\n");
        hex_dump(S);
        delete S;
    }

    return 0;
}
