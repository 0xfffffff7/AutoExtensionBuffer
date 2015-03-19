#pragma once
#include <stdlib.h>
#include <string>
#include <cstdint>

#define AUTOEXTENSION_BUFFER_BASEBUFSIZE		4096
#define AUTOEXTENSION_BUFFER_EXTENSION_RATE		1.2

class AutoExtensionBuffer
{
public:
	AutoExtensionBuffer(void){
		memset(_base, 0x00, AUTOEXTENSION_BUFFER_BASEBUFSIZE);
        _buf = (int8_t*)::malloc(0);
		_length = 0;
		_capacity = AUTOEXTENSION_BUFFER_BASEBUFSIZE;
		_bover = false;
		_rate = AUTOEXTENSION_BUFFER_EXTENSION_RATE;
		_writable_size = 0;
	}

	~AutoExtensionBuffer(void){
        AutoExtensionBuffer::free();
	}

	int8_t* write(const int8_t* p, uint32_t n){

		if (p == NULL) return NULL;

		if ((_length + n)  > _capacity){
			if (!reserve(static_cast<uint32_t>(_length + (static_cast<double>(n)* _rate)))){
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


		int8_t *p = (int8_t*)::realloc((void*)_buf, n);
		
		if (p == NULL) return false;

		_buf = p;

		if ((orver == false) && (_bover == true)){
			memcpy(_buf, _base, _length);
		}

		_capacity = n;

		return true;
	}

	int8_t* transaction(uint32_t n){

		if ((_length + n)  > _capacity){
			if (!reserve(static_cast<uint32_t>(_length + (static_cast<double>(n)* _rate)))){
				return NULL;
			}
		}

		_writable_size = n;
	
		return last();
	}

	int8_t* commit(int32_t n){
		if ((_length + n) < _capacity){
			_length += n;
			_writable_size = 0;
		}
		else{
			return NULL;
		}
		return get();
	}

	int8_t*  commit(){
		_length += _writable_size;
		_writable_size = 0;
		return get();
	}

	int8_t* get(){
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
			return &_buf[_capacity -1];
		}
		else{
			return &_base[_capacity - 1];
		}
	}

	void free(){
		if (_bover){
            ::free(_buf);
			_buf = NULL;
		}
		_length = 0;
		_bover = false;
	}

	void set_rate(double n){ _rate = n; }
	void update_length(uint32_t n) { _length = n; }
	uint32_t length() { return _length; }
	uint32_t capacity() { return _capacity; }
	uint32_t get_writable_size(){ return _writable_size; }

private:
	int8_t _base[AUTOEXTENSION_BUFFER_BASEBUFSIZE];
	int8_t* _buf;
	uint32_t _length;
	uint32_t _capacity;
	double _rate;
	bool _bover;
	uint32_t _writable_size;
};

