HOME=`pwd`
echo "-------- downloading telos.contracts ---------"
mkdir -p $HOME/contracts/_examples/
if [ ! -d $HOME/contracts/_examples/telos.contracts ]; then
    cd $HOME/contracts/_examples/
    echo "Cloning eosio.contracts (eosio.trail & eosio.token)"
    git clone https://github.com/telosnetwork/telos.contracts.git
else
    echo "$HOME/contracts/_examples/telos.contracts OK!"
fi



# echo "-------- eosio.trail (Voting system) ---------"
# cd $HOME/contracts/_examples/telos.contracts/contracts/eosio.trail
# if [[ src/eosio.trail.cpp -nt eosio.trail.wasm ]]; then
#     eosio-cpp -o eosio.trail.wasm src/eosio.trail.cpp --abigen -I include
# fi
# cleos set contract eosio.trail $PWD -p eosio.trail@active

echo "-------- eosio.token (standar token) ---------"
cd $HOME/contracts/_examples/telos.contracts/contracts/eosio.token
if [[ src/eosio.token.cpp -nt eosio.token.wasm ]]; then
    eosio-cpp -o eosio.token.wasm src/eosio.token.cpp --abigen -I include
fi

echo "-------- eosio.token (TLOS) ---------"
cleos set contract eosio.token $PWD -p eosio.token@active
echo "-- create --"
cleos push action eosio.token create '[ "eosio", "1000000000.0000 TLOS"]' -p eosio.token@active
echo "-- issue --"
cleos push action eosio.token issue '["eosio", "4000.0000 TLOS", ""]' -p eosio@active
echo "-- transfer --"
cleos push action eosio.token transfer '["eosio", "alice", "1000.0000 TLOS", ""]' -p eosio@active
cleos push action eosio.token transfer '["eosio", "bob", "1000.0000 TLOS", ""]' -p eosio@active
cleos push action eosio.token transfer '["eosio", "tom", "1000.0000 TLOS", ""]' -p eosio@active
cleos push action eosio.token transfer '["eosio", "kate", "1000.0000 TLOS", ""]' -p eosio@active

echo "-------- acornaccount (ACORN) ---------"
cleos set contract acornaccount $PWD -p acornaccount@active
echo "-- create --"
cleos push action acornaccount create '[ "eosio", "461168601842738.0000 ACORN"]' -p acornaccount@active
echo "-- issue --"
cleos push action acornaccount issue '["eosio", "4000.0000 ACORN", ""]' -p eosio@active
echo "-- transfer --"
cleos push action acornaccount transfer '["eosio", "alice", "1000.0000 ACORN", ""]' -p eosio@active
cleos push action acornaccount transfer '["eosio", "bob", "1000.0000 ACORN", ""]' -p eosio@active
cleos push action acornaccount transfer '["eosio", "tom", "1000.0000 ACORN", ""]' -p eosio@active
cleos push action acornaccount transfer '["eosio", "kate", "1000.0000 ACORN", ""]' -p eosio@active

echo "-------- revelation21 (HEART) ---------"
cleos set contract revelation21 $PWD -p revelation21@active
echo "-- create --"
cleos push action revelation21 create '[ "eosio", "2100000000.0000 HEART"]' -p revelation21@active
echo "-- issue --"
cleos push action revelation21 issue '["eosio", "400.0000 HEART", ""]' -p eosio@active
echo "-- transfer --"
cleos push action revelation21 transfer '["eosio","alice", "100.0000 HEART", ""]' -p eosio@active
cleos push action revelation21 transfer '["eosio","bob", "100.0000 HEART", ""]' -p eosio@active
cleos push action revelation21 transfer '["eosio","tom", "100.0000 HEART", ""]' -p eosio@active
cleos push action revelation21 transfer '["eosio","kate", "100.0000 HEART", ""]' -p eosio@active
