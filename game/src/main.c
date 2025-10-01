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

    printf("%s\n", ns_get_error().message);

    return EXIT_SUCCESS;
}