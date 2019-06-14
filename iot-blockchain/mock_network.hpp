//
//  mock_network.hpp
//  iot-blockchain
//
//  Created by aco on 13/06/2019.
//  Copyright © 2019 aco. All rights reserved.
//

#ifndef mock_network_hpp
#define mock_network_hpp

#include <stdio.h>

#include "log.hpp"
#include "device.hpp"
#include "blockchain.hpp"

class MockNetwork {
public:
    MockNetwork(std::string admin_profile_identifier)
    {
        this->admin_profile_identifier = admin_profile_identifier;
    }
    
    template<typename DerivedDevice>
    DerivedDevice *registerNetworkDevice(std::string identifier, BlockchainConfiguration configuration)
    {
        if (network_devices.size() < 1)
        {
            logOutput(LOG_ADD, "Formulating network using device: %s (%s) as administrator", identifier.c_str(),
                      typeid(DerivedDevice).name());
            
            std::unique_ptr<Device> device(new DerivedDevice(identifier, this->admin_profile_identifier, configuration));
            
            device->transaction_responder = std::bind(&MockNetwork::broadcastTransaction, this, std::placeholders::_1);
            device->online = true;
            
            network_devices.push_back(std::move(device));
        }
        else
        {
            logOutput(LOG_ADD, "Registering device: %s (%s) onto network", identifier.c_str(),
                      typeid(DerivedDevice).name());
            
            auto existing_device_index = rand() % network_devices.size();
            auto existing_device = network_devices.at(existing_device_index).get();
            
            std::unique_ptr<Device> device(new DerivedDevice(identifier, existing_device->getChain(), configuration));
            
            device->sealed_block_responder = std::bind(&MockNetwork::broadcastBlock, this, std::placeholders::_1);
            device->transaction_responder = std::bind(&MockNetwork::broadcastTransaction, this, std::placeholders::_1);
            device->online = true;
            
            device->registerResponders();
            
            network_devices.push_back(std::move(device));
        }
        
        return (DerivedDevice*)network_devices.back().get();
    }
    
    void broadcastTransaction(Transaction *transaction)
    {
        logOutput(LOG_OUTPUT, "%s", transaction->description().c_str());
        
        for (auto &device : network_devices)
        {
            auto contained_device = device.get();
            
            if (transaction->getAuthorProfileIdentifier() == contained_device->getIdentifier() ||
                !contained_device->online) // do not submit to offline or origin devices
            {
                continue;
            }
            
            contained_device->submitTransaction(transaction);
        }
    }
    
    void broadcastBlock(Block *block)
    {
        logOutput(LOG_OUTPUT, "Broadcasting block #%d (%hhu txn)", block->getIndex(), block->size());
        
        for (auto &device : network_devices)
        {
            auto contained_device = device.get();
            
            if (block->getAuthorProfileIdentifier() == contained_device->getIdentifier() || !contained_device->online) // do not submit to offline or origin devices
            {
                continue;
            }
            
            contained_device->submitBlock(block);
        }
    }
    
private:
    std::string admin_profile_identifier;
    std::vector<std::unique_ptr<Device>> network_devices;
};

#endif /* mock_network_hpp */
