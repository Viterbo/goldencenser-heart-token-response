#!/bin/bash
# cleos create account eosio goldencenser EOS8RoCAXxWYUW2v4xkG19F57BDVBzpt9NN2iDsD1ouQNyV2BkiNc
# cleos set account permission goldencenser active '{"threshold": 1,"keys": [{"key": "EOS8RoCAXxWYUW2v4xkG19F57BDVBzpt9NN2iDsD1ouQNyV2BkiNc","weight": 1}],"accounts": [{"permission":{"actor":"goldencenser","permission":"eosio.code"},"weight":1}]}' owner -p goldencenser
# push action revelation21 '["viterbotelos","goldencenser","1.0000 HEART","test"]' -p viterbotelos
# telosmain set account permission goldencenser active '{"threshold": 1,"keys": [{"key": "EOS7JYMQL2QE8XLYqoEwHgAhbhdf3nhZFnCpeRrdRyLVTWUcBza9N","weight": 1}],"accounts": [{"permission":{"actor":"goldencenser","permission":"eosio.code"},"weight":1}]}' owner -p goldencenser
# telosmain set account permission goldencenser active '{"threshold": 1,"keys": [{"key": "EOS61qh4BFJJhyJgUwDVpNCDwXHz29hnB3ARiKjpwb9Fha9vMRaVH","weight": 1}],"accounts": [{"permission":{"actor":"goldencenser","permission":"eosio.code"},"weight":1}]}' owner -p goldencenser
# telosmain push action revelation21 transfer '["viterbotelos","goldencenser","1.0000 HEART","test"]' -p viterbotelos

NET=
if [ "$1" == "prod" ]; then
   NET='--url https://telos.caleos.io'
fi

if [ "$1" != "prod" ]; then
   cleos set account permission goldencenser active '{"threshold": 1,"keys": [{"key": "EOS8RoCAXxWYUW2v4xkG19F57BDVBzpt9NN2iDsD1ouQNyV2BkiNc","weight": 1}],"accounts": [{"permission":{"actor":"goldencenser","permission":"eosio.code"},"weight":1}]}' owner -p goldencenser
fi



GOLDENCENSER_HOME=../src/goldencenser
echo "-------- goldencenser ---------"
cd $GOLDENCENSER_HOME
eosio-cpp -o goldencenser.wasm goldencenser.cpp --abigen
echo "cleos $NET set contract goldencenser $PWD -p goldencenser@active"
cleos $NET set contract goldencenser $PWD -p goldencenser@active



