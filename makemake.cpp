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
/*	cout << "{"; //debug
	for (int i = 0; i < defines.size(); i++) 
		cout << defines[i] + ",";
	cout << "}";//*/
	return ret;
}

//Runs the program
int main(int argc, char *argv[]) {
	string compiler = "g++"; //default program switches -- compiler
	vector<string> files = vector<string>();
	string out = "a.out"; //executable name
	string prefix = "/usr"; //installation prefix
	string flags = ""; //compiler flags
	string language = "c++"; //default language
	bool debug = false; //use debugging flag

	vector<string> languages = vector<string>(); //each vector contains a specification for a language
	vector<string> extensions = vector<string>(); //the index holds between each vector
	vector<string> comMacros = vector<string>();
	vector<string> defComps = vector<string>();

	languages.push_back("c++"); //c++
	extensions.push_back("cpp");
	comMacros.push_back("CXX");
	defComps.push_back("g++");

	languages.push_back("c"); //c
	extensions.push_back("c");
	comMacros.push_back("CC");
	defComps.push_back("gcc");
	int ei = 0; //default is c++
	
	for (int i = 1; i < argc; i++) { //iterate through parameters
		if (argv[i][0] == '-') {
			string conv;
			bool compilerSpecified = false;
			switch (argv[i][1]) {
			case 'c': //compiler
				conv = ((string)argv[i]); 
				if (conv.size() > 3) {//if a compiler was given
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
                                if (conv.size() > 3)
                                        out = conv.substr(3,conv.size()-3);
                                else {
                                        cout << "usage: " << argv[0] << " (-o=[executable]) files" << endl;
                                        cout << "error: 'o' specified without a parameter" << endl;
                                        return 0;
                                }
			break;
			case 'p': //installation prefix
				conv = ((string)argv[i]);
                                if (conv.size() > 3)
                                        prefix = conv.substr(3,conv.size()-3);
                                else {
                                        cout << "usage: " << argv[0] << " (-p=[bin prefix]) files" << endl;
                                        cout << "error: 'p' specified without a parameter" << endl;
                                        return 0;
                                }
			break;
			case 'f': //compiler flags (not used)
				conv = ((string)argv[i]);
                                if (conv.size() > 3)
                                        flags = conv.substr(3,conv.size()-3);
                                else {
                                        cout << "usage: " << argv[0] << " (-f=[flags]) files" << endl;
                                        cout << "error: 'f' specified without a paramter" << endl;
                                        return 0;
                                }
			break;
			case 'g': debug = true; //debug flags
			break;
			case 'l': //change language
				  conv = ((string)argv[i]);
				  if (conv.size() > 3) {
					  int ind = find(languages, conv.substr(3,conv.size()-3)); //find language if specified
					  if (ind != -1) { //if specified
						  language = languages[ind]; //set language to specification
						  ei = ind; //set extension index to specification
						  if (!compilerSpecified) //if a compiler hasn't already been specified
							  compiler = defComps[ei]; //set compiler to makemake's default
					  }
					  else { //show error, language, and quit if unknown
						  cout << "error: unknown language " + conv.substr(3,conv.size()-3) << endl;
						  return 0;
					  }
				  }	  
				  else { //if nothing was specified afterwards
					  cout << "usage: " << argv[0] << " (-l=[language]) files" << endl;
					  cout << "error: 'l' specified without a parameter" << endl;
					  return 0;
				  }
			break;
			default: //if the switch is unknown
				cout << "error: unkown switch " + argv[i][0] << endl;
				return 0;
			break;
			}
		}
		else //if anything else, assume its a file and push it to the list
			files.push_back((string)argv[i]);
	}
	if (files.size() == 0) { //terminate if there are no files given
		cout << "usage: " << argv[0] << " files" << endl;
		cout << "error: nothing to do" << endl;
		return 0;
	}

	//start by specifying compiler flags if any
	cout << ((flags != "") ? (comMacros[ei] + "FLAGS=" + flags + "\n") : "");
	//specify compiler and language macros
	cout << comMacros[ei] + "=" << compiler /*<< " $(" + comMacros[ei] + "FLAGS)"*/ << endl;
	//define the debug macro, but not necessarily give it a parameter
	cout << "DEBUG=" << ((debug)? "-g" : "") << endl;
	cout << endl;
	
	vector<string> names = vector<string>();
//	names.push_back("NULL");
	vector<string> extens = vector<string>();
//	extens.push_back("NULL");
	for (unsigned i = 0; i < files.size(); i++) { //get the names and extensions of the files
		names.push_back(baseof(files[i]));
		string e = extof(files[i]);
		extens.push_back(strip(e)); //strip -- bug on my part for editing file names wrong -- it doesn't hurt
//		cout << names[i] << ":" <<  files[i] << ":" << extens[i] << "  ";
	}

	//uses suffix rules
	cout << ".SUFFIXES: ." + extensions[ei] + " .o"  << endl << endl;

	string os = ""; //as in "o's"
	for (unsigned i = 0; i < names.size(); i++) { //list the o files into a macro
		if (extens[i] == extensions[ei])
			os += names[i] + ".o" + ((i + 1 < names.size()) ? " " : "");
	}
	os = strip(os);
	//we use the $(OFILES) to save some file space
	cout << "OFILES= " + os << endl;
	cout << out + ": $(OFILES)" << endl;
	cout << "\t$(" + comMacros[ei] + ") $(CXXFLAGS) $(DEBUG) $(OFILES) -o " + out << endl;
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
	cout << "clean:" << endl; //specify a default clean rule
	cout << "\t-rm *.o " + out << endl;
	cout << endl;
	//installation prefix
	cout << "prefix=" << ((prefix[prefix.size()-1] == '/') ? prefix.substr(0,prefix.size()-1) : prefix) << endl << endl;
	cout << "install: " + out << endl; //install rule
	cout << "\tinstall -m 0755 " + out + " $(prefix)/bin" << endl;
	cout << endl;
	cout << "uninstall: " + out << endl; //uninstall rule
	cout << "\trm $(prefix)/bin/" + out << endl << endl;

	for (unsigned i = 0; i < names.size(); i++) { //list the files and dependencies
//		cout << extens[i] + " == " + extensions[ei] + " " + ((extens[i] == extensions[ei]) ? "true" : "false") << endl;
		if (extens[i] == extensions[ei]) { //if the file suffix is eq to the specified suffix
			string dep = files[i];
//			cout << files[i];
			cout << names[i] + ".o: " + dependencies(dep,files) << endl; //dependency(): just 30 easy steps!
		}
	}
	return 0;
}
