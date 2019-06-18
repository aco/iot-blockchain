#pragma once

#include <typeindex>
#include <string>

#include "policy_manager.hpp"

#include "picosha2.hpp"
#include "json.hpp"
using JSON = nlohmann::json;

class Transaction
{
public:
	Transaction(std::string author_profile_identifier, bool expedited);

    virtual Transaction *clone(void);
	std::string getAuthorProfileIdentifier(void);
	std::string getHash(void);

	virtual bool authorise(Policy *policy);
	virtual void execute(PolicyManager *policy_manager);
    virtual bool readyToAppend(void);

	virtual JSON json(void);
	virtual std::string description(void);
    
    bool requestsExpeditedBroadcast(void); // todo include expedited status in description

protected:
	uint64_t timestamp;

	std::string hash;
	std::string author_profile_identifier;

	virtual void computeHashAndSeal(void);
    
    bool expedited;
};
