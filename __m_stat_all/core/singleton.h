#ifndef SINGLETON_H
#define SINGLETON_H

template <class T>
class Singleton
{
public:

    static T& instance()
    {
        static T _instance;
        return _instance;
    }

private:
    friend T;

    Singleton(){}
    Singleton(const Singleton<T>&);
    Singleton(Singleton<T>&&);
};

#endif // SINGLETON_H
