#include "PerformanceDebugSystem.h"
#include <ImGui/imgui.h>

void PerformanceDebugSystem::OnDebugUpdate(World* world, float deltaTime)
{
	ImGui::Begin("Debug Tool");

	ImGui::Text("Frame Time:");
	ImGui::Text("%.2f fps", 1 / deltaTime);
	ImGui::Text("%.2f ms", deltaTime * 1000);

	ImGui::End();
}
