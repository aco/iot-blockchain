//
//  blockchain.hpp
//  iot-blockchain
//
//  Created by aco on 13/06/2019.
//  Copyright © 2019 aco. All rights reserved.
//

#ifndef blockchain_hpp
#define blockchain_hpp

#include <stdio.h>
#include <vector>

#include "policy_manager.hpp"
#include "block.hpp"
#include "blockchain_configuration.hpp"

class Blockchain : protected Policy
{
public:
    Blockchain(std::string admin_profile_identifier, BlockchainConfiguration configuration);
    Blockchain(const std::vector<std::unique_ptr<Block>> *blocks, BlockchainConfiguration configuration);
    
    void submitTransaction(Transaction *transaction, bool reconciling = false);
    void submitBlock(Block *block);
    
    Block *getLeadBlock(void);
    Block *getBlockAt(size_t index);
    std::vector<std::unique_ptr<Block>> *getChain(void);
    
    std::function<void(Block*)> sealed_block_responder;
    std::function<void(Transaction*)> transaction_responder;
    std::function<void(Transaction*)> rejection_responder;
    
    template<typename DerivedTransaction>
    void registerTransactionResponder(std::function<void(Transaction*)> callback)
    {
        this->transaction_responder_registrar.emplace(typeid(DerivedTransaction), callback);
    }
    
protected:
    void spawnNewBlock(bool suppress);
    BlockchainConfiguration configuration;
    
    std::vector<std::unique_ptr<Block>> blocks;
    
private:
    void invokeResponder(Transaction *transaction, std::type_index transaction_type = typeid(Transaction));;
    
    std::map<std::type_index, std::function<void(Transaction*)>> transaction_responder_registrar;
    std::vector<std::type_index> administrative_transaction_types;
};

#endif /* blockchain_hpp */
