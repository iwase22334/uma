#ifndef BUYER_HPP
#define BUYER_HPP
#include <vector>
#include <list>
#include <algorithm>
#include "../analyzer/DataStructure.hpp"
#include "../linear/linear.hpp"

struct Buyer{
public:
	static constexpr double BUY_RATE_THRESH = 2.0;
	static constexpr double BUY_UNDER_THRESH = 0.10;
	static constexpr int INIT_MONEY = 1000000;
	static constexpr unsigned int PRICE = 100;
	static constexpr unsigned int BUY_NUM = 100;
	static constexpr double PERCENTAGE = 0.01;
private:
	double percentage_;
	double buy_rate_thresh_;
	double buy_under_thresh_;

public:
	int money;
	std::list<int> history;

	std::list<int> win_history;
	std::list<int> pay_history;

public:
	Buyer();
	Buyer(const double percentage, const double buy_rate_thresh, const double buy_under_thresh);
	bool buy(const Horse& horse, const std::vector<double>& win_prob) const;
	bool bankruptcy() const;
	int pay();
	int pay(const Horse& horse, const std::vector<double>& win_prob);
	void set_return(int m);
	void check_history();
};
#endif
