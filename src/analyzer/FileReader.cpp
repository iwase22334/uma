#include "FileReader.hpp"
bool FileReader::NameCell::analyze(Horse* dest, const std::string& cell) const 
{
	dest->name = cell;
	return !cell.empty();
};
bool FileReader::NoCell::analyze(Horse* dest, const std::string& cell) const 
{
	bool res = true;
	if (cell.empty())	res = false;
	else dest->no = stoi(cell);
	return res;
};
bool FileReader::PayCell::analyze(Horse* dest, const std::string& cell) const 
{
	bool res = true;
	if (cell.empty())	res = false;
	else dest->pay_rate = stod(cell);
	return res;
};
bool FileReader::PlaceCell::analyze(Horse* dest, const std::string& cell) const 
{
	bool res = true;
	if (cell.empty())	res = false;
	else dest->place = stoi(cell);
	return res;
};
bool FileReader::OAIndexCell::analyze(Horse* dest, const std::string& cell) const 
{
	bool res = true;
	if (cell.empty())	res = false;
	else if(std::string("---") == cell)	res = false;
	else dest->over_all_index = stoi(cell);
	return res;
};
bool FileReader::CIndexCell::analyze(Horse* dest, const std::string& cell) const 
{
	bool res = true;
	if (cell.empty())	res = false;
	else if(std::string("---") == cell)	res = false;
	else dest->curr_index = stoi(cell);
	return res;
};

void FileReader::get_file_name_in_dir(std::list<std::string>& dest, const std::string& path, const std::string& extention)
{
	// Open dir
	DIR *dp;
	if ((dp = opendir(path.c_str())) == NULL){
		std::cerr << "FileReader::get_file_path_in_dir - cant open dir." << std::endl;
		exit(1);
	}
	// search *.<extention> file
	dirent* entry;
	while((entry = readdir(dp)) != NULL){
		std::string target_file(entry->d_name);

		if(target_file.size() > extention.size()){
			// Starting position of extension
			size_t ext_begin = target_file.size() - extention.size();

			// Get extention in target file
			std::string target_file_extention = target_file.substr(ext_begin, extention.size());

			// Check extention
			if(target_file_extention == extention && target_file[ext_begin - 1] == '.')
				dest.push_back(target_file);
		}
	}
	closedir(dp);
};

void FileReader::read(const std::string& file_name)
{  
	// Open file
	std::ifstream ifs(file_name); 
	if (!ifs){
		std::cout << "Error: cannot open file(" << file_name << ")" << std::endl;
		exit(1);
	}

	// Get one line
	std::string str_line;
	while(getline(ifs, str_line)){
		std::string element;
		std::stringstream stream(str_line);

		// Separated by commas
		std::vector<std::string> devided_line;
		while (getline(stream, element, ',')){
			devided_line.push_back(element);
		}
		data.push_back(devided_line);
	}
};

void FileReader::get_track_info(TrackInfo& ti) const { analyze(ti); };

void FileReader::analyze(TrackInfo& track_info) const 
{

	int curr_row = DATE_ABS_ROW;
	int curr_col = DATE_ABS_ROW;
	track_info.date = data[curr_row][curr_col];

	int base_row = BASE_ABS_ROW;
	int base_col = BASE_ABS_COL;

	for (int i = 0; i < RACE_NUM; ++i){
		Race race;

		base_row = BASE_ABS_ROW + RACE_HEIGHT * (i % 3);
		base_col = BASE_ABS_COL + RACE_WIDTH * (i / 3);

		race.no = i + 1;
		curr_row = base_row + DESCRIPTION_ROW;
		curr_col = base_col + DESCRIPTION_COL;

		// Read Races in Track on the day.
		if (!(data[curr_row][curr_col]).empty()){

			race.description = data[curr_row][curr_col];

			curr_row = base_row + LEVEL_ROW;
			curr_col = base_col + LEVEL_COL;
			if (!(data[curr_row][curr_col]).empty())
				race.level = data[curr_row][curr_col];

			curr_row = base_row + RACE_NAME_ROW;
			curr_col = base_col + RACE_NAME_COL;
			if (!(data[curr_row][curr_col]).empty())
				race.name = data[curr_row][curr_col];

			// Read Horses in race
			for (int j = 0; (!data[base_row + TOP_ROW + j][base_col].empty() && j < BOTTOM_ROW - TOP_ROW) ; ++j){
				Horse horse;
				curr_row = base_row + TOP_ROW + j;
				curr_col = base_col;

				// Get curr_line
				std::vector<std::string> curr_line;
				for(int i = 0; i < RACE_WIDTH; ++ i){
					curr_line.push_back(data[curr_row][curr_col + i]);
				}

				// analyze current line
				race.valid = str_manip::ManipulatorArray<Horse, 
					PlaceCell, NoCell, NameCell, OAIndexCell, 
					str_manip::Manipulator<Horse>, str_manip::Manipulator<Horse>, CIndexCell, str_manip::Manipulator<Horse>, 
					str_manip::Manipulator<Horse>, str_manip::Manipulator<Horse>, PayCell >().analyze(&horse, curr_line);

				race.horse_list.push_back(horse);
			}

			track_info.race_list.push_back(race);
		}
			
	}
};

