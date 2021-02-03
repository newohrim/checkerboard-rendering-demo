#include "configuration.h"

void configuration::init()
{
    in.open(path);

    if (!in.is_open())
        cout << "CONFIG::ERROR : Unable to open configuration file." << endl;

    string field;
    while (getline(in, field))
    {
        vector<string> parts = split(field);
        config_data[parts[0]] = stoi(parts[1]);
    }
    in.close();
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
