# HOMEWORK-9

+ packet commands processing with multithreading!

## REQUIREMENTS

1. meson > 0.58
2. liboost
3. libgoogletest
4. ninja

# meson setup and run

1.  Clone repo `git clone https://github.com/Kerosin3/meson-cpp.git --branch HW9 --single-branch`
2.  Setup build `meson setup build`
3.  Compile build `meson compile -C build`
4.  Run executable `./build/subprojects/executable`

# good coding

1. ninja -C build cppcheck
2. ninja -C build clang-tidy
2. ninja -C build clang-format
