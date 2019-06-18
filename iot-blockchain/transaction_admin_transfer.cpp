#include "transaction_admin_transfer.hpp"

AdminTransferTransaction::AdminTransferTransaction(std::string author_profile_identifier,
                                                   std::string new_admin_profile_identifier, bool expedited) :
	Transaction(author_profile_identifier, expedited)
{
	this->new_admin_profile_identifier = new_admin_profile_identifier;
}

AdminTransferTransaction* AdminTransferTransaction::clone(void)
{
    return new AdminTransferTransaction(*this);
}

std::string AdminTransferTransaction::getAdminProfileIdentifier(void)
{
	return this->new_admin_profile_identifier;
}

bool AdminTransferTransaction::authorise(Policy *policy)
{
	return policy->administrativePermissible(this->author_profile_identifier);
}

void AdminTransferTransaction::execute(PolicyManager *policy_manager)
{
	policy_manager->updateAdmin(this->new_admin_profile_identifier);
}

void AdminTransferTransaction::computeHashAndSeal(void)
{
	std::stringstream string_stream;

	string_stream << this->timestamp
		<< this->author_profile_identifier
		<< this->new_admin_profile_identifier;

	auto result = std::vector<unsigned char>(picosha2::k_digest_size);
	auto hash_digest_input = string_stream.str();

	string_stream.clear();

	picosha2::hash256(hash_digest_input.begin(), hash_digest_input.end(), result.begin(), result.end());

	this->hash = picosha2::bytes_to_hex_string(result.begin(), result.end());
}

JSON AdminTransferTransaction::json(void)
{
	auto json = Transaction::json();

	json["new_admin_profile_identifier"] = this->new_admin_profile_identifier;

	return json;
}

std::string AdminTransferTransaction::description(void)
{
	char buffer[64];

	sprintf(buffer, "Transfer admin rights to %s (req. by: %s)", this->new_admin_profile_identifier.c_str(), 
		this->author_profile_identifier.c_str());

	return buffer;
}
