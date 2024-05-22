# add google-test
gtest_proj = subproject('gtest')

# mock includes google-test!
gmock_dep = gtest_proj.get_variable('gmock_dep')
gmock_main_dep = gtest_proj.get_variable('gmock_main_dep')

google_tests_exe = executable('gtests',
                                ['test_main.cpp'],
                                dependencies : [gmock_dep,gmock_main_dep]
)

test('my google tests', google_tests_exe)
