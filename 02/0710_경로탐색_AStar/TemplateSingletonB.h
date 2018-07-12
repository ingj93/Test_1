#pragma once

template <typename T>
class TemplateSingletonB
{
private:
	static T	*m_instance;

public:
	static	T*		GetInstance();
			void	Destroy();
};

template <typename T>
T	*TemplateSingletonB<T>::m_instance = nullptr;


template <typename T>
T*	TemplateSingletonB<T>::GetInstance()
{
	if (m_instance == nullptr)
	{
		m_instance = new T;
	}
	return m_instance;
}
template <typename T>
void	TemplateSingletonB<T>::Destroy()
{
	delete m_instance;
	m_instance = nullptr;
}