#include "policy_manager.hpp"

PolicyManager::PolicyManager(std::string admin_profile_identifier)
{
	this->updateAdmin(admin_profile_identifier);
}

Policy::Policy(std::string admin_profile_identifier) :
PolicyManager(admin_profile_identifier)
{
	;
}

void PolicyManager::emplace(std::string device_identifier, std::string subject_device_identifier, std::uint8_t access_flags)
{
	this->policies[device_identifier][subject_device_identifier] = access_flags;
}

void PolicyManager::updateAdmin(std::string admin_profile_identifier)
{
	this->admin_profile_identifier = admin_profile_identifier;
}

bool Policy::permissible(std::string device_identifier, std:: string author_device_identifier, std::uint8_t desired_access_flags)
{
	auto policy = this->policies[device_identifier][author_device_identifier];
	
	return policy && (policy & desired_access_flags) == desired_access_flags;
}

bool Policy::administrativePermissible(std::string author_profile_identifier)
{
	return this->admin_profile_identifier == author_profile_identifier;
}
