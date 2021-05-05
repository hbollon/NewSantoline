#pragma once

#include <string>
#include <utility>
#include <fstream>

#include "NoConfigException.hpp"
#include "../Utils/Configuration.h"

class Epilobe
{
public:
    Epilobe();

    ~Epilobe();

    bool isConfigValid();

    Configuration *config;

    void readConfigParams();

    bool can_generate();

    void generate(const string& tifPath);

    bool is_generated();

    double direction() const;

    double force() const;

    std::pair<double, double> origin() const;

    std::pair<double, double> dimension() const;

    bool subzone(const string& tifPath);

    bool alti_to_ascii();

    bool windninja();

    void setAngle(int m_angle);

    void setSpeed(int m_speed);

private:
    bool m_generated;

    std::string m_axe;
    double m_direction;
    double m_force;
    int m_nb_process;

    std::pair<double, double> m_origin;
    std::pair<double, double> m_dimension;
};
