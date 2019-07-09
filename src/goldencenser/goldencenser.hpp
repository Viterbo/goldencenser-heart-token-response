#pragma once

#include <eosiolib/eosio.hpp>
#include <eosiolib/symbol.hpp>
#include <eosiolib/asset.hpp>
#include <eosiolib/print.hpp>
#include <eosiolib/singleton.hpp>

#include "vapaee.dispatcher.hpp"

using namespace eosio;
using namespace std;

//*
#define PRINT(...) print(__VA_ARGS__)
#define AUX_DEBUG_ACTIONS(...) __VA_ARGS__
/*/
#define PRINT(...)
#define AUX_DEBUG_ACTIONS(...)
//*/


CONTRACT goldencenser : public eosio::contract {


    
    public:
        struct globals {
            uint32_t count;
            name owner;
        };
        typedef singleton<name("global"), globals> Global;
        Global global;

        goldencenser(name receiver, name code, datastream<const char*> ds)
            :contract(receiver, code, ds),
            global(_self,_self.value)
            {}

    private:


        // TABLE accounts (balances) -----------
        TABLE quote {
            uint64_t id;
            string quote;
            uint64_t primary_key() const { return id; }
        };
        typedef eosio::multi_index< "quotes"_n, quote > quotes;
        // ------------------------------------

        // https://eosio.stackexchange.com/a/1657/2452
        uint32_t range(uint64_t seed, uint32_t to) {
            capi_checksum256 result;
            sha256((char *)&seed, sizeof(seed), &result);
            seed = result.hash[1];
            seed <<= 32;
            seed |= result.hash[0];
            return (uint32_t)(seed % to);
        }

        void check_signature() {
            // check signature
            name owner = global.get().owner;
            require_auth(owner);
        }

    public:
        using contract::contract;
        ACTION init(name owner) {
            PRINT("\nACTION goldencenser.init()\n");

            // check list is empty
            quotes table(get_self(), get_self().value);
            eosio_assert(table.available_primary_key() == 0, "The quote list is not empty. Previous owner must execute 'reset' action to clean the list.");

            require_auth(owner);
            global.set(globals{0, owner}, get_self());

            PRINT(" global.get().count: ", std::to_string((int) global.get().count), "\n");
            PRINT("\nACTION goldencenser.init()...\n");
        }

        ACTION newowner(name old_owner, name new_owner) {
            PRINT("\nACTION goldencenser.newowner()\n");

            require_auth(old_owner);
            uint32_t count = global.get().count;
            global.set(globals{count, new_owner}, get_self());

            PRINT("\nACTION goldencenser.newowner()...\n");
        }

        ACTION reset(int max) {
            PRINT("\nACTION goldencenser.init()\n");
            check_signature();
            int deleted = 0;

            quotes table(get_self(), get_self().value);
            for (auto ptr = table.begin(); ptr != table.end(); ptr = table.begin()) {
                if (deleted >= max) break;
                table.erase(*ptr);
                deleted++;
            }

            uint32_t count = global.get().count;
            name owner = global.get().owner;
            eosio_assert(count-deleted >= 0, "Can't delete more quotes than the list contains");
            global.set(globals{count-deleted, owner}, get_self());

            PRINT(" global.get().count: ", std::to_string((int) global.get().count), "\n");
            PRINT("\nACTION goldencenser.init()...\n");
        }

        ACTION addquote(string quote) {
            PRINT("\nACTION goldencenser.addquote()\n");

            // theck quote length
            eosio_assert(quote.size() < 256, "quote is too long. Max characters allowed is 256");

            quotes table(get_self(), get_self().value);
            table.emplace(get_self(), [&](auto & a) {
                a.id = table.available_primary_key();
                a.quote = quote;
            });

            // check signature
            name owner = global.get().owner;
            require_auth(owner);

            uint32_t count = global.get().count;
            global.set(globals{count+1, owner}, get_self());
            PRINT(" global.get().count: ", std::to_string((int) global.get().count), "\n");

            PRINT("\nACTION goldencenser.addquote()...\n");
        };

        ACTION removequote(uint64_t id) {
            PRINT("\nACTION goldencenser.removequote()\n");
            quotes table(get_self(), get_self().value);
            auto entry = table.get(id, "Quote not found");
            table.erase(entry);

            // check signature
            name owner = global.get().owner;
            require_auth(owner);
            
            uint32_t count = global.get().count;
            eosio_assert(count > 0, "count is zero (this is inconsistent)");
            global.set(globals{count-1, owner}, get_self());
            PRINT(" global.get().count: ", std::to_string((int) global.get().count), "\n");

            PRINT("\nACTION goldencenser.removequote()...\n");
        };

        ACTION modifyquote(uint64_t id, string quote) {
            PRINT("\nACTION goldencenser.modifyquote()\n");

            // theck quote length
            eosio_assert(quote.size() < 256, "quote is too long. Max characters allowed is 256");

            quotes table(get_self(), get_self().value);
            auto entry = table.find(id);
            eosio_assert(entry != table.end(), "Quote not found");
            table.modify(*entry, get_self(), [&](auto & a) {
                a.quote = quote;
            });
            
            // check signature
            name owner = global.get().owner;
            require_auth(owner);
            PRINT("\nACTION goldencenser.modifyquote()...\n");
        };

        string pick_quote(name for_whom) {
            quotes table(get_self(), get_self().value);
            string quote = string("");
            
            uint32_t count = global.get().count;
            PRINT(" global.get().count: ", std::to_string((int) global.get().count), "\n");
            eosio_assert(count > 0, "There're no quotes loaded yet");

            uint64_t seed = current_time() + for_whom.value;
            uint32_t skip = range(seed, (int)count);
            
            auto ptr = table.begin();
            eosio_assert(ptr != table.end(), "There're no quotes loaded yet");

            for (int i=0; i<skip; i++) {
                ptr++;
                if (ptr == table.end()) {
                    ptr = table.begin();
                }
            }

            quote = ptr->quote;
            PRINT("random: ", std::to_string((int) skip), " id: ", std::to_string((int) ptr->id), " quote: ", quote.c_str(), "\n");
            
            return quote;  
        };

        // Handler ------------------------------------------------------------------------------------------------------
        HANDLER htransfer(name from, name to, asset quantity, string  memo ) {
            PRINT("\nHANDLER vapaeetokens.htransfer()\n");

            // skip handling outcoming transfers from this contract to outside
            if (from == get_self()) return;

            // skip handling outcoming transfers of another token
            if (quantity.symbol.code() != symbol_code("HEART")) return;

            string message;

            if (memo == string("")) {
                message = pick_quote(from);
            } else if (memo == string("info")) {
                message = string("current owner is '" + global.get().owner.to_string() + "' with " + std::to_string((int) global.get().count) + " quotes loaded.");
            }

            action(
                permission_level{get_self(), name("active")},
                name("revelation21"),
                name("transfer"),
                std::make_tuple(get_self(), from, quantity, message)
            ).send();

            PRINT("\nHANDLER vapaeetokens.htransfer()...\n");
        }        
};
