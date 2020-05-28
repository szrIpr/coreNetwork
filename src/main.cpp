#include <chrono>
#include <random>
#include "./NeuralNetwork.hpp"
#include "./Matrix.hpp"

#define OLC_PGE_APPLICATION
#include "./olcPixelGameEngine.h"

class Test: public olc::PixelGameEngine
{
private:
    std::default_random_engine gen;
    std::uniform_int_distribution<int> dist;

    std::vector<Matrix> inputs = {{{1.0f}, {1.0f}},
                                {{1.0f}, {0.0f}},
                                {{0.0f}, {1.0f}},
                                {{0.0f}, {0.0f}}};

    std::vector<Matrix> targets = {{{0.0f}}, {{1.0f}}, {{1.0f}}, {{0.0f}}};

    NeuralNetwork nn;

public:
	Test() : dist(0, 3), nn(2, {2}, 1)
	{
		sAppName = "Test";
	}

public:
	bool OnUserCreate() override
	{
		// Called once at the start, so create things here
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		// called once per frame
        for (int i = 0; i < 1000; i++)
        {
            int rand = dist(gen);
            nn.backward(inputs[rand], targets[rand]);
        }

	    for (int x = 0; x < ScreenWidth(); x++)
        {
			for (int y = 0; y < ScreenHeight(); y++)
            {
                float width = ScreenWidth()-1;
                float height = ScreenHeight()-1;

                Matrix in { {1.0f}, {1.0f} };
                print(in);
                Matrix out = nn.forward(in);

                uint8_t val = out[0][0] * 255.0f;
				Draw(x, y, olc::Pixel(val, val, val, 255));	
            }
        }
		return true;
	}
};

int main()
{
    Test test;
    if(test.Construct(200, 200, 4, 4))
        test.Start();

    return 0;
}