# HOMEWORK-7

+ packet commands processing

## REQUIREMENTS

1. meson > 0.58
2. liboost
3. libgoogletest
4. ninja

# meson setup and run

1.  Clone repo `git clone https://github.com/Kerosin3/meson-cpp.git --branch HW7 --single-branch`
2.  Setup build `meson setup build`
3.  Compile build `meson compile -C build`
4.  Run tests `meson test -C build --verbose`
5.  Run executable `./build/bulk`

# good coding

1. ninja -C build cppcheck
2. ninja -C build clang-tidy
2. ninja -C build clang-format
