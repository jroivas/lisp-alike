catch_header = custom_target('catch-header',
    output: 'catch.hpp',
    command: [
        meson.current_source_dir() + '/update_catch.sh'
    ])

test_lips = executable('test-lips',
    sources: [
        catch_header,
        'test.cpp',
        'test_tokenize.cpp',
        'test_parse.cpp'
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
