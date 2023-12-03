#include <iostream>
#include <string>
#include <vector>

#include "graph.hpp"
#include "image.hpp"
#include "options.hpp"

using namespace std;
using namespace std::string_literals;

int main(int argc, char** argv) {
	string f, path;
	double xmin = -10, xmax = 10, ymin = -10, ymax = 10;
	int height, width;
	vector<string> args;
	
	for (int i=1; i<argc; i++) {
		args.emplace_back(argv[i]);
	}

	try {
		f = get_option<string>(args, "-f");
		path = get_option<string>(args, "-o");
		height = get_option<int>(args, "-height");
		width = get_option<int>(args, "-width");
		
		if (option_exist(args, "-xmin")) 
			xmin = get_option<double>(args, "-xmin");
		if (option_exist(args, "-xmax")) 
			xmax = get_option<double>(args, "-xmax");
		if (option_exist(args, "-ymin")) 
			ymin = get_option<double>(args, "-ymin");
		if (option_exist(args, "-ymax"))
			ymax = get_option<double>(args, "-ymax");

		if (xmin >= xmax) throw "invalid argument: xmin cannot be larger than xmax"s;
		if (ymin >= ymax) throw "invalid argument: ymin cannot be larger than ymax"s;

		if (width <= 0) throw "invalid argument: width cannot be negative"s;
		if (height <= 0) throw "invalid argument: height cannot be negative"s;

		grapher g(f);
		g.draw(xmin, xmax, ymin, ymax, height, width).save(path);
		cout << "done!\n";
	}
	catch (string msg) {
		cout << msg << '\n';
		return 1;
	}
	catch (const parser_exception& ex) {
		cout << f << '\n';
		for (int i = 0; i < ex.pos; i++) cout << ' ';
		cout << '^' << ex.msg << '\n';
		return 1;
	}
	

	return 0;
}