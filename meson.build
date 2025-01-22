project('test_prj', 'cpp',  
  meson_version: '>=0.58.0',
  default_options: [
    'cpp_std=c++23',
    'build.cpp_std=c++2b',
    'warning_level=3',
    'buildtype=release',
    'b_ndebug=if-release',
    'force_fallback_for=wlroots,libliftoff,vkroots',
  ],
  subproject_dir : 'subprojects',
  version : '1.0.0',
  license : 'MIT')

# BUILD TYPE DEFINES HERE  AND OVERWRITE SUBPROJECTS' SETTINGS#

cppc = meson.get_compiler('cpp')
add_project_arguments(cppc.get_supported_arguments([
  '-Wno-sign-compare',
  '-Wno-unused-parameter',
  '-Wno-missing-field-initializers',
  '-Wno-c99-designator',
  '-Wno-invalid-offsetof',
  '-Wno-unused-const-variable',
  '-Wno-volatile', # glm warning
  '-Wno-deprecated-volatile',
  '-Wno-ignored-qualifiers', # reshade warning
  '-Wno-missing-braces',
  '-Wshadow',
  '-Wconversion'
]), language: 'cpp')

add_project_arguments(cppc.get_supported_arguments([
  '-ffast-math',
]), language: 'cpp')
######################################################
######################################################
## set root
sourceRoot = meson.project_source_root()
# add tests
subdir('tests')
#############################################################
#add executable
exec_prj = subproject('executable-prj')
