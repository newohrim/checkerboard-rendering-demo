#include "configuration.h"

void configuration::init()
{
    in.open(path);
    in.exceptions(std::ifstream::badbit);

    try 
    {
        if (!in.is_open())
            cout << "CONFIG::ERROR Unable to open configuration file." << endl;
        string field;
        while (getline(in, field))
        {
            vector<string> parts = split(field);
            config_data[parts[0]] = stoi(parts[1]);
        }
    }
    catch (std::ifstream::failure & e)
    {
        cout << "CONFIG::ERROR Configuration file format exception." << endl;
    }
    catch (std::invalid_argument & e)
    {
        cout << "CONFIG::ERROR Configuration file format exception." << endl;
    }
    in.close();
    check_values();

    WINDOW_WIDTH = config_data["window_width"];
    WINDOW_HEIGHT = config_data["window_height"];
}

vector<string> configuration::split(const string& sequence)
{
    vector<string> words;
    string word = "";
    for (int i = 0; i < sequence.size(); i++)
    {
        if (sequence[i] != ' ' && i == sequence.size() - 1) {
            word += sequence[i];
            words.push_back(word);
        }
        else if (sequence[i] != ' ')
            word += sequence[i];
        else {
            words.push_back(word);
            word = "";
        }
    }

    return words;
}

void configuration::check_values()
{
    int value = config_data["window_width"];
    if (value < MIN_WINDOW_WIDTH || value > MAX_WINDOW_WIDTH || value % 2 == 1) 
    {
        std::cout << "CONFIG::ERROR window_width must be an even value at least " << MIN_WINDOW_WIDTH <<
            " and at most " << MAX_WINDOW_WIDTH << endl;
        config_data["window_width"] = DEFFAULT_WINDOW_WIDTH;
    }
    value = config_data["window_height"];
    if (value < MIN_WINDOW_HEIGHT || value > MAX_WINDOW_HEIGHT || value % 2 == 1)
    {
        std::cout << "CONFIG::ERROR window_height must be an even value at least " << MIN_WINDOW_HEIGHT <<
            " and at most " << MAX_WINDOW_HEIGHT << endl;
        config_data["window_height"] = DEFFAULT_WINDOW_HEIGHT;
    }
    value = config_data["cube_rotation_speed"];
    if (value < MIN_ROTATION_SPEED || value > MAX_ROTATION_SPEED)
    {
        std::cout << "CONFIG::ERROR cube_rotation_speed must be at least " << MIN_ROTATION_SPEED <<
            " and at most " << MAX_ROTATION_SPEED << endl;
        config_data["cube_rotation_speed"] = DEFFAULT_ROTATION_SPEED;
    }
    value = config_data["cube_count"];
    if (value < MIN_CUBE_COUNT || value > MAX_CUBE_COUNT)
    {
        std::cout << "CONFIG::ERROR cube_count must at least " << MIN_CUBE_COUNT <<
            " and at most " << MAX_CUBE_COUNT << endl;
        config_data["cube_count"] = DEFFAULT_CUBE_COUNT;
    }
    value = config_data["checkerboard_r"];
    if (value < 0 || value > 1) 
    {
        std::cout << "CONFIG::ERROR checkerboard_r must be at least " << 0 << " and at most " << 1 << endl;
        config_data["checkerboard_r"] = 1;
    }
    value = config_data["fps_counter"];
    if (value < 0 || value > 1)
    {
        std::cout << "CONFIG::ERROR fps_counter must be at least " << 0 << " and at most " << 1 << endl;
        config_data["fps_counter"] = 1;
    }
    value = config_data["carcas_mode"];
    if (value < 0 || value > 1)
    {
        std::cout << "CONFIG::ERROR carcas_mode must be at least " << 0 << " and at most " << 1 << endl;
        config_data["carcas_mode"] = 0;
    }
    value = config_data["interpolation_count"];
    if (value < MIN_INTERPOLATION_COUNT || value > MAX_INTERPOLATION_COUNT) 
    {
        std::cout << "CONFIG::ERROR interpolation_count must be at least " << MIN_INTERPOLATION_COUNT <<
            " and at most " << MAX_INTERPOLATION_COUNT << endl;
        config_data["interpolation_count"] = DEFFAULT_INTERPOLATION_COUNT;
    }
}
