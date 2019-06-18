#pragma once

#include <iostream>
#include <map>
#include <string>

enum AccessFlags : uint32_t
{
	ACCESS_NONE = 1 << 0,
	ACCESS_WRITE = 1 << 1,
	ACCESS_READ = 1 << 2
};

class PolicyManager
{
public:
	PolicyManager(std::string admin_profile_identifier);

	void emplace(std::string device_identifier, std::string subject_device_identifier, std::uint8_t access_flags);
	void updateAdmin(std::string admin_profile_identifier);

protected:
	std::string admin_profile_identifier;
	std::map<std::string, std::map<std::string, std::uint8_t>> policies;
};

class Policy : public PolicyManager
{
public:
	Policy(std::string admin_profile_identifier);

	bool permissible(std::string device_identifier, std::string author_device_identifier, std::uint8_t desired_access_flags);
	bool administrativePermissible(std::string author_profile_identifier);
};
