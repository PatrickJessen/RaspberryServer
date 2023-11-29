#include "ValidatorManager.h"

const bool ValidatorManager::ValidateData(const std::string& data)
{
    std::string key = SeperateKeyFromString(data);
    if (!CompareEncryptionKey(key, random_secret_key)) {
        return false;
    }
    return true;
}

const bool ValidatorManager::CompareEncryptionKey(const std::string& key1, const std::string& key2)
{
    return key1 == key2;
}

const std::string& ValidatorManager::SeperateKeyFromString(const std::string& str)
{
    std::string key = "";
    std::string temp = "";
    int start = str.find("key ");
    int end = str.find("endKey ");
    for (int i = start; i < end; i++) {
        key += str[i];
    }
    return key;
}
