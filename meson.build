project('gmtheory', 'cpp', default_options: ['cpp_std=c++23'])

add_project_arguments('-fmax-errors=1', language: 'cpp')

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
    'src/g_chord_database.cpp',
    'src/g_chord_modifiers.cpp',
    'src/g_chords.cpp',
    'src/g_note_intervals.cpp',
    'src/g_notes.cpp',
    'src/g_scale_degrees.cpp',
    'src/g_scales.cpp',
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
    'test/g_chords_test.cpp',
    'test/g_chord_database_test.cpp',
    'test/g_harmony_test.cpp',
    'test/g_notes_test.cpp',
    'test/g_scales_test.cpp',
    'test/g_note_intervals_test.cpp',
]

executable(
    'run_tests',
    'test/run_tests.cpp',
    test_sources,
    dependencies: [gtest_dep, gbase_dep],
    include_directories: [gmtheory_includes],
    link_with: gmtheory_lib,
)