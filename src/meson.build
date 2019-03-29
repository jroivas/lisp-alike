lipslib = library('lipsl',
    sources: [
        'history.cpp',
        'tokenize.cpp',
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
