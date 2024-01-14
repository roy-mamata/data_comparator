// Code Specific Header File(s)
#include "inc/cc_device_data_aggregator_handler.hpp"
#include <nlohmann/json.hpp>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <unordered_set>

using json = nlohmann::json;
using namespace std;

dataAggregatorHandler* dataAggregatorHandler::m_pInstance = NULL_PTR;

dataAggregatorHandler* dataAggregatorHandler::getInstance()
{
    if (NULL_PTR == m_pInstance) {
        m_pInstance = new dataAggregatorHandler;
    }
    return m_pInstance;
}

bool dataAggregatorHandler::dataAggregator_output()
{
    
    ifstream read_adas(file_path1);
    ifstream read_bigmodel(file_path2);
    
    json jsonData1;
    read_adas >> jsonData1;

    json jsonData2;
    read_bigmodel >> jsonData2;

    int Data2_size = jsonData2["predictions"].size();

    // Remove predictions of jsonContent1 with confidence < 70
    if (jsonData1.contains("predictions") && jsonData1["predictions"].is_array()) {
        auto& predictionsArray1 = jsonData1["predictions"];
        
        // Use erase-remove_if idiom with lambda function
        predictionsArray1.erase(
            std::remove_if(predictionsArray1.begin(), predictionsArray1.end(),
                [](const nlohmann::json& prediction) {
                    return prediction.value("confidence", 0) < CONFIDENCE_THRESHOLD;
                }
            ),
            predictionsArray1.end()
        );
    }

    // Remove predictions of jsonContent2 with confidence < 70
    if (jsonData2.contains("predictions") && jsonData2["predictions"].is_array()) {
        auto& predictionsArray2 = jsonData2["predictions"];
        
        // Use erase-remove_if idiom with lambda function
        predictionsArray2.erase(
            std::remove_if(predictionsArray2.begin(), predictionsArray2.end(),
                [](const nlohmann::json& prediction) {
                    return prediction.value("confidence", 0) < CONFIDENCE_THRESHOLD;
                }
            ),
            predictionsArray2.end()
        );
    }

   // Print the updated JSON content
    std::cout << jsonData1.dump(4) << std::endl;
    std::cout << jsonData2.dump(4) << std::endl; 

    // Set to store unique labels
    unordered_set<string> uniqueLabels1;
    unordered_set<string> uniqueLabels2;

    // Extract values of the "label" key and add them to the set
    if (jsonData1.contains("predictions") && jsonData1["predictions"].is_array()) {
        for (const auto& prediction : jsonData1["predictions"]) {
            if (prediction.contains("label") && prediction["label"].is_string()) {
                uniqueLabels1.insert(prediction["label"].get<string>());
            }
        }
    }

    // Extract values of the "label" key and add them to the set
    if (jsonData2.contains("predictions") && jsonData2["predictions"].is_array()) {
        for (const auto& prediction : jsonData2["predictions"]) {
            if (prediction.contains("label") && prediction["label"].is_string()) {
                uniqueLabels2.insert(prediction["label"].get<string>());
            }
        }
    }

    // Print unique labels
    cout << "Unique Labels1:\n";
    for (const auto& label : uniqueLabels1) {
        cout << label << std::endl;
    }

    cout<< endl;

    cout << "Unique Labels2:\n";
    for (const auto& label : uniqueLabels2) {
        cout << label << std::endl;
    }

    // Difference in the unique labels
    unordered_set<string> differenceSet;

    for (const auto& element : uniqueLabels2) {
        // Check if the element is not in set2
        if (uniqueLabels1.find(element) == uniqueLabels1.end()) {
            differenceSet.insert(element);
        }
    }

    // Print the difference
    cout<<endl;
    cout<<"The differnce in labels: ";
    for (const auto& element : differenceSet) {
        cout << element << " ";
    }
    cout << std::endl;

    int diff_count=differenceSet.size();

     // Convert unordered set to array
    vector<string> diff_of_detection(differenceSet.size());
    std::copy(differenceSet.begin(), differenceSet.end(), diff_of_detection.begin());

    string missing_detections;

    for(int i = 0; i < diff_count; i++){
        for(int j = 0; j < Data2_size; j++){
            if (diff_of_detection[i] == jsonData2["predictions"][j]["label"]) {
                // Append the JSON representation of the matching prediction to missing_detections
                missing_detections += jsonData2["predictions"][j].dump() + ",";
            
                // Add a comma if it's not the last element in the array
                // WHY THIS DIDNT WORK
                /* if (!(i == diff_count - 1 && j == Data2_size - 1)) {
                    missing_detections += ",";
                } */
            }
        }
    }

    // Remove the last trailing comma
    if (!missing_detections.empty() && missing_detections.back() == ',') {
        missing_detections.pop_back();
    }
    // Before formatting
    cout << "Intermediate Missing Detections: " << missing_detections << std::endl;
    cout<<endl;

    // formatting the json data
    missing_detections = "{\"predictions\":[" + missing_detections + "], \"success\":true}";
    cout << "Missing Detections: " << missing_detections << std::endl;
    
    // writing difference in data to text file
    ofstream file("../data/missing_detection.json");
    file << missing_detections;
    file.close(); 

    // Drawing bounding box
    DrawingBox* draw = DrawingBox::getInstance();
    draw->drawBox(MISSING_DETECTION_PATH,INPUT_IMAGE); 

    return true;

}
