#include "ModuleDemo1.h"

#define LOCTEXT_NAMESPACE "FModuleDemo1Module"

void FModuleDemo1Module::StartupModule()
{
	UE_LOG(LogTemp, Log,TEXT("FModuleDemo1Module::StartupModule"));
}

void FModuleDemo1Module::ShutdownModule()
{
	UE_LOG(LogTemp, Log,TEXT("FModuleDemo1Module::ShutdownModule"));
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FModuleDemo1Module, ModuleDemo1)