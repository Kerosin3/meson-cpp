project('libprocessor', 'cpp', 
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

# add sources var
sources = []
# add to sources variable
subdir('src')
# add include dir
libprocessor_inc = include_directories('include')
# create shared library
libprocessor = shared_library('processor',
  sources,
  include_directories : libprocessor_inc,
  install : true)
# create var to export to top
libprocessor_dep = declare_dependency(include_directories : libprocessor_inc,
  link_with : libprocessor)
