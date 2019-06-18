//
//  device_participant.hpp
//  iot-blockchain
//
//  Created by aco on 14/06/2019.
//  Copyright © 2019 aco. All rights reserved.
//

#ifndef device_participant_hpp
#define device_participant_hpp

#include <stdio.h>

#include "device.hpp"

class ParticipantDevice : public Device
{
public:
    ParticipantDevice(std::string identifier, std::string admin_profile_identifier, BlockchainConfiguration configuration);
    ParticipantDevice(std::string identifier, const std::vector<std::unique_ptr<Block>> *blocks, BlockchainConfiguration configuration);
    
    void receiveRejectedTransactionFromBlockchain(Transaction *transaction);
    void registerResponders(void) override;
};

#endif /* device_participant_hpp */
