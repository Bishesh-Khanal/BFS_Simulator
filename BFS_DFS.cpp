#include <cstdlib>
#include <ctime>
#include <queue>
#include "GameEngine.h"
#include <fstream>

int main()
{
    srand(time(NULL));

    std::vector<int> BFS;
    std::queue<int> BFS_queue;
    int fringe;
    std::vector<std::vector<int>> graph = 
    {
        {2,4,5},
        {1,3,6,7},
        {2},
        {1},
        {1,8},
        {2},
        {2},
        {5}
    };


    fringe = 1 + rand() % 8;
    std::vector<bool> visited(graph.size(), false);
    BFS.push_back(fringe);
    BFS_queue.push(fringe);
    visited[fringe - 1] = true;

    while (!BFS_queue.empty())
    {
        fringe = BFS_queue.front();
        BFS_queue.pop();
        for (auto& adj : graph[fringe - 1])
        {
            if (!visited[adj-1])
            {
                visited[adj - 1] = true;
                BFS.push_back(adj);
                BFS_queue.push(adj);
            }
        }
    }

    for (auto& node : BFS)
    {
        std::cout << node << "  ";
    }

    std::shared_ptr<GameEngine> game = std::make_shared<GameEngine>();
    game->run(game);

    return 0;
}

