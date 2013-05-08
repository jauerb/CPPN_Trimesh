#include "JGTL_Random.h"
#include <iostream>
#include <string>
#include <cstdio>
#include <cstdlib>

using namespace JGTL;
using namespace std;

int main()
{
	for(int a=2;a<20;a++)
	{
		Random RNG(a/2);

		cout << RNG.getRandomInt(1) << ", ";
		cout << RNG.getRandomInt(1) << ", ";
		cout << RNG.getRandomInt(1) << ", ";
		cout << RNG.getRandomInt(1) << ", ";
		cout << RNG.getRandomInt(1) << ", ";

		cout << RNG.getRandomInt(10) << ", ";
		cout << RNG.getRandomInt(10) << ", ";
		cout << RNG.getRandomInt(10) << ", ";
		cout << RNG.getRandomInt(10) << ", ";
		cout << RNG.getRandomInt(10) << ", ";

		cout << RNG.getRandomInt(100) << ", ";
		cout << RNG.getRandomInt(100) << ", ";
		cout << RNG.getRandomInt(100) << ", ";
		cout << RNG.getRandomInt(100) << ", ";
		cout << RNG.getRandomInt(100) << ", ";
		cout << endl;

		cout << RNG.getRandomWithinRange(1,100) << ", ";
		cout << RNG.getRandomWithinRange(1,100) << ", ";
		cout << RNG.getRandomWithinRange(1,100) << ", ";
		cout << RNG.getRandomWithinRange(1,100) << ", ";
		cout << RNG.getRandomWithinRange(1,100) << ", ";

		cout << RNG.getRandomWithinRange(100,100) << ", ";
		cout << RNG.getRandomWithinRange(100,100) << ", ";
		cout << RNG.getRandomWithinRange(100,100) << ", ";
		cout << RNG.getRandomWithinRange(100,100) << ", ";
		cout << RNG.getRandomWithinRange(100,100) << ", ";

		cout << RNG.getRandomWithinRange(10,100) << ", ";
		cout << RNG.getRandomWithinRange(10,100) << ", ";
		cout << RNG.getRandomWithinRange(10,100) << ", ";
		cout << RNG.getRandomWithinRange(10,100) << ", ";
		cout << RNG.getRandomWithinRange(10,100) << ", ";
		cout << endl;

		cout << RNG.getRandomDouble() << ", ";
		cout << RNG.getRandomDouble() << ", ";
		cout << RNG.getRandomDouble() << ", ";
		cout << RNG.getRandomDouble() << ", ";
		cout << RNG.getRandomDouble() << ", ";

		cout << RNG.getRandomDouble(0.0,1.0) << ", ";
		cout << RNG.getRandomDouble(0.0,1.0) << ", ";
		cout << RNG.getRandomDouble(0.0,1.0) << ", ";
		cout << RNG.getRandomDouble(0.0,1.0) << ", ";
		cout << RNG.getRandomDouble(0.0,1.0) << ", ";

		cout << RNG.getRandomDouble(0.0,10.0) << ", ";
		cout << RNG.getRandomDouble(0.0,10.0) << ", ";
		cout << RNG.getRandomDouble(0.0,10.0) << ", ";
		cout << RNG.getRandomDouble(0.0,10.0) << ", ";
		cout << RNG.getRandomDouble(0.0,10.0) << ", ";

		cout << RNG.getRandomDouble(0.0,100.0) << ", ";
		cout << RNG.getRandomDouble(0.0,100.0) << ", ";
		cout << RNG.getRandomDouble(0.0,100.0) << ", ";
		cout << RNG.getRandomDouble(0.0,100.0) << ", ";
		cout << RNG.getRandomDouble(0.0,100.0) << ", ";
		cout << endl;

		cout << RNG.rollD2() << ", ";
		cout << RNG.rollD2() << ", ";
		cout << RNG.rollD2() << ", ";
		cout << RNG.rollD2() << ", ";
		cout << RNG.rollD2() << ", ";

		cout << RNG.rollD4() << ", ";
		cout << RNG.rollD4() << ", ";
		cout << RNG.rollD4() << ", ";
		cout << RNG.rollD4() << ", ";
		cout << RNG.rollD4() << ", ";

		cout << RNG.rollD6() << ", ";
		cout << RNG.rollD6() << ", ";
		cout << RNG.rollD6() << ", ";
		cout << RNG.rollD6() << ", ";
		cout << RNG.rollD6() << ", ";

		cout << RNG.rollD8() << ", ";
		cout << RNG.rollD8() << ", ";
		cout << RNG.rollD8() << ", ";
		cout << RNG.rollD8() << ", ";
		cout << RNG.rollD8() << ", ";

		cout << RNG.rollD10() << ", ";
		cout << RNG.rollD10() << ", ";
		cout << RNG.rollD10() << ", ";
		cout << RNG.rollD10() << ", ";
		cout << RNG.rollD10() << ", ";

		cout << RNG.rollD20() << ", ";
		cout << RNG.rollD20() << ", ";
		cout << RNG.rollD20() << ", ";
		cout << RNG.rollD20() << ", ";
		cout << RNG.rollD20() << ", ";

		cout << RNG.rollD100() << ", ";
		cout << RNG.rollD100() << ", ";
		cout << RNG.rollD100() << ", ";
		cout << RNG.rollD100() << ", ";
		cout << RNG.rollD100() << ", ";
		cout << endl;

		cout << RNG.rollDice(3,6) << ", ";
		cout << RNG.rollDice(3,6) << ", ";
		cout << RNG.rollDice(3,6) << ", ";
		cout << RNG.rollDice(3,6) << ", ";
		cout << RNG.rollDice(3,6) << ", ";

		cout << RNG.rollDice(3,6,2) << ", ";
		cout << RNG.rollDice(3,6,2) << ", ";
		cout << RNG.rollDice(3,6,2) << ", ";
		cout << RNG.rollDice(3,6,2) << ", ";
		cout << RNG.rollDice(3,6,2) << ", ";
		cout << endl;

		cout << RNG.getNormalDistributionNumber(1.0,-3.0,3.0) << ", ";
		cout << RNG.getNormalDistributionNumber(1.0,-3.0,3.0) << ", ";
		cout << RNG.getNormalDistributionNumber(1.0,-3.0,3.0) << ", ";
		cout << RNG.getNormalDistributionNumber(1.0,-3.0,3.0) << ", ";
		cout << RNG.getNormalDistributionNumber(1.0,-3.0,3.0) << ", ";
		cout << RNG.getNormalDistributionNumber(1.0,-3.0,3.0) << ", ";
		cout << RNG.getNormalDistributionNumber(1.0,-3.0,3.0) << ", ";
		cout << RNG.getNormalDistributionNumber(1.0,-3.0,3.0) << ", ";
		cout << RNG.getNormalDistributionNumber(1.0,-3.0,3.0) << ", ";
		cout << RNG.getNormalDistributionNumber(1.0,-3.0,3.0) << ", ";
		cout << endl;

		cout << endl;
	}

	system("PAUSE");

	return 0;
}