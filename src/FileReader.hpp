#ifndef FILE_READER_HPP
#define FILE_READER_HPP
//----------------------------------------------------------
// include
//----------------------------------------------------------
#include <string>
#include <sstream>
#include <vector>
#include <list>
#include <iostream>
#include <fstream>
#include <dirent.h>

#include "DataStructure.hpp"
#include "StringManipulator.hpp"
//----------------------------------------------------------
// FileReader 
// Read race date on csv
//----------------------------------------------------------
class FileReader
{
public:
	struct NoCell : public str_manip::Manipulator<Horse>
	{
		bool analyze(Horse* dest, const std::string& cell) const;
	};
	struct PlaceCell : public str_manip::Manipulator<Horse>
	{
		bool analyze(Horse* dest, const std::string& cell) const;
	};
	struct NameCell : public str_manip::Manipulator<Horse>
	{
		bool analyze(Horse* dest, const std::string& cell) const;
	};
	struct OAIndexCell : public str_manip::Manipulator<Horse>
	{
		bool analyze(Horse* dest, const std::string& cell) const;
	};
	struct CIndexCell : public str_manip::Manipulator<Horse>
	{
		bool analyze(Horse* dest, const std::string& cell) const;
	};
	struct PayCell : public str_manip::Manipulator<Horse>
	{
		bool analyze(Horse* dest, const std::string& cell) const;
	};

public:
	static constexpr int RACE_NUM = 12;
	static constexpr int NEW_COL = 3;

	static constexpr int DATE_ABS_ROW = 0;
	static constexpr int DATE_ABS_COL = 0;

	static constexpr int BASE_ABS_ROW = 1;
	static constexpr int BASE_ABS_COL = 0;

	static constexpr int RACE_WIDTH = 12;
	static constexpr int RACE_HEIGHT = 25;

	static constexpr int RACE_NO_ROW = 0;
	static constexpr int RACE_NO_COL = 0;
	static constexpr int DESCRIPTION_ROW = 0;
	static constexpr int DESCRIPTION_COL = 0;
	static constexpr int LEVEL_ROW = 0;
	static constexpr int LEVEL_COL = 7;
	static constexpr int RACE_NAME_ROW = 1;
	static constexpr int RACE_NAME_COL = 1;

	static constexpr int TOP_ROW = 3;
	static constexpr int BOTTOM_ROW = 21;


public:
	//----------------------------------------------------------
	// Get file path in dir
	// 		get file name in directory.
	// Parameters:
	// 		dest				search result.
	//	 	directory_path		directory path
	//		extention			the extention in target file
	//----------------------------------------------------------
	static void get_file_name_in_dir(std::list<std::string>& dest, const std::string& directory_path, const std::string& extention);

public:
	std::vector< std::vector<std::string> > data;

public:

	//----------------------------------------------------------
	// Constructor
	//----------------------------------------------------------
	FileReader(){};
	//----------------------------------------------------------
	// Constructor
	// 		Read inputed file and update data.
	// Parameters:
	// 		file name
	//----------------------------------------------------------
	FileReader(const std::string& file_name){ read(file_name); };
	//----------------------------------------------------------
	// read
	// 		Read inputed file and update data.
	// Parameters:
	// 		file name
	//----------------------------------------------------------
	void read(const std::string& file_name);
	//----------------------------------------------------------
	// get_track_info
	// 		convert std::vector< std::vector<std::string> > data
	//		property to TrackInfo
	// Parameters:
	// 		track_info 	return value
	//----------------------------------------------------------
	void get_track_info(TrackInfo& track_info) const;
	
private:
	//----------------------------------------------------------
	// analyze
	// 		called from to_track_info
	//		analyze vectored data and convert to TrackInfo
	// Parameters:
	// 		track_info 	return value
	//----------------------------------------------------------
	void analyze(TrackInfo& track_info) const;

};
#endif