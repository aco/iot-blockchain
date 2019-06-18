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

#include "device_cluster.hpp"
#include "blockchain.hpp"

class MockNetwork
{
public:
    MockNetwork(std::string admin_profile_identifier)
    {
        this->admin_profile_identifier = admin_profile_identifier;
    }
    
    template<typename DerivedDevice>
    DerivedDevice *registerNetworkDevice(std::string identifier, bool observer)
    {
        auto blockchain_configuration = BlockchainConfiguration
        {
            identifier,
            observer,
            5 // default reserve - devices may modify this to suit themselves
        };
        
        if (network_devices.size() < 1)
        {
            logOutput(LOG_ADD, "Formulating network using device: %s (%s) as administrator", identifier.c_str(),
                      typeid(DerivedDevice).name());
            
            std::unique_ptr<Device> device(new DerivedDevice(identifier, this->admin_profile_identifier, blockchain_configuration));
            
            device->sealed_block_responder = std::bind(&MockNetwork::broadcastBlock, this, std::placeholders::_1);
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
            
            std::unique_ptr<Device> device(new DerivedDevice(identifier, existing_device->getChain(), blockchain_configuration));

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
            
            contained_device->submitTransaction(transaction, false);
        }
    }
    
    void broadcastBlock(Block *block)
    {
        logOutput(LOG_ALERT, "Broadcasting block #%d (%hhu txn) from %s", block->getIndex(),
                  block->size(), block->getAuthorProfileIdentifier().c_str());
        
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
    
    void printDeviceLocalBlockchainHashes(void)
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
    
    std::vector<std::string> getDeviceIdentifiers(void)
    {
        std::vector<std::string> identifiers;
        identifiers.reserve(this->network_devices.size());
        
        for (auto &device : this->network_devices)
        {
            identifiers.emplace_back(device->getIdentifier());
        }
        
        return identifiers;
    }
    
private:
    std::string admin_profile_identifier;
    std::vector<std::unique_ptr<Device>> network_devices;
};

#endif /* mock_network_hpp */
