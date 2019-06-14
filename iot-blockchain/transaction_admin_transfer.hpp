#pragma once

#include "transaction.hpp"

class AdminTransferTransaction : public Transaction
{
public:
	AdminTransferTransaction(std::string author_profile_identifier, std::string new_admin_profile_identifier, bool expedited = false);

	bool authorise(Policy *policy) final override;
	void execute(PolicyManager *policy_manager) final override;

	std::string getAdminProfileIdentifier(void);

	JSON json(void) override;
	std::string description(void) override;

	void computeHashAndSeal(void) override;
    
    virtual AdminTransferTransaction *clone(void) override
    {
        return new AdminTransferTransaction(*this);
    }
    
private:
	std::string new_admin_profile_identifier;
};
