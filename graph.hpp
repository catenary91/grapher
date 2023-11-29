#ifndef __GRAPH__
#define __GRAPH__

#include <string>
#include <cmath>
#include "image.hpp"
#include "parser.hpp"

class grapher {
public:
    grapher(const std::string& f) : p(f) { }
    image draw(double xmin, double xmax, double ymin, double ymax, int height, int width) {
        image img(height, width);

        for (int i=0; i<height; i++) {
            for (int j=0; j<width; j++) {
                img[i][j] = Pixel(0, 0, 0);
            }
        }

        int y_prev = -2; // [-1, height], -2 means invalid
        // graph
        for (int i=0; i<width; i++) {
            double x = (xmax - xmin) / width * i + xmin;
            double y = p.evaluate(x);
            // std::printf("x=%lf, i=%d, y=%lf, valid=%d, y_prev=%d\n", x, i, y.v, y.valid, y_prev);

            if (std::isnan(y)) {
                y_prev = -2;
                continue;
            }

            int y_pixel = std::round(height - (y - ymin) / (ymax - ymin) * height);
            if (y_pixel < 0) {
                y_prev = -1;
                continue;
            }
            else if (y_pixel >= height) {
                y_prev = height;
                continue;
            }
            img[y_pixel][i] = Pixel(255, 255, 255);

            // y interpolation
            if (i != 0 && y_prev != -2 && abs(y_pixel - y_prev)>=2) {
                int a = std::min(y_prev, y_pixel);
                int b = std::max(y_prev, y_pixel);
                for (int j=a+1; j<b; j++) {
                    img[j][i-1] = Pixel(255, 255, 255);
                }
            }
            if (y_prev == -1 || y_prev == height) y_prev = -2;
            else y_prev = y_pixel;
        }
        return img;
    }
private:
    parser p;
};


#endif