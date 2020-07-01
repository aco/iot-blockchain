#include "transaction_policy_amendment.hpp"

PolicyAmendmentTransaction::PolicyAmendmentTransaction(std::string author_profile_identifier, std::string device_identifier,
																											 std::string subject_device_identifier, std::uint8_t access_flags, bool expedited) :
Transaction(author_profile_identifier, expedited)
{
	this->device_identifier = device_identifier;
	this->subject_device_identifier = subject_device_identifier;
	this->access_flags = access_flags;
}

PolicyAmendmentTransaction* PolicyAmendmentTransaction::clone(void)
{
	return new PolicyAmendmentTransaction(*this);
}

std::string PolicyAmendmentTransaction::getDeviceIdentifier(void)
{
	return this->device_identifier;
}

std::string PolicyAmendmentTransaction::getTargetProfileIdentifier(void)
{
	return this->subject_device_identifier;
}

bool PolicyAmendmentTransaction::authorise(Policy *policy)
{
	return policy->administrativePermissible(this->author_profile_identifier);
}

void PolicyAmendmentTransaction::execute(PolicyManager *policy_manager)
{
	policy_manager->emplace(this->device_identifier, this->subject_device_identifier, this->access_flags);
}

void PolicyAmendmentTransaction::computeHashAndSeal(void)
{
	std::stringstream string_stream;
	
	string_stream << this->timestamp
	<< this->author_profile_identifier
	<< this->device_identifier
	<< this->subject_device_identifier;
	
	auto result = std::vector<unsigned char>(picosha2::k_digest_size);
	auto hash_digest_input = string_stream.str();
	
	string_stream.clear();
	
	picosha2::hash256(hash_digest_input.begin(), hash_digest_input.end(), result.begin(), result.end());
	
	this->hash = picosha2::bytes_to_hex_string(result.begin(), result.end());
}

JSON PolicyAmendmentTransaction::json(void)
{
	auto json = Transaction::json();
	
	json["device_identifier"] = this->device_identifier;
	json["subject_device_identifier"] = this->subject_device_identifier;
	
	return json;
}

std::string PolicyAmendmentTransaction::description(void)
{
	char buffer[64];
	
	sprintf(buffer, "Amend %s policy for %s (req. by: %s)", this->subject_device_identifier.c_str(),
					this->device_identifier.c_str(), this->author_profile_identifier.c_str());
	
	return buffer;
}
