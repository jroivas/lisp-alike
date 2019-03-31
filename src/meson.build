lipslib = library('lipsl',
    sources: [
        'history.cpp',
        'symbols.cpp',
        'builtin.cpp',
        'tokenize.cpp',
        'value.cpp',
        'parse.cpp',
        'eval.cpp'
    ],
    include_directories: [
        top_inc,
        main_inc
    ]
)

my_lips = executable('my-lips',
    sources: [
        'main.cpp'
    ],
    include_directories: [
        top_inc,
        main_inc
    ],
    link_with: [
        lipslib
    ]
)
