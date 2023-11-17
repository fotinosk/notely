#include <iostream>
#include <stdio.h>
#include <filesystem>
#include <cstring>
#include <string>
#include <fstream>
#include <sstream>
#include <format>
#include "utils.h"

using namespace std;

const string dir_name = ".notes";
const string home = getenv("HOME");
const string full_path = home + "/" + dir_name;
const string master_file_name = full_path + "/master.txt";

void preinit(bool verbose=false) {
	bool exists = folderExists(full_path);

	if (!exists) {
		int tries = 0;
		string x;
		cout << ".notes directory not found. If you have already configured the solution, then please check your home directory" << endl;
		
		while (tries < 3)
		{
			cout << "Would you like to continue with the initialization? [y/n]: ";
			cin >> x;

			if (x.compare("n") == 0) {
				exit(0);
			} else if (x.compare("y") == 0) {
				cout << "Initializing resources" << endl;
				filesystem::create_directory(full_path);
				std::ofstream masterfile(master_file_name);
				masterfile.close();
				cout << "Initialization complete" << endl;
				return;
			}
			else {
				++tries;
			}
		}
		exit(1);
	}

}

int projectExists(string cwd, string project_name) {
	// check the master file to ensure that the file does not already exist

	/*
	Codes:
	0 - all good, does not exist, therefore create
	1 - project already exists, same name - no need to init
	2 - project already has a dir under different name - also return the name
	*/

	std::ifstream masterfile(master_file_name);

	bool path_exists = false;

	string line;
    while ( getline(masterfile, line) ) {
        istringstream iss(line);
        
        // Process each word in the line
		int word_num = 0;
        string word;
        while (iss >> word) {
			if (word_num == 0 && cwd.compare(word) == 0) {
				path_exists = true;
			}

			if (path_exists && word_num == 1 && project_name.compare(word) == 0) {
				// project exists and has the same name
				cout << "Notes already exist for current directory, no need to initialize" << endl;
				return 1;
			} else if (path_exists && word_num == 1 && project_name.compare(word) != 0) {
				cout << "Notes already exist for current directory under a differnt project name: " << word << endl;
				return 2;
			}
			++word_num;
        }
    }

    masterfile.close();
	return 0;
}


void init(char* project_name, bool verbose=false) {
	/*
	Init with the project name
	Will create the needed metadata for the file, including the file location
	There should also be a master file for name to path routing
	*/

	// step 1 check that the file does not already exists
	// step 2 create if it does not already exist - create folder and write to master
	filesystem::path cwd = filesystem::current_path();
	
	string scwd = cwd.string();

	if (verbose) {
		cout << scwd << endl;
		cout << project_name << endl;
	}

	if (projectExists(scwd, string(project_name)) != 0) {
		cout << "Initialization failed" << endl;
		exit(1);
	}

	// create project dir, append to master file and create project root note
	const string project_dir = full_path + "/" + project_name;
	const string root_note = project_dir + "/root.txt";

	filesystem::create_directory(project_dir);

	std::ofstream masterfile(master_file_name, ios::app);
	masterfile << scwd << " " << project_name << " " << project_dir << endl;
	masterfile.close();
}


int main(int argc, char* argv[]) {
	preinit();

	bool verbose = isVerboseFlagPresent(argc, argv);
	
	if (strcmp(argv[1], "--init") == 0) {
		if (argc < 3) {
			cout << "Unable to initialize, project name required for initialization" << endl;
			throw exception();
		}
		char* project_name = argv[2];
		init(project_name, verbose);
	}
	return 0;
}