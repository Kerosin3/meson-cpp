project('libexample', 'cpp', 
  meson_version: '>=0.58.0',
  default_options: [
    'cpp_std=c++20',
    'build.cpp_std=c++20',
    'warning_level=3',
    'b_ndebug=if-release',
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
#add cpp-check
compile_commands_json = join_paths(meson.project_build_root(), 'compile_commands.json')
run_target('cppcheck',
		   command: [
			 'cppcheck',
			 '--project=' + compile_commands_json,
			 '--suppress-xml=' + join_paths(meson.project_source_root(), 'cppcheck-suppressions.xml'),
			 '--error-exitcode=2',
                         '--enable=warning,style,performance,portability,unusedFunction',
                         '--std=c++20',
		   ], )
######################################################
# add include dir
libexample_inc = include_directories('headers')
libexample_inc_local = include_directories('headers/libexample')
# create sources env var
sources = []
# add sources from src dir
subdir('src')
# add boost
boost_dep = dependency('boost')
#############################################################
# create shared library
libexample = static_library('example',
  sources,
  include_directories : libexample_inc_local,
  install : true,
  )
# create var to export to top
libexample_dep = declare_dependency(include_directories : libexample_inc,
  link_with : libexample)
