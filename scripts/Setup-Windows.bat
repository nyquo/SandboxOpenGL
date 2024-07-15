pushd ..
cmake -G "Visual Studio 17 2022" . -Bbuild -DCMAKE_INSTALL_PREFIX="bin"
popd