project('gmtheory', 'cpp', default_options: ['cpp_std=c++23'])

add_project_arguments('-std=c++23', '-fmax-errors=1', language: 'cpp')

if get_option('buildtype') == 'debug'
    add_project_arguments('-g', language: 'cpp')
endif

gmtheory_includes = include_directories('src')
gmtheory_dep = declare_dependency(include_directories: gmtheory_includes)

###################################################################################################
# SUBPROJECTS

gtest_proj = subproject('gtest')
gtest_dep = gtest_proj.get_variable('gtest_dep')

gbase_proj = subproject('gbase')
gbase_dep = gbase_proj.get_variable('gbase_dep')

###################################################################################################
# LIBRARY

gmtheory_sources = [
    'src/gchorddatabase.cpp',
    'src/gchordmodifiers.cpp',
    'src/gchords.cpp',
    'src/gnoteintervals.cpp',
    'src/gnotes.cpp',
    'src/gscaledegrees.cpp',
    'src/gscales.cpp',
]

gmtheory_lib = static_library(
    'gmtheory',
    gmtheory_sources,
    dependencies: [gbase_dep],
    include_directories: gmtheory_includes,
)

gmtheory_dep = declare_dependency(link_with: gmtheory_lib, include_directories: gmtheory_includes)

###################################################################################################
# TEST

test_sources = [
    'test/gchordtest.cpp',
    'test/gchorddatabasetest.cpp',
    'test/gharmonytest.cpp',
    'test/gnotestest.cpp',
    'test/gscaletest.cpp',
    'test/noteintervalstest.cpp',
]

executable(
    'runtests',
    'test/runtests.cpp',
    test_sources,
    dependencies: [gtest_dep, gbase_dep],
    include_directories: [gmtheory_includes],
    link_with: gmtheory_lib,
)