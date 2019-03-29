test_lips = executable('test-lips',
    sources: [
        'test.cpp',
        'test_tokenize.cpp'
    ],
    include_directories: [
        test_inc,
        top_inc,
        main_inc
    ],
    link_with: [
        lipslib
    ]
)

test('Test lips', test_lips)
