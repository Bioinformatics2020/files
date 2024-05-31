#include <iostream>
#include <chrono>
#include <Windows.h>

using std::cout;
using std::endl;
using namespace std::chrono;


int TestFun1()
{
    cout<<"TestFun1"<<endl;
    return 1;
}

int TestFun2()
{
    cout<<"TestFun2"<<endl;
    return 2;
}

//指定帧数触发案例
class FCountTrigger
{
    using FunType = int(*)();
    
public:
    FCountTrigger(int InCount):Count(InCount){}

    void Bind(FunType InFun)
    {
        Fun=InFun;
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
            if(Fun)
            {
                cout<<"CountTrigger "<<Fun()<<endl;
            }
            NeedTick = false;
        }
    }
private:
    int Count = 0;
    FunType Fun = nullptr;
    bool NeedTick = true;
};


//倒计时触发案例
class FTimerTrigger
{
    using FunType = int(*)();
    
public:
    FTimerTrigger(int InTime):DelayTime(InTime)
    {
        BeginTime = system_clock::now();
    }

    void Bind(FunType InFun)
    {
        Fun=InFun;
    }
    
    void Tick()
    {
        if(!NeedTick)
            return;

        if(duration_cast<seconds>(system_clock::now() - BeginTime).count() > DelayTime)
        {
            if(Fun)
            {
                Fun();
            }
            NeedTick = false;
        }
    }
private:
    int DelayTime = 0;
    system_clock::time_point BeginTime;
    FunType Fun = nullptr;
    bool NeedTick = true;
};

int main(int argc, char* argv[])
{
    FCountTrigger CountTrigger(10);
    CountTrigger.Bind(TestFun1);

    FTimerTrigger TimerTrigger(1);
    TimerTrigger.Bind(TestFun2);
    
    while (true)
    {
        CountTrigger.Tick();
        TimerTrigger.Tick();
        Sleep(5);
    }
    return 0;
}
