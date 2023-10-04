// Day 1: Dream
#include <nds.h>
#include <stdio.h>
#include <filesystem.h>
#include <nf_lib.h>


int main() {
    // Initialize NitroFS and set it as the root folder of the filesystem
    nitroFSInit(NULL);
    NF_SetRootFolder("NITROFS");

    // Setup 
    NF_Set2D(0, 0);
    NF_Set2D(1, 0);

    // Initialize tiled backgrounds system
    NF_InitTiledBgBuffers();    // Initialize storage buffers
    NF_InitTiledBgSys(0);       // Top screen
    NF_InitTiledBgSys(1);       // Bottom screen

    // Load background
    NF_LoadTiledBg("bg", "bg", 256, 256);

    // Create on both screens on layer 0
    NF_CreateTiledBg(0, 0, "bg");
    NF_CreateTiledBg(1, 0, "bg");

    
    int frame = 0;

    while (true)
    {

        swiWaitForVBlank();
        frame++;
    }
}