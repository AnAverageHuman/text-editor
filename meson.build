project('text-editor', 'c')

src_files = files(
  'src/buffer.c',
  'src/client.c',
  'src/networking.c',
  'src/server.c',
  'src/utilities.c',
)

src_main = files(
  'src/main.c'
)

cursesdep = dependency('curses')

inc_include = include_directories('include')

executable('text-editor',
  src_files + src_main,
  dependencies: cursesdep,
  include_directories: inc_include,
)
