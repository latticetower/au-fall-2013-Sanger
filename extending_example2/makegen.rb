require 'mkmf'  
$libs = append_library($libs, "supc++")  
create_makefile('test_module') 