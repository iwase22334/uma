#include "UmaDataAnalyzer.hpp" 
bool GetWinSample::compare(const Horse& h1, const Horse& h2) const
{
	return h1.place < h2.place;
};
bool GetLoseSample::compare(const Horse& h1, const Horse& h2) const
{
	return h1.place > h2.place;
};