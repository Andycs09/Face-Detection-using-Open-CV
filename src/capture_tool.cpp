#include <opencv2/opencv.hpp>
#include <iostream>
#include <filesystem>
#include <thread>
#include <chrono>

namespace fs = std::filesystem;

int main() {
    // ensuring storage directory exists for the new subject
    std::string outputDir = "assets/dataset/s41";
    if (!fs::exists(outputDir)) {
        fs::create_directories(outputDir);
    }

    cv::CascadeClassifier faceCascade;
    if (!faceCascade.load("assets/haarcascade_frontalface_default.xml")) {
        std::cerr << "error loading cascade" << std::endl;
        return -1;
    }

    cv::VideoCapture cap(0);
    if (!cap.isOpened()) {
        std::cerr << "error accessing camera" << std::endl;
        return -1;
    }

    cv::Mat frame, gray;
    std::vector<cv::Rect> faces;
    int photosTaken = 0;
    int maxPhotos = 20;

    std::cout << "press 'space' to capture a photo. need " << maxPhotos << " samples." << std::endl;

    while (photosTaken < maxPhotos) {
        cap >> frame;
        if (frame.empty()) break;

        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
        cv::equalizeHist(gray, gray);

        // using same detection parameters as runtime for consistency
        faceCascade.detectMultiScale(gray, faces, 1.1, 8, 0, cv::Size(100, 100));

        // drawing feedback loop
        for (const auto& face : faces) {
            cv::rectangle(frame, face, cv::Scalar(0, 255, 0), 2);
        }

        cv::imshow("capture tool", frame);

        char key = (char)cv::waitKey(10);
        
        // space bar triggers save
        if (key == 32 && !faces.empty()) {
            // cropping the first detected face
            cv::Mat faceROI = gray(faces[0]);
            
            // resizing to match at&t dataset dimensions (92x112) ensures model stability
            cv::Mat resized;
            cv::resize(faceROI, resized, cv::Size(92, 112));

            // saving as pgm to match existing dataset format
            std::string filename = outputDir + "/" + std::to_string(photosTaken) + ".pgm";
            cv::imwrite(filename, resized);

            photosTaken++;
            std::cout << "captured image " << photosTaken << "/" << maxPhotos << std::endl;
            
            // simple visual flash effect
            cv::rectangle(frame, cv::Rect(0,0,640,480), cv::Scalar(255,255,255), -1);
            cv::imshow("capture tool", frame);
            cv::waitKey(50);
        } else if (key == 27) {
            break;
        }
    }

    return 0;
}