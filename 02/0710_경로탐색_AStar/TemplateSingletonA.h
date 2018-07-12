#pragma once

template <typename T>
class TemplateSingletonA
{
public:
	static T&	GetInstance();
};

template <typename T>
T&	TemplateSingletonA<T>::GetInstance()
{
	static T	instance;

	return instance;
}