// Copyright Epic Games, Inc. All Rights Reserved.
using System.IO;
using UnrealBuildTool;

public class UnrealOpenCV : ModuleRules
{
	public UnrealOpenCV(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				"Core",
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"Projects",
                "Slate",
                "SlateCore",
                "CoreUObject",
				//"UnrealOpenCVLibrary",
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);

		/********************************************************************************/
		// inclued、lib、dll所在的文件夹
		string OpencvPath = Path.Combine(ModuleDirectory, "../ThirdParty/OpenCV");

		string OpenCVIncludePath = Path.Combine(OpencvPath, "include");
        string OpenCVLibPath = Path.Combine(OpencvPath, "x64", "vc15", "lib");
        string OpenCVDllPath = Path.Combine(OpencvPath, "x64", "vc15", "bin");

        PublicIncludePaths.Add(OpenCVIncludePath);
        PublicIncludePaths.Add(Path.Combine(OpenCVIncludePath, "opencv2"));
		PublicLibraryPaths.Add(OpenCVLibPath);

		// 添加lib
		PublicAdditionalLibraries.Add(Path.Combine(OpenCVLibPath, "opencv_world455.lib"));
        //PublicAdditionalLibraries.Add(Path.Combine(OpenCVLibPath, "opencv_world455d.lib"));

        // 添加dll
       

        RuntimeDependencies.Add(Path.Combine(OpenCVDllPath, "opencv_videoio_ffmpeg455_64.dll"));
        RuntimeDependencies.Add(Path.Combine(OpenCVDllPath, "opencv_videoio_msmf455_64.dll"));
        //RuntimeDependencies.Add(Path.Combine(OpenCVDllPath, "opencv_videoio_msmf455_64d.dll"));
        RuntimeDependencies.Add(Path.Combine(OpenCVDllPath, "opencv_world455.dll"));
        //RuntimeDependencies.Add(Path.Combine(OpenCVDllPath, "opencv_world455d.dll"));
        PublicDelayLoadDLLs.Add(Path.Combine(OpenCVDllPath, "opencv_videoio_ffmpeg455_64.dll"));
        PublicDelayLoadDLLs.Add(Path.Combine(OpenCVDllPath, "opencv_videoio_msmf455_64.dll"));
        //PublicDelayLoadDLLs.Add(Path.Combine(OpenCVDllPath, "opencv_videoio_msmf455_64d.dll"));
        PublicDelayLoadDLLs.Add(Path.Combine(OpenCVDllPath, "opencv_world455.dll"));
        //PublicDelayLoadDLLs.Add(Path.Combine(OpenCVDllPath, "opencv_world455d.dll"));


        /********************************************************************************/

    }
}
