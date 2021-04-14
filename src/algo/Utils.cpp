#include "Utils.h"

double deg_to_rad(double degrees) { return degrees * M_PI / 180.0; }

double rad_to_deg(double radian) { return radian * 180.0 / M_PI; }

double timeToDouble(std::string time){
    double result = 0;
    char str[time.size() +1];
    strcpy(str, time.c_str());
    char *tokens[time.size() +1];
    char delims[] = ":";

    int i = 0;
    tokens[i] = strtok( str, delims );
    while( tokens[i] != nullptr){
        tokens[++i] = strtok(nullptr, delims );
    }

    result = std::stod(tokens[0]) * 3600 + std::stod(tokens[1]) * 60 + std::stod(tokens[2]);

    return result;
}

//convert hh:mm:ss to second
std::string doubleToTime(double time){
    return {std::to_string((int)time/3600) + ":" + std::to_string((int)(fmod(time, 3600)/60)) + ":" + std::to_string(fmod(time, 60))};
}


double lerp(double a, double b, double c) { return (1 - c) * a + c * b; }


void progressBar(std::ostream& output,const double currentValue, const double maximumValue) {
    static const int PROGRESSBARWIDTH = 60;
    static int myProgressBarRotation = 0;
    static int myProgressBarCurrent = 0;
    // how wide you want the progress meter to be
    double fraction = currentValue /maximumValue;

    // part of the progressmeter that's already "full"
    int dotz = static_cast<int>(floor(fraction * PROGRESSBARWIDTH));
    if (dotz > PROGRESSBARWIDTH) dotz = PROGRESSBARWIDTH;

    // if the fullness hasn't changed skip display
    if (dotz == myProgressBarCurrent) return;
    myProgressBarCurrent = dotz;
    myProgressBarRotation++;

    // create the "meter"
    int ii=0;
    output << "[";
    // part  that's full already
    for ( ; ii < dotz;ii++) output<< "#";
    // remaining part (spaces)
    for ( ; ii < PROGRESSBARWIDTH;ii++) output<< " ";
    static const char* rotation_string = "|/-\\";
    myProgressBarRotation %= 4;
    output << "] " << rotation_string[myProgressBarRotation]
           << " " << (int)(fraction*100)<<"/100\r";
    output.flush();
}