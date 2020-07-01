#include <iostream>
#include <chrono>

#include "transaction.hpp"

Transaction::Transaction(std::string author_profile_identifier, bool expedited)
{
	this->author_profile_identifier = author_profile_identifier;
	this->expedited = expedited;
	
	this->timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now()
																																					.time_since_epoch()).count();
}

Transaction* Transaction::clone(void)
{
	return new Transaction(*this);
}


std::string Transaction::getHash(void)
{
	if (this->hash.empty())
	{
		this->computeHashAndSeal();
	}
	
	return this->hash;
}

std::string Transaction::getAuthorProfileIdentifier(void)
{
	return this->author_profile_identifier;
}

bool Transaction::authorise(Policy *policy)
{
	return false;
}

void Transaction::execute(PolicyManager *policy_manager)
{
	return;
}

bool Transaction::readyToAppend(void)
{
	return true;
}

bool Transaction::requestsExpeditedBroadcast(void)
{
	return this->expedited;
}

JSON Transaction::json(void)
{
	return
	{
		{ "timestamp", this->timestamp },
		{ "author", this->author_profile_identifier },
		{ "hash", this->getHash() },
		{ "expedited", this->expedited }
	};
}

std::string Transaction::description(void)
{
	return "Base transaction";
}

void Transaction::computeHashAndSeal(void)
{
	this->hash = "Base hash";
}
