#include <iostream>
#include <iomanip>
#include <fstream>
#include <algorithm>
#include <string>

#include "simulator/simulator.hpp"
#include "analyzer/FileReader.hpp"
#include "analyzer/DataStructure.hpp"
#include "analyzer/UmaDataAnalyzer.hpp"
#include "linear/linear.hpp"

static void get_track_info_list(std::list<TrackInfo>& ti_list, unsigned int year){
    assert(year >= 2005);
    assert(year <= 2015);

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

		++ i;
		if(i > max_file_num) break;
	}
}

static void print_win_rate(const std::vector<std::vector<double> >& win_prob)
{
    int uma_no = 1;
    for(const auto& pv : win_prob){
        std::cout << uma_no ++ << " : " << std::endl;
        for(auto p : pv){
            std::cout << std::setw(5) << std::right << std::fixed << std::setprecision(2) << p << " ";
        }
        std::cout << std::endl;
        for(auto p : pv){
            std::cout << std::setw(5) << std::right << std::fixed << std::setprecision(2) << 1 / p << " ";
        }
        std::cout << std::endl;
    }
}

int main(int argc, char** argv){
	std::list<la::Vec2d> vec_win_list;
	std::list<la::Vec2d> vec_lose_list;

    // get track infomation
	{
		std::list<TrackInfo> ti_list;
		// Get file names
		const int target_year[] =
        {
            2006,
            2007,
            2008,
            2009,
            2010,
            2011,
            2012,
            2013,
            2014,
            2015
        };

        for(auto year : target_year){
            get_track_info_list(ti_list, year);
        }


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
        Race race {true};
        const unsigned int simulation_num(10000);

        std::string buf;
        int i = 1;
        std::cout << "input_over_all_index" << std::endl;
        std::cout << i << " :";
        while(std::cin >> buf){
            if(buf.c_str()[0] == '0') break;
            assert(std::atoi(buf.c_str()) > 20);
            assert(std::atoi(buf.c_str()) < 120);
            race.horse_list.push_back(Horse(std::atoi(buf.c_str())));
            ++ i;
            std::cout << i << " :";
        }

        std::cout << "start_simulation" << std::endl;
        Simulator simulator(simulation_num, race, vec_win_list, vec_lose_list);

        std::vector<std::vector<double> > win_prob_table;
        simulator.run(win_prob_table);

        std::cout << "result" << std::endl;
        print_win_rate(win_prob_table);
		//Race target_race = ti_list.front().race_list.front();
	}

}
