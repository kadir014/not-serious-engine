/*

  This file is a part of the Not Serious Engine
  project and distributed under the GNU GPL v3 license.

  Copyright © Kadir Aksoy
  https://github.com/kadir014/not-serious-engine

*/

#include "engine/include/engine.h"
#include "game/src/scenes/materialdemo.h"


int main(int argc, char **argv) {
    nsLogger *logger = ns_get_logger();
    logger->outs[0] = stdout;

    nsAppDefinition app_def = {
        .window_title = "Not-Serious-Engine App",
        .window_width = 1280,
        .window_height = 720,
        .target_fps = 60,
        .vsync = true
    };
    nsApp *app = nsApp_new(app_def);
    
    nsApp_push_scene(app, &nsMaterialDemoScene);

    nsApp_run(app);

    nsApp_free(app);

    return EXIT_SUCCESS;
}