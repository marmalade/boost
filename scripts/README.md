Conversion scripts
------------------

The conversion scripts automatically create mkf and mkb files for boost libraries
and convert boost tests from boost testing framework to Marmalade SDK testing 
framework.
To use them please create a subset of boost libraries using ``bcp <list of libraries to extract> <output folder>``
command. Next put the extracted sources to 'script/in' folder and run 'boost_create_mkfs.py' command.

 

