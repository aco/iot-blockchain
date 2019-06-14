//
//  blockchain.cpp
//  iot-blockchain
//
//  Created by aco on 13/06/2019.
//  Copyright © 2019 aco. All rights reserved.
//

#include "blockchain.hpp"

Blockchain::Blockchain(std::string admin_profile_identifier, BlockchainConfiguration configuration) :
    Policy(admin_profile_identifier)
{
    this->configuration = configuration;
    this->administrative_transaction_types =
    {
        typeid(PolicyAmendmentTransaction),
        typeid(AdminTransferTransaction)
    };
    
    this->spawnNewBlock(); // save informing device of genesis block spawn
    this->submitTransaction(new AdminTransferTransaction(admin_profile_identifier, admin_profile_identifier)); // install administrator in genesis transaction
}

Blockchain::Blockchain(const std::vector<std::unique_ptr<Block>> *blocks, BlockchainConfiguration configuration) :
    Policy(blocks->at(0).get()->getTransactionAt(0)->getAuthorProfileIdentifier()) // genesis transaction always declares admin profile identifier
{
    this->configuration = configuration;
    this->administrative_transaction_types =
    {
        typeid(PolicyAmendmentTransaction),
        typeid(AdminTransferTransaction)
    };
    
    for (auto &block : *blocks)
    {
        this->blocks.emplace_back(std::make_unique<Block>(block.get())); // clone into local chain
        
        for (size_t i = 0; i < block->size(); i++)
        {
            this->submitTransaction(block->getTransactionAt(i)->clone());
        }
    }
}

void Blockchain::submitTransaction(Transaction *transaction)
{
    std::type_index transaction_type = typeid(*transaction);
    
    if (transaction->authorise((Policy*)this))
    {
        auto is_administrative = std::find(this->administrative_transaction_types.begin(),
                                           this->administrative_transaction_types.end(), transaction_type) != this->administrative_transaction_types.end();
        
        transaction->execute((is_administrative ? (PolicyManager*)this : nullptr)); // send policy manager only if transaction is administrative
    }
    else // rejected
    {
        if (this->rejection_responder != nullptr)
        {
            this->rejection_responder(transaction);
        }
        
        return;
    }
    
    auto lead_block = this->getLeadBlock();
    
    if (!this->configuration.observe_only && transaction->readyToAppend())
    {
        lead_block->appendTransaction(transaction->clone()); // clone into local lead block
    }
    
    this->invokeResponder(transaction, transaction_type);
    
    if (transaction->getAuthorProfileIdentifier() == this->configuration.host_profile_identifier) // transaction originated internally
    {
        if (transaction->requestsExpeditedBroadcast() && transaction->readyToAppend()) // transaction originated internally & requests an expedited broadcast
        {
            if (this->sealed_block_responder != nullptr) // invoke device responder (if exists)
            {
//                (void)lead_block->getHash(true); // seal block
                this->sealed_block_responder(lead_block);
                this->spawnNewBlock();
            }
        }
        else if (this->transaction_responder != nullptr) // regular transaction, broadcast as such (if responder registered)
        {
            this->transaction_responder(transaction);
        }
    }
}

void Blockchain::submitBlock(Block *block)
{
    if (this->getChain()->size() < 1)
    {
        this->blocks.emplace_back(std::make_unique<Block>(block)); // clone into local chain
    }
    
    auto lead_block = this->getLeadBlock();
    
    if (block->getIndex() > lead_block->getIndex() && block->getPreviousBlockHash() == lead_block->getHash())
    {
        this->blocks.emplace_back(std::make_unique<Block>(block)); // clone into local chain
        
        for (size_t i = 0; i < block->size(); i++)
        {
            this->submitTransaction(block->getTransactionAt(i)->clone());
        }
    }
    else if (block->getIndex() == lead_block->getIndex() && block->size() > lead_block->size())
    {
        for (size_t i = lead_block->size(); i < block->size(); i++)
        {
            this->submitTransaction(block->getTransactionAt(i)->clone());
        }
    }
    
    this->spawnNewBlock();
}

void Blockchain::spawnNewBlock(void)
{
    auto chain_size = this->blocks.size();
    
    if (chain_size > 0) // append from previous block
    {
        auto lead_block = this->getLeadBlock();
        auto lead_block_hash = lead_block->getHash(true); // compute hash and seal
        
        this->blocks.emplace_back(std::make_unique<Block>(lead_block->getAuthorProfileIdentifier(), lead_block->getIndex() + 1,
                                                          this->configuration.block_size_reserve, lead_block_hash)); // reference previous hash in new block
    }
    else // spawn genesis block
    {
        this->blocks.push_back(std::make_unique<Block>(this->configuration.host_profile_identifier, 0,
                                                       this->configuration.block_size_reserve));
    }
}

Block *Blockchain::getBlockAt(size_t index)
{
    return this->blocks.at(index).get();
}

Block *Blockchain::getLeadBlock(void)
{
    return this->blocks.back().get();
}

std::vector<std::unique_ptr<Block>> *Blockchain::getChain(void)
{
    return &this->blocks;
}

