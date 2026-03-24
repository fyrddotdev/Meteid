/*******************************************************************************************
 *
 *   GameOver v1 - tool description
 *
 *   MODULE USAGE:
 *       #define GUI_GAMEOVER_IMPLEMENTATION
 *       #include "gui_GameOver.h"
 *
 *       INIT: GuiGameOverState state = InitGuiGameOver();
 *       DRAW: GuiGameOver(&state);
 *
 *   LICENSE: Propietary License
 *
 *   Copyright (c) 2022 . All Rights Reserved.
 *
 *   Unauthorized copying of this file, via any medium is strictly prohibited
 *   This project is proprietary and confidential unless the owner allows
 *   usage in any other form by expresely written permission.
 *
 **********************************************************************************************/

#include "raylib.h"

// WARNING: raygui implementation is expected to be defined before including this header
#undef RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include <string.h> // Required for: strcpy()

#ifndef GUI_GAMEOVER_H
#define GUI_GAMEOVER_H

typedef struct
{
    bool GameOverWBoxActive;

    Rectangle layoutRecs[3];

    // Custom state variables (depend on development software)
    // NOTE: This variables should be added manually if required

} GuiGameOverState;

#ifdef __cplusplus
extern "C"
{ // Prevents name mangling of functions
#endif

    //----------------------------------------------------------------------------------
    // Defines and Macros
    //----------------------------------------------------------------------------------
    //...

    //----------------------------------------------------------------------------------
    // Types and Structures Definition
    //----------------------------------------------------------------------------------
    // ...

    //----------------------------------------------------------------------------------
    // Module Functions Declaration
    //----------------------------------------------------------------------------------
    GuiGameOverState InitGuiGameOver(void);
    void GuiGameOver(GuiGameOverState *state);
    static void ExitMainMenuBtn();

#ifdef __cplusplus
}
#endif

#endif // GUI_GAMEOVER_H

/***********************************************************************************
 *
 *   GUI_GAMEOVER IMPLEMENTATION
 *
 ************************************************************************************/
#if defined(GUI_GAMEOVER_IMPLEMENTATION)

#include "raygui.h"

//----------------------------------------------------------------------------------
// Global Variables Definition
//----------------------------------------------------------------------------------
//...

//----------------------------------------------------------------------------------
// Internal Module Functions Definition
//----------------------------------------------------------------------------------
//...

//----------------------------------------------------------------------------------
// Module Functions Definition
//----------------------------------------------------------------------------------
GuiGameOverState InitGuiGameOver(void)
{
    GuiGameOverState state = {0};

    state.GameOverWBoxActive = true;

    float boxW = 320.0f;
    float boxH = 180.0f;

    state.layoutRecs[0] = Rectangle{
        (GetScreenWidth() - boxW) / 2.0f,
        (GetScreenHeight() - boxH) / 2.0f,
        boxW,
        boxH};

    state.layoutRecs[1] = Rectangle{
        state.layoutRecs[0].x + 10,
        state.layoutRecs[0].y + 40,
        300,
        40};

    state.layoutRecs[2] = Rectangle{
        state.layoutRecs[0].x + (boxW - 200) / 2.0f,
        state.layoutRecs[0].y + 120,
        200,
        40};

    // Custom variables initialization

    return state;
}
static void ExitMainMenuBtn()
{
    // TODO: Implement control logic
}

void GuiGameOver(GuiGameOverState *state)
{
    if (state->GameOverWBoxActive)
    {
        state->GameOverWBoxActive = !GuiWindowBox(state->layoutRecs[0], "YOU DEAD");
        GuiLabel(state->layoutRecs[1], "SCORE : ");
        if (GuiButton(state->layoutRecs[2], "EXIT TO MAIN MENU"))
            ExitMainMenuBtn();
    }
}

#endif // GUI_GAMEOVER_IMPLEMENTATION
