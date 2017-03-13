include_directories("${carve_SOURCE_DIR}/include")
include_directories("${carve_SOURCE_DIR}/external/GLOOP/include")
include_directories("${carve_SOURCE_DIR}/external/GLUI/include")

if(GLEW_FOUND)
  include_directories("${GLEW_INCLUDE_DIR}")
else(GLEW_FOUND)
  include_directories("${carve_SOURCE_DIR}/external/GLEW/include")
endif(GLEW_FOUND)

include_directories(${OPENGL_INCLUDE_DIR})

include_directories("${carve_SOURCE_DIR}/common")

link_directories("${gloop_BINARY_DIR}")
link_directories("${carve_BINARY_DIR}/common")

if(CARVE_INTERSECT_GLU_TRIANGULATOR AND CARVE_WITH_GUI)
  add_executable       (intersect   glu_triangulator.cpp intersect.cpp)
  target_link_libraries(intersect   carve_fileformats carve_misc carve gloop_model ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})
  target_compile_features(intersect PUBLIC cxx_uniform_initialization)
  add_executable       (slice       glu_triangulator.cpp slice.cpp)
  target_link_libraries(slice       carve_fileformats carve_misc carve gloop_model ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})
  target_compile_features(slice PUBLIC cxx_uniform_initialization)
else(CARVE_INTERSECT_GLU_TRIANGULATOR AND CARVE_WITH_GUI)
  add_definitions(-DDISABLE_GLU_TRIANGULATOR)
  add_executable       (intersect   intersect.cpp)
  target_link_libraries(intersect   carve_fileformats carve_misc carve gloop_model)
  add_executable       (slice       slice.cpp)
  target_link_libraries(slice       carve_fileformats carve_misc carve gloop_model)
endif(CARVE_INTERSECT_GLU_TRIANGULATOR AND CARVE_WITH_GUI)

if(CARVE_WITH_GUI)
  add_executable       (view   view.cpp)
  target_link_libraries(view   carve_fileformats carve_misc carve_ui carve gloop_model glui ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})
  target_compile_features(view PUBLIC cxx_uniform_initialization)
  install(TARGETS view RUNTIME DESTINATION "${CMAKE_INSTALL_PREFIX}/bin")
endif(CARVE_WITH_GUI)

add_executable       (face_merge face_merge.cpp)
target_link_libraries(face_merge carve_fileformats carve gloop_model)
target_compile_features(face_merge PUBLIC cxx_uniform_initialization)

add_executable       (mesh_simplify mesh_simplify.cpp)
target_link_libraries(mesh_simplify carve_fileformats carve gloop_model)
target_compile_features(mesh_simplify PUBLIC cxx_uniform_initialization)

add_executable       (triangulate triangulate.cpp)
target_link_libraries(triangulate carve_fileformats carve gloop_model)
target_compile_features(triangulate PUBLIC cxx_uniform_initialization)

add_executable       (cutgraph    cut.cpp)
target_link_libraries(cutgraph    carve_fileformats carve gloop_model)
target_compile_features(cutgraph PUBLIC cxx_uniform_initialization)

add_executable       (extrude     extrude.cpp)
target_link_libraries(extrude     carve_fileformats carve gloop_model)
target_compile_features(extrude PUBLIC cxx_uniform_initialization)

add_executable       (convert     convert.cpp)
target_link_libraries(convert     carve_fileformats carve gloop_model)
target_compile_features(convert PUBLIC cxx_uniform_initialization)

add_executable       (close_manifold     close_manifold.cpp)
target_link_libraries(close_manifold     carve_fileformats carve gloop_model)
target_compile_features(close_manifold PUBLIC cxx_uniform_initialization)

add_executable       (selfintersect     selfintersect.cpp)
target_link_libraries(selfintersect     carve_fileformats carve gloop_model)
target_compile_features(selfintersect PUBLIC cxx_uniform_initialization)

foreach(tgt slice intersect triangulate convert)
  install(TARGETS ${tgt}
          RUNTIME DESTINATION "${CMAKE_INSTALL_PREFIX}/bin")
endforeach(tgt)
