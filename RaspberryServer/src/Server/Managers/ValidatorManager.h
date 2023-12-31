#pragma once
#include <string>

const std::string random_secret_key = "{secret_code}";

class ValidatorManager
{
public:
	const bool ValidateData(std::string& data);
private:
	const bool CompareEncryptionKey(const std::string& key1, const std::string& key2);
	void RemoveSecretKey(std::string& data);
};