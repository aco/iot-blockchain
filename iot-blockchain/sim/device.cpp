//
//  device.cpp
//  iot
//
//  Created by aco on 06/06/2019.
//  Copyright © 2019 aco. All rights reserved.
//

#include "device.hpp"

Device::Device(std::string identifier, std::string admin_profile_identifier, LocalConfiguration configuration) :
	Blockchain(admin_profile_identifier, configuration)
{
	
	this->identifier = identifier;
	
	this->registerResponders();
	
//	this.blocks.emplace_back(std::make_unique<Block>(configuration.host_profile_identifier, 0,
//																										configuration.block_size_reserve));
}

Device::Device(std::string identifier, const std::vector<std::unique_ptr<Block>> *blocks, LocalConfiguration configuration) :
	Blockchain(blocks, configuration)
{
	this->identifier = identifier;
	
	this->registerResponders();
}

void Device::receiveTransactionFromBlockchain(Transaction *transaction)
{
	std::cout << "test";
}

void Device::receiveDeviceTransactionFromBlockchain(Transaction *transaction)
{
	auto device_transaction = (DeviceTransaction*)transaction;
	
	if (device_transaction->getValue().has_value()) // transaction includes a value; assign
	{
		this->d_value = device_transaction->getValue().value_or(0);
	}
	else // transaction is requesting a reading; author new transaction with value
	{
		this->submitTransaction(new DeviceTransaction(this->identifier, device_transaction->getDeviceIdentifier(),
																									this->d_value.value_or(0), transaction->requestsExpeditedBroadcast()), false);
	}
}

void Device::registerResponders(void)
{
	this->registerTransactionResponder<Transaction>(std::bind(&Device::receiveTransactionFromBlockchain,
																														this, std::placeholders::_1));
	
	this->registerTransactionResponder<DeviceTransaction>(std::bind(&Device::receiveDeviceTransactionFromBlockchain,
																																	this, std::placeholders::_1));
}

std::string Device::getIdentifier(void)
{
	return this->identifier;
}

Blockchain *Device::getBlockchain(void)
{
	return (Blockchain*)this;
}
