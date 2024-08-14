#include "ModuleDemo2.h"

#define LOCTEXT_NAMESPACE "FModuleDemo2Module"

void FModuleDemo2Module::StartupModule()
{
	UE_LOG(LogTemp, Log,TEXT("FModuleDemo2Module::StartupModule"));
}

void FModuleDemo2Module::ShutdownModule()
{
	UE_LOG(LogTemp, Log,TEXT("FModuleDemo2Module::StartupModule"));
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FModuleDemo2Module, ModuleDemo2)