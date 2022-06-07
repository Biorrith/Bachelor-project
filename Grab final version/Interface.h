#pragma once
#include <iostream>
#include "MessageHandler.h";
#include <Windows.h>
#include <limits>
#include <conio.h>
//https://stackoverflow.com/questions/20446373/cin-ignorenumeric-limitsstreamsizemax-n-max-not-recognize-it?msclkid=42ce42dbd10411ec91038e1096f88c41
#undef max
#include <limits>



class Interface
{
private:
	MessageHandler* handler;
	void clearConsole();
	void printSwitch();

public:
	Interface();

	void provideUI();

	~Interface();
};

