#ifndef _DRAWING_BOX_H_
#define _DRAWING_BOX_H_

#include <iostream>
#include "inc/cc_device_common.hpp"
#include <string>

using namespace std;

class DrawingBox
{
    private:

    /// @brief Static pointer instance to make this class singleton.
    static DrawingBox* m_pInstance;

    public:
    /// @brief Public static method getInstance(). This function is
    // responsible for object creation.
    static DrawingBox* getInstance();

    /// @return Drawn missing detections on the image
    bool drawBox(const string& json_file_path,const string& image_file_path);

    private:

    /// @brief Define constructor in the private section to make this class as singleton.
    DrawingBox() = default;

    /// @brief Define destructor in private section, so no one can delete the instance of this class.
    ~DrawingBox() = default;

    /// @brief Define copy constructor in the private section, so that no one can voilate the singleton policy of this class
    DrawingBox(const DrawingBox& obj){}

    /// @brief Define assignment operator in the private section, so that no one can voilate the singleton policy of this class
    void operator=(const DrawingBox& obj){}

};

#endif // End of _DRAWING_BOX_H_
