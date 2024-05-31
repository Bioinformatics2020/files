#include <assert.h>
#include <iostream>
#include <chrono>
#include <Windows.h>

using std::cout;
using std::endl;
using namespace std::chrono;

//委托的核心
template<typename FunReturn,typename... VarTypes>
class TDelegateCore
{
    using FunType = FunReturn(*)(VarTypes...);
public:
    void Bind(FunType InFun)
    {
        Fun=InFun;
    }
    
    FunReturn Run(VarTypes... Args)
    {
        assert(Fun);
        return Fun(Args...);
    }
private:
    FunType Fun = nullptr;
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
    CountTrigger.GetDelegate().Bind(TestFun1);

    FTimerTrigger TimerTrigger(1);
    TimerTrigger.GetDelegate().Bind(TestFun2);
    
    while (true)
    {
        CountTrigger.Tick();
        TimerTrigger.Tick();
        Sleep(5);
    }
    return 0;
}
