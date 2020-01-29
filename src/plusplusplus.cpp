#include <plusplusplus.hpp>
#include <eosiolib/asset.hpp>
#include <eosiolib/transaction.hpp>
#include <eosiolib/action.hpp>
#include <eosio.token/eosio.token.hpp>
#include <string>

void plusplusplus::receive_pretreatment(name from, name to, asset quantity, string memo)
{
  // important check
  if (to != get_self())
  //
  {
    return;
  }
  else
  {
    // receive system token
    if (get_code() == SYSTEM_TOKEN_CONTRACT && quantity.symbol == BASE_SYMBOL)
    // buy plus
    {
      // get base balance
      auto pool_balance = eosio::token::get_balance(SYSTEM_TOKEN_CONTRACT, POOL_ACCOUNT, BASE_SYMBOL.code()).amount;
      auto amount = quantity.amount;
      int times = ((10 * amount) / pool_balance) + 1;
      times = times > 5 ? times : 5;
      auto each = amount / times;
      for (int i = 0; i < times; i++)
      {
        /* send defer check action, cancel within 24 hours */
        eosio::transaction txn{};
        txn.actions.emplace_back(
            permission_level{_self, "active"_n},
            SYSTEM_TOKEN_CONTRACT,
            "transfer"_n,
            std::make_tuple(get_self(), EXCHANGE_CONTRACT, asset(each, BASE_SYMBOL), string("exchange|plusyaspools")));
        auto ct = now() + i;
        txn.delay_sec = ct % (60 * 60 * 24);
        txn.send(ct, _self, false);
      }
    }
    // receive dapp token
    else if (get_code() == PLUS_CONTRACT && quantity.symbol == PLUS_SYMBOL)
    // retire
    {
      plusplusplus::inline_transfer(PLUS_CONTRACT, get_self(), RETIRE_CONTRACT, quantity, string("retire"));
    }
  }
}

extern "C" {
void apply(uint64_t receiver, uint64_t code, uint64_t action)
{
  if (code != receiver && action == "transfer"_n.value)
  {
    eosio::execute_action(
        name(receiver), name(code), &plusplusplus::receive_pretreatment);
  }
}
}