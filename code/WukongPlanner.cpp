#include "WukongPlanner.hpp"
#include <algorithm>
#include <numeric>

namespace tools
{
	WukongPlanner::WukongPlanner(std::vector <int> plannedSP, std::vector <int> completedSP, std::vector <int> plannedWeekStart, std::vector <int> capacityPerWeek)
		: m_plannedSP(std::move(plannedSP)), m_completedSP(std::move(completedSP)), m_plannedWeekStart(std::move(plannedWeekStart)), m_capacityPerWeek(std::move(capacityPerWeek)), m_itemsPerWeek(helpers::populateItemsPerWeek(m_plannedWeekStart))
	{

	}

	std::vector <PlanItem> WukongPlanner::buildRemainingSPItems()
	{
		std::vector <PlanItem> planItems;
		auto completedSPIterator = m_completedSP.begin();
		std::for_each(m_plannedSP.begin(), m_plannedSP.end(), [&](int value)
		{
			int remainingSP = value - *completedSPIterator;
			planItems.emplace_back(0, remainingSP, 0, 1 - (remainingSP / value));
			++completedSPIterator;
		});
		return std::move(planItems);
	}

	void WukongPlanner::setFixedWeekStart(std::vector <PlanItem> &planItems) const
	{
		std::for_each(m_itemsPerWeek.begin(), m_itemsPerWeek.end(), [&](const auto &item)
		{
			if (item.first != 0)
			{
				std::for_each(item.second.begin(), item.second.end(), [&](int position)
				{
					planItems[position].m_plannedWeekStart = item.first;
				});
			}
		});
	}

	std::vector <PlanItem> WukongPlanner::buildPlan()
	{
		auto planItems = buildRemainingSPItems();
		setFixedWeekStart(planItems);


		auto itemsPerWeekEndIterator = m_itemsPerWeek.end();
		int weekPosition = 0, nbOfWeeks = m_capacityPerWeek.size();

		while (weekPosition < nbOfWeeks)
		{
			auto it = m_itemsPerWeek.find(weekPosition);
			if (m_itemsPerWeek.find(weekPosition) != itemsPerWeekEndIterator)
			{
				int featuresCost = std::accumulate(it->second.begin(), it->second.end(), 0, [&](int total, int position)
				{
					return total + planItems[position].m_currentSP;
				});
				if (featuresCost == m_capacityPerWeek[weekPosition])
				{
					finalizeThisWeekFeatures(planItems, weekPosition, featuresCost);
					++weekPosition;
				}
				else if (featuresCost < m_capacityPerWeek[weekPosition])
				{
					if (addUnplannedFeatureToWeek(planItems, weekPosition))
					{
						finalizeThisWeekFeatures(planItems, weekPosition, featuresCost);
						++weekPosition;
					}
				}
				else
				{
					rollThisWeekFeatures(planItems, weekPosition);
					++weekPosition;
				}
			}
			else
			{
				if (addUnplannedFeatureToWeek(planItems, weekPosition) == true)
				{
					break;
				}
			}
		}
		return planItems;
	}

	bool WukongPlanner::finalizeThisWeekFeatures(std::vector <PlanItem> &planItems, int week, int featuresCost)
	{
		auto it = m_itemsPerWeek.find(week);
		std::for_each(it->second.begin(), it->second.end(), [&](int position)
		{
			planItems[position].m_expectedDuration = week - planItems[position].m_plannedWeekStart;
		});
		m_capacityPerWeek[week] -= featuresCost;
		return (m_capacityPerWeek[week] == 0);
	}

	void WukongPlanner::rollThisWeekFeatures(std::vector <PlanItem> &planItems, int week)
	{
		int capacity = m_capacityPerWeek[week];
		m_capacityPerWeek[week] = 0;
		
		auto it = m_itemsPerWeek.find(week);
		if (it != m_itemsPerWeek.end())
		{
			reduceFeaturesCapacity(planItems, it->second, capacity, week);
			++week;
			std::for_each(it->second.begin(), it->second.end(), [&](int position)
			{
				m_itemsPerWeek[week].push_back(position);
			});
		}
	}
	void WukongPlanner::reduceFeaturesCapacity(std::vector <PlanItem> &planItems, std::vector <int> &positions, int capacity, int week)
	{
		int nbOfFeatures = positions.size();
		int capacityPerFeature = capacity / nbOfFeatures;
		int remainingCapacity = capacity % nbOfFeatures;
		bool endedFeatures = false;
		std::map <int, int> featuresByWeekStart;

		std::for_each(positions.begin(), positions.end(), [&](int position)
		{
			featuresByWeekStart[planItems[position].m_plannedWeekStart] = position;
			if (planItems[position].m_currentSP > capacityPerFeature)
			{
				planItems[position].m_currentSP -= capacityPerFeature;
			}
			else
			{
				remainingCapacity += capacityPerFeature - planItems[position].m_currentSP;
				planItems[position].m_currentSP = 0;
				planItems[position].m_expectedDuration = week - planItems[position].m_plannedWeekStart;
				endedFeatures = true;
			}
		});
		if (remainingCapacity > 0)
		{
			auto it = featuresByWeekStart.begin();
			while (remainingCapacity > 0)
			{
				if (planItems[it->second].m_currentSP > 0)
				{
					if (planItems[it->second].m_currentSP > remainingCapacity)
					{
						planItems[it->second].m_currentSP -= remainingCapacity;
						remainingCapacity = 0;
					}
					else
					{
						remainingCapacity -= planItems[it->second].m_currentSP;
						planItems[it->second].m_currentSP = 0;
						planItems[it->second].m_expectedDuration = week - planItems[it->second].m_plannedWeekStart;
						endedFeatures = true;
					}
				}
				++it;
			}
		}
		if (endedFeatures)
		{
			positions.erase(std::remove_if(positions.begin(), positions.end(), [&](int position)
			{
				return planItems[position].m_currentSP == 0;
			}), positions.end());
		}
	}

	bool WukongPlanner::addUnplannedFeatureToWeek(std::vector <PlanItem> &planItems, int week)
	{
		auto it = m_itemsPerWeek.find(-1);
		if (m_itemsPerWeek.find(week) != m_itemsPerWeek.end())
		{
			int featurePosition = *(it->second.begin());
			m_itemsPerWeek[week].push_back(featurePosition);
			it->second.erase(it->second.begin());
			if (it->second.size() == 0)
			{
				m_itemsPerWeek.erase(it);
			}
			planItems[featurePosition].m_plannedWeekStart = week;
			return false;
		}
		return true;
	}

	PlanItem::PlanItem(int plannedWeekStart, int remainingSP, int expectedDuration, int percentageCompleted)
		: m_plannedWeekStart(plannedWeekStart), m_remainingSP(remainingSP), m_currentSP(remainingSP), m_expectedDuration(expectedDuration), m_percentageCompleted(percentageCompleted)
	{}

	namespace helpers
	{
		std::map <int, std::vector <int> > populateItemsPerWeek(const std::vector <int> &plannedWeekStart)
		{
			std::map <int, std::vector <int> > itemsPerWeek;
			int position = 0;
			std::for_each(plannedWeekStart.begin(), plannedWeekStart.end(), [&](int week)
			{
				itemsPerWeek[week].push_back(position);
				++position;
			});
			return std::move(itemsPerWeek);
		}
	}
}

