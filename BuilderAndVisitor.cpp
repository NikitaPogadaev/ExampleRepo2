
#include <algorithm>
#include <iostream>
#include <limits>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace Graphs {

using ClassicVertex = size_t;
using ClassicEdgeData = size_t;

template <typename Vertex, typename EdgeData>
class Graph {
public:
    void AddEdge(Vertex from, Vertex to, EdgeData edge) {
        edges_.push_back({to, edge});
        edgesOut_[from].push_back(edges_.size() - 1);
    }

    std::vector<size_t> OutgoingEdges(Vertex vert) const {
        auto it = edgesOut_.find(vert);
        if (it != edgesOut_.end()) {
            return it->second;
        }
        return {};
    }

    Vertex GetTarget(size_t edge_id) const { return edges_[edge_id].to; }

    const EdgeData& GetEdgeData(size_t edge_id) const {
        return edges_[edge_id].edge;
    }

private:
    struct Edge {
        Vertex to;
        EdgeData edge;
    };
    std::vector<Edge> edges_;
    std::unordered_map<Vertex, std::vector<size_t>> edgesOut_;
};

template <typename Vertex, typename Edge, typename Predicate>
class FilteredGraph {
public:
    FilteredGraph(const Graph<Vertex, Edge>& graph, Predicate predicate)
        : graph_(graph), predicate_(predicate) {}

    std::vector<size_t> OutgoingEdges(Vertex vert) const {
        std::vector<size_t> result;
        for (auto edge_id : graph_.OutgoingEdges(vert)) {
            if (predicate_(graph_.GetEdgeData(edge_id))) {
                result.push_back(edge_id);
            }
        }
        return result;
    }

    Vertex GetTarget(size_t edge_id) const { return graph_.GetTarget(edge_id); }

private:
    const Graph<Vertex, Edge>& graph_;
    Predicate predicate_;
};

template <typename Vertex>
class BFSVisitor {
public:
    virtual ~BFSVisitor() = default;
    virtual void DiscoverVertex(Vertex vert) = 0;
    virtual void ExamineVertex(Vertex vert) = 0;
    virtual void ExamineEdge(size_t edge_id) = 0;
};

template <class Vertex, class Graph, class Visitor>
void BreadthFirstSearch(Vertex root, const Graph& graph, Visitor& visitor) {
    std::queue<Vertex> queue;
    std::unordered_set<Vertex> discovered;

    queue.push(root);
    discovered.insert(root);
    visitor.DiscoverVertex(root);

    while (!queue.empty()) {
        Vertex cur = queue.front();
        queue.pop();
        visitor.ExamineVertex(cur);

        for (const auto& edge_id : graph.OutgoingEdges(cur)) {
            visitor.ExamineEdge(edge_id);
            Vertex target = graph.GetTarget(edge_id);
            if (discovered.find(target) == discovered.end()) {
                discovered.insert(target);
                visitor.DiscoverVertex(target);
                queue.push(target);
            }
        }
    }
}
}  // namespace Graphs

namespace FlowNetwork {

class Builder;

class FlowNetwork {
public:
    struct EdgePayload {
        int64_t capacity;
        int64_t flow;
    };

    friend class Builder;

    int64_t GetResidualCapacity(size_t edge_id) const {
        return edges_[edge_id].capacity - edges_[edge_id].flow;
    }

    void PushFlow(size_t edge_id, int64_t delta) {
        edges_[edge_id].flow += delta;
        edges_[edge_id ^ 1].flow -= delta;
    }

    auto ResidualNetworkView() const {
        auto predicate = [this](size_t edge_id) {
            return this->GetResidualCapacity(edge_id) > 0;
        };
        return Graphs::FilteredGraph<size_t, size_t, decltype(predicate)>(
            graph_, predicate);
    }

    int64_t GetFlow(size_t edge_id) const { return edges_[edge_id].flow; }

private:
    FlowNetwork(Graphs::Graph<size_t, size_t>&& graph,
                std::vector<EdgePayload>&& edges)
        : graph_(std::move(graph)), edges_(std::move(edges)) {}

    Graphs::Graph<size_t, size_t> graph_;
    std::vector<EdgePayload> edges_;
};

class Builder {
public:
    Builder() {}

    Builder& AddEdge(size_t from, size_t to, int64_t capacity) {
        graph_.AddEdge(from, to, edges_.size());
        edges_.push_back({capacity, 0});
        graph_.AddEdge(to, from, edges_.size());
        edges_.push_back({0, 0});
        return *this;
    }

    FlowNetwork Build() {
        return FlowNetwork(std::move(graph_), std::move(edges_));
    }

private:
    Graphs::Graph<size_t, size_t> graph_;
    std::vector<FlowNetwork::EdgePayload> edges_;
};
}  // namespace FlowNetwork

class EdmondsKarpVisitor : public Graphs::BFSVisitor<size_t> {
public:
    EdmondsKarpVisitor(size_t sink) : sink_(sink), ended_(false) {}

    void DiscoverVertex(size_t vert) override {
        if (vert == sink_) {
            ended_ = true;
        }
    }

