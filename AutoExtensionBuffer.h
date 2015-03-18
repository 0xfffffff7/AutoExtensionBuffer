#pragma once
#include <stdlib.h>
#include <string>
#include <cstdint>

#define AUTOEXTENSION_BUFFER_BASEBUFSIZE		4096

class AutoExtensionBuffer
{
public:
	AutoExtensionBuffer(void){
		memset(_base, 0x00, AUTOEXTENSION_BUFFER_BASEBUFSIZE);
		_buf = NULL;
		_length = 0;
		_capacity = AUTOEXTENSION_BUFFER_BASEBUFSIZE;
		_bover = false;
		_extension_size = AUTOEXTENSION_BUFFER_BASEBUFSIZE;
	}

	~AutoExtensionBuffer(void){
		if (_buf != NULL){
			::free(_buf);
			_buf = NULL;
		}
	}

	char* write(const char* p, uint32_t n){

		if (n < 0) return false;
		if (p == NULL) return false;

		if ((_length + n)  > _capacity){
			if (!reserve(_length + n)){
				return NULL;
			}
		}

		if (_bover){
			memcpy(&_buf[_length], p, n);
		}
		else{
			memcpy(&_base[_length], p, n);
		}
		_length += n;

		if (_bover){
			return &_buf[_length];
		}
		else{
			return &_base[_length];
		}
	}

	bool reserve(uint32_t n){

		bool orver = _bover;

		if (n < 1) return false;

		if (n <= _capacity){
			return true;
		}
		else{
			_bover = true;
		}

		if (_buf == NULL){
			_buf = new char[n];
		}
		else{
			_buf = (char*)::realloc((void*)_buf, n);
		}

		if (_buf == NULL) return false;

		if ((orver == false) && (_bover == true)){
			memcpy(_buf, _base, _length);
		}

		_capacity = n;

		return true;
	}

	char* get(bool bExt = true){ 

		if (bExt && (_length + _extension_size)  > _capacity){
			if (!reserve(_length + _extension_size)){
				return NULL;
			}
		}

		if (_bover){
			return &_buf[_length];
		}
		else{
			return &_base[_length];
		}
	}

	char* front(){
		if (_bover){
			return _buf;
		}
		else{
			return _base;
		}
	}

	char* last(){
		if (_bover){
			return &_buf[_length];;
		}
		else{
			return &_base[_length];
		}
	}

	void free(){
		if (_buf != NULL){
			delete _buf;
			_buf = NULL;
		}
		_length = 0;
		_bover = false;
	}

	void update_length(uint32_t n) { _length += n; }
	uint32_t length() { return _length; }
	uint32_t capacity() { return _capacity; }
	void set_extension_size(u_int n){ _extension_size = n; }
	uint32_t get_extension_size(){ return _extension_size; }


private:
	char _base[AUTOEXTENSION_BUFFER_BASEBUFSIZE];
	char* _buf;
	uint32_t _extension_size;
	uint32_t _length;
	uint32_t _capacity;
	bool _bover;
};

