#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>

#include "statistics.hpp"
#include "FileReader.hpp"
#include "DataStructure.hpp"
#include "UmaDataAnalyzer.hpp"
#include "linear/linear.hpp"
#include "timer/timer.h"

int main(){
	// Get file names
	int year = 2006;
	std::string csv_path("../csvfiles/");
	std::list<std::string> files;
	FileReader::get_file_name_in_dir(files, csv_path + std::to_string(year) + std::string("/"), std::string("csv"));

	// Read files
	std::list<TrackInfo> ti_list;
	for(auto name : files)
	{
		std::cout << csv_path + std::to_string(year) + std::string("/") + name << std::endl;	
		FileReader fr(csv_path + std::to_string(year) + std::string("/") + name);
		TrackInfo ti;
		fr.get_track_info(ti);
		ti_list.push_back(ti);

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
	}
	
	// compare all horses
	std::cout << "read :" << ti_list.size() << " files." << std::endl;
	std::list< std::pair<Horse, Horse> > win_list, lose_list;
	for(auto ti : ti_list){
		for(auto race : ti.race_list){
			compare_all_sample<Horse, GetWinSample>(win_list, race.horse_list);
			compare_all_sample<Horse, GetLoseSample>(lose_list, race.horse_list);
		}
	}
	std::cout << "sample num :" << win_list.size() << " " << lose_list.size() << std::endl;

	{
		std::ofstream ofs("wins.dat");
		for(auto winner : win_list){
			ofs << winner.first.curr_index << " " << winner.second.curr_index << std::endl;
		}
	}
	{
		std::ofstream ofs("loses.dat");
		for(auto loser : lose_list){
			ofs << loser.first.curr_index << " " << loser.second.curr_index << std::endl;
		}	
	}
	{
		std::ofstream ofs("3d.dat");
		for(auto winner : win_list){
			ofs << winner.first.curr_index << " " << winner.second.curr_index << " 1" << std::endl;
		}
		for(auto loser : lose_list){
			ofs << loser.first.curr_index << " " << loser.second.curr_index << " 0" << std::endl;
		}	
	}

	std::list<la::Vec2d> vec_win_list;
	std::list<la::Vec2d> vec_lose_list;
	pair_to_vec(vec_win_list, win_list);
	pair_to_vec(vec_lose_list, lose_list);
	{
		std::ofstream ofs_win("density_win.dat");
		std::ofstream ofs_lose("density_lose.dat");
		std::ofstream ofs("density.dat");
		double min_x(-20);
		double max_x(120);
		double min_y(-20);
		double max_y(120);

		double step(1);
		la::Vec2d x;
		Timer t;

		for(x[0] = min_x; x[0] < max_x; x[0] += step){

			t.start();
			for(x[1] = min_y; x[1] < max_y; x[1] += step){
				std::cout << "Estimating : " << x[0] << " " << x[1] << std::endl;

				double band_width(10);
				double ans_win = sta::KerDensityEstimation<la::Vec2d, sta::EpanechnikovKernel<la::Vec2d> >()(band_width, x, vec_win_list);
				double ans_lose = sta::KerDensityEstimation<la::Vec2d, sta::EpanechnikovKernel<la::Vec2d> >()(band_width, x, vec_lose_list);

				ofs_win << x[0] << " " << x[1] << " " << ans_win << std::endl;
				ofs_lose << x[0] << " " << x[1] << " " << ans_lose << std::endl;
				ofs << x[0] << " " << x[1] << " " << ans_win / (ans_win + ans_lose) << std::endl;
				
			}
			t.stop();
			std::cout << "Time : " << t.get_interval() << "remaining time: " <<  t.get_interval() * (max_x - x[0]) << "[sec]" << std::endl;
		}


	}
}
