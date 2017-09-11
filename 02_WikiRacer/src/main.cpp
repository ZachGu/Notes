
#include <algorithm>
#include <iostream>
#include <unordered_set>
#include <vector>
#include <queue>
#include <unordered_map>
#include "wikiscraper.h"


using std::cout;            using std::endl;
using std::string;          using std::vector;
using std::priority_queue;  using std::unordered_map;
using std::unordered_set;



// A helper function to get the common number between two unordered_set
int getNumOfCommonlinks(unordered_set<string> set1, unordered_set<string> set2) {
    int count = 0;
    if (set1.size() > set2.size())
        return getNumOfCommonlinks(set2, set1);
    for (auto &it : set1) {
        if (set2.count(it))
            count++;
    }
    return count;
}


/*
 * This is the function you will be implementing. It takes
 * two string representing the names of a start_page and
 * end_page and is supposed to return a ladder, represented
 * as a vector<string>, of links that can be followed from
 * start_page to get to the end_page.
 *
 * For the purposes of this algorithm, the "name" of a Wikipedia
 * page is what shows at the end of the URL when you visit that page
 * in your web browser. For ex. the name of the Stanford University
 * Wikipedia page is "Stanford_University" since the URL that shows
 * in your browser when you visit this page is:
 *
 *       https://en.wikipedia.org/wiki/Stanford_University
 */
vector<string> findWikiLadder(const string& start_page, const string& end_page) {
    // TODO: Fill in the findWikiLinks method in wikiscraper.cpp,
    //       then write this  function as per the handout.
    //
    //                Best of luck!

    // create WikiScraper object'
    WikiScraper scraper;

    auto target_set = scraper.getLinkSet(end_page);

    // compare function
    auto cmpFn = [&target_set, &scraper] (vector<string> ladder1, vector<string> ladder2) {
        string page1 = ladder1.back();
        string page2 = ladder2.back();
        auto link_set1 = scraper.getLinkSet(page1);
        auto link_set2 = scraper.getLinkSet(page2);
        //int num1 = getNumOfCommonlinks(link_set1, target_set);
        //int num2 = getNumOfCommonlinks(link_set2, target_set);
        int num1 = 0;
        int num2 = 0;
        for (auto i : target_set) {
            if (link_set1.count(i))
                num1++;
            if (link_set2.count(i))
                num2++;
        }
        return num1 < num2;
    };

    std::priority_queue<vector<string>, vector<vector<string>>, decltype(cmpFn)> ladderQueue(cmpFn);
    vector<string> ladder;
    ladder.push_back(start_page);
    ladderQueue.push(ladder);

    unordered_set<string> visited;
    visited.insert(start_page);

    while (!ladderQueue.empty()) {
        vector<string> curr_ladder = ladderQueue.top();
        ladderQueue.pop();
        string curr_page = curr_ladder.back();

        // print the intermediate result
//        size_t size = curr_ladder.size();
//        cout << "{";
//        for(size_t i = 0; i < size - 1; i++) {
//            cout<< curr_ladder[i] << ", ";
//        }
        cout<< curr_ladder[size - 1] << "}" << endl;
        unordered_set<string> curr_links = scraper.getLinkSet(curr_page);
        if (curr_links.find(end_page) != curr_links.end()) {
            // The ladder is found
            curr_ladder.push_back(end_page);
            return curr_ladder;
        }

        //int count = 0;
        for (auto it : curr_links) {
            if (visited.count(it))
                continue;
            vector<string> new_ladder(curr_ladder);
            new_ladder.push_back(it);
            ladderQueue.push(new_ladder);
            visited.insert(it);
        //    cout << "adding the " <<++count<<"th to the queue......" << endl;
        }
    }

    return {};
}



int main() {
    auto ladder = findWikiLadder("Albert_Einstein", "Scientology");
    cout << endl;

    if(ladder.empty()) {
        cout << "No ladder found!" << endl;
    } else {
        cout << "Ladder found:" << endl;
        cout << "\t";

        // Print the ladder here!
        size_t size = ladder.size();
        for(size_t i = 0; i < size - 1; i++) {
            cout<< ladder[i] << " -> ";
        }
        cout<< ladder[size - 1] << endl;
    }

    return 0;
}




