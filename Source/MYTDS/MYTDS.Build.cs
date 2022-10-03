
using UnrealBuildTool;

public class MYTDS : ModuleRules
{
	public MYTDS(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.AddRange(new string[] { "MYTDS/Weapons", "Game", "Character", "FunctionLibrary" });
        PublicDependencyModuleNames.AddRange(new string[] { "physicsCore", "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "NavigationSystem", "AIModule", "Niagara"});
    }
}
