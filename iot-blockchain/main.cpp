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
    auto n = MockNetwork(admin_profile_identifier);
    
    auto d = n.registerNetworkDevice<Device>(admin_profile_identifier, BlockchainConfiguration
                                    {
                                        admin_profile_identifier,
                                        false,
                                        5
                                    });
    
    d->submitTransaction(new PolicyAmendmentTransaction("admin", "gas", "admin", ACCESS_READ | ACCESS_WRITE));
    
    auto pd = n.registerNetworkDevice<Device>("lightbulb", BlockchainConfiguration
                                              {
                                                  "lightbulb",
                                                  false,
                                                  2
                                              });
    
    d->submitTransaction(new PolicyAmendmentTransaction("admin", "lightbulb", "admin", ACCESS_READ | ACCESS_WRITE));
    auto dt = new DeviceTransaction("admin", "gas");
    d->submitTransaction(dt);
    
    auto b = d->getBlockchain()->getChain();
    
    for (auto &e : *b)
    {
        std::cout << e->getIndex() << std::endl;
        
        for (auto i = 0; i < e->size(); i++)
        {
            std:: cout << e->getTransactionAt(i)->description() << std::endl;
        }
    }
    
    std::cout << d->getLeadBlock()->getHash() << std::endl;
    std::cout << pd->getLeadBlock()->getHash() << std::endl;
    b = pd->getBlockchain()->getChain();
    
    for (auto &e : *b)
    {
        std::cout << e->getIndex() << std::endl;
        
        for (auto i = 0; i < e->size(); i++)
        {
            std:: cout << e->getTransactionAt(i)->description() << std::endl;
        }
    }
    
    auto dlb = d->getBlockchain()->getBlockAt(0);
    auto plb = pd->getBlockchain()->getBlockAt(0);
    
    
    return 0;
}
