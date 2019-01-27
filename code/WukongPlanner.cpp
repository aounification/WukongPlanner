#include "WukongPlanner.hpp"
#include <algorithm>
namespace tools
{
	WukongPlanner::WukongPlanner(std::vector <int> plannedSP, std::vector <int> completedSP, std::vector <int> plannedWeekStart, std::vector <int> capacityPerWeek)
		: m_plannedSP(std::move(plannedSP)), m_completedSP(std::move(completedSP)), m_plannedWeekStart(std::move(plannedWeekStart)), m_capacityPerWeek(std::move(capacityPerWeek)), m_itemsPerWeek(helpers::populateItemsPerWeek(plannedWeekStart))
	{

	}

	namespace helpers
	{
		std::map <int, std::vector <int> > populateItemsPerWeek(const std::vector <int> &plannedWeekStart)
		{
			std::map <int, std::vector <int> > itemsPerWeek;
			int position = 0;
			std::for_each(plannedWeekStart.begin(), plannedWeekStart.end(), [&](int week)
			{
				itemsPerWeek[week].push_back(position++);
			});
			return std::move(itemsPerWeek);
		}
	}
}

