//
//  device_cluster.cpp
//  iot-blockchain
//
//  Created by aco on 15/06/2019.
//  Copyright © 2019 aco. All rights reserved.
//

#include "device_cluster.hpp"

ClusterDevice::ClusterDevice(std::string identifier, std::string admin_profile_identifier, BlockchainConfiguration configuration) :
    ParticipantDevice(identifier, admin_profile_identifier, configuration)
{
    this->registerResponders();
}

ClusterDevice::ClusterDevice(std::string identifier, const std::vector<std::unique_ptr<Block>> *blocks, BlockchainConfiguration configuration) :
    ParticipantDevice(identifier, blocks, configuration)
{
    this->registerResponders();
}

void ClusterDevice::receiveDeviceTransactionFromBlockchain(Transaction *transaction)
{
    auto device_transaction = (DeviceTransaction*)transaction;
    auto target_device = device_transaction->getDeviceIdentifier();
    
    auto subordinate_device = this->subordinate_devices.find(target_device);
    
    if (subordinate_device == this->subordinate_devices.end()) // device is not within this cluster - exit
    {
        return;
    }
    
    if (device_transaction->getValue().has_value()) // transaction includes a value; assign
    {
        subordinate_device->second = device_transaction->getValue().value_or(0);
    }
    else // transaction is requesting a reading; complete and broadcast
    {
        auto respond_transaction = new DeviceTransaction(this->identifier, device_transaction->getDeviceIdentifier(),
                                                         subordinate_device->second.value_or(0), false);
        
        this->submitTransaction(respond_transaction, false);
    }
}

void ClusterDevice::receiveRejectedTransactionFromBlockchain(Transaction *transaction) // here is where a capable device may monitor
{
    logOutput(LOG_ALERT, "Rejected %s", transaction->description().c_str());
}

void ClusterDevice::registerResponders(void)
{
    this->rejection_responder = std::bind(&ParticipantDevice::receiveRejectedTransactionFromBlockchain, this, std::placeholders::_1);
}

void ClusterDevice::registerSubordinateDevices(std::vector<std::string> subordinate_device_identifiers)
{
    for (auto &subordinate_device_identifier : subordinate_device_identifiers)
    {
        this->subordinate_devices.emplace(subordinate_device_identifier, 0);
    }
}
