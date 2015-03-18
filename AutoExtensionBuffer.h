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

	int8_t* write(const int8_t* p, uint32_t n){

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
			_buf = new int8_t[n];
		}
		else{
			_buf = (int8_t*)::realloc((void*)_buf, n);
		}

		if (_buf == NULL) return false;

		if ((orver == false) && (_bover == true)){
			memcpy(_buf, _base, _length);
		}

		_capacity = n;

		return true;
	}

	int8_t* get(bool bExtension = false){

		if (bExtension && (_length + _extension_size)  > _capacity){
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

	int8_t* front(){
		if (_bover){
			return _buf;
		}
		else{
			return _base;
		}
	}

	int8_t* last(){
		if (_bover){
			return &_buf[_length];
		}
		else{
			return &_base[_length];
		}
	}

	void free(){
		if (_buf != NULL){
            ::free(_buf);
			_buf = NULL;
		}
		_length = 0;
		_bover = false;
	}

	void add_length(uint32_t n) { _length += n; }
	uint32_t length() { return _length; }
	uint32_t capacity() { return _capacity; }
	void set_extension_size(uint32_t n){ _extension_size = n; }
	uint32_t get_extension_size(){ return _extension_size; }


private:
	int8_t _base[AUTOEXTENSION_BUFFER_BASEBUFSIZE];
	int8_t* _buf;
	uint32_t _extension_size;
	uint32_t _length;
	uint32_t _capacity;
	bool _bover;
};

