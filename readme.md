# FILE FINDER PROJECT

## REQUIREMENTS

1. meson > 0.58
2. ninja
3. clang-format
4. clang-tidy
5. cppcheck

# meson setup and run

1.  Setup build `meson setup builddir --buildtype=release`
2.  Compile build `meson compile -C builddir`
3.  Run finder `./build/subprojects/executable-prj/bayan`


+ program cli arguments:
```
Allowed options:
  --help                         produce help message
  -d [ --dirs ] arg              directoris to scan
  -m [ --minfile ] arg (=1)      set min filesize to index,bytes
  -i [ --idirs ] arg             set exclude directories
  -l [ --level ] arg (=1)        recursive search
  -f [ --filter pattern ] arg    set min filesize to index,bytes
  -t [ --target files ] arg      setup target files to analyze
  -b [ --blocksize ] arg (=1024) read block size,bytes
```

# good coding

1. ninja -C builddir cppcheck
2. ninja -C builddir clang-tidy
2. ninja -C builddir clang-format
