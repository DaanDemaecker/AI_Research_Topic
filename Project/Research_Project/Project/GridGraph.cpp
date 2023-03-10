#include "pch.h"
#include "GridGraph.h"

GridGraph::GridGraph(Grid* pGrid, bool removeConnectionsToSnake)
	:m_GridAmount{pGrid->GetGridSize()}, m_CellSize(pGrid->GetCellSize()), m_RemoveConnectionsToSnake{removeConnectionsToSnake}
{
	InitializeGrid(pGrid);
}

GridGraph::~GridGraph()
{
	for (int i{}; i < m_Nodes.size(); i++)
	{
		delete m_Nodes[i];
		m_Nodes[i] = nullptr;
	}
	m_Nodes.clear();

	for (int i{}; i < m_Connections.size(); i++)
	{
		delete m_Connections[i];
		m_Connections[i] = nullptr;
	}
	m_Connections.clear();
}

void GridGraph::InitializeGrid(Grid* pGrid)
{
	auto nodeCenters{*pGrid->GetGrid()};

	m_Nodes.resize(nodeCenters.size());

	for (int i{}; i < nodeCenters.size(); i++)
	{
		m_Nodes[i] = new Node{ i };
		m_Nodes[i]->SetPosition(nodeCenters[i]);
		AddConnectionsToAdjacentCells(i);
	}
}

void GridGraph::Render()
{
	for (auto& node : m_Nodes)
	{
		node->Render(m_CellSize);
	}

	utils::SetColor(Color4f{ 0.f, 0.f, 0.f, 1.f });
	for (auto& connection : m_Connections)
	{
		connection->Render(m_Nodes[connection->GetFrom()]->GetPosition(), m_Nodes[connection->GetTo()]->GetPosition());
	}
}

void GridGraph::Update(const Grid* pGrid, const std::deque<int>& snake)
{
	ResetGraph();

	m_Snake = snake;

	for (int i{}; i < snake.size(); i++)
	{
		int index{ snake[i] };

		m_Nodes[index]->SetType(NodeType::Snake);

		//Remove connections to all of the snake if it's necesarry , except the head
		if(i != 0 && m_RemoveConnectionsToSnake)
			RemoveConnectionssToAdjacentNodes(index);
	}

	m_Head = snake[0];
	m_Apple = pGrid->GetApplePos();

	m_Nodes[m_Apple]->SetType(NodeType::Apple);
}

void GridGraph::ResetGraph()
{
	for (int i{}; i < m_Nodes.size(); i++)
	{
		switch (m_Nodes[i]->GetNodeType())
		{
		case NodeType::Normal:
		default:
			break;
		case NodeType::Snake:
			AddConnectionsToAdjacentCells(i);
		case NodeType::Apple:
			m_Nodes[i]->SetType(NodeType::Normal);
			break;
		}
	}
}

void GridGraph::RemoveConnectionssToAdjacentNodes(int idx)
{
	for(int i{}; i < m_Connections.size(); i++)
	{
		if (m_Connections[i]->GetFrom() == idx || m_Connections[i]->GetTo() == idx)
		{
			delete m_Connections[i];
			m_Connections[i] = nullptr;
		}
	}


	m_Connections.erase(std::remove_if(m_Connections.begin(), m_Connections.end(), [](Connection* connection)
		{
			return connection == nullptr;
		}
	), m_Connections.end());
}

void GridGraph::AddConnectionsToAdjacentCells(int idx)
{
	int newIdx{};

	//If node is in upper row, don't add connection up
	if (idx >= m_GridAmount)
	{
		newIdx = idx - m_GridAmount;
		if (IsUniqueConnection(idx, newIdx))
		{
			m_Connections.push_back(new Connection{idx, newIdx});
		}
		if (IsUniqueConnection(newIdx, idx))
		{
			m_Connections.push_back(new Connection{ newIdx, idx });
		}
	}

	//If node is in right column, don't add connection to the right
	if (idx % m_GridAmount != m_GridAmount - 1)
	{
		newIdx = idx + 1;
		if (IsUniqueConnection(idx, newIdx))
		{
			m_Connections.push_back(new Connection{ idx, newIdx });
		}
		if (IsUniqueConnection(newIdx, idx))
		{
			m_Connections.push_back(new Connection{ newIdx, idx });
		}
	}

	//If node is in lower row, don't add connection down
	if (idx < m_GridAmount * (m_GridAmount - 1))
	{
		newIdx = idx + m_GridAmount;
		if (IsUniqueConnection(idx, newIdx))
		{
			m_Connections.push_back(new Connection{ idx, newIdx });
		}
		if (IsUniqueConnection(newIdx, idx))
		{
			m_Connections.push_back(new Connection{ newIdx, idx });
		}
	}

	//If node is in left column, don't add connection to the left
	if (idx % m_GridAmount != 0)
	{
		newIdx = idx - 1;
		if (IsUniqueConnection(idx, newIdx))
		{
			m_Connections.push_back(new Connection{ idx, newIdx });
		}
		if (IsUniqueConnection(newIdx, idx))
		{
			m_Connections.push_back(new Connection{ newIdx, idx });
		}
	}
}

bool GridGraph::IsUniqueConnection(int from, int to) const
{
	for (int i{}; i < m_Connections.size(); i++)
	{
		if (m_Connections[i]->GetFrom() == from)
		{
			if (m_Connections[i]->GetTo() == to)
			{
				return false;
			}
		}
	}

	return true;
}

std::vector<Connection*> GridGraph::GetNodeConnections(Node* pNode)
{
	std::vector<Connection*> connections{};

	for (auto& connection : m_Connections)
	{
		if (connection->GetFrom() == pNode->GetIndex() ||
			connection->GetTo() == pNode->GetIndex())
			connections.push_back(connection);
	}



	return connections;
}
