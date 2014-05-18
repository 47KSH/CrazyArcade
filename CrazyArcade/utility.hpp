#pragma once

template<typename T>
class singleton
{
protected :
	singleton(){}
	virtual ~singleton(){}

public :
	static T& getReference()
	{
		static T inst;
		return inst;
	}
};
