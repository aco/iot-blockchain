//
//  device.hpp
//  iot
//
//  Created by aco on 06/06/2019.
//  Copyright © 2019 aco. All rights reserved.
//

#ifndef device_hpp
#define device_hpp

#include <stdio.h>

#include "blockchain.hpp"

class Device : public Blockchain
{
public:
    Device(std::string identifier, std::string admin_profile_identifier, BlockchainConfiguration configuration);
    Device(std::string identifier, const std::vector<std::unique_ptr<Block>> *blocks, BlockchainConfiguration configuration);
    
    void receiveTransactionFromBlockchain(Transaction *transaction);
    virtual void receiveDeviceTransactionFromBlockchain(Transaction *transaction);
    
    std::string getIdentifier(void);
    Blockchain *getBlockchain(void);
    
    bool operator == (Device const &lhs) const
    {
        return lhs.identifier == this->identifier;
    }

    bool online;
    bool observer;
    
    virtual void registerResponders(void);
    
private:
    std::string identifier;
    std::optional<std::uint16_t> d_value;
};

#endif /* device_hpp */
