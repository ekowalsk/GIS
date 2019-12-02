#include <limits.h>
#include <iostream>

#include "Dijkstra.h"

Dijkstra::Dijkstra(Graph pGraph): graph(pGraph){
    initState();
}

void Dijkstra::initState(){
    predecessors = new std::map<int, int>();
    priorityQueue = new std::map<int, int>();
    for (auto it = graph.edges->begin(); it != graph.edges->end(); it = graph.edges->upper_bound(it->first)){
        priorityQueue->insert(std::make_pair(it->first, INT_MAX));
        predecessors->insert(std::make_pair(it->first, NULL));
    }
}

std::pair<int, std::list<int>> Dijkstra::shortest_path(int source, int destination){
    if (!isValidVertex(source) || !isValidVertex(destination)){
        std::cerr << "Invalid source or destination vertex. " << std::endl;
        exit(-1);
    }
    clearState();
    updateDistance(source, 0);
    int currentVertex = peekPriorityQueue();
    while (currentVertex != destination) {
        updateSuccessorsDistances(currentVertex);
        eraseFromPriorityQueue(currentVertex);
        currentVertex = peekPriorityQueue();
    }
    printPath(reconstructPath(source, destination));
}

bool Dijkstra::isValidVertex(int vertex){
    return predecessors->find(vertex) != predecessors->end();
}

void Dijkstra::clearState(){
    for (auto & predecessor : *predecessors){
        predecessor.second = NULL;
        priorityQueue->insert(std::make_pair(predecessor.first, INT_MAX));
    }
}

void Dijkstra::updateDistance(int vertex, int distance){
    priorityQueue->at(vertex) = distance;
}

int Dijkstra::peekPriorityQueue(){
    int minDistance = priorityQueue->begin()->second;
    int minVertex = priorityQueue->begin()->first;
    for (auto & element : *priorityQueue){
        if (minDistance > element.second){
            minDistance = element.second;
            minVertex = element.first;
        }
    }
    return minVertex;
}

void Dijkstra::updateSuccessorsDistances(int vertex){
    auto edges = graph.getOutgoingEdges(vertex);
    for (auto edge = edges.first; edge != edges.second; edge++){
        /* graph.getOutgoingEdges(currentVertex) can return edges, that have already been deleted from priority queue */
        try{
            if (getDistance(edge->first) > getDistance(vertex) + edge->second.getWeight())
                updateDistance(edge->first, getDistance(vertex) + edge->second.getWeight());
        } catch (const std::out_of_range& e){
            continue;
        }
    }
}

int Dijkstra::getDistance(int vertex) {
    return priorityQueue->at(vertex);
}
void Dijkstra::eraseFromPriorityQueue(int vertex){
    priorityQueue->erase(vertex);
}

std::list<int> Dijkstra::reconstructPath(int source, int destination){
    std::list<int> path = std::list<int>();
    path.push_back(destination);
    int currentVertex = predecessors->at(destination);
    while(currentVertex != source){
        path.push_front(currentVertex);
        currentVertex = predecessors->at(currentVertex);
    }
    path.push_front(source);
    return path;
}

std::string Dijkstra::printPath(const std::list<int>& path){
    std::string pathString;
    for (int it : path)
        pathString += std::to_string(it) + " -> ";
    return pathString;
}

Dijkstra::~Dijkstra(){
    delete priorityQueue;
    delete predecessors;
}


