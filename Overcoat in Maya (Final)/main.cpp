#include <stdio.h>
#include <conio.h>
#include <math.h>
#include <iostream>
#include "LSystem.h"

int main(int argc, char **argv)
{
    LSystem system;
    system.loadProgramFromString("F\nF->F[+F]F[-F]F");
    system.setDefaultAngle(25.7f);
    system.setDefaultStep(1.0f);
    for (int i = 0; i < 2; i++)
    {
        std::string insn = system.getIteration(i);
        std::cout << insn << std::endl;

        std::vector<LSystem::Branch> branches;
        system.process(i, branches);
    }

	getch();
    return 0;      
}
