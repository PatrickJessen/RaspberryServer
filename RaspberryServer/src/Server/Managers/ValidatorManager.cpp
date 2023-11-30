#include "ValidatorManager.h"

const bool ValidatorManager::ValidateData(std::string& data)
{
    if (!CompareEncryptionKey(data, random_secret_key)) {
        return false;
    }
    RemoveSecretKey(data);
    return true;
}

const bool ValidatorManager::CompareEncryptionKey(const std::string& key1, const std::string& key2)
{
    // should use encryption library for this
    if (key1.size() > key2.size()) {
        return false;
    }
    std::string temp = "";
    for (int i = 0; i < key1.size(); i++) {
        temp += key2[i];
    }
    return key1 == temp;
}

void ValidatorManager::RemoveSecretKey(std::string& data)
{
    std::string temp = "";
    for (int i = 0; i < data.size(); i++) {
        if (data[i] != '}') {
            temp += data[i];
        }
        else {
            temp += data[i];
            for (int j = 0; j < temp.size(); j++) {
                data.erase(data.begin() + j);
            }
            return;
        }
    }
}
