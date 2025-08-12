#include <iostream>
#include <tuple>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <queue>

int main()
{
    srand(time(NULL));

    std::vector<int> BFS_result;
    std::vector<int> BFS_queue;
    int current = 0;
    int fringe;
    bool visited = false;

    int nodes = 8;
    std::vector<std::tuple<int, int>> graph = { {5,1}, {4,1}, {1,2}, {7,2}, {2,6}, {2,3}, {5,8} };
    const int inner_pos = 0 + rand() % 2;
    const int outer_pos = 0 + rand() % graph.size();

    switch (inner_pos)
    {
        case 0:
            fringe = std::get<0>(graph[outer_pos]);
        break;
        default:
            fringe = std::get<1>(graph[outer_pos]);
        break;
    }
    BFS_result.push_back(fringe);
    while (true)
    {
        std::cout << fringe << "  ";
        for (auto& relation : graph)
        {
            if (std::get<0>(relation) == fringe)
            { 
                for (auto& node : BFS_result)
                {
                    if (node == std::get<1>(relation))
                    {
                        visited = true;
                        break;
                    }
                }
                if (!visited)
                {
                    BFS_result.push_back(std::get<1>(relation));
                }
            }
            else if (std::get<1>(relation) == fringe)
            {
                for (auto& node : BFS_result)
                {
                    if (node == std::get<0>(relation))
                    {
                        visited = true;
                        break;
                    }
                }
                if (!visited)
                {
                    BFS_result.push_back(std::get<0>(relation));
                }
            }
            visited = false;
        }
        if (current >= nodes-1)
        {
            break;
        }
        current++;
        fringe = BFS_result[current];
    }
    std::cout << std::endl;
    return 0;
}

