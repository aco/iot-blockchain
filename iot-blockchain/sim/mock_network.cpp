//
//  mock_network.cpp
//  iot-blockchain
//
//  Created by aco on 13/06/2019.
//  Copyright © 2019 aco. All rights reserved.
//

#include "mock_network.hpp"

void MockNetwork::broadcastTransaction(Transaction *transaction)
{
    logOutput(LOG_OUTPUT, "%s", transaction->description().c_str());
    
    for (auto &device : this->network_devices)
    {
        auto contained_device = device.get();
        
        if (transaction->getAuthorProfileIdentifier() == contained_device->getIdentifier() ||
            !contained_device->online) // do not submit to offline or origin devices
        {
            continue;
        }
        
        contained_device->submitTransaction(transaction, false);
    }
}

void MockNetwork::broadcastBlock(Block *block)
{
    logOutput(LOG_ALERT, "Broadcasting block #%d (%hhu txn) from %s", block->getIndex(),
              block->size(), block->getAuthorProfileIdentifier().c_str());
    
    for (auto &device : this->network_devices)
    {
        auto contained_device = device.get();
        
        if (block->getAuthorProfileIdentifier() == contained_device->getIdentifier() || !contained_device->online) // do not submit to offline or origin devices
        {
            continue;
        }
        
        contained_device->submitBlock(block);
    }
}

void MockNetwork::printDeviceLocalBlockchainHashes(void)
{
    logOutput(LOG_INFO, "Local lead block hashes of all network devices:");
    
    for (auto &device : this->network_devices)
    {
        std::cout << device->getIdentifier()
        << '\t' << device->getBlockchain()->getChain()->size()
        << device->getLeadBlock()->getHash(false)
        << std::endl;
    }
}

std::vector<std::string> MockNetwork::getDeviceIdentifiers(void)
{
    std::vector<std::string> identifiers;
    identifiers.reserve(this->network_devices.size());
    
    for (auto &device : this->network_devices)
    {
        identifiers.emplace_back(device->getIdentifier());
    }
    
    return identifiers;
}
