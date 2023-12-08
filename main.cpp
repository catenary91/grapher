#include <iostream>
#include <string>
#include <vector>

#include "graph.hpp"
#include "image.hpp"
#include "options.hpp"

using namespace std;
using namespace std::string_literals;

const char* help_msg = "\
grapher : a graphing program based on libpng\n\n\
usage:\n\
grapher [-xmin X_MIN] [-xmax X_MAX] [-ymin Y_MIN] [-ymax Y_MAX] -width WIDTH -height HEIGHT -o FILE_PATH -f \"EQUATION\"\n\n\
-xmin\t\tminimum of the domain (default is -10)\n\
-xmax\t\tmaximum of the domain (default is 10)\n\
-ymin\t\tminimum of the codomain (default is -10)\n\
-ymax\t\tmaximum of the codomain (default is 10)\n\
-width\t\twidth of the output image\n\
-height\t\theight of the output image\n\
-o\t\timage file path to write\n\
-f\t\tfunction to plot; must be an equation respect to \"x\"\n\n\
available functions:\n\n\
sin, cos, tan\t     trigonometric functions\n\
sinh, cosh, tanh     hyperbolic functions\n\
asin, acos, atan     inverse trigonometric functions\n\
asinh, acosh, atanh  inverse hyperbolic functions\n\
exp\t\t     exponential function (e^x)\n\
ln\t\t     natural logarithm\n\
sqrt\t\t     square root\n\
abs\t\t     absolute value\n\
ceil\t\t     ceiling function\n\
floor\t\t     floor function\n\
log(a, b)\t     the logarithm of b to base a\n\
max(a, b)\t     maximum value of a, b\n\
min(a, b)\t     minimum value of a, b \n\
\
";

int main(int argc, char** argv) {
	string f, path;
	double xmin = -10, xmax = 10, ymin = -10, ymax = 10;
	int height, width;
	vector<string> args;
	
	for (int i=1; i<argc; i++) {
		args.emplace_back(argv[i]);
	}

	try {
		if (args[0]=="-help") {
			cout << help_msg;
			return 0;
		}

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
	catch (string& msg) {
		cout << msg;
		cout << "\nenter \"grapher -help\" to see usages\n";
		return 1;
	}
	catch (parser_exception& ex) {
		cout << f << '\n';
		for (int i = 0; i < ex.pos; i++) cout << ' ';
		cout << '^' << ex.msg << '\n';
		return 1;
	}
	

	return 0;
}