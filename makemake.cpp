/* makemake.cpp
 * C++
 * Louis Thomas - Section 104 - 02/28/16
 * 
 * Purpose: Create make files
 */

#include <iostream>
#include <fstream>
#include <dirent.h>
#include <vector>
using namespace std;

// Get the base of a file name
string baseof(string & x) {
	/*for (int i = 0; i < x.size(); i++)
		if (x[i] == '.')
			break;
		else
			y += x[i];*/
	return x.substr(0,x.find_last_of('.'));
}

// Get the extension of a file name
string extof(string & x) {
	//cout << x.substr(x.find_last_of('.') + 1,x.size()-x.find_last_of('.') - 1) << "\\";
	return x.substr(x.find_last_of('.') + 1,x.size()-x.find_last_of('.'));
}

// Find the index of an element in a vector<string>
int find(vector<string> & v, string  x) {
	int i = 0;
	for (vector<string>::iterator itr = v.begin(); itr != v.end(); itr++) {
		if (*itr == x)
			return i;
		i++;
	}
	return -1;
}

// String trailing spaces on the right end
string strip(string & x) {
	if (x[x.size() - 1] == ' ') {
		string y = x.substr(0,x.size() - 1);
		return strip(y);
	}
	else return x;
}

// Return a vector<string> of strings split by the specified character
vector<string> split(string x, char c) {
	vector<string> ret = vector<string>();
	string buf = "";
	for (unsigned i = 0; i < x.size(); i++)
		if (x[i] != c)
			buf += x[i];
		else {
			ret.push_back(buf);
			buf = "";
		}
	ret.push_back(buf);
	return ret;
}

// Return the list of headers depended on by the file and its dependencies
string dependencies(string & cpp, vector<string> & files, vector<string> & defines) {
	ifstream file;
	file.open(cpp.c_str());
//	cout << cpp; //debug
	string line, ret = "";
	while (true) {
		getline(file, line); //get every line in the file
		if (file.eof())	//if at end
			break;
		if ((line.substr(0,8) == "#define ") && (line.find_last_of(' ') == 7)) { //if we are in a header and hit a define
			defines.push_back(line.substr(8, line.size() - 8)); //then define to vector
/*			for (int i = 0; i < defines.size(); i++)
				ret += "{" + defines[i] + "}";*/ //debug
		}
		else if (line.substr(0,8) == "#ifndef " ) { //if reached an #ifndef
//			ret += "[" + line.substr(8, line.size() - 8) + "]"; //debug
			if (find(defines, line.substr(8, line.size() - 8)) != -1) //if already defined
				break;
		}
		if (line.substr(0,10) == "#include \"") { //if a custom file is included
			string header = line.substr(10,line.find_last_of('"')-10); //get the file name
//			if (ret.find(line.substr(10,line.find_last_of('"')-12) + ".cpp ") == -1) //debug
//				ret += line.substr(10,line.find_last_of('"')-12) + ".cpp ";
			if (find(files,header) != -1) {
				string dep = dependencies(header, files, defines); //get the dependencies of the specified header
//				cout << "{" + dep + "}" << endl; //debug
				if ((find(files,header) != -1) && (ret.find(header) == -1)) //if file is valid and not already in the list
					ret += header + " ";
				vector<string> splitup = split(dep, ' '); //split the string by spaces
				for (int i = 0; i < splitup.size(); i++)
					if (ret.find(splitup[i]) == -1) //if not in string
						ret += splitup[i] + " "; //add to return string
			}
		}
	}
	file.close();
	return strip(ret);
}

//Should use this one
//Returns the dependincies of a given file
string dependencies(string & cpp, vector<string> & files) {
	vector<string> defines = vector<string>();
	string ret = dependencies(cpp, files, defines);
	if (ret.find(cpp) == -1)
		ret = strip(cpp) + " " + ret;
/*	cout << "{";
	for (int i = 0; i < defines.size(); i++) 
		cout << defines[i] + ",";
	cout << "}";//*/
	return ret;
}

