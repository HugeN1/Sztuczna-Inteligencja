#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <sstream>
int main(int argc, char** argv) {
    bool capturing = true;

    cv::VideoCapture cap(0);
    if (!cap.isOpened()) {
        std::cerr << "error opening frames source" << std::endl;
        return -1;
    }
    std::cout << "Video size: " << cap.get(cv::CAP_PROP_FRAME_WIDTH) //rozdzielczość
        << "x" << cap.get(cv::CAP_PROP_FRAME_HEIGHT) << std::endl;
    // wartości HSV
    int iLowH = 0;
    int iHighH = 179;

    int iLowS = 0;
    int iHighS = 255;

    int iLowV = 0;
    int iHighV = 255;

    cv::namedWindow("Control", cv::WINDOW_AUTOSIZE);
    //Create trackbars in "Control" window
    cv::createTrackbar("LowH", "Control", &iLowH, 255); //Hue (0 - 179)
    cv::createTrackbar("HighH", "Control", &iHighH, 255);

    cv::createTrackbar("LowS", "Control", &iLowS, 255); //Saturation (0 - 255)
    cv::createTrackbar("HighS", "Control", &iHighS, 255);

    cv::createTrackbar("LowV", "Control", &iLowV, 255); //Value (0 - 255)
    cv::createTrackbar("HighV", "Control", &iHighV, 255);

    int w, h;
    if (argc == 1) {
        w = 320;
        h = 200;
    }
    else {
        w = std::stoi(argv[1]);
        h = std::stoi(argv[2]);
        std::cout << w << " " << h << std::endl;
    }

    do {
        cv::Mat frame, flipped, blurred, resized, imgHSV, imgRestricted;// zmienne które trzymają obrazy macierze
        if (cap.read(frame)) { // zczytywanie pojedyńczej klatki z kamery
            cv::flip(frame, flipped, 1);// odbitka
            cv::GaussianBlur(flipped, blurred, cv::Size(5, 5), 0);
            cv::cvtColor(blurred, imgHSV, cv::COLOR_BGR2HSV); 
            cv::putText(imgHSV, std::to_string(iLowH), cv::Point(1, 25), cv::FONT_HERSHEY_DUPLEX, 1, cv::Scalar(179, 255, 255), 2, false);
            cv::putText(imgHSV, std::to_string(iHighH), cv::Point(1, 50), cv::FONT_HERSHEY_DUPLEX, 1, cv::Scalar(179, 255, 255), 2, false);
            cv::putText(imgHSV, std::to_string(iLowS), cv::Point(1, 75), cv::FONT_HERSHEY_DUPLEX, 1, cv::Scalar(179, 255, 255), 2, false);
            cv::putText(imgHSV, std::to_string(iHighS), cv::Point(1, 100), cv::FONT_HERSHEY_DUPLEX, 1, cv::Scalar(179, 255, 255), 2, false);
            cv::putText(imgHSV, std::to_string(iLowV), cv::Point(1, 125), cv::FONT_HERSHEY_DUPLEX, 1, cv::Scalar(179, 255, 255), 2, false);
            cv::putText(imgHSV, std::to_string(iHighV), cv::Point(1, 150), cv::FONT_HERSHEY_DUPLEX, 1, cv::Scalar(179, 255, 255), 2, false);
            cv::resize(blurred, resized, cv::Size(w, h), 0, 0, cv::INTER_CUBIC);
            cv::inRange(flipped, cv::Scalar(iLowH, iLowS, iLowV), cv::Scalar(iHighH, iHighS, iHighV), imgRestricted); //suwaki czarnobiały obraz
            cv::imshow("Rescaled", flipped);
            cv::imshow("Control", imgRestricted);
           cv::imshow("HSV", imgHSV);
        }
        else {
            // stream finished
            capturing = false;
        }
        //czekaj na klawisz, sprawdz czy to jest 'esc'
        int key = cv::waitKey(1000.0 / 60.0) & 0x0ff;
        if (key == 113) capturing = false;
        if (key == 120) {
            cv::Rect2d screenshot;
            cv::Mat cropped;
            screenshot = cv::selectROI("Cropping", imgRestricted, false, true);
            cropped = imgRestricted(screenshot);
            cv::imshow("Cropping", cropped);
            cv::imwrite("photo.jpg", cropped);
        }
    } while (capturing);
    return 0;
}
