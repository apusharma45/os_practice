#include<iostream>
#include<queue>
#include<vector>
#include <utility> 
#include <algorithm>

using namespace std;

queue<int> ready_queue;
int system_time = 0;
int time_quantum = 4;

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
    int term[pr];         // Completion time
    int remaining[pr];    // Remaining burst time
    bool queued[pr];      // Whether process is in queue
    int start_time[pr];   // First execution (for response time)
    bool first_response[pr];

    for (int i = 0; i < pr; i++)
    {
        term[i] = -1;
        queued[i] = false;
        remaining[i] = burst[i];
        start_time[i] = -1;
        first_response[i] = false;
    }

    sort_using_arrival(pr, arrival, burst);
    update_ready_queue(pr, arrival, term, queued);

    // Gantt chart printing
    while (!ready_queue.empty())
    {
        int idx = ready_queue.front();
        ready_queue.pop();

        if (!first_response[idx])
        {
            start_time[idx] = system_time;
            first_response[idx] = true;
        }

        int exec_time = min(time_quantum, remaining[idx]);
        int start = system_time;
        int end = system_time + exec_time;

        cout << "P" << idx << " " << start << "-" << end << "\n";

        system_time = end;
        remaining[idx] -= exec_time;
        update_ready_queue(pr, arrival, term, queued);

        if (remaining[idx] > 0)
            ready_queue.push(idx);
        else
            term[idx] = system_time;

        if (ready_queue.empty())
        {
            int next_arrival = 1e9;
            for (int i = 0; i < pr; i++)
                if (term[i] == -1 && arrival[i] > system_time)
                    next_arrival = min(next_arrival, arrival[i]);
            if (next_arrival != 1e9)
            {
                system_time = next_arrival;
                update_ready_queue(pr, arrival, term, queued);
            }
        }
    }

    // Print table of metrics
    cout << "\nProcess\tArrival\tBurst\tCompletion\tTurnaround\tWaiting\tResponse\n";
    for (int i = 0; i < pr; i++)
    {
        int tat = term[i] - arrival[i];
        int wt = tat - burst[i];
        int rt = start_time[i] - arrival[i];
        cout << "P" << i << "\t" << arrival[i] << "\t" << burst[i] << "\t" << term[i]
             << "\t\t" << tat << "\t\t" << wt << "\t" << rt << "\n";
    }
}


int main()
{
    int pr_arrival[] = {20, 0, 10};
    int pr_burst[] = {24, 4, 3};
    int num_pr = 3;
    
    schedule_and_print_gantt_chart(num_pr, pr_arrival, pr_burst);

    return 0;
}