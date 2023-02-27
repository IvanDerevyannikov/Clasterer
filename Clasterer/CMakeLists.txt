

#include("C:/DEV/vcpkg/scripts/buildsystems/vcpkg.cmake")
set(CMAKE_CXX_STANDARD 17)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

set(OpenCV_DIR "C:/opencv/build")
option( OpenCV_STATIC OFF )
find_package(OpenCV  REQUIRED)

include_directories( ${OpenCV_INCLUDE_DIRS} )
link_directories( ${OpenCV_LIB_DIR} )


add_executable (Clasterer "main.cpp"
				"image.h" "image.cpp" "point.h" "clasters.h" 
				"clasters.cpp" "kdtree.h" "kdtree.cpp" "gui.h" "gui.cpp")

target_link_libraries(Clasterer PRIVATE Qt6::Widgets ${OpenCV_LIBS})


if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Clasterer PROPERTY CXX_STANDARD 20)
endif()