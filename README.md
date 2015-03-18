# AutoExtensionBuffer
AutoExtensionBuffer for c++

# Example  

    AutoExtensionBuffer buf;
    
    std::string str = "abcde";
    
    // using internal static buffer.
    buf.write((char*)str.c_str(), str.length());	// cpacity is 4096.
    
    // front
    char* p = buf.front();
    
    // end
    p = buf.last();
    
    
    // length is 4096 orver.
    str = "abcde......."; 
    
    // using dynamic buffer.
    buf.write(str.c_str(), str.length());
    
    
    // get() is auto extension capacity. [_length + _extension_size(4096)].
    int r = socket.recv( buf.get(), buf.get_extension_size() );
    buf.update_length(r);
    
    // change extension size.
    buf.set_extension_size(2048);
    
    
    // no extension.
    // p = buf.get(false);  // Equivalence last().
    
    
    // Danger. orver capacity.
    socket.recv( buf.get(), 5000 );

