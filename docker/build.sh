#!/usr/bin/env bash

cd /build/emsdk
. ./emsdk_env.sh
export LDFLAGS="-O2 -s TOTAL_MEMORY=268435456 --preload-file /path/to/monkey1-amiga-demo-en/@/monkey/"

cd /build/emscripten-scummvm

emconfigure ./configure --disable-all-engines --enable-engine-static=scumm --backend=sdl --disable-bink --disable-mt32emu --disable-16bit --disable-scalers --disable-hq-scalers --disable-alsa --disable-vorbis --disable-tremor --disable-mad --disable-flac --disable-zlib --disable-opengl --disable-png --disable-theoradec --disable-faad --disable-fluidsynth --disable-nasm --disable-readline --disable-libunity --disable-sndio --disable-timidity --with-sdl-prefix=/build/emsdk/fastcomp/emscripten/system/bin
emmake make


# Sword 25
# Error: assigning to 'sSDL_Surface *' from incompatible type 'SDL_Surface *'
#export USE_LIBPNG=1
#export USE_ZLIB=1
#emconfigure ./configure --disable-all-engines --enable-engine-static=sword25 --backend=sdl --disable-bink --disable-mt32emu  --disable-scalers --disable-hq-scalers --disable-alsa --disable-vorbis --disable-tremor --disable-mad --disable-flac --disable-opengl --disable-theoradec --disable-faad --disable-fluidsynth --disable-nasm --disable-readline --disable-libunity --disable-sndio --disable-timidity --with-sdl-prefix=/build/emsdk/fastcomp/emscripten/system/bin --enable-zlib --enable PNG
