#include <vector>
#include <algorithm>
#include "WukongPlanner.hpp"

int _stdcall entryPoint(int *plannedSPPtr, int *completedSPPtr, int *plannedWeekStartPtr,
	int *plannedRemainingSPPtr, int *expectedDurationPtr, int *percentageCompletePtr, int *sizeOfArraysPtr,
	int *capacityPtr, int *sizeOfCapacityPtr)
{
	tools::WukongPlanner planner(std::vector<int>(plannedSPPtr, plannedSPPtr + *sizeOfArraysPtr),
		std::vector<int>(completedSPPtr, completedSPPtr + *sizeOfArraysPtr),
		std::vector<int>(plannedWeekStartPtr, plannedWeekStartPtr + *sizeOfArraysPtr),
		std::vector<int>(capacityPtr, capacityPtr + *sizeOfCapacityPtr));
	auto result = planner.buildPlan();

	int position = 0;
	std::for_each(result.begin(), result.end(), [&](const auto &item)
	{
		plannedWeekStartPtr[position] = item.m_plannedWeekStart;
		plannedRemainingSPPtr[position] = item.m_remainingSP;
		expectedDurationPtr[position] = item.m_expectedDuration;
		percentageCompletePtr[position] = item.m_percentageCompleted;
		++position;
	});
	return 0;
}
