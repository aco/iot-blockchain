#pragma once

#include <optional>

#include "transaction.hpp"

class DeviceTransaction : public Transaction
{
public:
	DeviceTransaction(std::string author_profile_identifier, std::string device_identifier, bool expedited = false);
	DeviceTransaction(std::string author_profile_identifier, std::string device_identifier, std::uint16_t value, bool expedited = false);

    DeviceTransaction *clone(void) override;
    
	bool authorise(Policy *policy) final override;
	void execute(PolicyManager *policy_manager) final override;
    bool readyToAppend(void) final override;

	std::optional<std::uint16_t> getValue(void);

	std::string getDeviceIdentifier(void);

	JSON json(void) override;
	std::string description(void) override;

	void computeHashAndSeal(void) override;

private:
	;
	std::string device_identifier;

	std::optional<std::uint16_t> t_value;
};
