#pragma once

#include<stdio.h>
#include<stdlib.h>
#include<vector>
#include<unordered_map>
#include <locale>         // std::locale, std::toupper

class RecipeBook
{
public:
	std::unordered_map<std::string, RecipePage> pages;

	RecipeBook() {
		 pages = std::unordered_map<std::string, RecipePage>();
		 populateBook();
	}

	void addRecipePage(RecipePage p) {
		pages.insert({ p.title, p });
	}

	void populateBook() {
		RecipePage cookie = RecipePage( std::string("COOKIE"), false);

	}

	~RecipeBook() {}

private:

};


class RecipePage
{
public:
	std::string title;
	bool inProgress;
	std::vector<std::string> instructions;

	RecipePage() {
		title = "";
		inProgress = false;
		instructions =  std::vector<std::string>();
	}

	RecipePage(std::string t, bool inprogress) {
		title = t;
		inProgress = inprogress;
		instructions = std::vector<std::string>();
	}

	RecipePage( RecipePage &p) {
		title = p.title;
		inProgress = p.inProgress;
		instructions = std::vector<std::string> (p.instructions);
	}

	void appendInstruction(std::string instr) {
		instructions.push_back(instr);
	}

	





	~RecipePage() {}

private:

};

