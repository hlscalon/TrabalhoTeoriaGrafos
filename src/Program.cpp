#include <iostream>
#include <memory>

#include "Program.hpp"
#include "Functions.hpp"
#include "GraphGeneratorFile.hpp"
#include "GraphGeneratorConsole.hpp"

#include "algorithms/Algorithm.hpp"
#include "algorithms/Isomorphism.hpp"
#include "algorithms/BFS.hpp"
#include "algorithms/DFS.hpp"
#include "algorithms/BiconnectedComponents.hpp"
#include "algorithms/ConnectedComponents.hpp"

void Program::run()
{
	bool exit = false;
	while(!exit) 
	{
		this->showInitialMenu();
		exit = this->processDataInitialMenu();
	}
	std::cout << "Programa encerrado\n";
}

void Program::showInitialMenu() const
{
	std::cout 
		<< "|-----------------------------------------------------|\n"
		<< "|               Trabalho de grafos                    |\n"
		<< "|-----------------------------------------------------|\n"
		<< "|" << static_cast<char>(Option::A) << "- Identificação de isomorfismo entre dois grafos |\n"
		<< "|" << static_cast<char>(Option::B) << "- Busca em amplitude e profundidade |\n"
		<< "|" << static_cast<char>(Option::Quit) << "- Sair |\n"
		<< "|-----------------------------------------------------|\n"
	;
}

bool Program::processDataInitialMenu()
{
	char option;
	while (!(std::cin >> option) || 
		   !(((option >= static_cast<char>(Option::A) && option <= static_cast<char>(Option::H)) || option == static_cast<char>(Option::Quit))))
		functions::readInputClear();

	if (option == static_cast<char>(Option::Quit))
		return true;

	this->runInner(static_cast<Option>(option));

	return false;
}

void Program::runInner(Option option)
{
	this->showInnerMenu(option);

	int optionInner;
	while (!(std::cin >> optionInner) || (static_cast<ReadFrom>(optionInner) != ReadFrom::File && static_cast<ReadFrom>(optionInner) != ReadFrom::Console))
		functions::readInputClear();

	this->processDataInnerMenu(option, static_cast<ReadFrom>(optionInner));
}

void Program::showInnerMenu(Option option) const
{
	std::cout << "|-----------------------------------------------------|\n";
	if (option == Option::A) 
	{
		std::cout 
			<< "| " << static_cast<int>(ReadFrom::File) << " - Ler 2 arquivos com grafos, separados por espaço |\n"
		    << "| Ex. grafo1.g grafo2.g |\n";
	}
	else
	{
		std::cout << "| " << static_cast<int>(ReadFrom::File) << " - Ler arquivo com grafo Ex. grafo.g |\n";
	}

	std::cout
		<< "| " << static_cast<int>(ReadFrom::Console) << " - Inserir dados no console |\n"
		<< "|-----------------------------------------------------|\n"
	;
}

void Program::processDataInnerMenu(Option option, ReadFrom optionInner)
{
	if (optionInner == ReadFrom::File)
	{
		this->processDataFromFile(option);
	}
	else if (optionInner == ReadFrom::Console)
	{
		this->processDataFromConsole(option);
	}
	else
	{
		std::cout << "Opção inválida.\n";
	}
}

void Program::processDataFromFile(Option option)
{
	std::string fileGraph1;
	if (option == Option::A) 
	{
		std::string fileGraph2;
		while ((std::cout << "Digite o arquivo dos dois grafo (ie. grafo1.g grafo2.g): ") && !(std::cin >> fileGraph1 >> fileGraph2))
			functions::readInputClear();

		try
		{
			GraphGeneratorFile graphGenerator1{fileGraph1};
			GraphGeneratorFile graphGenerator2{fileGraph2};
			this->runAlgorithm(option, graphGenerator1.getGraph(), graphGenerator2.getGraph());
		}
		catch (const std::runtime_error & e)
		{
			std::cout << e.what() << "\n";
		}
	}
	else
	{
		while ((std::cout << "Digite o nome do arquivo do grafo (ie. grafo1.g): ") && !(std::cin >> fileGraph1))
			functions::readInputClear();

		try
		{
			GraphGeneratorFile graphGenerator1{fileGraph1};
			this->runAlgorithm(option, graphGenerator1.getGraph());
		}
		catch (const std::runtime_error & e)
		{
			std::cout << e.what() << "\n";
		}
	}
}

void Program::processDataFromConsole(Option option)
{
	if (option == Option::A) 
	{
		GraphGeneratorConsole graphGenerator1{};
		GraphGeneratorConsole graphGenerator2{};

		this->runAlgorithm(option, graphGenerator1.getGraph(), graphGenerator2.getGraph());
	}
	else
	{
		GraphGeneratorConsole graphGenerator1{};
		
		this->runAlgorithm(option, graphGenerator1.getGraph());
	}
}

void Program::runAlgorithm(Option option, const Graph & graph1, const Graph & graph2)
{
	if (option == Option::A)
	{
		algorithms::Isomorphism isomorphism{graph1, graph2};
		isomorphism.run();
		std::cout << "Resultado isomorfismo\n";
		isomorphism.showResults();
	}
}

void Program::runAlgorithm(Option option, const Graph & graph1)
{
	switch (option)
	{
		case Option::B:
			{
				int initialVertex;
				while (((std::cout << "Digite o vértice inicial: ") && !(std::cin >> initialVertex)) || initialVertex < 1)
					functions::readInputClear();

				--initialVertex;  // zero based

				algorithms::BFS bfs{graph1, initialVertex};
				bfs.run();
				
				std::cout << "Resultado bfs\n";
				bfs.showResults();

				algorithms::DFS dfs{graph1, initialVertex};
				dfs.run();

				std::cout << "Resultado dfs\n";
				dfs.showResults();
			}
			break;
		case Option::C:
		case Option::D:
		case Option::E:
		case Option::F:
		case Option::G:
		case Option::H:
		case Option::A: // do nothing
		case Option::Quit: // do nothing
			break;
	}
}
