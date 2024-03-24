project('test_prj', 'c', 
  meson_version: '>=0.58.0',
  default_options: [
    'cpp_std=c11',
    'buildtype=release',
    'build.cpp_std=c11',
    'warning_level=3',
  ],
  version : '1.0.0',
  license : 'MIT')

cxx = meson.get_compiler('c')
add_project_arguments(cxx.get_supported_arguments([
  '-Wall', 
  '-Wextra', 
  '-Wpedantic',
  '-pedantic-errors',
  '-Wno-unused-parameter',
  '-Wno-missing-field-initializers',
  '-Wno-c99-designator',
  '-Wno-invalid-offsetof',
  '-Wno-unused-const-variable',
  '-Wno-missing-braces',
  '-Wfloat-equal',
  '-Wundef',
  '-Wshadow',
  '-Wpointer-arith',
  '-Wcast-align',
  '-Wstrict-prototypes',
  '-Wstrict-overflow=5',
  '-Wwrite-strings',
  '-Waggregate-return',
  '-Wcast-qual',
  '-Wswitch-default',
  '-Wswitch-enum',
  '-Wconversion',
  '-Wunreachable-code',
  '-Wformat=2',
  '-fsanitize={address,thread,undefined}'

]), language: 'c')
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
                         '--std=c11',
		   ], )
######################################################
## set root
sourceRoot = meson.project_source_root()
# add include dir
incdir = include_directories('main/include')
# create sources env var
sources = []
# add sources from src dir
subdir('main/src')
# add boost
#boost_dep = dependency('boost')

#############################################################
executable(
  meson.project_name(), 
  sources,
  include_directories : incdir,
  #dependencies : [libgpiod_dep],
  #native: true
  )


