// Copyright Epic Games, Inc. All Rights Reserved.
using UnrealBuildTool;
using System.IO;
using System;

public class SDL2 : ModuleRules
{
	protected virtual string SDL2Version { get { return "SDL-gui-backend"; } }
	protected virtual string IncRootDirectory { get { return Target.UEThirdPartySourceDirectory; } }
	protected virtual string LibRootDirectory { get { return Target.UEThirdPartySourceDirectory; } }

	protected virtual string SDL2IncPath { get { return Path.Combine(IncRootDirectory, "SDL2", SDL2Version, "include"); } }
	protected virtual string SDL2LibPath { get { return Path.Combine(LibRootDirectory, "SDL2", SDL2Version, "lib"); } }

	public SDL2(ReadOnlyTargetRules Target) : base(Target)
	{
		Type = ModuleType.External;

		// assume SDL to be built with extensions
		PublicDefinitions.Add("SDL_WITH_EPIC_EXTENSIONS=1");

		PublicIncludePaths.Add(SDL2IncPath);

		if (Target.IsInPlatformGroup(UnrealPlatformGroup.Unix))
		{
			// string linuxExtraLib = "/usr/lib/x86_64-linux-gnu/libXrender.so";
			string linuxExtraLib = Path.Combine(SDL2LibPath, "Linux", "x86_64-unknown-linux-gnu", "libXrender.so");
			PublicAdditionalLibraries.Add(linuxExtraLib);
			if (Target.Configuration == UnrealTargetConfiguration.Debug)
			{
				// Debug version should be built with -fPIC and usable in all targets
				PublicAdditionalLibraries.Add(Path.Combine(SDL2LibPath, "Linux", Target.Architecture, "libSDL2_fPIC_Debug.a"));
			}
			else if (Target.LinkType == TargetLinkType.Monolithic)
			{
				PublicAdditionalLibraries.Add(Path.Combine(SDL2LibPath, "Linux", Target.Architecture, "libSDL2.a"));
			}
			else
			{
				PublicAdditionalLibraries.Add(Path.Combine(SDL2LibPath, "Linux", Target.Architecture, "libSDL2_fPIC.a"));
			}

			
			//PublicLibraryPaths.Add("/usr/lib/x86_64-linux-gnu/");
			/*string linuxExtraLib = "/usr/lib/x86_64-linux-gnu/libXrender.so";
			RuntimeDependencies.Add(linuxExtraLib);
			
			linuxExtraLib = "/snap/gnome-42-2204/141/usr/lib/x86_64-linux-gnu/libXrender.so";
			RuntimeDependencies.Add(linuxExtraLib);
			
			linuxExtraLib = "/snap/gnome-42-2204/120/usr/lib/x86_64-linux-gnu/libXrender.so";
			RuntimeDependencies.Add(linuxExtraLib);
			
			linuxExtraLib = "/snap/gnome-3-38-2004/143/usr/lib/x86_64-linux-gnu/libXrender.so";
			RuntimeDependencies.Add(linuxExtraLib);*/
		} 
		else if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			PublicAdditionalLibraries.Add(Path.Combine(SDL2LibPath, "Win64", "SDL2.lib"));

			RuntimeDependencies.Add(Path.Combine("$(EngineDir)", "Binaries", "ThirdParty", "SDL2", "Win64/SDL2.dll"));
			PublicDelayLoadDLLs.Add("SDL2.dll");
		}
	}
}
