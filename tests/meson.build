# add google-test
gtest_proj = subproject('gtest')
gtest_dep = gtest_proj.get_variable('gtest_dep')
gtest_main_dep = gtest_proj.get_variable('gtest_main_dep')
# mock includes google-test!
gmock_dep = gtest_proj.get_variable('gmock_dep')
gmock_main_dep = gtest_proj.get_variable('gmock_main_dep')

# add processor library
libprocessor_proj = subproject('libprocessor')
libprocessor_dep = libprocessor_proj.get_variable('libprocessor_dep')
#libprocessor_inc = libprocessor_proj.get_variable('libprocessor_inc')

google_tests_exe = executable('gtests',
                                ['test_main.cpp','test_mock.cpp'],
                                dependencies : [gmock_dep,gmock_main_dep,
                                                libprocessor_dep],
)

test('my google tests', google_tests_exe)


