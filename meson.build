project('lisp-alike', 'cpp')

add_global_arguments('-std=c++11', language : 'cpp')
add_global_arguments('-fPIC', language : 'cpp')
add_global_arguments('-Wall', language : 'cpp')
add_global_arguments('-Werror', language : 'cpp')
add_global_arguments('-Wuninitialized', language : 'cpp')

main_inc = include_directories('include')
top_inc = include_directories('.')
test_inc = include_directories('test')

subdir('src')
subdir('test')
