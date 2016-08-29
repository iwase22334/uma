#ifndef DATA_STRUCTURE_HPP
#define DATA_STRUCTURE_HPP

#include <iostream>
#include <list>
//----------------------------------------------------------
// Horse
// 		horse infomation
// Parameters:
//	int place 			
//	int final_pace		
//	int over_all_index	
//	int curr_index		
//	int no				The number int this race
//	int place			The place in this race
//	int popularity		
//	double pay_rate		The rate of pay out
//	std::string name			Horse name
//	std::string jockey		Jockey name
//----------------------------------------------------------
struct Horse
{
	int pace;
	int final_pace;
	int over_all_index;
	int curr_index;
	int no;
	int place;
	int popularity;
	double pay_rate;
	std::string name;
	std::string jockey;
};
//----------------------------------------------------------
// Race
// 		Race infomation
// Parameters:
//	bool valid				Is the race is valid
//	int no					Race number in the day
//	std::string name				The name of the race
//	std::string description		Race description
//	std::string level			
//	list<Horse> horse_list	
//----------------------------------------------------------
struct Race
{
	bool valid;
	int no;
	std::string name;
	std::string description;
	std::string level;
	std::list<Horse> horse_list;
};
//----------------------------------------------------------
// Race
// 		Track infomation
// Parameters:
//	std::string date;
//	list<Race> race_list;
//----------------------------------------------------------
struct TrackInfo
{
	std::string date;
	std::list<Race> race_list;
}; 

std::ostream& operator<<(std::ostream& os, const Race& r);
std::ostream& operator<<(std::ostream& os, const Horse& h);
std::ostream& operator<<(std::ostream& os, const TrackInfo& t);

namespace strout
{
	struct HorseDescription{};
	struct Line{};
	struct DoubleLine{};
};

std::ostream& operator<<(std::ostream& os, const strout::HorseDescription& h);
std::ostream& operator<<(std::ostream& os, const strout::Line& h);
std::ostream& operator<<(std::ostream& os, const strout::DoubleLine& h);

#endif