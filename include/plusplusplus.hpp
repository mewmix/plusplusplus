#include <eosiolib/eosio.hpp>
#include <eosiolib/print.hpp>
#include <eosiolib/asset.hpp>
#include <eosiolib/singleton.hpp>
#include <math.h>

#define BASE_SYMBOL symbol("YAS", 4)
#define PLUS_SYMBOL symbol("PLUS", 4)
#define SYSTEM_TOKEN_CONTRACT "eosio.token"_n
#define PLUS_CONTRACT "pluspluscoin"_n
#define EXCHANGE_CONTRACT "plusexchange"_n
#define RETIRE_CONTRACT "plusplusvote"_n
#define POOL_ACCOUNT "plusyaspools"_n

using namespace eosio;
using namespace std;

CONTRACT plusplusplus : public contract
{

public:
  using contract::contract;
  plusplusplus(eosio::name receiver, eosio::name code, datastream<const char *> ds)
      : contract(receiver, code, ds)
  {
  }

  void receive_pretreatment(name from, name to, asset quantity, string memo);

private:
  void inline_transfer(name token_contract, name from, name to, asset qunatity, string memo)
  {
    action(
        permission_level{from, "active"_n},
        token_contract,
        "transfer"_n,
        make_tuple(from, to, qunatity, memo))
        .send();
  }
};