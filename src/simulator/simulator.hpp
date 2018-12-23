#ifndef SIMULATOR_HPP
#define SIMULATOR_HPP
#include <random>
#include <vector>
#include <list>
#include <algorithm>
#include "../analyzer/DataStructure.hpp"
#include "../analyzer/statistics.hpp"
#include "../linear/linear.hpp"

class Simulator{
private:
	static const int maximum_horse_num;
	static std::random_device seed_gen;
	static std::mt19937 rand_engine;

private:
	unsigned int simulation_num_;
	unsigned int horse_num_;
	Race race_;
	std::vector<std::vector<double> > win_rate_table_;

public:
	//----------------------------------------------------------
	// constructor
	// Parameters:
	// 		num 	simulatino num
	//		win_list 	win horse's value list
	//		lose_list	lose horse's value list
	//----------------------------------------------------------
	Simulator(const unsigned int num,
				const Race& r,
				const std::list<la::Vec2d>& win_list,
				const std::list<la::Vec2d>& lose_list);
	void set_simulation_num(const unsigned int num);
	//----------------------------------------------------------
	// 		montecarlo simulaton
	//		Predict the order probability
	// Parameters:
	// 		place_prob 	return value. probability array
	//					A 1 2 3 4 ...
	//					B 1 2 3 4 ...
	//					. . .
	//----------------------------------------------------------
	void run(std::vector<std::vector<double> > & place_prob);

    /**
     * get probability of race order for umaren
     * @tparam T number of return value
     * @param order_prob [description]
     * @param place_prob [description]
     */
    template<int T>
    static void get_umatan_prob(
        std::vector< OrderProbability<2> >& order_prob,
        const std::vector< std::vector<double> >& place_prob)
    {
        static_assert(T > 0, "number of array elements must greater than 0");
        assert(place_prob.size() > 0);
        assert(place_prob.size() * place_prob.size() >= T);

        std::list< OrderProbability<2> > order_list;

        for (int i = 0; i < place_prob.size(); ++ i){
            double prob_sum = 0;

            // calculate secont rank probability
            for (int j = 0; j < place_prob.size(); ++ j) {
                if (j != i) {
                    prob_sum += place_prob[j][1];
                }
            }

            for (int j = 0; j < place_prob.size(); ++ j) {
                if (j != i) {
                    order_list.push_back(
                        OrderProbability<2>(
                            {{i, j}},
                            place_prob[i][0] * (place_prob[j][1] / prob_sum)
                        )
                    );
                }
            }

        }

        order_list.sort();
        order_list.reverse();

        // copy result
        auto it = order_list.begin();
        for(int i = 0; i < T; ++ i) {
            order_prob.push_back(*it);
            ++ it;
        }

    };

    /**
     * get probability of race order for umatan
     * @param order_prob return value
     * @param place_prob
     */
    template<int T>
    static void get_umaren_prob(
        std::vector< OrderProbability<2> >& order_prob,
        const std::vector< std::vector<double> >& place_prob)
    {
        static_assert(T > 0, "number of array elements must greater than 0");
        assert(place_prob.size() > 0);
        assert(place_prob.size() * place_prob.size() / 2 >= T);

        std::list< OrderProbability<2> > order_list;

        for (int i = 0; i < place_prob.size(); ++ i){
            double prob_sum = 0;

            // calculate secont rank probability
            for (int j = 0; j < place_prob.size(); ++ j) {
                if (j != i) {
                    prob_sum += place_prob[j][1];
                }
            }

            for (int j = 0; j < place_prob.size(); ++ j) {
                if (j > i) {
                    order_list.push_back(
                        OrderProbability<2>(
                            {{i, j}},
                            place_prob[i][0] * (place_prob[j][1] / prob_sum) * 2));
                }
            }

        }

        order_list.sort();
        order_list.reverse();

        // copy result
        auto it = order_list.begin();
        for(int i = 0; i < T; ++ i) {
            order_prob.push_back(*it);
            ++ it;
        }

    };
private:
	//----------------------------------------------------------
	// make_win_rate_table
	//		win
	// Parameters:
	// 		win_rate_table 	return value. probability array
	//					   A   B   C   D
	//					A  -  0.3 0.3 0.4 ...
	//					B 0.7  -  0.5 0.7 ...
	//					. . .
	//		win_list 	win horse's value list
	//		lose_list	lose horse's value list
	//----------------------------------------------------------
	void make_win_rate_table(const Race& race,
							 const std::list<la::Vec2d>& win_list,
							 const std::list<la::Vec2d>& lose_list);
	// Used in run() compete by win_rate
	bool compete(const double win_rate) const;
	// replace front and behind
	void swap(std::vector<int>& place, const unsigned int i) const;
	void random_start(std::vector<int>& place) const;
	void check_place(std::vector<int>& place,
				 const std::vector<int>& win_num_vector) const;
};
#endif
