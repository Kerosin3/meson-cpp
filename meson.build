project('bulk', 'cpp', 
  meson_version: '>=0.58.0',
  default_options: [
    'cpp_std=c++20',
    'build.cpp_std=c++20',
    'warning_level=2',
    'force_fallback_for=wlroots,libliftoff,vkroots',
  ],
  version : '1.0.0',
  license : 'MIT')

cppc = meson.get_compiler('cpp')
add_project_arguments(cppc.get_supported_arguments([
  '-Wno-unused-parameter',
  '-Wno-missing-field-initializers',
  '-Wno-c99-designator',
  '-Wno-invalid-offsetof',
  '-Wno-unused-const-variable',
  '-Wno-volatile', # glm warning
  '-Wno-deprecated-volatile',
  '-Wno-ignored-qualifiers', # reshade warning
  '-Wno-missing-braces',
]), language: 'cpp')

add_project_arguments(cppc.get_supported_arguments([
  '-ffast-math',
]), language: 'cpp')
######################################################
# set root
sourceRoot = meson.project_source_root()
# add include dir
incdir = include_directories('main/include')
# create sources env var
sources = []
# add sources from src dir
subdir('main/src')
# add tests
subdir('tests')
####################DEPENDENCIES#############################
# add procesor library
libprocessor_proj = subproject('libprocessor')
libprocessor_dep = libprocessor_proj.get_variable('libprocessor_dep')
# add boost
boost_dep = dependency('boost')
#############################################################
executable(
  meson.project_name(), 
  sources,
  include_directories : incdir,
  dependencies : [libprocessor_dep,boost_dep],
  native: true)


