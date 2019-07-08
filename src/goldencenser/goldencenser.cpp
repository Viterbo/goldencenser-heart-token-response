#include "goldencenser.hpp"

EOSIO_DISPATCH_VAPAEE (
    goldencenser,
    (init)(reset)(addquote)(removequote)(modifyquote),
    (htransfer)
)