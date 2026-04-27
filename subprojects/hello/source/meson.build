incdirs = include_directories(
    'include',
)

sources = files(
    'main.cpp',
    'libsnow.cpp'
)

add_project_arguments(
    '--target=x86_64-unknown-windows-gnu',
    '-nostdinc',
    '-ffreestanding',
    '-fno-stack-protector',
    '-fno-stack-check',
    '-fno-PIC',
    '-ffunction-sections',
    '-fdata-sections',
    '-m64',
    '-march=x86-64',
    '-mno-80387',
    '-mno-mmx',
    '-mno-sse',
    '-mno-sse2',
    '-mno-red-zone',
    '-fdeclspec',
    '-MMD',
    '-MP',
    '-g',

    language: ['cpp', 'c']
)

add_project_arguments(
    '-fwin64',

    language: 'nasm'
)

add_project_link_arguments(
    '-Wl,/subsystem:NATIVE',
    '-Wl,/entry:_start',
    '-Wl,/largeaddressaware',
    '-Wl,/base:0x1000',
    '-Wl,/nodefaultlib',
    '-Wl,/verbose',
    '-static',

    language: ['cpp', 'c']
)

executable('hello.exe', sources, include_directories: [incdirs])