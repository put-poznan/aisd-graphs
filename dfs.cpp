#include <iostream>
#include <vector>
#include <list>
#include <algorithm>

class Digraph {
  public:
    typedef unsigned int size_type;
    typedef std::list<size_type> Neighbours;

  private:
    size_type m_nodes_count, m_vertices_count;

  public:
    Digraph(size_type nodes_count = 0) :
      m_nodes_count(nodes_count), m_vertices_count(0) {}

    virtual bool is_edge(size_type from, size_type to) = 0;
    virtual Neighbours neighbours(size_type node) = 0;

    virtual Digraph& add_vertice(size_type from, size_type to) = 0;

    size_type nodes_count() { return m_nodes_count; }
    size_type vertices_count() { return m_vertices_count; }

    virtual ~Digraph() {}
};

class AdjMatrix : public Digraph {
  private:
    typedef std::vector<bool> Row;
    std::vector<Row> m_matrix;

  public:
    AdjMatrix(size_type nodes_count) : Digraph(nodes_count) {
      m_matrix.resize(nodes_count);

      for (Row& row : m_matrix)
        row.resize(nodes_count);
    }

    virtual bool is_edge(size_type from, size_type to) override {
      return m_matrix[from][to];
    }

    virtual std::list<size_type> neighbours(size_type node) override {
      Row row = m_matrix[node];
      std::remove_if(row.begin(), row.end(), [](bool b) { return !b; });

      return std::list<size_type>(row.begin(), row.end());
    }

    virtual Digraph& add_vertice(size_type from, size_type to) override {
      m_matrix[from][to] = true;

      return *this;
    }
};

class AdjList : public Digraph {
  private:
    typedef std::list<size_type> Row;
    std::vector<Row> m_list;

  public:
    AdjList(size_type nodes_count) : Digraph(nodes_count) {
      m_list.resize(nodes_count);
    }

    virtual bool is_edge(size_type from, size_type to) override {
      return std::find(m_list[from].begin(), m_list[from].end(), to) != m_list[from].end();
    }

    virtual Digraph& add_vertice(size_type from, size_type to) override {
      m_list[from].push_back(to);
      return *this;
    }

    virtual Neighbours neighbours(size_type node) {
      return m_list[node];
    }
};

enum Color {
  WHITE, GREY, BLACK
};

struct Times {
  size_t in, out;
};

void visit_node(Digraph& graph,
    Digraph::size_type node,
    std::vector<Color>& colors,
    std::list<Digraph::size_type>& ret) {
  Digraph::Neighbours neighs = graph.neighbours(node);

  for (auto& neigh : neighs) {
    if (colors[neigh] == WHITE) {
      colors[neigh] = GREY;
      visit_node(graph, neigh, colors, ret);
    }
  }

  ret.push_front(node);
  colors[node] = BLACK;
}

std::list<Digraph::size_type> top_sort(Digraph& graph) {
  std::list<Digraph::size_type> ret;
  std::vector<Times> times;
  std::vector<Color> colors(graph.nodes_count(), WHITE);

  for (int i = 0; i < graph.nodes_count(); i++) {
    if (colors[i] == WHITE) {
      visit_node(graph, i, colors, ret);
    }
  }

  return ret;
}

int main(int argc, char* argv[]) {
  if (argc < 3) return 1;

  int size = atoi(argv[1]);

  Digraph *graph;

  if (std::string(argv[2]) == "list")
    graph = new AdjList(size);
  else
    graph = new AdjMatrix(size);

  int a, b;
  while (size--) {
    std::cin >> a >> b;
    graph->add_vertice(a, b);
  }

  std::list<Digraph::size_type> sorted = top_sort(*graph);

  for (auto& i : sorted) {
    std::cout << i << '\n';
  }

  delete graph;
}
