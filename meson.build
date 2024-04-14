project('test_prj', 'cpp',  
  meson_version: '>=0.58.0',
  default_options: [
    'cpp_std=c++20',
    'build.cpp_std=c++20',
    'warning_level=3',
    'buildtype=debugoptimized',
    'b_ndebug=if-release',
    'force_fallback_for=wlroots,libliftoff,vkroots',
  ],
  subproject_dir : 'subprojects',
  version : '1.0.0',
  license : 'MIT')

# BUILD TYPE DEFINES HERE  AND OVERWRITE SUBPROJECTS' SETTINGS#

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
  '-Wshadow'
]), language: 'cpp')

add_project_arguments(cppc.get_supported_arguments([
  '-ffast-math',
]), language: 'cpp')
######################################################
#add cpp-check
compile_commands_json = join_paths(meson.build_root(), 'compile_commands.json')
run_target('cppcheck',
		   command: [
			 'cppcheck',
			 '--project=' + compile_commands_json,
			 '--suppress-xml=' + join_paths(meson.source_root(), 'cppcheck-suppressions.xml'),
			 '--error-exitcode=2',
                         '--enable=warning,style,performance,portability,unusedFunction',
                         '--std=c++20',
		   ], )
######################################################
## set root
sourceRoot = meson.project_source_root()
# add tests
subdir('tests')
#############################################################
#add executable
exec_prj = subproject('executable-prj')
# add library
libexample_proj = subproject('libexample')
libexample_dep = libexample_proj.get_variable('libexample_dep')
