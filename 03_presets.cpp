
#include "allocore/io/al_App.hpp"
#include "allocore/ui/al_Parameter.hpp"
#include "allocore/ui/al_Preset.hpp"
#include "allocore/math/al_Random.hpp"

using namespace al;

// Parameter declaration
static Parameter X("X", "Position", 0.0, "", -1.0, 1.0);
static Parameter Y("Y", "Position", 0.0, "", -1.0, 1.0);
static Parameter Size("Scale", "Size", 1.0, "", 0.1, 3.0);

/*
	A PresetHandler object groups parameters and stores presets for them.

	Parameters are registered using the streaming operator.

	You need to specify the path where presets will be stored as the first
	argument to the constructor.

	A PresetHandler can store and recall presets using the storePreset() and
	recallPreset() functions. When a preset is recalled, the values are
	gradually "morphed" (i.e. interpolated linearly) until they reach their
	destination. The time of this morph is set using the setMorphTime()
	function.
*/
static PresetHandler presetHandler("sequencerPresets");

static rnd::Random<> randomGenerator;

class MyApp : public App
{
public:
	MyApp() {
		initWindow(Window::Dim(800,600), "Parameters");
		nav().pos(Vec3d(0,0,8));
		addCone(graphics().mesh());
	}

	virtual void onDraw(Graphics &g) override
	{
		g.lighting(true);
		light();
		g.pushMatrix();
		g.translate(X.get(), Y.get(), 0);
		g.scale(Size.get());
		g.draw(g.mesh());
		g.popMatrix();
	}
/*
	The keyboard is used here to store and recall presets, and also to
	randomize the parameter values. See instructions below.
*/
	virtual void onKeyDown(const Keyboard& k)
	{
		std::string presetName = std::to_string(k.keyAsNumber());
		if (k.alt()) {
			if (k.isNumber()) { // Store preset
				presetHandler.storePreset(presetName);
				std::cout << "Storing preset:" << presetName << std::endl;
			}
		} else {
			if (k.isNumber()) { // Recall preset
				presetHandler.recallPreset(presetName);
				std::cout << "Recalling preset:" << presetName << std::endl;
			} else if (k.key() == ' ') { // Randomize parameters
	 			X = randomGenerator.uniformS();
		 		Y = randomGenerator.uniformS();
		 		Size = 0.1 + randomGenerator.uniform() * 2.0;
			}

		}
	}
private:
	Light light;
};

/*
	To use this application, press the space bar to randomize the position and
	size of the cone. You can then store presets by pressing the alt (option)
	key together with a number. Try storing multiple different postions and
	sizes on different numbers.

	To recall a preset just press a number. The position and size will morph
	to the preset values.
*/

int main(int argc, char *argv[])
{
/*
	To register Parameters with a PresetHandler, you use the streaming
	operator, just as you did for the ParameterServer.
*/
	presetHandler << X << Y << Size;
	presetHandler.setMorphTime(2.0); // Presets will take 2 seconds to "morph"
	MyApp().start();
	return 0;
}
