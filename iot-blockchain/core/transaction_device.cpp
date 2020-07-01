#include "transaction_device.hpp"

DeviceTransaction::DeviceTransaction(std::string author_profile_identifier, std::string device_identifier, bool expedited) :
Transaction(author_profile_identifier, expedited)
{
	this->device_identifier = device_identifier;
}

DeviceTransaction::DeviceTransaction(std::string author_profile_identifier, std::string device_identifier, std::uint16_t value, bool expedited) :
DeviceTransaction(author_profile_identifier, device_identifier, expedited)
{
	this->t_value = value;
}

DeviceTransaction* DeviceTransaction::clone(void)
{
	return new DeviceTransaction(*this);
}

std::optional<std::uint16_t> DeviceTransaction::getValue(void)
{
	return this->t_value;
}

std::string DeviceTransaction::getDeviceIdentifier(void)
{
	return this->device_identifier; 
}

bool DeviceTransaction::authorise(Policy *policy)
{
	auto transaction_flags = ACCESS_READ | ACCESS_WRITE; // this->t_value.has_value() ? ACCESS_WRITE : ACCESS_READ;
	
	return policy->permissible(this->device_identifier, this->author_profile_identifier, transaction_flags);
}

void DeviceTransaction::execute(PolicyManager * policy_manager)
{
	;
}

bool DeviceTransaction::readyToAppend(void)
{
	return this->t_value.has_value();
}

void DeviceTransaction::computeHashAndSeal(void)
{
	std::stringstream string_stream;
	
	string_stream << this->timestamp // re-add valuae
	<< this->author_profile_identifier
	<< this->device_identifier;
	
	auto result = std::vector<unsigned char>(picosha2::k_digest_size);
	auto hash_digest_input = string_stream.str();
	
	picosha2::hash256(hash_digest_input.begin(), hash_digest_input.end(), result.begin(), result.end());
	
	this->hash = picosha2::bytes_to_hex_string(result.begin(), result.end());
}

JSON DeviceTransaction::json(void)
{
	auto json = Transaction::json();
	
	json["device_identifier"] = this->device_identifier;
	
	if (this->t_value.has_value())
	{
		json["value"] = this->t_value.value_or(0);
	}
	else
	{
		json["value"] = nullptr;
	}
	
	return json;
}

std::string DeviceTransaction::description(void)
{
	char buffer[128] = "";
	
	if (this->expedited)
	{
		sprintf(buffer, "[Expedited] ");
	}
	
	if (this->t_value.has_value()) // transaction includes a value; assign
	{
		sprintf(buffer, "Set %s to %d (req: %s)", this->device_identifier.c_str(), this->t_value.value_or(0),
						this->author_profile_identifier.c_str());
	}
	else // transaction is requesting a reading; complete and broadcast
	{
		sprintf(buffer, "Probe device: %s (req. by: %s) (read %d) [not recorded]", this->device_identifier.c_str(), this->author_profile_identifier.c_str(),
						this->t_value.value_or(0));
	}
	
	return buffer;
}
