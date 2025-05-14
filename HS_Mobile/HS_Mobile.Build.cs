// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class HS_Mobile : ModuleRules
{
	public HS_Mobile(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        // 이 모듈을 사용하는 다른 모듈에서 "HS_Mobile" 경로의 헤더 파일을 사용할 수 있도록 포함시킴
        PublicIncludePaths.AddRange(new string[] { "HS_Mobile" });

        // EnhancedInput, UI 모듈을 사용하기 위해 추가
        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "UMG", "NavigationSystem" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
