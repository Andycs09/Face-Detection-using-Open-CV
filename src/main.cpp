#include <opencv2/opencv.hpp>
#include <opencv2/face.hpp>
#include <opencv2/dnn.hpp>
#include <iostream>
#include <map>

int main() {
    // initializing local binary patterns histograms for identity classification
    cv::Ptr<cv::face::LBPHFaceRecognizer> recognizer = cv::face::LBPHFaceRecognizer::create();
    try {
        recognizer->read("assets/face_model.yml");
    } catch (const cv::Exception& e) {
        std::cerr << "error loading lbph model: " << e.what() << std::endl;
        return -1;
    }

    // loading haar cascade for rapid face localization
    cv::CascadeClassifier faceCascade;
    if (!faceCascade.load("assets/haarcascade_frontalface_default.xml")) {
        std::cerr << "error loading cascade" << std::endl;
        return -1;
    }

    // loading pre-trained onnx network for emotion classification
    cv::dnn::Net emotionNet;
    try {
        emotionNet = cv::dnn::readNetFromONNX("assets/emotion-ferplus-8.onnx");
    } catch (const cv::Exception& e) {
        std::cerr << "error loading emotion network: " << e.what() << std::endl;
        return -1;
    }

    const std::vector<std::string> emotionLabels = {
        "Neutral", "Happy", "Surprise", "Sad", "Anger", "Disgust", "Fear", "Contempt"
    };

    std::map<int, std::string> names;
    names[41] = "Amani"; 

    cv::VideoCapture cap(0);
    if (!cap.isOpened()) return -1;
    
    cap.set(cv::CAP_PROP_FRAME_WIDTH, 640);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480);

    cv::Mat frame, gray;
    std::vector<cv::Rect> faces;
    const double CONFIDENCE_THRESHOLD = 80.0;

    while (true) {
        cap >> frame;
        if (frame.empty()) break;

        // converting to grayscale since lbph relies on luminance patterns
        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
        cv::equalizeHist(gray, gray);

        // increasing min neighbors to 8 reduces false positive detections
        faceCascade.detectMultiScale(gray, faces, 1.1, 8, 0, cv::Size(100, 100));

        for (const auto& face : faces) {
            cv::Mat faceROI = gray(face);

            int label = -1;
            double confidence = 0.0;
            recognizer->predict(faceROI, label, confidence);

            std::string idText = "Unknown";
            cv::Scalar boxColor = cv::Scalar(0, 0, 255);

            // filtering weak matches based on distance threshold
            if (confidence < CONFIDENCE_THRESHOLD) {
                if (names.find(label) != names.end()) {
                    idText = names[label];
                    boxColor = cv::Scalar(0, 255, 0);
                }
            }

            // converting roi to 4-dimensional blob for neural net input
            cv::Mat emotionBlob = cv::dnn::blobFromImage(faceROI, 1.0, cv::Size(64, 64), cv::Scalar(), false, false);
            emotionNet.setInput(emotionBlob);
            
            // performing forward pass to compute class probabilities
            cv::Mat prob = emotionNet.forward();
            
            // extracting index of highest probability class
            cv::Point classIdPoint;
            double confidenceValue;
            cv::minMaxLoc(prob, 0, &confidenceValue, 0, &classIdPoint);
            
            std::string emotionText = emotionLabels[classIdPoint.x];

            cv::rectangle(frame, face, boxColor, 2);
            
            cv::putText(frame, idText, cv::Point(face.x, face.y - 25),
                        cv::FONT_HERSHEY_SIMPLEX, 0.7, boxColor, 2);
            
            cv::putText(frame, emotionText, cv::Point(face.x, face.y - 5),
                        cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(0, 255, 255), 1);
        }

        cv::imshow("AI Vision System", frame);
        if (cv::waitKey(10) == 27) break;
    }

    return 0;
}