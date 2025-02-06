project('mbulk', 'cpp',
  meson_version: '>=0.58.0',
  default_options: [
    'cpp_std=c++20',
    'build.cpp_std=c++20',
    'warning_level=2',
    'strip=yes',
    'buildtype=release',
    'force_fallback_for=wlroots,libliftoff,vkroots'
  ],
  version : '1.1.0',
  license : 'MIT')

add_project_link_arguments(
  '-static-libgcc',
  '-static-libstdc++',
  '-static',
  language: 'cpp',
)

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
message('OUTPUT ARE: ',meson.global_build_root())
# add library async
lib1 = 'libasync'
lib1_proj = subproject(lib1)
lib1_dep = lib1_proj.get_variable('libasync_dep')
#############################################################
executable(
  meson.project_name(),
  sources,
  include_directories : incdir,
  dependencies : [boost_dep,lib1_dep],
  install: true,
  install_dir: meson.global_build_root()
  )

