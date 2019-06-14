//
//  block.cpp
//  iot
//
//  Created by aco on 06/06/2019.
//  Copyright © 2019 aco. All rights reserved.
//

#include "block.hpp"

Block::Block(std::string author_profile_identifier, uint16_t index, size_t reserve)
{
    this->timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now()
                                                                            .time_since_epoch()).count();
    this->index = index;
    this->transactions.reserve(reserve);
    this->author_profile_identifier = author_profile_identifier;
}

Block::Block(std::string author_profile_identifier, uint16_t index, size_t reserve, std::string prev_block_hash) :
    Block(author_profile_identifier, index, reserve)
{
    this->prev_block_hash = prev_block_hash;
}

void Block::appendTransaction(Transaction *transaction)
{
    if (this->hash.empty())
    {
        this->transactions.emplace_back(transaction);
    }
    else
    {
        throw "Attempt to append transaction to a sealed block.";
    }
}

size_t Block::size(void)
{
    return this->transactions.size();
}

Transaction *Block::getTransactionAt(size_t position)
{
    return this->transactions.at(position).get();
}

std::string Block::getHash(bool seal)
{
    auto hash = this->computeHash();
    
    if (seal)
    {
        this->hash = hash;
    }
    
    return hash;
}

std::string Block::computeHash(void)
{
    std::stringstream string_stream;
    string_stream << this->timestamp;
    
    for (auto &transaction : this->transactions)
    {
        string_stream << transaction->getHash();
    }
    
    if (!this->prev_block_hash.empty())
    {
        string_stream << picosha2::bytes_to_hex_string(this->prev_block_hash.begin(), this->prev_block_hash.end());
    }
    
    auto result = std::vector<unsigned char>(picosha2::k_digest_size);
    auto hash_digest_input = string_stream.str();
    
    picosha2::hash256(hash_digest_input.begin(), hash_digest_input.end(), result.begin(), result.end());
    
    return picosha2::bytes_to_hex_string(result.begin(), result.end());
}

uint16_t Block::getIndex(void)
{
    return this->index;
}

JSON Block::json(void)
{
    return
    {
        { "timestamp", this->timestamp },
        { "index", this->index },
        { "prev_hash", this->prev_block_hash }
    };
}
