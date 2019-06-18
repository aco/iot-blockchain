//
//  block.hpp
//  iot
//
//  Created by aco on 06/06/2019.
//  Copyright © 2019 aco. All rights reserved.
//

#ifndef block_hpp
#define block_hpp

#include <stdio.h>
#include <chrono>

#include "transaction_device.hpp"
#include "transaction_policy_amendment.hpp"
#include "transaction_admin_transfer.hpp"

class Block
{
public:
    Block(std::string author_profile_identifier, uint16_t index, size_t reserve);
    Block(std::string author_profile_identifier, uint16_t index, size_t reserve, std::string prev_block_hash);
    Block(Block *block);
    
    void appendTransaction(Transaction *transaction);
    
    size_t size(void);
    Transaction *getTransactionAt(size_t position);
    
    std::string getHash(bool seal = false);
    std::string computeHash(void);
    
    uint16_t getIndex(void);
    std::string getAuthorProfileIdentifier(void);
    std::string getPreviousBlockHash(void);
    
    JSON json(void);

private:
    std::string author_profile_identifier;
    std::string prev_block_hash;
    std::string hash;
    
    uint64_t timestamp;
    uint16_t index;
    
    std::vector<std::unique_ptr<Transaction>> transactions;
};

#endif /* block_hpp */
