#include<bits/stdc++.h>
using namespace std;

const int pages = 5;
const int frames = 3;

int page_table[pages];
bool valid[pages];

int RAM[frames];
bool frame_full[frames];

int last_used_time[frames];
int current_time;

void print_state() {
    cout << " --------------- ---------------" << endl;
    cout << "| Page Table    | Valid Bit     |" << endl;
    cout << " --------------- ---------------" << endl;
    for(int i=0; i<pages; i++) {
        cout << "|\t" << page_table[i] << "\t|\t" << valid[i] << "\t|" << endl;
    }
    cout << " --------------- ---------------" << endl;
    cout << endl;
    
    cout << " --------------- ---------------" << endl;
    cout << "| RAM           | Frame Full    |" << endl;
    cout << " --------------- ---------------" << endl;
    for(int i=0; i<frames; i++) {
        cout << "|\t" << RAM[i] << "\t|\t" << frame_full[i] << "\t|" << endl;
    }
    cout << " --------------- ---------------" << endl;
    
    cout << "Last Used Times: ";
    for(int i=0; i<frames; i++) {
        cout << last_used_time[i] << " ";
    }
    cout << endl;
    cout << "Current Time: " << current_time << endl;
    cout << endl;
}

int find_replacement_frame_lru(int requested_page) {
    int least_used = 0;
    int least_used_time = last_used_time[0];
    for(int i=1; i<frames; ++i) {
        if(last_used_time[i] < least_used_time) {
            least_used = i;
            least_used_time = last_used_time[i];
        }
    }
    int current_page = RAM[least_used];
    valid[current_page] = 0;
    page_table[current_page] = -1;
    return least_used;
}

void handle_request(int requested_page) {
    if(valid[requested_page]) {
        int frame = page_table[requested_page];
        cout << "Cache hit. Found page " << requested_page << " in frame " << frame << endl;
        ++current_time;
        last_used_time[frame] = current_time;
        print_state();
        return;
    }
    else {
        cout << "cache missed!" << endl;
        cout << "finding available frame..." << endl;
        for (int i = 0; i < frames; i++) {
            if (frame_full[i] == 0) {
                cout << "Found available frame. Frame no: " << i << endl;
                ++current_time;
                last_used_time[i] = current_time;
                RAM[i] = requested_page;
                frame_full[i] = 1;
                page_table[requested_page] = i;
                valid[requested_page] = 1;    
                print_state();
                return;
            }
        }
        cout << "No available frame!" << endl;
        int replaced_frame = find_replacement_frame_lru(requested_page);
        cout << "Frame " << replaced_frame << " is replaced and page " << requested_page << " is loaded" << endl;
        ++current_time;
        last_used_time[replaced_frame] = current_time;
        RAM[replaced_frame] = requested_page;
        frame_full[replaced_frame] = 1;
        page_table[requested_page] = replaced_frame;
        valid[requested_page] = 1;    
        print_state();
    }
}
void initialize() {
    for(int i = 0; i < pages; i++) {
        page_table[i] = -1;
        valid[i] = false;
    }
    
    for(int i = 0; i < frames; i++) {
        RAM[i] = -1;
        frame_full[i] = false;
        last_used_time[i] = 0;
    }
    
    current_time = 0;
}

int main() {
    
    initialize();
    
    handle_request(1);
    handle_request(2);
    handle_request(3);
    handle_request(1);  // Cache hit
    handle_request(4);  // Should cause LRU replacement
    return 0;
}
