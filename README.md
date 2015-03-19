# AutoExtensionBuffer
AutoExtensionBuffer for c++
  
# Example  

    AutoExtensionBuffer buf;
    
    std::string str = "abcde";
    
    // using internal static buffer.
    buf.write((int8_t*)str.c_str(), str.length());	// cpacity is 4096.
    
    // front
    int8_t* p = buf.front();
    
    // writable position.
    p = buf.get();

    // capacity end
    p = buf.last();
    
    
    // length is 4096 orver.
    str = "abcde......."; 
    
    // using dynamic buffer.
    buf.write((int8_t*)str.c_str(), str.length());
    
    
    // allocate [(_length + 1024) * 1.2]
    p = buf.transaction(1024);
    
    int r = socket.recv(p, buf.get_writable_size());
    
    // data length update.
    buf.commit(r);
    
    
    // Danger. orver capacity.
    p = buf.transaction(4096);
    int r = socket.recv(p, 8192);


