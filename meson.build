project('my-lips', 'cpp')

add_global_arguments('-std=c++11', language : 'cpp')
add_global_arguments('-fPIC', language : 'cpp')
add_global_arguments('-Wall', language : 'cpp')
add_global_arguments('-Wno-error=unused-local-typedefs', language : 'cpp')
add_global_arguments('-Wno-unused-local-typedefs', language : 'cpp')
add_global_arguments('-Wno-error=pedantic', language : 'cpp')
add_global_arguments('-DGC_OPERATOR_NEW_ARRAY', language : 'cpp')

main_inc = include_directories('include')
top_inc = include_directories('.')
test_inc = include_directories('test')

subdir('src')
subdir('test')
