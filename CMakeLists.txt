cmake_minimum_required(VERSION 3.27)
project(Bomberman LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR}/bin)

add_executable(Bomberman src/main.cpp 
                         src/game/Partie.cpp
                         src/game/GUI/Gui.cpp
                         src/game/Graphics/Graphics.cpp 
                         src/game/elements/element.cpp
                         src/game/carte/Monde.cpp
                         src/game/carte/TileMap.cpp
                         src/game/player/Player.cpp
                         src/game/player/Debouncer.cpp
                         src/game/menu/MainMenu.cpp
                         src/game/lobby/Lobby.cpp
                         src/game/elements/bombe/Bombe.cpp
                         src/game/elements/bombe/Ray.cpp
                         src/game/elements/personnage/Personnage.cpp )
 
                         
add_executable(Server src/game/server/Server.cpp
                      src/game/player/Player.cpp
                      src/game/player/Debouncer.cpp
                     )

target_link_libraries(Bomberman PRIVATE sfml-graphics sfml-window sfml-network sfml-system sfml-audio )
target_link_libraries(Server PRIVATE  sfml-graphics sfml-network)

target_compile_features(Bomberman PRIVATE cxx_std_17)
target_compile_features(Server PRIVATE cxx_std_17)

if(WIN32)
    add_custom_command(
        TARGET Bomberman
        TARGET Server
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:Bomberman>
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:Server>
        VERBATIM)
endif()


install(TARGETS Bomberman Server)
