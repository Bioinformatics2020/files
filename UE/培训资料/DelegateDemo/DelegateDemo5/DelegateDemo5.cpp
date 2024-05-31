#include <cassert>
#include <iostream>
#include <chrono>
#include <vector>
#include <Windows.h>

using std::cout;
using std::endl;
using std::vector;
using namespace std::chrono;


template<typename FunReturn,typename... VarTypes>
class TFunBase
{
public:
    virtual FunReturn Run(VarTypes... Args) = 0;
};

template<typename FunReturn,typename... VarTypes>
class TFunRaw : public TFunBase<FunReturn,VarTypes...>
{
    using FunType = FunReturn(*)(VarTypes...);
public:
    TFunRaw(FunType InFun):Fun(InFun){}
    
    virtual FunReturn Run(VarTypes... Args) override
    {
        return Fun(Args...);
    }
private:
    FunType Fun;
};

template<typename Class, typename FunReturn,typename... VarTypes>
class FClassFun : public TFunBase<FunReturn,VarTypes...>
{
    using FunType = FunReturn(Class::*)(VarTypes...);
public:
    FClassFun(Class* InObj, FunType InFun):Fun(InFun),Obj(InObj){}
    
    virtual FunReturn Run(VarTypes... Args) override
    {
        return (Obj->*Fun)(Args...);
    }
private:
    FunType Fun;
    Class* Obj;
};

//委托的核心
template<typename FunReturn,typename... VarTypes>
class TDelegateCore
{
public:
    ~TDelegateCore()
    {
        if(FunPtr)
        {
            delete FunPtr;
        }
    }
    
    void BindRaw(FunReturn(*InFun)(VarTypes...))
    {
        if(FunPtr || !InFun)
        {
            return;
        }
        FunPtr = new TFunRaw<FunReturn, VarTypes...>(InFun);
    }

    template<typename Class>
    void BindClass(Class* Obj, FunReturn(Class::*InFun)(VarTypes...))
    {
        if(FunPtr || !Obj ||!InFun)
        {
            return;
        }
        FunPtr = new FClassFun<Class, FunReturn, VarTypes...>(Obj, InFun);
    }
    
    FunReturn Run(VarTypes... Args)
    {
        assert(FunPtr);
        return FunPtr->Run(Args...);
    }
private:
    TFunBase<FunReturn,VarTypes...>* FunPtr = nullptr;
};

//多播委托
template<typename... VarTypes>
class TMultiDelegate
{
public:
    ~TMultiDelegate()
    {
        for (auto FunPtr : FunPtrArr)
        {
            if(FunPtr)
            {
                delete FunPtr;
            }
        }
    }
    
    void AddRaw(void(*InFun)(VarTypes...))
    {
        if(!InFun)
        {
            return;
        }
        FunPtrArr.push_back(new TFunRaw<void, VarTypes...>(InFun));
    }

    template<typename Class>
    void AddClass(Class* Obj, void(Class::*InFun)(VarTypes...))
    {
        if(!Obj ||!InFun)
        {
            return;
        }
        FunPtrArr.push_back(new FClassFun<Class, void, VarTypes...>(Obj, InFun));
    }
    
    void Run(VarTypes... Args)
    {
        for (auto FunPtr : FunPtrArr)
        {
            if(FunPtr)
            {
                FunPtr->Run(Args...);
            }
        }
    }
private:
    vector<TFunBase<void,VarTypes...>*> FunPtrArr;
};

#define DECLARE_DELEGATE( DelegateName ) \
    typedef TDelegateCore<void> DelegateName;

#define DECLARE_MULTICAST_DELEGATE_TwoParams( DelegateName, Param1Type, Param2Type ) \
    typedef TMultiDelegate<Param1Type, Param2Type> DelegateName;

//函数案例1
void TestFun1(int a,int b)
{
    cout<<"TestFun1 "<< a <<" "<< b <<endl;
}
//函数案例2
void TestFun2()
{
    cout<<"TestFun2"<<endl;
}
//函数案例3
class TestClass
{
public:
    void TestFun3(int a,int b)
    {
        cout<<"TestFun3 "<< a <<" "<< b <<endl;
    }
};

//指定帧数触发案例
class FCountTrigger
{
    DECLARE_MULTICAST_DELEGATE_TwoParams( FCountDelegate,int,int )
public:
    FCountTrigger(int InCount):Count(InCount){}

    FCountDelegate& GetDelegate()
    {
        return Delegate;
    }
    
    void Tick()
    {
        if(!NeedTick)
            return;
        
        if(Count > 0)
        {
            Count--;
        }
        else
        {
            cout<<"CountTrigger"<<endl;
            Delegate.Run(1,2);
            NeedTick = false;
        }
    }
private:
    int Count = 0;
    bool NeedTick = true;
    FCountDelegate Delegate;
};


//倒计时触发案例
class FTimerTrigger
{
    DECLARE_DELEGATE(FTimerDelegate)
public:
    FTimerTrigger(int InTime):DelayTime(InTime)
    {
        BeginTime = system_clock::now();
    }

    FTimerDelegate& GetDelegate()
    {
        return Delegate;
    }
    
    void Tick()
    {
        if(!NeedTick)
            return;

        if(duration_cast<seconds>(system_clock::now() - BeginTime).count() > DelayTime)
        {
            Delegate.Run();
            NeedTick = false;
        }
    }
private:
    int DelayTime = 0;
    system_clock::time_point BeginTime;
    FTimerDelegate Delegate;
    bool NeedTick = true;
};


int main(int argc, char* argv[])
{
    FCountTrigger CountTrigger(10);
    CountTrigger.GetDelegate().AddRaw(TestFun1);

    TestClass ClassA;
    CountTrigger.GetDelegate().AddClass(&ClassA, &TestClass::TestFun3);
    
    CountTrigger.GetDelegate().AddRaw([](int a,int b)
    {
        cout<<"TestLambda "<< a <<" "<< b <<endl;
    });

    FTimerTrigger TimerTrigger(1);
    TimerTrigger.GetDelegate().BindRaw(TestFun2);
    
    while (true)
    {
        CountTrigger.Tick();
        TimerTrigger.Tick();
        Sleep(5);
    }
    return 0;
}
