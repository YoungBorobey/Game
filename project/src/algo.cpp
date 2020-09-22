#include <limits>
#include <algorithm>
#include <stdexcept>
#include <exception>
#include <queue>
#include "game.h"



template <typename T>
class Pr_Queue {
 private:
  std::vector<std::pair<T, int>> Data;
  std::map<T, size_t> Position;
  size_t size;

 public:
    Pr_Queue() : size(0) {
        Data.resize(0);
    }

    size_t left(size_t index) {
        return index * 2 + 1;
    }

    size_t right(size_t index) {
        return index * 2 + 2;
    }

    size_t parent(size_t index) {
        return (index - 1) / 2;
    }


    void heap_down(T elem) {
        size_t pos_elem = Position[elem], pos_left = left(pos_elem);
        size_t pos_right = right(pos_elem);
        if (pos_left < size) {
            T left_el = Data[pos_left].first;
            if (pos_right >= size) {
                if (Data[pos_elem].second > Data[pos_left].second) {
                    Position[elem] = pos_left;
                    Position[left_el] = pos_elem;
                    std::swap(Data[pos_elem], Data[pos_left]);
                    heap_down(elem);
                }
                return;
            }
            T right_el = Data[pos_right].first;
            if (Data[pos_right].second < Data[pos_left].second) {
                left_el = right_el;
                pos_left = pos_right;
            }
            if (Data[pos_elem].second > Data[pos_left].second) {
                Position[elem] = pos_left;
                Position[left_el] = pos_elem;
                std::swap(Data[pos_elem], Data[pos_left]);
                heap_down(elem);
            }
        }
    }

    void heap_up(T elem) {
        size_t pos_elem = Position[elem], pos_par = parent(pos_elem);
        if (pos_par < size && Data[pos_elem].second < Data[pos_par].second) {
            Position[elem] = pos_par;
            Position[Data[pos_par].first] = pos_elem;
            std::swap(Data[pos_elem], Data[pos_par]);
            heap_up(elem);
        }
    }

    void insert(T elem, int priority) {
        size_t curr_size = size;
        ++size;
        if (Data.size() <= curr_size) {
            Data.resize(size);
        }
        Data[curr_size] = std::make_pair(elem, priority);
        Position[elem] = curr_size;
        heap_up(elem);
    }

    std::pair<T, int> extract_min() {
        size_t pos = size - 1;
        Position.erase(Data[0].first);
        Position[Data[pos].first] = 0;
        auto ans = Data[0];
        std::swap(Data[0], Data[pos]);
        --size;
        heap_down(Data[0].first);
        return ans;
    }

    bool decrease_key(T elem, int new_value) {
        if (Position.find(elem) != Position.end() && Data[Position[elem]].second > new_value) {
            Data[Position[elem]].second = new_value;
            heap_up(elem);
            return true;
        }
        return false;
    }

    bool is_empty() const {
        return size == 0;
    }
};


int get_node_numb(const T_MAP& DATA, size_t pos_x, size_t pos_y) {
  return static_cast<int>(pos_y * DATA[0].size() + pos_x);
}

std::pair<size_t, size_t> get_pos(int node_numb, const T_MAP& DATA) {
  return std::make_pair(static_cast<size_t>(node_numb) % DATA[0].size(), static_cast<size_t>(node_numb) / DATA[0].size());
}


std::vector<std::vector<std::pair<int, int>>> get_graph(const T_MAP& DATA, const C_PLAYER& player) {
    std::vector<std::vector<std::pair<int, int>>> graph;
    graph.reserve(DATA.size() * DATA[0].size());
    for (size_t i = 0; i < DATA.size(); ++i) {
        for (size_t j = 0; j < DATA[0].size(); ++j) {
            std::vector<std::pair<int, int>> curr;
            int dist = 1;
            if (DATA[i][j].is_enemy_here()) {
                dist += (DATA[i][j].get_enemy().get_hp() + player.get_dmg() - 1) / player.get_dmg();
            }
            if (i + 1 < DATA.size()) {
                curr.push_back(std::make_pair(get_node_numb(DATA, j, i + 1), dist));
            }
            if (i >= 1) {
                curr.push_back(std::make_pair(get_node_numb(DATA, j, i - 1), dist));
            }
            if (j + 1 < DATA[0].size()) {
                curr.push_back(std::make_pair(get_node_numb(DATA, j + 1, i), dist));
            }
            if (j >= 1) {
                curr.push_back(std::make_pair(get_node_numb(DATA, j - 1, i), dist));
            }
            graph.push_back(curr);
        }
    }
    return graph;
}

