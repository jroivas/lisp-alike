lisplib = library('lisp-alike',
    sources: [
        'symbols.cpp',
        'builtin.cpp',
        'tokenize.cpp',
        'value.cpp',
        'parse.cpp',
        'eval.cpp',
        'env.cpp'
    ],
    include_directories: [
        top_inc,
        main_inc
    ]
)

deps = []
if readline.found()
  deps += readline
endif

lisp_alike = executable('lisp-alike',
    sources: [
        'main.cpp'
    ],
    include_directories: [
        top_inc,
        main_inc
    ],
    link_with: [
        lisplib,
    ],
    dependencies: deps
)
