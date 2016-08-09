
#include <fstream>

#include "allocore/io/al_App.hpp"
#include "allocore/ui/al_Parameter.hpp"
#include "allocore/ui/al_Preset.hpp"
#include "allocore/ui/al_HtmlInterfaceServer.hpp"

using namespace al;

// Parameter declaration
static Parameter X("X", "Position", 0.0, "", -1.0, 1.0);
static Parameter Y("Y", "Position", 0.0, "", -1.0, 1.0);
static Parameter Size("Scale", "Size", 1.0, "", 0.1, 3.0);
static ParameterServer parameterServer("127.0.0.1", 9010);

// Preset Handler
static PresetHandler presetHandler("sequencerPresets");
static PresetServer presetServer("127.0.0.1", 9011);

/*
	An HtmlInterfaceServer object runs interface.js as a separate process and
	connects the GUI controls bidirectionally, i.e. changes are sent by the
	interface, but also any changes received by the program by OSC or
	programatically are sent to the browser displaying the interface.

	This relies on an existing and working installation of interface.js. It
	expects interface.js to be located in ../interface.js but you can provide
	a path in the constructor.
*/

HtmlInterfaceServer interfaceServer;

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
		/* Recalling existing presets will show the changes in the html GUI */
		if (k.isNumber()) { // Recall preset
			presetHandler.recallPreset(k.keyAsNumber());
		}
	}

private:
	Light light;
};

int main(int argc, char *argv[])
{
	parameterServer << X << Y << Size;
	presetHandler << X << Y << Size;
	presetServer << presetHandler;

/*
	You can register a ParameterServer and a PresetServer object and display
	the parameters and presets in two separate auto-generated interfaces.

	Point your browser to localhost:8080. The files AlloParameters.html and
	AlloPresets.html will display the interfaces. Remember to reload if
	interface.js has restarted.
*/
	interfaceServer << parameterServer;
	interfaceServer << presetServer;

	MyApp().start();
	return 0;
}
