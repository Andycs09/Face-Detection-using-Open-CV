# Real-Time Face Recognition & Emotion Analysis System

A high-performance **C++17 computer vision application** that combines real-time face detection, identity recognition, and facial emotion classification in a unified video-processing pipeline.

Built using **OpenCV, CMake, and ONNX-based deep learning inference**, this project demonstrates the integration of traditional computer vision algorithms with neural networks in a compiled systems-programming environment.

## Overview

The application processes live camera input through a multi-stage computer vision pipeline.

- **Face Detection:** Detects faces using Haar Cascade classifiers.
- **Identity Recognition:** Recognizes enrolled individuals using Local Binary Patterns Histograms (LBPH).
- **Emotion Classification:** Estimates facial expressions using a pretrained ResNet-based FER+ model in ONNX format.
- **Performance Monitoring:** Displays real-time frames-per-second (FPS) statistics.
- **Dataset Collection:** Captures facial images for training new identities.
- **Offline Training:** Generates and saves the LBPH recognition model.
- **Local Processing:** Processes camera input and stores recognition data locally.

## Demo

| Identity Recognition | Emotion Classification |
|:---:|:---:|
| ![Identity Recognition](https://github.com/Dronquavious/real-time-face-recognition/raw/main/docs/neutral.png) | ![Emotion Classification](https://github.com/Dronquavious/real-time-face-recognition/raw/main/docs/happy.png) |
| Recognizes an enrolled individual and displays the prediction. | Classifies a facial expression as happy. |

## Technology Stack

| Component | Technology |
|---|---|
| Language | C++17 |
| Computer Vision | OpenCV 4.x |
| Face Detection | Haar Cascade Classifiers |
| Face Recognition | LBPH |
| Emotion Classification | ResNet-based FER+ |
| Model Format | ONNX |
| Inference Engine | OpenCV DNN |
| Build System | CMake |
| Video Input | OpenCV VideoCapture |

## System Architecture

The system consists of three main executables: `CaptureTool`, `TrainModel`, and `FaceRecApp`.

```text
                    Webcam / Camera
                           |
                           v
                   Frame Acquisition
                           |
                           v
                   Image Preprocessing
                    Grayscale / Resize
                           |
                           v
                     Face Detection
                    Haar Classifiers
                           |
                 +---------+---------+
                 |                   |
                 v                   v
          Identity Recognition   Emotion Classification
                 |                   |
                LBPH              ONNX / FER+
                 |                   |
                 +---------+---------+
                           |
                           v
                   Result Visualization
                  Labels / Bounding Boxes
                       FPS Display
```

### Processing Pipeline

1. Capture frames from the camera.
2. Preprocess each frame for face detection.
3. Detect faces and extract their regions of interest.
4. Predict identities using the trained LBPH model.
5. Classify facial expressions using the ONNX model.
6. Display predictions and performance information.

## Project Structure

```text
real-time-face-recognition/
├── src/
│   ├── main.cpp
│   ├── train_model.cpp
│   └── capture_tool.cpp
├── assets/
│   ├── dataset/
│   ├── face_model.yml
│   ├── emotion.onnx
│   └── haarcascade_frontalface_default.xml
├── docs/
│   ├── neutral.png
│   └── happy.png
├── CMakeLists.txt
├── .gitignore
├── LICENSE
└── README.md
```

*Note: File names and directory locations should match the actual repository implementation.*

## Prerequisites

- C++17-compatible compiler
- CMake
- OpenCV 4.x
- OpenCV Contrib `face` module for LBPH recognition
- Webcam or compatible camera
- Pretrained ONNX emotion classification model

### Install Dependencies on macOS

Install CMake and OpenCV using Homebrew:

```bash
brew install cmake opencv
```

Verify the installations:

```bash
cmake --version
pkg-config --modversion opencv4
```

Ensure your OpenCV installation includes the `opencv_face` module.

## Build Instructions

### 1. Clone the Repository

```bash
git clone https://github.com/Dronquavious/real-time-face-recognition.git
cd real-time-face-recognition
```

### 2. Configure the Project

```bash
cmake -S . -B build
```

### 3. Compile the Application

```bash
cmake --build build --config Release
```

The resulting executable paths depend on the platform and CMake configuration.

## Model Setup

### Face Detection

The application uses Haar Cascade classifiers to detect faces.

If the classifier is not included in the repository, download it from the official OpenCV repository:

[OpenCV Haar Cascade Classifiers](https://github.com/opencv/opencv/tree/4.x/data/haarcascades)

Configure the application to load the correct XML file.

### Emotion Classification

The emotion detection component requires a compatible pretrained ONNX model.

Place the model in the expected location, for example:

```text
assets/emotion.onnx
```

The model may need to be downloaded separately if it is excluded from version control. Ensure its input dimensions, preprocessing requirements, and output labels match the application code.

## Usage

The project follows a three-stage workflow: dataset collection, model training, and real-time inference.

### 1. Capture Training Images

Run the dataset collection utility:

```bash
./build/CaptureTool
```

**Controls**

- `SPACE` — Capture an image.
- `ESC` — Exit the utility.

Captured images are stored in the configured dataset directory.

Collect multiple images per person under different lighting conditions and facial poses for better recognition performance.

### 2. Train the Recognition Model

After collecting training images, run:

```bash
./build/TrainModel
```

The utility processes the dataset and saves the trained LBPH model, typically to:

```text
assets/face_model.yml
```

Run the training utility again whenever the dataset changes.

### 3. Start Real-Time Recognition

Launch the main application:

```bash
./build/FaceRecApp
```

The application captures live camera frames and displays face detection, identity predictions, emotion classifications, and FPS information supported by the implementation.

**Controls**

- `ESC` — Quit the application.

Ensure the required model files exist and the camera is accessible before starting the application.

## Algorithms Explained

### Haar Cascade Face Detection

Haar Cascades use learned image features and a cascade of classifiers to detect potential faces.

**Advantages**
- Lightweight and relatively fast.
- Suitable for CPU-based processing.
- Supported directly by OpenCV.

**Limitations**
- Sensitive to lighting, pose, and occlusion.
- Can produce false detections in challenging conditions.

### LBPH Face Recognition

Local Binary Patterns Histograms represent local facial texture patterns and compare the resulting histograms with enrolled training samples.

LBPH is computationally efficient and suitable for lightweight face-recognition demonstrations.

The prediction distance is not a probability score. Lower distances generally indicate closer matches, and an appropriate threshold is needed to reject unknown identities.

### ONNX-Based Emotion Classification

The emotion classification pipeline uses a pretrained neural network through OpenCV DNN.

The general workflow is:

1. Extract the detected face region.
2. Resize and normalize the image.
3. Prepare the model input tensor.
4. Run neural-network inference.
5. Interpret the output scores using the model's label mapping.
6. Display the predicted expression.

Emotion predictions are estimates based on facial appearance and should not be interpreted as reliable measurements of a person's internal emotional state.

## Performance Considerations

The application demonstrates several practical optimization techniques and opportunities:

- Reuse loaded models rather than loading them for every frame.
- Minimize unnecessary image copies and memory allocations.
- Process detected face regions instead of entire frames where appropriate.
- Adjust camera resolution to balance processing speed and image quality.
- Measure FPS and inference latency to identify bottlenecks.
- Tune recognition thresholds using representative validation data.

Actual performance depends on hardware, camera resolution, the number of detected faces, and the OpenCV build configuration.

## Privacy and Responsible Use

This project processes facial images and biometric information.

- Obtain consent before collecting facial data.
- Store datasets and trained models securely.
- Avoid committing personal images or biometric models to public repositories.
- Delete biometric data when it is no longer required.
- Do not use recognition or emotion predictions as the sole basis for consequential decisions.

Local processing does not automatically guarantee data security.

## Troubleshooting

| Problem | Possible Solution |
|---|---|
| OpenCV not found | Verify the OpenCV installation and CMake configuration. |
| `cv::face` unavailable | Install or build OpenCV with the required Contrib face module. |
| Camera fails to open | Check device availability, permissions, and camera index. |
| Face model missing | Collect training data and run `TrainModel`. |
| ONNX model fails to load | Verify the model file and OpenCV DNN compatibility. |
| Incorrect emotion predictions | Check preprocessing, normalization, and output-label mapping. |
| False identity matches | Tune the LBPH threshold and test with unknown individuals. |
| Low FPS | Reduce resolution and profile individual processing stages. |

## Future Improvements

- Integrate a modern deep-learning face detector.
- Add face alignment and improved recognition models.
- Improve unknown-person rejection and threshold calibration.
- Introduce automated accuracy and performance benchmarks.
- Add unit and integration tests.
- Support configurable cameras and inference backends.
- Improve enrollment management and biometric data deletion.
- Explore GPU acceleration where supported.

## Learning Outcomes

This project explores:

- Modern C++17 development
- Computer vision using OpenCV
- Image processing and video capture
- Classical face recognition algorithms
- ONNX-based deep learning inference
- Dataset preparation and model training
- Real-time pipeline architecture
- Performance measurement and optimization
- Resource management and modular software design

## License

This project is licensed under the **MIT License**. See the [LICENSE](LICENSE) file for details.

## Acknowledgements

- [OpenCV](https://opencv.org/) — Computer vision and deep learning inference.
- [OpenCV Contrib](https://github.com/opencv/opencv_contrib) — Additional modules, including face recognition.
- [ONNX](https://onnx.ai/) — Open neural network interoperability format.
- [ONNX Model Zoo](https://github.com/onnx/models) — Reference pretrained models.

## Author

**Dronquavious**

- GitHub: [@Dronquavious](https://github.com/Dronquavious)
- Email: [tambweamani@gmail.com](mailto:tambweamani@gmail.com)

---

*Built with C++17, OpenCV, and ONNX for real-time computer vision and AI experimentation.*
