cmd_Debug/serverz.node := c++ -bundle -undefined dynamic_lookup -Wl,-search_paths_first -mmacosx-version-min=10.5 -arch x86_64 -L./Debug  -o Debug/serverz.node Debug/obj.target/serverz/lib/serverz.o 
