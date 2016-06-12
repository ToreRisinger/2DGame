#pragma once

template<class T>
class Callback {

public:
	typedef void(T::* FuncT)();
	FuncT m_function;
	T* m_class;
	Callback(T* c, FuncT function) :m_function(function), m_class(c){}
	
	void func(){
		return (m_class->*m_function)();
	}
};