std::vector<std::pair<int, int>> find_distance(const std::vector<std::vector<std::pair<int, int>>>& graph, int start, int finish) {
    std::vector<std::pair<int, int>> distance(graph.size(), {std::numeric_limits<int>::max(), -1});
    Pr_Queue<int> queue = Pr_Queue<int>();
    queue.insert(start, 0);
    for (size_t i = 0; i < graph.size(); ++i) {
        if (i != static_cast<size_t>(start)) {
            queue.insert(i, std::numeric_limits<int>::max());
        }
    }
    if (finish == -1) {
        while (!queue.is_empty()) {
            auto curr = queue.extract_min();
            distance[curr.first].first = curr.second;
            for (size_t i = 0; i < graph[curr.first].size(); ++i) {
                if (queue.decrease_key(graph[curr.first][i].first, curr.second + graph[curr.first][i].second)) {
                    distance[graph[curr.first][i].first].second = curr.first;
                }
            }
        }
    } else {
        while (!queue.is_empty() && distance[finish].first == std::numeric_limits<int>::max()) {
            auto curr = queue.extract_min();
            distance[curr.first].first = curr.second;
            for (size_t i = 0; i < graph[curr.first].size(); ++i) {
                if (queue.decrease_key(graph[curr.first][i].first, curr.second + graph[curr.first][i].second)) {
                    distance[graph[curr.first][i].first].second = curr.first;
                }
            }
        }
    }
    return distance;
}

std::optional<std::vector<std::pair<int, int>>> nodes_way(const std::vector<std::vector<std::pair<int, int>>>& graph, int start, int finish) {
    std::optional<std::vector<std::pair<int, int>>> ans;
    if (start == finish) {
        ans.emplace(std::vector<std::pair<int, int>>());
    }
    auto dist = find_distance(graph, start, finish);
    if (dist[finish].first != std::numeric_limits<int>::max()) {
        std::vector<std::pair<int, int>> way;
        int curr = finish;
        while (curr != start) {
            int next_node = dist[curr].second;
            int curr_distance;
            for (size_t i = 0; i < graph[next_node].size(); ++i) {
                if (graph[next_node][i].first == curr) {
                    curr_distance = graph[next_node][i].second;
                }
            }
            way.push_back(std::make_pair(curr, curr_distance));
            curr = next_node;
        }
        way.push_back(std::make_pair(start, 0));
        std::reverse(way.begin(), way.end());
        ans.emplace(way);
    }
    return ans;
}

std::string get_move(const std::pair<size_t, size_t>& start_pos, const std::pair<size_t, size_t>& finish_pos) {
    if (start_pos.first + 1 == finish_pos.first) {
        return "move right";
    }
    if (start_pos.first - 1 == finish_pos.first) {
        return "move left";
    }
    if (start_pos.second + 1 == finish_pos.second) {
        return "move up";
    }
    return "move down";
}


std::vector<std::string> action_list(const std::vector<std::pair<int, int>>& node_sequence, const T_MAP& DATA) {
    std::vector<std::string> ans;
    for (size_t i = 0; i + 1 < node_sequence.size(); ++i) {
        if (node_sequence[i + 1].second != 1) {
            for (int j = 0; j + 1 < node_sequence[i + 1].second; ++j) {
                ans.push_back("kick enemy");
            }
        }
        ans.push_back(get_move(get_pos(node_sequence[i].first, DATA), get_pos(node_sequence[i + 1].first, DATA)));
    }
    return ans;
}

std::optional<std::vector<std::string>> try_fast_travel(const T_MAP& DATA, const C_PLAYER& player,
        const std::vector<std::vector<std::pair<int, int>>>& graph, const std::pair<int, int>& destination) {
    if (destination.first < 0 || destination.second < 0 || static_cast<size_t>(destination.first) >= DATA[0].size()
    || static_cast<size_t>(destination.second) >= DATA.size()) {
        throw std::runtime_error("invalid destination");
    }
    std::optional<std::vector<std::string>> ans;
    auto try_nodes_way = nodes_way(graph, get_node_numb(DATA, player.get_x(), player.get_y()),
    get_node_numb(DATA, destination.first, destination.second));
    if (try_nodes_way) {
        ans.emplace(action_list(try_nodes_way.value(), DATA));
    }
    return ans;
}



void fix_graph(std::vector<std::vector<std::pair<int, int>>>& graph, size_t x, size_t y, const T_MAP& DATA) {
    int node = get_node_numb(DATA, x, y);
    for (size_t i = 0; i < graph[node].size(); ++i) {
        graph[node][i].second = 1;
    }
}



std::pair<size_t, size_t> get_closest_enemy(const C_PLAYER& player, const std::vector<std::vector<std::pair<int, int>>>& graph, const T_MAP& Map) {   // assuming, that such one exist
    int player_pos = get_node_numb(Map, player.get_x(), player.get_y());
    std::queue<int> node_queue;
    node_queue.push(player_pos);
    std::map<int, bool> visited;
    while (!node_queue.empty()) {
        int curr_node = node_queue.front();
        visited[curr_node] = true;
        node_queue.pop();
        for (size_t i = 0; i < graph[curr_node].size(); ++i) {
            if (!visited[graph[curr_node][i].first]) {
                node_queue.push(graph[curr_node][i].first);
            }
            if (graph[curr_node][i].second != 1) {
                return get_pos(curr_node, Map);
            }
        }
    }
    throw std::runtime_error("Game error");
}
