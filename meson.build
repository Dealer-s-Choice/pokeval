project(
  'pokeval',
  'c',
  version: '0.0.1',
  meson_version: '>= 0.56.0',
  default_options: ['warning_level=3', 'c_std=c99'],
)

cc = meson.get_compiler('c')

if cc.get_id() != 'msvc'
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
endif

version_components = meson.project_version().split('.')
conf = configuration_data()
conf.set('POKEVAL_VERSION_MAJOR', version_components[0])
conf.set('POKEVAL_VERSION_MINOR', version_components[1])
conf.set('POKEVAL_VERSION_PATCH', version_components[2])
configure_file(output: 'config.h', configuration: conf)

deckhandler_dep = dependency(
  'deckhandler',
  default_options: 'default_library=static',
)

buildtarget = library(
  meson.project_name(),
  'pokeval.c',
  c_args: '-DPOKEVAL_BUILD',
  dependencies: deckhandler_dep,
  version: meson.project_version(),
  install: not meson.is_subproject(),
)

# How to use in a superproject and other info
# https://mesonbuild.com/Subprojects.html
_compile_args = get_option('default_library') == 'static' ? ['-DPOKEVAL_STATIC'] : []

pokeval_dep = declare_dependency(
  link_with: buildtarget,
  compile_args: _compile_args,
  dependencies: deckhandler_dep,
  include_directories: '.',
)

if not meson.is_subproject()
  pkg = import('pkgconfig')
  pkg.generate(buildtarget, description: 'Library for evaluating poker hands')

  install_headers('pokeval.h')
  subdir('tests')
endif
