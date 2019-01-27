#pragma once
#include <vector>
#include <map>
namespace tools
{
	class WukongPlanner
	{
	public:
		WukongPlanner() = default;
		WukongPlanner(std::vector <int> plannedSP, std::vector <int> completedSP, std::vector <int> plannedWeekStart, std::vector <int> capacityPerWeek);
		~WukongPlanner() = default;

	private:
		std::vector <int> m_plannedSP;
		std::vector <int> m_completedSP;
		std::vector <int> m_plannedWeekStart;
		std::vector <int> m_capacityPerWeek;
		std::map <int, std::vector <int> > m_itemsPerWeek;
	};
	namespace helpers
	{
		std::map <int, std::vector <int> > populateItemsPerWeek(const std::vector <int> &plannedWeekStart);
	}
}
