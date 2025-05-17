#ifndef Singleton_H
#define Singleton_H
#include"global.h"
template <typename T>
class Singleton
{
protected:
    Singleton()=default;//编译器自动构建一个默认构造函数
    Singleton(const Singleton<T>&)=delete;//禁用拷贝
    Singleton operator=(const Singleton<T>& st)=delete;//禁用赋值
    static std::shared_ptr<T> _instance;//唯一化用于存储实例，且自动进行内存释放
public:
    static std::shared_ptr<T> GetInstance()//唯一化用于获取实例
    {
        static std::once_flag s_flag;//只会初始化一次
        std::call_once(s_flag,[&](){
            _instance=std::shared_ptr<T>(new T);
            //关于为什么不用make_share来构造智能指针，主要是因为make_share需要调用构造函数，但是这里的托管对象是单例，构造私有了
        });
        return _instance;
    }

    void Printaddress()
    {
        std::cout<<_instance.get()<<std::endl;
    }
    ~Singleton()
    {
        std::cout<<"this is a Singleton disturct"<<std::endl;
    }
};
template <typename T>
std::shared_ptr<T> Singleton<T>::_instance =nullptr;
#endif // Singleton_H
