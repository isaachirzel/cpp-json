project('cpp-json', 'cpp')

common_sources = [
	'src/hirzel/json/Deserialization.cpp',
	'src/hirzel/json/Error.cpp',
	'src/hirzel/json/Serialization.cpp',
	'src/hirzel/json/Token.cpp',
	'src/hirzel/json/TokenType.cpp',
	'src/hirzel/json/Value.cpp',
	'src/hirzel/json/ValueType.cpp'
]

unit_test_sources = [
	'src/hirzel/json/Token.test.cpp',
	'src/hirzel/json/TokenType.test.cpp',
	'src/hirzel/json/Value.test.cpp',
	'src/hirzel/json/ValueType.test.cpp'
]

fs = import('fs')
include_dirs = include_directories('include', 'src')

library('cpp-json', common_sources, include_directories: include_dirs)

foreach source: unit_test_sources
	source_file_name = fs.name(source).replace('.test.cpp', '')
	unit_test_name = source_file_name
	unit_test_exe_name = source_file_name + '.test'
	unit_test_exe = executable(unit_test_exe_name, source, common_sources, include_directories: include_dirs)
	
	test(unit_test_name, unit_test_exe)
endforeach
