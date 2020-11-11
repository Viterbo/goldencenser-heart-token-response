#!/bin/bash

./1_create_accounts.sh

./2_deploy_censer.sh $1 $2

./3_load_quotes.sh

./4_token.sh

./5_test.sh

# cleos push action revelation21 transfer '["alice","goldencenser", "0.0001 HEART", ""]' -p alice@active