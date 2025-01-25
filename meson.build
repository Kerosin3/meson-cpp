project('bulkmulti', 'cpp',
  meson_version: '>=0.58.0',
  default_options: [
    'cpp_std=c++23',
    'build.cpp_std=c++23',
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
## set root
sourceRoot = meson.project_source_root()
# add tests
#subdir('tests')
#############################################################
#add executable
exec_prj = subproject('executable')
# add library
libasync_proj = subproject('libasync')
libasync_dep = libasync_proj.get_variable('libasync_dep')
