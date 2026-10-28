# Real-Time Face Recognition & Emotion Analysis System

A high-performance **C++17 computer vision application** that combines real-time face detection, identity recognition, and facial emotion classification in a unified video-processing pipeline.

Built with **OpenCV, CMake, and ONNX-based deep learning inference**, this project demonstrates the integration of traditional computer vision algorithms with neural networks in a compiled systems-programming environment.

The system detects faces from a live camera feed, identifies enrolled individuals using Local Binary Patterns Histograms (LBPH), and estimates facial expressions using a pretrained emotion classification model.

## Overview

The application implements a modular, multi-stage computer vision pipeline designed for real-time processing.

- **Face Detection:** Detects faces in video frames using Haar Cascade classifiers.
- **Identity Recognition:** Recognizes enrolled individuals using OpenCV's LBPH face recognizer.
- **Emotion Classification:** Estimates facial expressions using a ResNet-based FER+ model in ONNX format.
- **Live Performance Monitoring:** Displays frames-per-second (FPS) statistics during execution.
- **Dataset Collection:** Provides a dedicated utility to capture and organize facial training images.
- **Offline Model Training:** Trains and saves the face recognition model without requiring training during runtime.
- **Local Processing:** Processes camera input and stores recognition data locally.

## Demo

| Identity Recognition | Emotion Classification |
|---|---|
| ![Neutral expression and identity recognition](https://github.com/Dronquavious/real-time-face-recognition/raw/main/docs/neutral.png) | ![Happy expression classification](https://github.com/Dronquavious/real-time-face-recognition/raw/main/docs/happy.png) |
| Identifies an enrolled individual and displays the recognition result. | Predicts a facial expression from the detected face. |

*Recognition and emotion predictions depend on image quality, lighting, camera positioning, and model performance.*

## Technology Stack

| Component | Technology |
|---|---|
| Programming Language | C++17 |
| Computer Vision | OpenCV 4.x |
| Face Detection | Haar Cascade Classifiers |
| Face Recognition | LBPH Face Recognizer |
| Emotion Inference | ONNX neural network |
| Emotion Model | ResNet-based FER+ |
| Deep Learning Runtime | OpenCV DNN |
| Build System | CMake |
| Video Input | OpenCV VideoCapture |
| Dataset Management | C++ filesystem utilities |

## System Architecture

The application separates dataset collection, model training, and real-time inference into three executables.

```text
                   ┌─────────────────────┐
                   │   Webcam / Camera   │
                   └──────────┬──────────┘
                              │
                              ▼
                   ┌─────────────────────┐
                   │   Frame Acquisition │
                   └──────────┬──────────┘
                              │
                              ▼
                   ┌─────────────────────┐
                   │  Image Preprocessing│
                   │ Grayscale / Resize  │
                   └──────────┬──────────┘
                              │
                              ▼
                   ┌─────────────────────┐
                   │    Face Detection   │
                   │    Haar Cascades    │
                   └──────────┬──────────┘
                              │
                    ┌─────────┴─────────┐
                    ▼                   ▼
          ┌─────────────────┐ ┌─────────────────┐
          │ Identity        │ │ Emotion         │
          │ Recognition     │ │ Classification  │
          │ LBPH            │ │ ONNX / FER+     │
          └────────┬────────┘ └────────┬────────┘
                   │                   │
                   └─────────┬─────────┘
                             ▼
                   ┌─────────────────────┐
                   │ Result Visualization│
                   │ Labels / Confidence │
                   │ FPS / Bounding Boxes│
                   └─────────────────────┘
```

### Processing Pipeline

1. Capture a frame from the webcam.
2. Convert the frame to the appropriate image format.
3. Detect faces and extract their regions of interest.
4. Run LBPH recognition to estimate the identity of each detected face.
5. Run ONNX inference to classify the facial expression.
6. Apply configured thresholds and display the results.
7. Calculate and display runtime performance metrics.

## Project Structure

```text
real-time-face-recognition/
│
├── src/
│   ├── main.cpp
│   ├── train_model.cpp
│   └── capture_tool.cpp
│
├── assets/
│   ├── dataset/
│   │   └── <person_id>/
│   │       └── *.pgm
│   ├── face_model.yml
│   ├── emotion.onnx
│   └── haarcascade_frontalface_default.xml
│
├── docs/
│   ├── neutral.png
│   └── happy.png
│
├── CMakeLists.txt
├── .gitignore
├── LICENSE
└── README.md
```

**Note:** The structure above describes the intended organization. Exact filenames and directories may vary depending on the repository version.

## Prerequisites

Before building the project, install the following:

- A C++17-compatible compiler.
- CMake.
- OpenCV 4.x with the required modules.
- A webcam or compatible video input device.
- The pretrained emotion classification model.
- The Haar Cascade XML file if it is not bundled with the repository.

The face recognition component requires OpenCV's `face` module, commonly distributed through OpenCV Contrib. Ensure your OpenCV installation includes `opencv_face`.

### Install dependencies on macOS

Install Homebrew if it is not already available, then run:

```bash
brew install cmake opencv
```

Verify the installation:

```bash
cmake --version
pkg-config --modversion opencv4
```

If the build cannot locate the `face` module, install or build an OpenCV version that includes the relevant OpenCV Contrib components.

### Windows and Linux

On Windows, use a compatible Visual Studio C++ toolchain and an OpenCV build with the required modules.

On Ubuntu or Debian-based Linux distributions, install CMake, a C++ compiler, and an OpenCV development package. You may need to build OpenCV with OpenCV Contrib to obtain the LBPH recognizer.

## Installation and Build

### 1. Clone the repository

```bash
git clone https://github.com/Dronquavious/real-time-face-recognition.git
cd real-time-face-recognition
```

### 2. Configure the build

```bash
cmake -S . -B build
```

### 3. Compile the executables

```bash
cmake --build build --config Release
```

CMake should generate the project executables according to the targets defined in `CMakeLists.txt`.

**Important:** The commands above assume that the CMake configuration defines the targets and dependencies correctly. Executable locations and names may differ by platform and build configuration.

## Download and Configure the AI Models

The application uses two distinct model types.

### Face Detection Model

The Haar Cascade classifier is used to locate faces in each video frame.

If it is not included in the repository, obtain the official OpenCV classifier from:

[OpenCV Haar Cascade Classifiers](https://github.com/opencv/opencv/tree/4.x/data/haarcascades)

Configure the application to use the correct XML file path.

### Emotion Classification Model

The emotion classification stage uses an ONNX model based on the FER+ approach.

If the model is not included in the repository, download the appropriate pretrained model from the [ONNX Model Zoo](https://github.com/onnx/models) or the original model publisher, and place it at the path expected by the application.

For example:

```text
assets/emotion.onnx
```

The model's expected input dimensions, color format, normalization, and output labels must match the application's preprocessing and postprocessing logic.

**Model availability:** The emotion model is not necessarily included in the Git repository. Verify its source, license, expected input format, and output mapping before using it.

## Usage

The project provides three executables for the complete workflow.

### 1. Capture Facial Training Data

Run the dataset collection utility:

```bash
./build/CaptureTool
```

On Windows, use the executable generated by your selected build configuration.

**Controls**

| Key | Action |
|---|---|
| `SPACE` | Capture a facial image |
| `ESC` | Exit the capture utility |

Captured images are saved in the dataset directory.

Recommended workflow:

1. Enroll one individual at a time.
2. Capture multiple images with different expressions and lighting conditions.
3. Keep the subject's face reasonably centered and visible.
4. Store images under a consistent identity label or directory.
5. Ensure the training utility interprets those labels consistently.

### 2. Train the Face Recognition Model

After collecting the dataset, run:

```bash
./build/TrainModel
```

The training utility processes the collected images and generates the LBPH model file.

Expected output:

```text
assets/face_model.yml
```

Make sure the dataset directory structure and identity-labeling scheme match the implementation in `train_model.cpp`.

Run the training utility again whenever you add or update enrolled individuals.

### 3. Launch Real-Time Recognition

Start the main application:

```bash
./build/FaceRecApp
```

The application opens the camera feed and displays the detected faces, predicted identities, emotion labels, and performance information supported by the implementation.

| Key | Action |
|---|---|
| `ESC` | Exit the application |

If the camera does not open, verify the camera permissions and device index. If the application cannot load a model, check the configured file paths and ensure the required model files exist.

## Algorithmic Details

### 1. Face Detection — Haar Cascades

Haar Cascade classifiers use a sequence of trained image features and cascade stages to detect potential face regions.

**Advantages**
- Relatively lightweight.
- Suitable for CPU-based processing.
- Available directly through OpenCV.

**Limitations**
- Sensitive to lighting, pose, occlusion, and image quality.
- May generate false positives in challenging environments.

### 2. Identity Recognition — LBPH

Local Binary Patterns Histograms encode local grayscale texture patterns and compare their histograms against enrolled training samples.

LBPH is useful for educational face-recognition projects because it is relatively lightweight and can run without a GPU.

The system uses the recognizer's prediction and distance score to determine whether a detected face resembles an enrolled individual.

**Important:** LBPH returns a distance-based score, not a calibrated probability. A lower distance generally indicates a closer match. The appropriate rejection threshold must be established experimentally using representative images.

### 3. Emotion Classification — ONNX / FER+

The emotion classification component performs neural-network inference through OpenCV's DNN module.

The general process is:

1. Extract the detected face region.
2. Resize and normalize it according to the model's requirements.
3. Construct the neural-network input tensor.
4. Run inference using the ONNX model.
5. Interpret the output scores using the model's label mapping.
6. Display the predicted expression.

Emotion labels describe model predictions from facial appearance; they do not reliably establish a person's actual internal emotional state.

## Performance and Optimization

The application is designed to demonstrate practical considerations in real-time computer vision.

Potential optimization strategies include:

- **Frame processing:** Avoid unnecessary image copies and allocations inside the video loop.
- **Region-of-interest processing:** Run recognition and emotion inference only on detected face regions.
- **Resolution management:** Select an appropriate camera resolution to balance speed and image quality.
- **Inference efficiency:** Reuse the loaded DNN model instead of reloading it for every frame.
- **Memory management:** Use RAII and standard C++ containers to manage resources safely.
- **Performance monitoring:** Measure FPS and inference latency to identify bottlenecks.
- **Threshold tuning:** Evaluate false matches and unknown-face rejection using a separate validation dataset.

Actual performance depends on the CPU, camera resolution, number of faces, OpenCV build configuration, and inference backend. No specific FPS or accuracy guarantee is implied.

## Privacy and Responsible Use

This project processes biometric information and facial imagery. Use it responsibly.

- Obtain informed consent before collecting or enrolling facial images.
- Store datasets and trained models securely.
- Avoid committing personal face images or biometric models to public repositories.
- Provide a way to delete enrolled identities and their associated data.
- Do not use facial recognition as the sole basis for consequential decisions.
- Treat emotion predictions as uncertain classifications rather than reliable measurements of psychological state.

The LBPH model file may contain identity-related biometric representations. Local storage does not automatically guarantee privacy or security.

## Troubleshooting

| Problem | Suggested solution |
|---|---|
| OpenCV cannot be found | Check `OpenCV_DIR`, `CMAKE_PREFIX_PATH`, and the installed OpenCV configuration. |
| `cv::face` is unavailable | Install or build OpenCV with the required OpenCV Contrib face module. |
| Webcam fails to open | Check camera permissions, device availability, and the configured camera index. |
| Haar Cascade cannot be loaded | Verify the XML file path and confirm that the file exists. |
| Face model is missing | Run `TrainModel` after collecting the required dataset. |
| ONNX model fails to load | Verify the model file, ONNX operator compatibility, and OpenCV DNN support. |
| Emotion labels appear incorrect | Check input preprocessing, normalization, output dimensions, and label ordering. |
| Recognition produces false matches | Tune the LBPH rejection threshold and evaluate using varied validation samples. |
| Low FPS | Reduce frame resolution, profile each pipeline stage, and avoid redundant image processing. |

## Future Improvements

Possible extensions include:

- Replacing Haar Cascades with a modern deep-learning face detector.
- Adding face alignment before recognition.
- Introducing a more robust face embedding model for identity matching.
- Supporting configurable camera devices and inference backends.
- Adding automated performance benchmarks and latency measurements.
- Improving dataset validation and unknown-identity rejection.
- Adding unit tests and integration tests for the processing pipeline.
- Supporting GPU acceleration where the installed OpenCV build and model permit it.
- Providing explicit dataset deletion and enrollment management tools.

## Learning Outcomes

This project provides practical experience with:

- Modern C++17 development.
- CMake-based project organization and compilation.
- OpenCV image processing and video capture.
- Classical computer vision and texture-based recognition.
- ONNX neural-network inference.
- Dataset preparation and offline model training.
- Real-time pipeline design and performance measurement.
- Resource management, error handling, and modular architecture.
- Privacy considerations in biometric applications.

## License

This project is licensed under the MIT License. See the [`LICENSE`](LICENSE) file for details.

## Acknowledgements

- [OpenCV](https://opencv.org/) — Computer vision and DNN inference.
- [OpenCV Contrib](https://github.com/opencv/opencv_contrib) — Additional modules, including face recognition.
- [ONNX](https://onnx.ai/) — Open neural network interoperability format.
- [ONNX Model Zoo](https://github.com/onnx/models) — Reference pretrained models.

## Author

**Dronquavious**

- GitHub: [@Dronquavious](https://github.com/Dronquavious)
- Email: tambweamani@gmail.com

---

*Built with C++17, OpenCV, and ONNX to explore real-time computer vision, identity recognition, and neural-network inference.*
#   F a c e - D e t e c t i o n - u s i n g - O p e n - C V  
 