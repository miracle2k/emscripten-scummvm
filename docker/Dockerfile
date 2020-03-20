# Relevant links:
# - https://willowtreeapps.com/ideas/how-to-use-native-libraries-on-node-js-with-emscripten

FROM ubuntu:18.04

RUN apt-get update && apt-get install -y git wget python clang build-essential libsdl1.2-dev

WORKDIR /build
RUN git clone https://github.com/emscripten-core/emsdk.git

# Or add the local repo
RUN git clone https://github.com/miracle2k/emscripten-scummvm.git
RUN cd emscripten-scummvm && git checkout emscripten

ADD install.sh /installapp
RUN /installapp

#ADD configure /build/emscripten-scummvm/configure

ADD build.sh /buildapp
RUN /buildapp
