#include "buyer.hpp"

Buyer::Buyer() : percentage_(PERCENTAGE), buy_rate_thresh_(BUY_RATE_THRESH), buy_under_thresh_(BUY_UNDER_THRESH), money(INIT_MONEY){};

Buyer::Buyer(const double percentage, const double buy_rate_thresh, const double buy_under_thresh) 
: percentage_(percentage), buy_rate_thresh_(buy_rate_thresh), buy_under_thresh_(buy_under_thresh), money(INIT_MONEY){};
bool Buyer::bankruptcy() const
{
	return money < PRICE;
};
bool Buyer::buy(const Horse& horse, const std::vector<double>& win_prob) const
{
	return horse.pay_rate * win_prob[0] > buy_rate_thresh_ && win_prob[0] > buy_under_thresh_;
};
int Buyer::pay()
{
	int pay = (int)(INIT_MONEY * percentage_);
	money -= pay;
	return pay;
};
int Buyer::pay(const Horse& horse, const std::vector<double>& win_prob)
{
	int pay = (int)(INIT_MONEY * percentage_);
	money -= pay;
	return pay;
};
void Buyer::set_return(int m) { money += m; };

void Buyer::check_history() { history.push_back(money); };