#!/bin/bash
pushd ..
cmake -G "Ninja Multi-Config" . -Bbuild -DCMAKE_INSTALL_PREFIX="bin"
popd
