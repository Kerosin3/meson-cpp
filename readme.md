# EXAMPLE C LANG PROJECT


## REQUIREMENTS

1. meson > 0.58
4. ninja

# meson setup and run

1.  Setup build `meson setup build --buildtype=release`
2.  Compile build `meson compile -C build`

# good coding

1. ninja -C build cppcheck
2. ninja -C build clang-tidy
2. ninja -C build clang-format
