project(
    'not-so-serious-engine',
    'c',
    license: 'GNU GPL v3',
    license_files: 'LICENSE',
    default_options: [
        'c_std=c99',
        'warning_level=1',
        'default_library=static'
    ]
)


compiler = meson.get_compiler('c')

c_args = []
link_args = []

if compiler.get_id() == 'msvc'
    c_args += ['/arch:AVX2', '/D_CRT_SECURE_NO_WARNINGS']
else
    c_args += '-march=native'
    link_args += '-lm'

    # When you target C99, you also have to specify POSIX clock
    # https://raspberrypi.stackexchange.com/a/95480
    c_args += '-D_POSIX_C_SOURCE=200809L'
endif


python = find_program('python3', 'python')

r = run_command(python, 'scripts/install_wraps.py')
if r.returncode() != 0
    error('install_wraps.py script failed.\n', r.stderr())
endif


sdl2_dep = dependency('sdl2')
sdl2_image_dep = dependency('sdl2_image')
opengl_dep = dependency('gl')

if compiler.get_id() == 'msvc'
    sdl2_main_dep = dependency('sdl2main')
    deps = [sdl2_dep, sdl2_main_dep, sdl2_image_dep, opengl_dep]
else
    deps = [sdl2_dep, sdl2_image_dep, opengl_dep]
endif


engine_src = [
    'engine/src/core/error.c',
    'engine/src/core/io.c',
    'engine/src/core/array.c',
    'engine/src/core/pool.c',
    'engine/src/graphics/material.c',
    'engine/src/graphics/mesh.c',
    'engine/src/graphics/buffer.c',
    'engine/src/graphics/uniform.c',
    'engine/src/graphics/texture.c',
    'engine/src/model/model.c',
    'engine/src/loaders/obj.c',
    'engine/src/app/app.c'
]

external_src = [
    'external/glad/gl.c'
]

engine_includes = ['engine/include', 'external']

libnsengine = library(
    'nsengine',
    sources: engine_src + external_src,
    include_directories: engine_includes,
    c_args: c_args,
    link_args: link_args,
    dependencies: deps,
    version: '0.0.0',
    install: true
)


game_src = [
    'game/src/main.c',
]
game_includes = engine_includes

executable(
    'game',
    sources: game_src,
    include_directories: game_includes,
    c_args: c_args,
    link_args: link_args,
    dependencies: deps,
    link_with: libnsengine
)