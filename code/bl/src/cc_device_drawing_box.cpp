#include "inc/cc_device_drawing_box.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <nlohmann/json.hpp>
#include <iostream>
#include <fstream>
#include <string>

using json = nlohmann::json;
using namespace std;
using namespace cv;

// Bounding box module to draw misiing detections on image

DrawingBox* DrawingBox::m_pInstance = NULL_PTR;

DrawingBox* DrawingBox::getInstance()
{
    if (NULL_PTR == m_pInstance) {
        m_pInstance = new DrawingBox;
    }
    return m_pInstance;
}

bool DrawingBox::drawBox(const string& json_file_path, const string& image_file_path)
{   
    // Load the JSON data
    ifstream file(json_file_path);
    json data;
    file >> data;
    
    // Load the image
    Mat image = imread(image_file_path);
    
    // Iterate over the predictions and draw the bounding boxes
    for (const auto& prediction : data["predictions"]) {
        // Parse the coordinates and dimensions of the bounding box
        int left_x = prediction["left_x"];
        int top_y = prediction["top_y"];
        int width = prediction["width"];
        int height = prediction["height"];
    
    	// Calculate the bottom right point of the bounding box
    	Point top_left(left_x,top_y);
    	Point bottom_right(width,height);
    
    	// Draw the bounding box on the image
    	rectangle(image, top_left, bottom_right, Scalar(0,0,255), 2);

    	// Draw a filled rectangle as the background for the text
    	Size text_size = getTextSize(prediction["label"], FONT_HERSHEY_SIMPLEX, 0.9, 2,0);
    	rectangle(image, Point(left_x, top_y - text_size.height - 5), Point(left_x + text_size.width, top_y), Scalar(0,0,255), -1);
    
    	// Draw the label on the image 
    	putText(image, prediction["label"], Point(left_x, top_y - 10), FONT_HERSHEY_SIMPLEX, 0.9, Scalar(0,255,0), 2);
    }
    
    imshow("Image with Bounding Boxes", image);
    imwrite(MISSING_DETECTION_IMAGE,image);
    waitKey(0);
    destroyAllWindows();

    return true;
}
