/*

  This file is a part of the Not Serious Engine
  project and distributed under the GNU GPL v3 license.

  Copyright © Kadir Aksoy
  https://github.com/kadir014/not-serious-engine

*/

#include "engine/include/engine.h"


int main(int argc, char **argv) {
    nsAppDefinition app_def = {
        .window_title = "Not-Serious-Engine App",
        .window_width = 1280,
        .window_height = 720,
        .target_fps = 60,
        .vsync = true
    };
    nsApp *app = nsApp_new(app_def);

    nsApp_run(app);

    nsApp_free(app);

    printf("Latest error before exiting:\n");
    printf("Code: %u\n", ns_get_error().code);
    printf("Severity: %u\n", ns_get_error().severity);
    printf("Message: %s\n", ns_get_error().message);

    return EXIT_SUCCESS;
}