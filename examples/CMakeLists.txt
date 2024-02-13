project(geomui-examples)

function(add_example name)
  add_executable(${name} ${name}.cpp)
  target_link_libraries(${name} geomui)
endfunction()

add_example(basic)
add_example(gui)
