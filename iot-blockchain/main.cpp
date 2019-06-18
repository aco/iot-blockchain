//
//  main.cpp
//  iot-blockchain
//
//  Created by aco on 13/06/2019.
//  Copyright © 2019 aco. All rights reserved.
//

#include <iostream>

#include "mock_network.hpp"

int main(int argc, const char * argv[])
{
    auto admin_profile_identifier = "admin";
    auto network = MockNetwork(admin_profile_identifier);
    
    // register standard device as administrator (first device to join the network)
    auto admin_device = network.registerNetworkDevice<Device>(admin_profile_identifier, true);

    // we do not manually use this participant device below
    (void)network.registerNetworkDevice<ParticipantDevice>("light", false);
    
    admin_device->submitTransaction(new PolicyAmendmentTransaction("admin", "light", "admin", ACCESS_READ | ACCESS_WRITE));

    // register cluster device
    auto cluster_device = network.registerNetworkDevice<ClusterDevice>("cluster", false);

    std::vector<std::string> cluster_subordinate_devices = { "gas", "flame" };
    cluster_device->registerSubordinateDevices(cluster_subordinate_devices);

    for (auto &network_device_identifier : network.getDeviceIdentifiers())
    {
        for (auto &cluster_subordinate_device : cluster_subordinate_devices)
        {
            admin_device->submitTransaction(new PolicyAmendmentTransaction("admin", cluster_subordinate_device, network_device_identifier,
                                                                ACCESS_READ | ACCESS_WRITE));
        }
    }

    admin_device->submitTransaction(new DeviceTransaction("admin", "light", 50, false));
    admin_device->submitTransaction(new DeviceTransaction("admin", "gas"));
    
    network.printDeviceLocalBlockchainHashes();
    
    return 0;
}
