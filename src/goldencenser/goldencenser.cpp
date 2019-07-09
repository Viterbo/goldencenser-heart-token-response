#include "goldencenser.hpp"

EOSIO_DISPATCH_VAPAEE (
    goldencenser,
    (init)(newowner)(reset)(addquote)(removequote)(modifyquote),
    (htransfer)
)