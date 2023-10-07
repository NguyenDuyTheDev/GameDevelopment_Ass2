#pragma once

class ScoreCounter
{
public:
    //Initializes variables
    ScoreCounter();

    //actions
    void reset();
    void plusScore(int pId);

    //render
    void render();

private:
    //The clock time when the timer started
    int p1Score, p2Score;
};