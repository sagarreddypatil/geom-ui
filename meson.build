project('libgeomui', 'cpp', version: '0.1.0')

srcs = ['src/test.cpp']
incl_dirs = include_directories('include', 'external/eigen')

libgeomui = static_library('geomui', srcs, include_directories: incl_dirs)

examples = ['basic']
example_incl_dirs = include_directories('include')
example_libs = [libgeomui]

foreach example : examples
  executable(example, 'examples/' + example + '.cpp', include_directories: example_incl_dirs, link_with: example_libs)
endforeach
