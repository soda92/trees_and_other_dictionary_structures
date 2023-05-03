Remove-Item -Recurse -Force .\build
cmake -Bbuild -GNinja -DCMAKE_BUILD_TYPE=Debug
cmake --build build
