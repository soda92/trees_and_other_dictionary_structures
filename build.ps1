# Remove-Item -Recurse -Force .\build
# cmake -Bbuild -GNinja -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=true
cmake --build build
# Copy-Item .\build\compile_commands.json .
