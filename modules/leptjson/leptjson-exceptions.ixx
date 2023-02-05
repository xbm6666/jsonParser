module;

#include<stdexcept>

export module Leptjson:Exceptions;

using namespace std;


namespace leptjson{
export namespace Exceptions {

class leptjson_exception :public runtime_error {
public:
	using runtime_error::runtime_error;
};

class novalue_error :public leptjson_exception {
public:
	using leptjson_exception::leptjson_exception;
};

class invaild_value_error :public leptjson_exception {
public:
	using leptjson_exception::leptjson_exception;
};

class root_not_singular_error :public leptjson_exception {
public:
	using leptjson_exception::leptjson_exception;
};


}
}
