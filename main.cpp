#include <iostream>
#include <stdio.h>
#include <filesystem>
#include <cstring>
#include <string>
#include <fstream>
#include <sstream>
#include <format>
#include <vector>
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

	std:ofstream rootnote(root_note);
	rootnote.close();
}

void append_note(string note, string path) {
	// add to the notes file
	
	// firstly if the path does not exist, create it
	if (!filesystem::exists(path)) {
		filesystem::create_directories(path);
	}

	string root = path + "/root.txt";
	std::ofstream notes_files(root, ios::app);
	notes_files << note << endl;
	notes_files.close();

}

vector<string> get_project_path() {
	// recursively check for a match 
	//  you might be at "~/dir/subdir" with the root being "~/dir" 
	// it should still match or give an error
	// full path should be a substring

	filesystem::path cwd = filesystem::current_path();
	ifstream masterfile(master_file_name);

	string line;
	while (getline(masterfile, line)) {
		// check if the current path is a substring of the note root path, 
		// which is the first entry of the master file line
		
		vector<string> resp;

		istringstream iss(line);
		string word;
		iss >> word;

		resp.push_back(word);
		if (string(cwd).find(word) != std::string::npos) {
			iss >> word;
			iss >> word;

			resp.push_back(word);
			return resp;
		}
	}
	cout << "Unable to find project, did you initialize?" << endl;
	throw exception();
	
}

int main(int argc, char* argv[]) {
	preinit();

	bool verbose = isVerboseFlagPresent(argc, argv);

	if (argc == 1) {
		cout << "No command line arguments given" << endl;
		throw exception();
	}
	
	if (strcmp(argv[1], "--init") == 0) {
		if (argc < 3) {
			cout << "Unable to initialize, project name required for initialization" << endl;
			throw exception();
		}
		char* project_name = argv[2];
		init(project_name, verbose);
	}
	else if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0) {
		cout << "usage: notes [--init] [-h | --help] [-f | --folder] [-l | --list] [<notes>]" << endl << endl;
		cout << "Here are some example uses: " << endl;
		cout << "\tnotes --init     Initialize project in this directory" << endl;
		cout << "\tnotes <note>      Write note to the root project directory" << endl;
		cout << "\tnotes  -f <note>  Write note to the current project subdirectory" << endl;
		cout << "\tnotes  -l         List notes about current project" << endl;
	}
	else if (strcmp(argv[1], "--folder") == 0 || strcmp(argv[1], "-f") == 0) {
		// write about this specific folder to a dedicated note about it

		if (argc < 3) {
			cout << "not enough entries provided" << endl;
			throw exception();
		}

		vector<string> paths = get_project_path(); 
		string path = paths[0];
		string project_path = paths[1];
		
		string note = argv[2];
		for (int i = 3; i < argc; i++) {
			note += " " + string(argv[i]);
		}

		// get the current file location relative to the parent path and mimic it in the notes
		string folder_path = filesystem::current_path();
		string relative_path = filesystem::relative(folder_path, path);

		if (relative_path.compare(".") == 0) {
			// write to root
			append_note(note, project_path);
		}
		else {
			string new_path = project_path + "/" + relative_path;
			append_note(note, new_path);
		}
	}
	else if (strcmp(argv[1], "--list") == 0 || strcmp(argv[1], "-l") == 0) {
		// list all available notes
		const int line_length = 100;
		
		vector<string> paths = get_project_path(); 
		string path = paths[0];
		string project_path = paths[1];

		// list all notes including the subnotes
		for(const auto& p: std::filesystem::recursive_directory_iterator(project_path)) {
			if (!std::filesystem::is_directory(p)) {
				string note_path = filesystem::relative(p, project_path);

				string title = " Notes for " + note_path + " "; 
				int header_len = (line_length - title.size())/2;
				if (header_len < 1) { header_len = 1; }
				cout << string(header_len, '=') + title + string(header_len, '=') << endl;

				ifstream notes(p.path());
				stringstream buffer;
				buffer << notes.rdbuf();
				cout << buffer.str() << endl << endl;
			}
		    }
	}
	else {
		// if nothing is provided, then just add the note to the root file
		vector<string> paths = get_project_path();
		string notes_path = paths[1];

		string note = argv[1];
		for (int i = 2; i < argc; i++) {
			note += " " + string(argv[i]);
		}

		// write the note to the root.txt 
		append_note(note, notes_path);
	}
	return 0;
}
