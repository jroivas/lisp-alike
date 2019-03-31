lipslib = library('lipsl',
    sources: [
        'history.cpp',
        'tokenize.cpp',
        'value.cpp',
        'parse.cpp'
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
