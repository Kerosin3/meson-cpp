# add google-test
gtest_proj = subproject('gtest')
#gtest_dep = gtest_proj.get_variable('gtest_dep')
#gtest_main_dep = gtest_proj.get_variable('gtest_main_dep')
# mock includes google-test!
gmock_dep = gtest_proj.get_variable('gmock_dep')
gmock_main_dep = gtest_proj.get_variable('gmock_main_dep')

# add library
lib1_proj = subproject('libexample')
libexample_dep = lib1_proj.get_variable('libexample_dep')

google_tests_exe = executable('gtests',
                                ['test_main.cpp'],
                                dependencies : [gmock_dep,gmock_main_dep,
                                                libexample_dep],
)

test('my google tests', google_tests_exe)
