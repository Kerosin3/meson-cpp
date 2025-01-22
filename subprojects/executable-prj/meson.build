project('bayan', 'cpp',  
  meson_version: '>=0.58.0',
  default_options: [
    'cpp_std=c++23',
    'build.cpp_std=c++2b',
    'warning_level=2',
    'buildtype=release',
    'b_ndebug=if-release',
    'force_fallback_for=wlroots,libliftoff,vkroots'
  ],
  version : '1.1.0',
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
  '-Wshadow',
  '-Wno-sign-compare'
]), language: 'cpp')

add_project_arguments(cppc.get_supported_arguments([
  '-ffast-math',
]), language: 'cpp')
######################################################
# add include dir
incdir = include_directories('include')
# create sources env var
sources = []
# add sources from src dir
subdir('src')
# add boost
boost_dep = dependency('boost',version: '>=1.83.0',include_type: 'system',
                        modules : [ 'thread',
                                    'filesystem',
                                    'program_options',
                                    'log_setup',
                                    'log', 
                                    'date_time',
                                    'system' ])
#############################################################
executable(
  meson.project_name(), 
  sources,
  include_directories : incdir,
  dependencies : [boost_dep],
  install: true,
  #native: true)
  )

