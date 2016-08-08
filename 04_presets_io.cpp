
#include "allocore/io/al_App.hpp"
#include "allocore/ui/al_Parameter.hpp"
#include "allocore/ui/al_Preset.hpp"
#include "allocore/math/al_Random.hpp"

using namespace al;

// Parameter declaration
static Parameter X("X", "Position", 0.0, "", -1.0, 1.0);
static Parameter Y("Y", "Position", 0.0, "", -1.0, 1.0);
static Parameter Size("Scale", "Size", 1.0, "", 0.1, 3.0);

// Preset Handler
static PresetHandler presetHandler("sequencerPresets");

/*
	A PresetServer exposes the preset handler via OSC. You provide the
	address and port on which to listen.
*/
static PresetServer presetServer("127.0.0.1", 9011);

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

	virtual void onKeyDown(const Keyboard& k)
	{
		std::string presetName = std::to_string(k.keyAsNumber());
		if (k.alt()) {
			if (k.isNumber()) { // Store preset
				presetHandler.storePreset(k.keyAsNumber(), presetName);
				std::cout << "Storing preset:" << presetName << std::endl;
			}
		} else {
			if (k.isNumber()) { // Recall preset
				presetHandler.recallPreset(k.keyAsNumber());
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

int main(int argc, char *argv[])
{
	presetHandler << X << Y << Size;
	presetHandler.setMorphTime(2.0); // Presets will take 2 seconds to "morph"
/*
	You need to register the PresetHandler object into the PresetServer
	in order to expose it via OSC.

	By default, a PresetServer will listen on OSC path "/preset", although this
	can be changed calling the setAddress() function. The OSC message must
	contain a float or an int value prroving the preset index.

	You can also change the morph time by sending a float value to OSC address
	"/preset/morphTime".
*/
	presetServer << presetHandler;
/*
	Adding a listener to a preset server makes the listener receive notification
	of any preset or morph time changes.
*/
	presetServer.addListener("127.0.0.1", 9050);

/*
	The PresetServer print() function gives details about the PresetServer
	configuration. It will print something like:

Preset server listening on: 127.0.0.1:9011
Communicating on path: /preset
Registered listeners:
127.0.0.1:9050
*/
	presetServer.print();
	MyApp().start();
	return 0;
}
