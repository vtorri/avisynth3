#!/bin/sh
rm -f sources.inc
echo SRC_DIR = ../../src > sources.inc
echo   >> sources.inc
echo SRC_GENERIC = \\ >> sources.inc
sed '/com\\/d;/vfw\\/d;/cpp/!d;s/^[ \t]*//;s/RelativePath=".\\src\\/  /;s/\\/\//g;s/">/ \\/' ../../avisynth.vcproj >>sources.inc
echo   >> sources.inc
echo SRC_IA32 = \\ >> sources.inc
find ../../src/ -name '*x86_nasm.asm' | sed 's/..\/..\/src\//  /;s/\\/\//g;s/asm/asm \\/' >>sources.inc
