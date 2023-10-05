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


struct Eye {
    int oam_id;

    int x;
    int y;

    // Current pupil offset and target position to animate to 
    float x_pupil_offset, x_pupil_offset_to;
    float y_pupil_offset, y_pupil_offset_to;
    int t_until_new_looking_offset;
};


std::vector<Eye> eyes;





Eye make_eye(int x, int y) {
    return Eye {
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

    // Load eye sprites
    NF_LoadSpriteGfx("pupil", 0, 16, 16);
    NF_LoadSpritePal("pupil", PAL);
    NF_VramSpriteGfx(0, 0, 0, false);
    NF_VramSpritePal(0, 0, PAL);


    // Add the eyes
    eyes.push_back(make_eye(100, 100));
    eyes.push_back(make_eye(102, 40));
    eyes.push_back(make_eye(71, 72));
    eyes.push_back(make_eye(84, 56));
    eyes.push_back(make_eye(101, 72));
    eyes.push_back(make_eye(87, 91));
    eyes.push_back(make_eye(120, 55));
    eyes.push_back(make_eye(120, 88));
    eyes.push_back(make_eye(137, 71));

    // Create the sprites of the eyes
    for (unsigned int i = 0; i < eyes.size(); i++) {
        Eye eye = eyes[i];
        NF_CreateSprite(0, eye.oam_id, 0, PAL, eye.x, eye.y);
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

        // Move eyes
        for (unsigned int i = 0; i < eyes.size(); i++) {
            Eye eye = eyes[i];

            // Move the eye
            NF_MoveSprite(0, eye.oam_id, eye.x + 6   + eye.x_pupil_offset, eye.y - y + 8 + eye.y_pupil_offset);

            // Smoothly move the pupil
            eye.x_pupil_offset = leerp(eye.x_pupil_offset_to, eye.x_pupil_offset, 0.9);
            eye.y_pupil_offset = leerp(eye.x_pupil_offset_to, eye.y_pupil_offset, 0.9);

            // Update the looking offset
            eye.t_until_new_looking_offset--;
            if (eye.t_until_new_looking_offset <= 0) {
                eye.x_pupil_offset_to = Random::get(-2, 2);
                eye.y_pupil_offset_to = Random::get(-2, 2);
                eye.t_until_new_looking_offset = Random::get(50, 100);
            }


            // Update the eye
            eyes[i] = eye;
        }


        // Update sprites
        NF_SpriteOamSet(0);
        oamUpdate(&oamMain);

        swiWaitForVBlank();
        frame++;
    }
}