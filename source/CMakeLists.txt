cmake_minimum_required (VERSION 3.8)

find_package(unofficial-enet CONFIG REQUIRED)

add_executable(${PROJECT_NAME}
  main.cpp
  main.hh

  utils/variant.hh

  server/server.hh

  objects/event.hh
  objects/player.hh
  objects/world.hh

  managers/player_manager.hh

)

target_link_libraries(${PROJECT_NAME} PRIVATE unofficial::enet::enet)