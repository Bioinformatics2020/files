#include <cassert>
#include <iostream>
#include <chrono>
#include <Windows.h>

using std::cout;
using std::endl;
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

int TestFun1(int a,int b)
{
    cout<<"TestFun1 "<< a <<" "<< b <<endl;
    return 1;
}

void TestFun2()
{
    cout<<"TestFun2"<<endl;
}

class TestClass
{
public:
    int TestFun3(int a,int b)
    {
        cout<<"TestFun3 "<< a <<" "<< b <<endl;
        return 3;
    }
};

//指定帧数触发案例
class FCountTrigger
{
public:
    FCountTrigger(int InCount):Count(InCount){}

    TDelegateCore<int,int,int>& GetDelegate()
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
            cout<<"CountTrigger "<<Delegate.Run(1,2)<<endl;
            NeedTick = false;
        }
    }
private:
    int Count = 0;
    bool NeedTick = true;
    TDelegateCore<int,int,int> Delegate;
};


//倒计时触发案例
class FTimerTrigger
{
public:
    FTimerTrigger(int InTime):DelayTime(InTime)
    {
        BeginTime = system_clock::now();
    }

    TDelegateCore<void>& GetDelegate()
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
    TDelegateCore<void> Delegate;
    bool NeedTick = true;
};

int main(int argc, char* argv[])
{
    FCountTrigger CountTrigger(10);
    CountTrigger.GetDelegate().BindRaw(TestFun1);

    TestClass ClassA;
    FCountTrigger CountTrigger2(10);
    CountTrigger2.GetDelegate().BindClass(&ClassA, &TestClass::TestFun3);
    
    FCountTrigger CountTrigger3(10);
    CountTrigger3.GetDelegate().BindRaw([](int a,int b)->int
    {
        cout<<"TestLambda "<< a <<" "<< b <<endl;
        return 4;
    });

    FTimerTrigger TimerTrigger(1);
    TimerTrigger.GetDelegate().BindRaw(TestFun2);
    
    while (true)
    {
        CountTrigger.Tick();
        CountTrigger2.Tick();
        CountTrigger3.Tick();
        TimerTrigger.Tick();
        Sleep(5);
    }
    return 0;
}
