#ifndef SINGLETON_H
#define SINGLETON_H

template <typename T>
class Singleton
{
public:
	static T* GetInstance();
protected:
	Singleton(){}
	virtual ~Singleton(){}
private:
	static T* CreateInstance();
	static T* m_Instance;
};

template <typename T>
T* Singleton<T>::m_Instance = 0;

template <typename T>
T* Singleton<T>::GetInstance()
{
    if (Singleton::m_Instance == 0)
    {
        Singleton::m_Instance = CreateInstance();
    }
    return Singleton::m_Instance;
}

template<typename T>
T* Singleton<T>::CreateInstance()
{
    return new T();
}

#endif