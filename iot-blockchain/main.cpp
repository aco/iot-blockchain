//
//  main.cpp
//  iot-blockchain
//
//  Created by aco on 13/06/2019.
//  Copyright © 2019 aco. All rights reserved.
//

#include <iostream>
#include <chrono>
#include <fstream>

#include "mock_network.hpp"

int main(int argc, const char * argv[])
{
	auto admin_profile_identifier = "admin";
	auto network = MockNetwork();
	
	// register standard device as administrator (first device to join the network)
	auto admin_device = network.registerNetworkDevice<ParticipantDevice>(admin_profile_identifier, true);
	
	// we do not manually use this participant device below
	(void)network.registerNetworkDevice<ParticipantDevice>("light", false);
	
	admin_device->submitTransaction(new PolicyAmendmentTransaction("admin", "light", "admin", ACCESS_READ | ACCESS_WRITE));
	
	// register cluster device & subordinate devices
	auto cluster_device = network.registerNetworkDevice<ClusterDevice>("cluster", false);

	std::vector<std::string> cluster_subordinate_devices = { "gas", "flame" };
	cluster_device->registerSubordinateDevices(cluster_subordinate_devices);
	
	// assign read permissions for all devices in cluster
	for (auto &network_device_identifier : network.getDeviceIdentifiers())
	{
		for (auto &cluster_subordinate_device : cluster_subordinate_devices)
		{
			admin_device->submitTransaction(new PolicyAmendmentTransaction("admin", cluster_subordinate_device, network_device_identifier,
																																		 ACCESS_READ));
		}
	}
	
	// test read + write permissions
	admin_device->submitTransaction(new DeviceTransaction("admin", "light", 50, false));
	admin_device->submitTransaction(new DeviceTransaction("admin", "light", 50, true));
	admin_device->submitTransaction(new DeviceTransaction("admin", "light", 1, false));
	admin_device->submitTransaction(new DeviceTransaction("admin", "gas"));
	
//	auto start = std::chrono::steady_clock::now();
//	int t_count = 0;
//	while (true)
//	{
//		auto value = rand();
//		admin_device->submitTransaction(new DeviceTransaction("admin", "light", value, false));
//		t_count++;
//
//		if (std::chrono::steady_clock::now() - start > std::chrono::seconds(1))
//		{
//			break;
//		}
//		else if (t_count % 15000 == 0)
//		{
//			admin_device->submitTransaction(new DeviceTransaction("admin", "light", 50, true));
//			t_count++;
//		}
//	}
	
//	std::cout << t_count << "-" << admin_device->getLeadBlock()->size() << std::endl;
	
	// print hashes of all devices' local lead blocks - expect match
	network.printDeviceLocalBlockchainHashes();
	
	for (auto &device: network.network_devices)
	{
		auto json = device->json();
		
		std::ofstream out("/Users/andrew/iot-blockchain/" + device->getIdentifier() + ".json");
    out << json.dump(2);
    out.close();
	}
	
	return 0;
}