    void ExamineVertex(size_t /*vert*/) override {}

    void ExamineEdge(size_t edge_id) override {
        if (!ended_) {
            edges_.push_back(edge_id);
        }
    }

    bool Found() const { return ended_; }
    const std::vector<size_t>& GetEdgeCalls() const { return edges_; }

private:
    size_t sink_;
    bool ended_;
    std::vector<size_t> edges_;
};

class EdmondsKarp {
public:
    EdmondsKarp(FlowNetwork::FlowNetwork& network, size_t source, size_t sink)
        : network_(network), source_(source), sink_(sink) {}

    int64_t ComputeMaxFlow() {
        int64_t max_flow = 0;
        while (true) {
            auto [resPath, paths] = NewFlowPath();
            if (!resPath) {
                break;
            }
            int64_t min_cap = std::numeric_limits<int64_t>::max();
            for (auto edge_id : paths) {
                min_cap =
                    std::min(min_cap, network_.GetResidualCapacity(edge_id));
            }

            for (auto edge_id : paths) {
                network_.PushFlow(edge_id, min_cap);
            }
            max_flow += min_cap;
        }
        return max_flow;
    }

private:
    std::pair<bool, std::vector<size_t>> NewFlowPath() {
        EdmondsKarpVisitor visitor(sink_);
        BreadthFirstSearch(source_, network_.ResidualNetworkView(), visitor);

        if (!visitor.Found()) {
            return {false, {}};
        }

        std::unordered_map<size_t, size_t> edge_to;
        auto residual_network = network_.ResidualNetworkView();
        for (auto edge_id : visitor.GetEdgeCalls()) {
            size_t target = residual_network.GetTarget(edge_id);
            if (edge_to.find(target) == edge_to.end() && target != source_) {
                edge_to[target] = edge_id;
            }
        }

        std::vector<size_t> paths;
        size_t current = sink_;
        while (current != source_) {
            size_t edge_id = edge_to[current];
            paths.push_back(edge_id);
            current = residual_network.GetTarget(edge_id ^ 1);
        }
        std::reverse(paths.begin(), paths.end());
        return {true, paths};
    }
    FlowNetwork::FlowNetwork& network_;
    size_t source_;
    size_t sink_;
};

// 3. Binary search utility
/**
 Бинарный поиск на отрезке [l, r], если ни на одном элементе предикат
 не выдает true, возвращается right. Для элементов больше либо равно результата
 поиска предикат выдает true, при меньших - false.
 */
template <typename TT, typename Predicate>
TT BinarySearch(TT left, TT right, Predicate predicate) {
    while (right > left + 1) {
        TT mid = left + (right - left) / 2;
        if (predicate(mid)) {
            right = mid;
        } else {
            left = mid;
        }
    }
    if (predicate(left)) {
        return left;
    }
    return right;
}

class RelocationSolver {
public:
    RelocationSolver(size_t len, const std::vector<int64_t>& gold,
                     const std::vector<std::pair<size_t, size_t>>& graph)
        : size_(len), gold_(gold), graph_(graph) {}

    int64_t Solve() {
        int64_t left = 0;
        int64_t right = *std::max_element(gold_.begin(), gold_.end());

        return BinarySearch(left, right, [this](int64_t max_load) {
            return TryMaxFlow(max_load);
        });
    }

private:
    bool TryMaxFlow(int64_t max_load) {
        FlowNetwork::Builder builder;
        size_t source = size_;
        size_t sink = size_ + 1;
        int64_t need_flow = 0;

        for (size_t i = 0; i < size_; ++i) {
            if (gold_[i] > max_load) {
                int64_t delta = gold_[i] - max_load;
                need_flow += delta;
                builder.AddEdge(source, i, delta);
            }
        }

        for (size_t i = 0; i < size_; ++i) {
            if (gold_[i] < max_load) {
                builder.AddEdge(i, sink, max_load - gold_[i]);
            }
        }
        for (const auto& [a, b] : graph_) {
            builder.AddEdge(a, b, std::numeric_limits<int64_t>::max());
        }

        FlowNetwork::FlowNetwork network = builder.Build();
        EdmondsKarp ek(network, source, sink);
        int64_t max_flow = ek.ComputeMaxFlow();

        return max_flow == need_flow;
    }
    size_t size_;
    std::vector<int64_t> gold_;
    std::vector<std::pair<size_t, size_t>> graph_;
};

RelocationSolver Reader(std::istream& input) {
    size_t vertice_size;
    size_t edge_size;
    input >> vertice_size >> edge_size;
    std::vector<int64_t> gold(vertice_size);
    for (size_t i = 0; i < vertice_size; ++i) {
        input >> gold[i];
    }
    std::vector<std::pair<size_t, size_t>> graph(edge_size);
    for (size_t i = 0; i < edge_size; ++i) {
        input >> graph[i].first >> graph[i].second;
        --graph[i].first;
        --graph[i].second;
    }
    return RelocationSolver(vertice_size, gold, graph);
}

int main() {
    auto solver = Reader(std::cin);
    std::cout << solver.Solve() << '\n';

    return 0;
}