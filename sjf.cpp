#include<iostream>
#include<queue>
#include<vector>
#include <utility> 
#include <algorithm>

using namespace std;

queue<int> ready_queue;
int system_time = 0;

void sort_using_arrival(int num_pr, int arrival[], int burst[])
{
	// Create a vector of pairs
	vector<pair<int, int>> combined_arrays;
	for (size_t i = 0; i < num_pr; ++i) {
	combined_arrays.push_back({arrival[i], burst[i]});
	}

	// Sort the vector of pairs based on the first element (reference_array values)
	sort(combined_arrays.begin(), combined_arrays.end());
	// sort(combined_arrays.begin());

	// Update the original arrays with sorted values
	for (size_t i = 0; i < combined_arrays.size(); ++i) {
	arrival[i] = combined_arrays[i].first;
	burst[i] = combined_arrays[i].second;
	}
}

void update_ready_queue(int pr, int arrival[], int term[], bool queued[])
{
	for (int i = 0; i < pr; i++) {
		if (arrival[i] <= system_time && !queued[i] && term[i] == -1) {
			ready_queue.push(i);
			queued[i] = true;
		}
	}
}

void schedule_and_print_gantt_chart(int pr, int arrival[], int burst[]) 
{
    int wt[pr], tt[pr], rt[pr];
    bool done[pr] = {false};
    int completed = 0;
    int system_time = 0;

    for (int i = 0; i < pr; i++) {
        wt[i] = 0;
        tt[i] = 0;
        rt[i] = -1;
    }

    sort_using_arrival(pr, arrival, burst);

    while (completed < pr) {
        int idx = -1;
        int min_burst = 1e9;

        for (int i = 0; i < pr; i++) {
            if (!done[i] && arrival[i] <= system_time && burst[i] < min_burst) {
                min_burst = burst[i];
                idx = i;
            }
        }

        if (idx == -1) {  // No process has arrived yet
            system_time++;
            continue;
        }

        if (rt[idx] == -1) rt[idx] = system_time;

        cout << "P" << idx << " " << system_time << "-" << system_time + burst[idx] << endl;

        wt[idx] = system_time - arrival[idx];
        system_time += burst[idx];
        tt[idx] = system_time - arrival[idx];
        done[idx] = true;
        completed++;
    }

    cout << "\nProcess\tWT\tTT\tRT\n";
    for (int i = 0; i < pr; i++)
        cout << "P" << i << "\t" << wt[i] << "\t" << tt[i] << "\t" << rt[i] << "\n";
}


int main()
{
    int pr_arrival[] = {20, 0, 10};
    int pr_burst[] = {24, 4, 3};
    int num_pr = 3;
    
    schedule_and_print_gantt_chart(num_pr, pr_arrival, pr_burst);

    return 0;
}