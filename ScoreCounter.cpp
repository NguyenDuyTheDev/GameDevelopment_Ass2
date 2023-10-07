#include "ScoreCounter.h"

ScoreCounter::ScoreCounter()
{
	p1Score = 0;
	p2Score = 0;
}

void ScoreCounter::reset()
{
	p1Score = 0;
	p2Score = 0;
}

void ScoreCounter::plusScore(int pId)
{
	if (pId == 1) {
		p1Score += 1;
	}
	if (pId == 2) {
		p2Score += 1;
	}
}
