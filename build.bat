windres assci.rc -O coff -o assci.res
windres icon.rc -O coff -o icon.res
windres version.rc -O coff -o version.res
windres sounds.rc -O coff -o sounds.res
gcc.exe main.c ball.c util.c stats.c shadow.c bat.c TextRenderer.c ragequit.c soundprayer.c assci.res icon.res version.res sounds.res -o KekPong.exe -lgdi32 -lWinmm -O2