// Day 1: Dream
#include <nds.h>
#include <stdio.h>
#include <filesystem.h>
#include <nf_lib.h>
#include <math.h>
#include <vector>   
#include "effolkronium/random.hpp"

// Better random function
using Random = effolkronium::random_static;


int current_oam_id;
const int PAL = 0;


struct Pupil {
    int oam_id;

    int x;
    int y;

    // Current pupil offset and target position to animate to 
    float x_pupil_offset, x_pupil_offset_to;
    float y_pupil_offset, y_pupil_offset_to;
    int t_until_new_looking_offset;
};


std::vector<Pupil> pupils;





Pupil make_pupil(int x, int y) {
    return Pupil {
        .oam_id = current_oam_id++,
        .x = x,
        .y = y,
        .x_pupil_offset = 0.0,
        .x_pupil_offset_to = 0.0,
        .y_pupil_offset = 0.0,
        .y_pupil_offset_to = 0.0,
        .t_until_new_looking_offset = 0
    };
}


// Utils
float leerp (float a, float b, float t) {
    return a + (b - a) * t;
}



// Main
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

    // Load backgrounds
    NF_LoadTiledBg("bg", "bg", 256, 256);
    NF_LoadTiledBg("bgbottom", "bgbottom", 256, 256);
    NF_LoadTiledBg("clip", "clip", 256, 256);

    // Init sprite system
    NF_InitSpriteBuffers();
    NF_InitSpriteSys(0);

    // Load pupil sprites
    NF_LoadSpriteGfx("pupil", 0, 16, 16);
    NF_LoadSpritePal("pupil", PAL);
    NF_VramSpriteGfx(0, 0, 0, false);
    NF_VramSpritePal(0, 0, PAL);


    // Add the pupils
    pupils.push_back(make_pupil(100, 100));
    pupils.push_back(make_pupil(102, 40));
    pupils.push_back(make_pupil(71, 72));
    pupils.push_back(make_pupil(84, 56));
    pupils.push_back(make_pupil(101, 72));
    pupils.push_back(make_pupil(87, 91));
    pupils.push_back(make_pupil(120, 55));
    pupils.push_back(make_pupil(120, 88));
    pupils.push_back(make_pupil(137, 71));

    // Create the sprites of the pupils
    for (unsigned int i = 0; i < pupils.size(); i++) {
        Pupil pupil = pupils[i];
        NF_CreateSprite(0, pupil.oam_id, 0, PAL, pupil.x, pupil.y);
    }

    // Create on both screens on layer 0
    NF_CreateTiledBg(0, 1, "bg");
    NF_CreateTiledBg(1, 0, "bgbottom");
    NF_CreateTiledBg(0, 0, "clip"); // clippy base

    int frame = 0;

    while (true)
    {
        // Move clippy
        float y = sin((float) frame / 50.0) * 10.0;
        NF_ScrollBg(0, 0, 0, y);

        // Move pupils
        for (unsigned int i = 0; i < pupils.size(); i++) {
            Pupil pupil = pupils[i];

            // Move the pupil
            NF_MoveSprite(0, pupil.oam_id, pupil.x + 6   + pupil.x_pupil_offset, pupil.y - y + 8 + pupil.y_pupil_offset);

            // Smoothly move the pupil
            pupil.x_pupil_offset = leerp(pupil.x_pupil_offset_to, pupil.x_pupil_offset, 0.9);
            pupil.y_pupil_offset = leerp(pupil.x_pupil_offset_to, pupil.y_pupil_offset, 0.9);

            // Update the looking offset
            pupil.t_until_new_looking_offset--;
            if (pupil.t_until_new_looking_offset <= 0) {
                pupil.x_pupil_offset_to = Random::get(-2, 2);
                pupil.y_pupil_offset_to = Random::get(-2, 2);
                pupil.t_until_new_looking_offset = Random::get(50, 100);
            }


            // Update the pupil
            pupils[i] = pupil;
        }


        // Update sprites
        NF_SpriteOamSet(0);
        oamUpdate(&oamMain);

        swiWaitForVBlank();
        frame++;
    }
}