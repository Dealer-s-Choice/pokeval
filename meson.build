project(
  'pokeval',
  'c',
  version: '0.1.0999',
  meson_version: '>= 0.56.0',
  default_options: ['warning_level=3', 'c_std=c99'],
)

cc = meson.get_compiler('c')

extra_flags = [
  '-fno-common',
  '-fstack-protector-strong',
  '-Wformat-security',
  '-Wshadow',
  '-Wstrict-overflow=5',
  '-Werror=odr',
  '-Werror=strict-aliasing',
]

add_project_arguments(cc.get_supported_arguments(extra_flags), language: 'c')

deckhandler_dep = dependency(
  'deckhandler',
  default_options: 'default_library=static',
)

buildtarget = library(
  meson.project_name(),
  'pokeval.c',
  dependencies: deckhandler_dep,
  version: meson.project_version(),
  install: not meson.is_subproject(),
)

# How to use in a superproject and other info
# https://mesonbuild.com/Subprojects.html
pokeval_dep = declare_dependency(
  link_with: buildtarget,
  dependencies: deckhandler_dep,
  include_directories: '.',
)

if not meson.is_subproject()
  pkg = import('pkgconfig')
  pkg.generate(buildtarget, description: 'Library for evaluating poker hands')

  install_headers('pokeval.h')
  subdir('tests')
endif
