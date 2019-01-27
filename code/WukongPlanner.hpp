#pragma once
#include <vector>
#include <map>
namespace tools
{
	struct PlanItem
	{
		PlanItem() = default;
		~PlanItem() = default;
		PlanItem(int plannedWeekStart, int remainingSP, int expectedDuration, int percentageCompleted);
		PlanItem(const PlanItem &) = default;
		PlanItem(PlanItem &&) = default;
		PlanItem &operator=(const PlanItem &rhs) = default;
		PlanItem &operator=(PlanItem &&rhs) = default;
		int m_plannedWeekStart;
		int m_remainingSP;
		int m_currentSP;
		int m_expectedDuration;
		int m_percentageCompleted;
	};
	class WukongPlanner
	{
	public:
		WukongPlanner() = default;
		WukongPlanner(std::vector <int> plannedSP, std::vector <int> completedSP, std::vector <int> plannedWeekStart, std::vector <int> capacityPerWeek);
		~WukongPlanner() = default;

		std::vector <PlanItem> buildPlan();

	private:
		std::vector <PlanItem> buildRemainingSPItems();
		void setFixedWeekStart(std::vector <PlanItem> &planItems) const;
		bool addUnplannedFeatureToWeek(std::vector <PlanItem> &planItems, int week);
		void rollThisWeekFeatures(std::vector <PlanItem> &planItems, int week);
		bool finalizeThisWeekFeatures(std::vector <PlanItem> &planItems, int week, int featuresCost);
		void reduceFeaturesCapacity(std::vector <PlanItem> &planItems, std::vector <int> &positions, int capacity, int week);
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
