//
//  device_cluster.hpp
//  iot-blockchain
//
//  Created by aco on 15/06/2019.
//  Copyright © 2019 aco. All rights reserved.
//

#ifndef device_cluster_hpp
#define device_cluster_hpp

#include <stdio.h>

#include "device_participant.hpp"

class ClusterDevice : public ParticipantDevice
{
public:
    ClusterDevice(std::string identifier, std::string admin_profile_identifier, LocalConfiguration configuration);
    ClusterDevice(std::string identifier, const std::vector<std::unique_ptr<Block>> *blocks, LocalConfiguration configuration);
    
    void receiveDeviceTransactionFromBlockchain(Transaction *transaction) override;
    void receiveRejectedTransactionFromBlockchain(Transaction *transaction);
    
    void registerResponders(void) override;
    void registerSubordinateDevices(std::vector<std::string> subordinate_device_identifiers);
    
private:
    std::map<std::string, std::optional<std::uint16_t>> subordinate_devices;
};

#endif /* device_cluster_hpp */
