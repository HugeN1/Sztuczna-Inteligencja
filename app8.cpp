#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <vector>
#include <cmath>

using namespace std;
using namespace cv;

int main(int argc, char** argv) {

    int lower_range[3] = {0,90,150} , higher_range[3] = {90,255,255};

    VideoCapture camera(0);

    if (!camera.isOpened()){
        cout<<"Cannot access camera";
    }
    else {

       do {

            Mat frame;
            Mat frameMask, frameMask2;
            Mat frameWithMask;

            camera >> frame;

            flip(frame, frame, 1);

            cvtColor(frame, frameMask, COLOR_BGR2HSV);
            inRange(frameMask, Scalar(lower_range[0], lower_range[1], lower_range[2]),
                    Scalar(higher_range[0], higher_range[1], higher_range[2]), frameMask2);
            vector<vector<Point>> contours;
            findContours(frameMask2, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);
            sort(contours.begin(), contours.end(),
                 [](auto &a, auto &b) {
                     return contourArea(a, false) > contourArea(b, false);
                 });

            Point mid[2];
            int size = contours.size();
            for (int i = 0; i < 2; i++)
            {
                drawContours(frame, contours, i, {0, 0, 255, 255});
                Rect rect = boundingRect(contours.at(i));
                mid[i].x = rect.width / 2 + rect.x;
                mid[i].y = rect.height / 2 + rect.y;

            }

            if (abs((mid[0].y - mid[1].y <= 40))) {

                line(frame, mid[0], mid[1], Scalar(200, 0, 0), 10);
            }
            imshow("IMAGE", frame);

        } while (waitKey(1) != 113);
    }
    return 0;
}
