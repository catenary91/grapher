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

            double interval_x, interval_y;
            double x = xmin, y = ymin;

            int pre_x = 0, pre_y = -1, diff;
            int pixel_x, pixel_y;

            int smaller = 0, bigger = 0;

            double value;

            int min_index = 0;
            double dif_min = fabs(x);

            interval_x = (xmax-xmin)/width;
            interval_y = (ymax-ymin)/height;

            for (int row = 0; row < width; row++) { //배경색 흰색으로 칠하기
                for (int col = 0; col < height; col++) {
                    img[row][col] = Pixel(255,255,255);
                }
            }


            for (int col = 0; col < width; col++) { //y축 긋기
                if (fabs(x) < dif_min) {
                    dif_min = fabs(x);
                    min_index = col;
                }

                x += interval_x;
            }

            for (int row = 0; row < height; row++) {
                img[row][min_index] = Pixel(0, 0, 0);
            }

            min_index = 0;
            dif_min = fabs(y);

            for (int row = 0; row < height; row++) { //y축 긋기
                if (fabs(y) < dif_min) {
                    dif_min = fabs(y);
                    min_index = row;
                }

                y += interval_y;
            }

            for (int col = 0; col < width; col++) {
                img[min_index][col] = Pixel(0, 0, 0);
            }

            //그래프 그리기
            x = xmin;
            y = ymin;

            for (pixel_x = 0; pixel_x < width; pixel_x++) {
                y = ymin;
                value = p.evaluate(x);
                if (std::isnan(value) || std::isinf(value)) { //만약 value가 부적절 하면 그 줄은 스킵
                    x += interval_x;
                    pre_x = pixel_x;
                    pre_y = -1;
                    continue;
                } else if (value > ymax) { //주어진 이미지의 크기를 벗어 나고, 그 이전의 좌표가 유효할 때
                    if (pre_y != -1) {
                        int i = pre_y;
                        while(i >= 0) {
                            img[pre_y - i][pre_x] = Pixel(0,0,0);
                            i--;
                        }
                    }

                    bigger = 1;
                    smaller = 0;
                    x += interval_x;
                    pre_x = pixel_x;
                    pre_y = -1;
                    continue;
                } else if (value < ymin) { //주어진 이미지의 크기를 벗어 나고, 그 이전의 좌표가 유효할 때
                    if (pre_y != -1) {
                        int i = pre_y;
                        while(i < height) {
                            img[i][pre_x] = Pixel(0,0,0);
                            i++;
                        }
                    }

                    smaller = 1;
                    bigger = 0;
                    x += interval_x;
                    pre_x = pixel_x;
                    pre_y = -1;
                    continue;
                }
                
                pixel_y = height - 1;
                dif_min = fabs(y-value);
                for (int row = height-1; row >= 0; row--) {
                    if (fabs(y-value) < dif_min) {
                        dif_min = fabs(y-value);    
                        pixel_y = row;
                    }
                    y += interval_y;
                }
                
                if (pre_y == -1) { //이전 좌표가 유효하지 않은 경우
                    diff = 0;
                } else { //이전 좌표가 유효한 경우 
                    diff = abs(pixel_y - pre_y);
                }
                img[pixel_y][pixel_x] = Pixel(0, 0, 0);
                
                if (diff >= 2) { //이전 좌표와 잇는다
                    if (pre_y < pixel_y) {
                        for (int i = 1; i < diff; i++) {
                            img[pre_y + i][pre_x] = Pixel(0,0,0);
                        }
                    } else {
                        for (int i = 1; i < diff; i++) {
                            img[pre_y - i][pre_x] = Pixel(0,0,0);
                        }
                    }
                }

                if (bigger == 1) { // 이전 좌표가 화면 위쪽에 있는 경우
                    int i = pixel_y;
                    while(i >= 0) {
                        img[i][pre_x] = Pixel(0,0,0);
                        i--;
                    }
                } else if (smaller == 1) { // 이전 좌표가 화면 아래쪽에 있는 경우
                    int i = pixel_y + 1;
                    while(i < height) {
                        img[i][pre_x] = Pixel(0,0,0);
                        i++;
                    }
                }

                x += interval_x;
                pre_x = pixel_x;
                pre_y = pixel_y;
                bigger = 0;
                smaller = 0;
            }
            return img;
        }
private:
    parser p;
};


#endif