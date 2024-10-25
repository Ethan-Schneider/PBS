/* Copyright (C) Jiaoyang Li
* Unauthorized copying of this file, via any medium is strictly prohibited
* Confidential
* Written by Jiaoyang Li <jiaoyanl@usc.edu>, May 2020
*/

/*driver.cpp
* Solve a MAPF instance on 2D grids.
*/
#include <boost/program_options.hpp>
#include <boost/tokenizer.hpp>
#include "PBS.h"


/* Main function */
int main(int argc, char** argv)
{
	namespace po = boost::program_options;
	// Declare the supported options.
	po::options_description desc("Allowed options");
	desc.add_options()
		("help", "produce help message")

		// params for the input instance and experiment settings
		("map,m", po::value<string>()->required(), "input file for map")
		("agents,a", po::value<string>()->required(), "input file for agents")
		("output,o", po::value<string>(), "output file for statistics")
		("outputPaths", po::value<string>(), "output file for paths")
		("agentNum,k", po::value<int>()->default_value(0), "number of agents")
		("cutoffTime,t", po::value<double>()->default_value(7200), "cutoff time (seconds)")
		("screen,s", po::value<int>()->default_value(1), "screen option (0: none; 1: results; 2:all)")
		("stats", po::value<bool>()->default_value(false), "write to files some detailed statistics")

		("sipp", po::value<bool>()->default_value(1), "using SIPP as the low-level solver")
		;
	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);

	if (vm.count("help")) {
		cout << desc << endl;
		return 1;
	}

	po::notify(vm);

	srand((int)time(0));

	///////////////////////////////////////////////////////////////////////////
	// load the instance
	Instance instance(vm["map"].as<string>(), vm["agents"].as<string>(),
		vm["agentNum"].as<int>());

	srand(0);
    PBS pbs(instance, vm["sipp"].as<bool>(), vm["screen"].as<int>());
    // run
    double runtime = 0;
	cout << "cutoffTime: " << vm["cutoffTime"].as<double>() << endl; 
    pbs.solve(vm["cutoffTime"].as<double>());
    if (vm.count("output"))
        pbs.saveResults(vm["output"].as<string>(), vm["agents"].as<string>());
    if (pbs.solution_found && vm.count("outputPaths"))
        pbs.savePaths(vm["outputPaths"].as<string>());
    /*size_t pos = vm["output"].as<string>().rfind('.');      // position of the file extension
    string output_name = vm["output"].as<string>().substr(0, pos);     // get the name without extension
    cbs.saveCT(output_name); // for debug*/
    pbs.clearSearchEngines();

	return 0;

}

vector<vector<tuple<int,int>>> pymain(string& map, int k, int t, double suboptimality, std::vector<std::tuple<int, int>> agent_start_locations = {}, std::vector<std::tuple<int, int>> agent_goal_locations = {})
{
	// TODO: Update Instance load map and agents file, 
	// Instance: Loadagentmap, loadagentlocations,etc. '
	string agent_dummy_file = "dummy";
	string agent_actual_file = "random-32-32-20-random-1.scen";

	srand((int)time(0));
	Instance instance(map, agent_actual_file, k, agent_start_locations, agent_goal_locations);
	srand(0);

	PBS pbs(instance, 0, 0);

	// run
	double runtime = 0;
	pbs.solve(60);

	vector<vector<tuple<int,int>>> paths;
	if (pbs.solution_found)
	{
		paths = pbs.returnPaths();
	}
	else
	{
		paths = {};
		cout << "Unable to find solution for goals: " << endl;
		// Print agent_start locations
		cout << "Agent Start Locations" << endl;
		for (std::tuple<int, int> i: agent_start_locations)
		{
			cout << "(" << get<0>(i) << ", " << get<1>(i) << ")" << endl;
		}

		cout << "Agent Goal Locations" << endl;
		// Print agent_goal locations
		for (std::tuple<int, int> i: agent_goal_locations)
		{
			cout << "(" << get<0>(i) << ", " << get<1>(i) << ")" << endl;
		}
	}
	pbs.clearSearchEngines();
	return paths;
}