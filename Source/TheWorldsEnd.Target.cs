// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class TheWorldsEndTarget : TargetRules
{
	public TheWorldsEndTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;

        // need to do the rest ...
        if (Target.Platform == UnrealTargetPlatform.Win32 || Target.Platform == UnrealTargetPlatform.Win64)
        {
            //bBuildAllPlugins = true;

            bUseCheckedPhysXLibraries = false;
            bUseShippingPhysXLibraries = true;
            bCompilePhysX = true;
            bCompileAPEX = true;
            bBuildRequiresCookedData = true;
            bUsesSteam = false;
            bCompileSpeedTree = true;
            bWithServerCode = false;
            bCompileWithStatsWithoutEngine = false;
            bCompileWithPluginSupport = true;
            bWithPerfCounters = false;
            bUseLoggingInShipping = false;
            bLoggingToMemoryEnabled = false;
            bUseLauncherChecks = false;
            bUseChecksInShipping = false;
            bCompileForSize = false;
            bForceCompileDevelopmentAutomationTests = false;
            bForceCompilePerformanceAutomationTests = false;
            bUseFastMonoCalls = true;
            bDisableDebugInfo = true;
            bDisableDebugInfoForGeneratedCode = true;
            bOmitPCDebugInfoInDevelopment = true;
            bEventDrivenLoader = true;
            bFormalBuild = true;

            AdditionalPlugins.Add("VoxelFarmPlugin");
        }

        ExtraModuleNames.AddRange( new string[] { "TheWorldsEnd" } );
	}
}
