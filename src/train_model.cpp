#include <opencv2/opencv.hpp>
#include <opencv2/face.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <filesystem> // C++17 Standard

namespace fs = std::filesystem;

int main() {
    // path
    std::string datasetPath = "assets/dataset";
    std::vector<cv::Mat> images;
    std::vector<int> labels;

    std::cout << "Reading dataset from: " << datasetPath << "..." << std::endl;

    // Iterate through the dataset folders
    if (!fs::exists(datasetPath)) {
        std::cerr << "Error: Dataset folder not found!" << std::endl;
        return -1;
    }

    for (const auto& entry : fs::directory_iterator(datasetPath)) {
        if (entry.is_directory()) {
            // Extract label from folder name
            std::string folderName = entry.path().filename().string();
            if (folderName[0] == 's') {
                int label = std::stoi(folderName.substr(1));

                // Loop through images inside this person's folder
                for (const auto& imgEntry : fs::directory_iterator(entry.path())) {
                    if (imgEntry.path().extension() == ".pgm") { // The dataset uses .pgm files
                        cv::Mat img = cv::imread(imgEntry.path().string(), cv::IMREAD_GRAYSCALE);
                        if (!img.empty()) {
                            images.push_back(img);
                            labels.push_back(label);
                        }
                    }
                }
            }
        }
    }

    if (images.empty()) {
        std::cerr << "Error: No images found. Check your path!" << std::endl;
        return -1;
    }

    std::cout << "Training on " << images.size() << " images..." << std::endl;

    // Create and Train the LBPH Recognizer
    cv::Ptr<cv::face::LBPHFaceRecognizer> model = cv::face::LBPHFaceRecognizer::create();
    
    // Train the model with the images and their corresponding labels
    model->train(images, labels);

    // Save to a file
    model->save("assets/face_model.yml");
    
    std::cout << "Success! Model saved to assets/face_model.yml" << std::endl;

    return 0;
}