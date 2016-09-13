#include <iostream>
#include <iomanip>
#include <fstream>
#include <algorithm>
#include <string>

#include "buyer/buyer.hpp"
#include "simulator/simulator.hpp"
#include "analyzer/FileReader.hpp"
#include "analyzer/DataStructure.hpp"
#include "analyzer/UmaDataAnalyzer.hpp"
#include "linear/linear.hpp"
#include "timer/timer.h"

void get_track_info_list(std::list<TrackInfo>& ti_list, const unsigned int year){
	std::string csv_path("../csvfiles/");
	std::list<std::string> files;
	FileReader::get_file_name_in_dir(files, csv_path + std::to_string(year) + std::string("/"), std::string("csv"));

	const unsigned int max_file_num(50);
	unsigned int i(0);
	// Read files
	for(auto name : files)
	{
		std::cout << csv_path + std::to_string(year) + std::string("/") + name << std::endl;	
		FileReader fr(csv_path + std::to_string(year) + std::string("/") + name);
		TrackInfo ti;
		fr.get_track_info(ti);
		ti_list.push_back(ti);

		/*
		for(auto race : ti.race_list){
			std::cout << strout::DoubleLine();
			
			std::cout << race;
			
			std::cout << strout::DoubleLine();
			std::cout << strout::HorseDescription();

			if(race.valid){
				for(auto horse : race.horse_list){
					std::cout << horse;
				}
			}

			std::cout << strout::Line();
		}
		*/

		++ i;
		if(i > max_file_num) break;
	}
}
void simulation( const unsigned int year,
	const unsigned int buy_rate_thresh,
	const unsigned int buy_under_thresh,
	const std::list<la::Vec2d>& vec_win_list,
	const std::list<la::Vec2d>& vec_lose_list)
{
	std::list< std::pair<Race, int> > biggest_win_list;
	std::list<TrackInfo> ti_list;
	get_track_info_list(ti_list, year);

	std::string ofs_all_name = std::string("all_info_") + std::to_string(year) + std::string("_") + std::to_string(buy_rate_thresh) + std::string("_") + std::to_string(buy_under_thresh) + std::string(".dat");
	std::ofstream ofs_all(ofs_all_name.c_str());

	std::string ofs_win_name = std::string("win_info_") + std::to_string(year) + std::string("_") + std::to_string(buy_rate_thresh) + std::string("_") + std::to_string(buy_under_thresh) + std::string(".dat");
	std::string ofs_lose_name = std::string("lose_info_") + std::to_string(year) + std::string("_") + std::to_string(buy_rate_thresh) + std::string("_") + std::to_string(buy_under_thresh) + std::string(".dat");
	std::ofstream ofs_win(ofs_win_name.c_str());
	std::ofstream ofs_lose(ofs_lose_name.c_str());

	const double percentage(0.01);
	Buyer iwase(percentage, buy_rate_thresh * 0.01, buy_under_thresh * 0.01);

	int file_no = 1;
	for(auto target_file : ti_list){
		std::cout << target_file << std::endl;
		std::pair<Race, int> biggest_win(Race(), 0);
		for(auto target_race : target_file.race_list){
			if(target_race.valid){
				Timer timer;
				
				timer.start();

				const unsigned int simulation_num(10000);
				//std::cout << "simulation_start" << std::endl;
				Simulator simulator(simulation_num, target_race, vec_win_list, vec_lose_list);

				std::vector<std::vector<double> > win_prob_table;
				simulator.run(win_prob_table);
				
				timer.stop();

				int pay_sum = 0;
				int pay_out = 0;

				for(unsigned int i = 0; i < target_race.horse_list.size(); ++ i){
					
					auto it = target_race.horse_list.begin();
					std::advance(it, i);

					ofs_all << win_prob_table[i][0] << " " << (*it).pay_rate << " " << win_prob_table[i][0] * (*it).pay_rate << std::endl;

					if(iwase.buy(*it, win_prob_table[i])){

						int ticket = iwase.pay();
						pay_sum += ticket;

						if(i == 0){
							pay_out = (*it).pay_rate * ticket;
							iwase.set_return(pay_out);
							ofs_win << win_prob_table[i][0] << " " << (*it).pay_rate << " " << win_prob_table[i][0] * (*it).pay_rate << std::endl;
							if(pay_out > biggest_win.second){
								biggest_win.first = target_race;
								biggest_win.second = pay_out;
							}
						}
						else{
							ofs_lose << win_prob_table[i][0] << " " << (*it).pay_rate << " " << win_prob_table[i][0] * (*it).pay_rate << std::endl;
						}

					}
				}
				iwase.check_history();
				iwase.win_history.push_back(pay_out);
				iwase.pay_history.push_back(pay_sum);

				std::cout << "payd : " << pay_sum << "[yen]" << std::endl;
				std::cout << "money : " << iwase.money << "[yen]" << std::endl;
				std::cout << "time : " << timer.get_interval() << "[s]" << std::endl;

				//if(iwase.bankruptcy()) break;
				/*
				std::cout << strout::DoubleLine();
				for(auto horse : target_race.horse_list){
					std::cout << horse;
				}
				std::cout << strout::DoubleLine();

				std::cout << strout::DoubleLine();
				for(auto prob : win_prob_table){
					for(unsigned int i = 0; i < prob.size(); ++ i){
						std::cout << std::setw(3) << i << " ";
						std::cout << std::fixed << std::setprecision(4) << prob[i];
						std::cout << " "; 
					}
					std::cout << std::endl;
				}
				std::cout << strout::DoubleLine();
				*/
			}
			biggest_win_list.push_back(biggest_win);
		}
		std::cout << "simulated : "<< file_no << " races in " << ti_list.size() << std::endl;
		std::cout << biggest_win.first << std::endl;
		std::cout << biggest_win.second << std::endl;
		for(auto horse : biggest_win.first.horse_list){
				std::cout << horse;
		}	
		++ file_no;
	}
	{

		std::string ofs_name = std::string("money_") + std::to_string(year) + std::string("_") + std::to_string(buy_rate_thresh) + std::string("_") + std::to_string(buy_under_thresh) + std::string(".dat");
		std::ofstream ofs(ofs_name.c_str());
		unsigned int i = 0;
		for(auto a : iwase.history ){
			ofs << i << " " << a << std::endl;
			++ i;
		}
	}
	{
		std::string ofs_name = std::string("pay_") + std::to_string(year) + std::string("_") + std::to_string(buy_rate_thresh) + std::string("_") + std::to_string(buy_under_thresh) + std::string(".dat");
		std::ofstream ofs(ofs_name.c_str());
		unsigned int i = 0;
		for(auto a : iwase.pay_history ){
			ofs << i << " " << a << std::endl;
			++ i;
		}
	}
	{
		std::string ofs_name = std::string("win_") + std::to_string(year) + std::string("_") + std::to_string(buy_rate_thresh) + std::string("_") + std::to_string(buy_under_thresh) + std::string(".dat");
		std::ofstream ofs(ofs_name.c_str());
		unsigned int i = 0;
		for(auto a : iwase.win_history ){
			ofs << i << " " << a << std::endl;
			++ i;
		}
	}
	{
		std::string ofs_name = std::string("biggest_win_") + std::to_string(year) + std::string("_") + std::to_string(buy_rate_thresh) + std::string("_") + std::to_string(buy_under_thresh) + std::string(".dat");
		std::ofstream ofs(ofs_name.c_str());
		for(auto a : biggest_win_list ){
			ofs << a.second << std::endl;
			for(auto horse : a.first.horse_list){
				std::cout << horse;
			}
		}
	}
};
int main(){
	std::list<la::Vec2d> vec_win_list;
	std::list<la::Vec2d> vec_lose_list;

	{
		std::list<TrackInfo> ti_list;
		// Get file names
		//int year = 2015;
		unsigned int year = 2006;
		get_track_info_list(ti_list, year);

		// compare all horses
		std::cout << "read :" << ti_list.size() << " files." << std::endl;
		std::list< std::pair<Horse, Horse> > win_list, lose_list;
	
		for(auto ti : ti_list){
			for(auto race : ti.race_list){
				uda::compare_all_sample<Horse, uda::GetWinSample>(win_list, race.horse_list);
				uda::compare_all_sample<Horse, uda::GetLoseSample>(lose_list, race.horse_list);
			}
		}

		uda::pair_to_vec(vec_win_list, win_list);
		uda::pair_to_vec(vec_lose_list, lose_list);
	}

	{ // simulation
		unsigned int year = 2015;
		std::list<unsigned int> buy_under_thresh_list{5, 10, 20, 30};
		std::list<unsigned int> buy_rate_thresh_list{105, 200, 300, 400};

		for(auto buy_under_thresh : buy_under_thresh_list){
			for(auto buy_rate_thresh : buy_rate_thresh_list){
				simulation(year, buy_under_thresh, buy_rate_thresh,vec_win_list, vec_lose_list);
			}
		}

		
		//Race target_race = ti_list.front().race_list.front();
	}

}
