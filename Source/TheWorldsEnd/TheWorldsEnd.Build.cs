// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class TheWorldsEnd : ModuleRules
{
	public TheWorldsEnd(ReadOnlyTargetRules Target) : base(Target)
	{
        //Public Dependency Modules
        PublicDependencyModuleNames.Add("Core");
        PublicDependencyModuleNames.Add("CoreUObject");
        PublicDependencyModuleNames.Add("Engine");
        PublicDependencyModuleNames.Add("Slate");
        PublicDependencyModuleNames.Add("SlateCore");
        PublicDependencyModuleNames.Add("InputCore");
        PublicDependencyModuleNames.Add("RHI");
        PublicDependencyModuleNames.Add("RenderCore");
        PublicDependencyModuleNames.Add("ShaderCore");
        PublicDependencyModuleNames.Add("ProceduralMeshComponent");
        PublicDependencyModuleNames.Add("PakFile");
        PublicDependencyModuleNames.Add("Foliage");
        PublicDependencyModuleNames.Add("AssetRegistry");
        PublicDependencyModuleNames.Add("PhysX");
        PublicDependencyModuleNames.Add("APEX");
        PublicDependencyModuleNames.Add("Navmesh");
        PublicDependencyModuleNames.Add("AIModule");       

        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "UMG", "HeadMountedDisplay" });

        //Private Dependency Modules
        PrivateDependencyModuleNames.Add("Core");
        PrivateDependencyModuleNames.Add("CoreUObject");
        PrivateDependencyModuleNames.Add("Engine");
        PrivateDependencyModuleNames.Add("Slate");
        PrivateDependencyModuleNames.Add("SlateCore");
        PrivateDependencyModuleNames.Add("InputCore");
        PrivateDependencyModuleNames.Add("RHI");
        PrivateDependencyModuleNames.Add("RenderCore");
        PrivateDependencyModuleNames.Add("ShaderCore");
        PrivateDependencyModuleNames.Add("ProceduralMeshComponent");
        PrivateDependencyModuleNames.Add("PakFile");
        PrivateDependencyModuleNames.Add("Foliage");
        PrivateDependencyModuleNames.Add("AssetRegistry");
        PrivateDependencyModuleNames.Add("PhysX");
        PrivateDependencyModuleNames.Add("APEX");
        PrivateDependencyModuleNames.Add("Navmesh");
        PrivateDependencyModuleNames.Add("AIModule");

        PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using Slate UI
		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}