//	This function uses double array dIn as an argument with dimensions iSizeIn
//	and puts results in double array dOut with dimensions iSizeOut
//	For this example, dIn and dOut are the same size so iSizeOut is ignored
//	But in general, dIn and dOut can have different dimensions
#include <iostream>
#include <vector>
int _stdcall entryPoint(int *plannedSPPtr, int *completedSPPtr, int *plannedWeekStartPtr,
	int *plannedRemainingSPPtr, int *expectedDurationPtr, int *percentageCompletePtr, int *sizeOfArraysPtr, 
	int *capacityPtr, int *sizeOfCapacityPtr)
{
	std::vector <int> plannedSP(plannedSPPtr, plannedSPPtr + *sizeOfArraysPtr);
	std::vector <int> completedSP(completedSPPtr, completedSPPtr + *sizeOfArraysPtr);
	std::vector <int> plannedWeekStart(plannedWeekStartPtr, plannedWeekStartPtr + *sizeOfArraysPtr);
	std::vector <int> capacity(capacityPtr, capacityPtr + *sizeOfCapacityPtr);

	if (sizeOfArraysPtr != nullptr && sizeOfCapacityPtr != nullptr)
	{
		int sizeOfArrays = *sizeOfArraysPtr;
		for (int i = 0; i < sizeOfArrays; ++i)
		{
			std::cout << "plannedSP | completedSP | plannedWeekStart" << std::endl;
			std::cout << plannedSPPtr[i] << " | " << completedSPPtr[i] << " | " << plannedWeekStartPtr[i] << std::endl;
		}
		int sizeOfCapacity = *sizeOfCapacityPtr;
		for (int i = 0; i < sizeOfCapacity; ++i)
		{
			std::cout << "Week | Capacity" << std::endl;
			std::cout << i << " | " << capacityPtr[i] << std::endl;
		}
	}
	return 0;
}
