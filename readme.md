# EXAMPLE C LANG PROJECT


## REQUIREMENTS

1. meson > 0.58
2. ninja
3. cppcheck
4. clang-tidy
5. clang-format

# meson setup and run

1.  Setup build `meson setup build --buildtype=release`
2.  Compile build `meson compile -C build`

# good coding

1. ninja -C build cppcheck
2. ninja -C build clang-tidy
2. ninja -C build clang-format