//Runs the program
int main(int argc, char *argv[]) {
	string compiler = "g++"; //default program switches
	vector<string> files = vector<string>();
	string out = "a.out";
	string prefix = "/usr";
	string flags = "";
	string language = "c++";
	bool debug = false;
	vector<string> languages = vector<string>();
	vector<string> extensions = vector<string>();
	vector<string> comMacros = vector<string>();
	vector<string> defComps = vector<string>();
	languages.push_back("c++");
	extensions.push_back("cpp");
	comMacros.push_back("CXX");
	defComps.push_back("g++");
	languages.push_back("c");
	extensions.push_back("c");
	comMacros.push_back("CC");
	defComps.push_back("gcc");
	int ei = 0; //c++
	for (int i = 1; i < argc; i++) { //iterate through parameters
		if (argv[i][0] == '-') {
			string conv;
			bool compilerSpecified = false;
			switch (argv[i][1]) {
			case 'c': //compiler
				conv = ((string)argv[i]); 
				if (conv.size() > 2) {//if a compiler was given
					compiler = conv.substr(3,conv.size()-3); //set compiler to text
					compilerSpecified = true;
				}
				else {
					cout << "usage: " << argv[0] << " (-c=[compiler]) files" << endl; //give usage hints
					cout << "error: 'c' specified without a parameter" << endl;
					return 0; //cut execution
				}
			break;
			case 'o': //renamed executable
				conv = ((string)argv[i]);
                                if (conv.size() > 2)
                                        out = conv.substr(3,conv.size()-3);
                                else {
                                        cout << "usage: " << argv[0] << " (-o=[executable]) files" << endl;
                                        cout << "error: 'o' specified without a parameter" << endl;
                                        return 0;
                                }
			break;
			case 'p': //installation prefix
				conv = ((string)argv[i]);
                                if (conv.size() > 2)
                                        prefix = conv.substr(3,conv.size()-3);
                                else {
                                        cout << "usage: " << argv[0] << " (-p=[bin prefix]) files" << endl;
                                        cout << "error: 'p' specified without a parameter" << endl;
                                        return 0;
                                }
			break;
			case 'f': //compiler flags (not used)
				conv = ((string)argv[i]);
                                if (conv.size() > 2)
                                        flags = conv.substr(3,conv.size()-3);
                                else {
                                        cout << "usage: " << argv[0] << " (-f=[flags]) files" << endl;
                                        cout << "error: 'f' specified without a paramter" << endl;
                                        return 0;
                                }
			break;
			case 'g': debug = true; //debug flags
			break;
			case 'l':
				  conv = ((string)argv[i]);
				  if (conv.size() > 2) {
					  int ind = find(languages, conv.substr(3,conv.size()-3));
					  if (ind != -1) {
						  language = languages[ind];
						  ei = ind;
						  if (!compilerSpecified)
							  compiler = defComps[ei];
					  }
					  else {
						  cout << "error: unknown language " + conv.substr(3,conv.size()-3) << endl;
						  return 0;
					  }
				  }	  
				  else {
					  cout << "usage: " << argv[0] << " (-l=[language]) files" << endl;
					  cout << "error: 'l' specified without a parameter" << endl;
					  return 0;
				  }
			break;
			default:
				cout << "error: unkown switch " + argv[i][0] << endl;
				return 0;
			break;
			}
		}
		else //if anything else, assume its a file and push it to the list
			files.push_back((string)argv[i]);
	}
	if (files.size() == 0) {
		cout << "usage: " << argv[0] << " files" << endl;
		cout << "error: nothing to do" << endl;
		return 0;
	}

	//cout << compiler << endl;

	/*R *dir;
	dir = opendir("./");
	dirent * drn;
	vector<string> files = vector<string>();
	while ((drn = readdir(dir)) != NULL)
		if (drn->d_name[0] != '.') 
			files.push_back(drn->d_name);
	closedir(dir);*/

	cout << ((flags != "") ? (comMacros[ei] + "FLAGS=" + flags + "\n") : "");
	cout << comMacros[ei] + "=" << compiler << " $(" + comMacros[ei] + "FLAGS)" << endl;
	cout << "DEBUG=" << ((debug)? "-g" : "") << endl;
	cout << endl;
	
	vector<string> names = vector<string>();
//	names.push_back("NULL");
	vector<string> extens = vector<string>();
//	extens.push_back("NULL");
	for (unsigned i = 0; i < files.size(); i++) { //get the names and extensions of the files
		names.push_back(baseof(files[i]));
		string e = extof(files[i]);
		extens.push_back(strip(e));
//		cout << names[i] << ":" <<  files[i] << ":" << extens[i] << "  ";
	}

	cout << ".SUFFIXES: ." + extensions[ei] + " .o"  << endl << endl; //I assume you know a thing or two about make files

	string os = ""; //as in "o's"
	for (unsigned i = 0; i < names.size(); i++) { //list the o files into a macro
		if (extens[i] == extensions[ei])
			os += names[i] + ".o" + ((i + 1 < names.size()) ? " " : "");
	}
	os = strip(os);
	cout << "OFILES= " + os << endl;
	cout << out + ": $(OFILES)" << endl;
	cout << "\t$(" + comMacros[ei] + ") $(DEBUG) $(OFILES) -o " + out << endl;
//	cout << endl;

	/*for (unsigned i = 0; i < names.size(); i++) {
		if (extens[i] == "cpp") {
			cout << endl;
			string dep = files[i];
//			cout << dep << endl; // debug
			cout << names[i] + ".o: " + dependencies(dep, files) << endl;
			cout << "\t$(CXX) -c " + files[i] << endl;
		}
	}*/
	
	cout << endl;
	cout << "clean:" << endl;
	cout << "\t-rm *.o " + out << endl;
	cout << endl;
	cout << "prefix=" << ((prefix[prefix.size()-1] == '/') ? prefix.substr(0,prefix.size()-1) : prefix) << endl << endl;
	cout << "install: " + out << endl;
	cout << "\tinstall -m 0755 " + out + " $(prefix)/bin" << endl;
	cout << endl;
	cout << "uninstall: " + out << endl;
	cout << "\trm $(prefix)/bin/" + out << endl << endl;

	for (unsigned i = 0; i < names.size(); i++) { //list the files and dependencies
//		cout << extens[i] + " == " + extensions[ei] + " " + ((extens[i] == extensions[ei]) ? "true" : "false") << endl;
		if (extens[i] == extensions[ei]) {
			string dep = files[i];
//			cout << files[i];
			cout << names[i] + ".o: " + dependencies(dep,files) << endl; //dependency(): just 30 easy steps!
		}
	}
	return 0;
}
