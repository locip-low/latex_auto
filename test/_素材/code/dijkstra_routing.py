# dijkstra_routing.py - Dijkstra shortest path for traffic rerouting
import heapq

def dijkstra(graph, start):
    """
    Dijkstra algorithm for single-source shortest path
    graph: adjacency list {node: [(neighbor, weight), ...]}
    """
    distances = {node: float('inf') for node in graph}
    distances[start] = 0
    previous = {node: None for node in graph}
    pq = [(0, start)]

    while pq:
        curr_dist, curr_node = heapq.heappop(pq)

        if curr_dist > distances[curr_node]:
            continue

        for neighbor, weight in graph[curr_node]:
            distance = curr_dist + weight
            if distance < distances[neighbor]:
                distances[neighbor] = distance
                previous[neighbor] = curr_node
                heapq.heappush(pq, (distance, neighbor))

    return distances, previous

# Example: road network graph
road_network = {
    'A': [('B', 5), ('C', 3)],
    'B': [('A', 5), ('D', 2)],
    'C': [('A', 3), ('D', 6), ('E', 4)],
    'D': [('B', 2), ('C', 6), ('E', 1), ('F', 8)],
    'E': [('C', 4), ('D', 1), ('F', 3)],
    'F': [('D', 8), ('E', 3)]
}

# Find shortest paths from intersection A
dists, prevs = dijkstra(road_network, 'A')
print(f'Shortest distances from A: {dists}')