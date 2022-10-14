
using UnrealBuildTool;

public class MYTDS : ModuleRules
{
	public MYTDS(ReadOnlyTargetRules target) : base(target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.AddRange(new string[] { "MYTDS/Game/Weapons", "MYTDS/Game", "MYTDS/Character", "MYTDS/FunctionLibrary" });
        PublicDependencyModuleNames.AddRange(new string[] { "PhysicsCore", "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "NavigationSystem", "AIModule", "Niagara"});
    }
}
