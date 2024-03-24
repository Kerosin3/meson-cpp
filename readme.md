# HOMEWORK-7

+ packet commands processing

## REQUIREMENTS

1. meson > 0.58
2. liboost
3. libgoogletest
4. ninja

# meson setup and run

1.  Setup build `meson setup build`
2.  Compile build `meson compile -C build`
3.  Run tests `meson test -C build --verbose`
4.  Run executable `./build/bulk`

# good coding

1. ninja -C build cppcheck
2. ninja -C build clang-tidy
2. ninja -C build clang-format
