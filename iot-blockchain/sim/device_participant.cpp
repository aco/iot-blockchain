//
//  device_participant.cpp
//  iot-blockchain
//
//  Created by aco on 14/06/2019.
//  Copyright © 2019 aco. All rights reserved.
//

#include "device_participant.hpp"

ParticipantDevice::ParticipantDevice(std::string identifier, std::string admin_profile_identifier, LocalConfiguration configuration) :
Device(identifier, admin_profile_identifier, configuration)
{
    this->registerResponders();
}

ParticipantDevice::ParticipantDevice(std::string identifier, const std::vector<std::unique_ptr<Block>> *blocks, LocalConfiguration configuration) :
Device(identifier, blocks, configuration)
{
    this->registerResponders();
}

void ParticipantDevice::receiveRejectedTransactionFromBlockchain(Transaction *transaction) // here is where a capable device may monitor
{
    logOutput(LOG_ALERT, "Rejected %s", transaction->description().c_str());
}

void ParticipantDevice::registerResponders(void)
{
    this->rejection_responder = std::bind(&ParticipantDevice::receiveRejectedTransactionFromBlockchain, this, std::placeholders::_1);
}
