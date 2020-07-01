#pragma once

#include "transaction.hpp"

class PolicyAmendmentTransaction : public Transaction
{
public:
	PolicyAmendmentTransaction(std::string author_profile_identifier, std::string device_identifier,
														 std::string subject_device_identifier, std::uint8_t access_flags, bool expedited = false);
	
	
	PolicyAmendmentTransaction *clone(void) override;
	
	bool authorise(Policy *policy) final override;
	void execute(PolicyManager *policy_manager) final override;
	
	std::string getDeviceIdentifier(void);
	std::string getTargetProfileIdentifier(void);
	
	virtual JSON json(void) override;
	std::string description(void) override;
	
	void computeHashAndSeal(void) override;
	
private:
	std::string device_identifier;
	std::string subject_device_identifier;
	
	std::uint8_t access_flags;
};
