
#include <fstream>

#include "allocore/io/al_App.hpp"
#include "allocore/ui/al_Parameter.hpp"
#include "allocore/ui/al_Preset.hpp"
#include "allocore/ui/al_PresetSequencer.hpp"

using namespace al;

// Parameter declaration
static Parameter X("X", "Position", 0.0, "", -1.0, 1.0);
static Parameter Y("Y", "Position", 0.0, "", -1.0, 1.0);
static Parameter Size("Scale", "Size", 1.0, "", 0.1, 3.0);

// Preset Handler
static PresetHandler presetHandler("sequencerPresets");
// Sequencer
static PresetSequencer presetSequencer;
/*
	A PresetSequencer can be exposed via OSC to trigger sequences through a
	SequenceServer object.
*/
static SequenceServer sequenceServer("127.0.0.1", 9012);

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
		if (k.key() == ' ') {
			presetSequencer.playSequence("demo");
		}
	}
private:
	Light light;
};

int main(int argc, char *argv[])
{
	presetHandler << X << Y << Size;
	presetSequencer << presetHandler;
	// This sequence file was written by the previous example. Uncomment below
	// to get it in case its missing.
// 	std::string presetFileText = R"(1:0.0:3.0
// 2:4.0:2.0
// 3:1.0:1.0
// ::
// )";
// 	std::ofstream ofs(presetHandler.getCurrentPath() + std::string("demo.sequence"), std::ofstream::out);
//     ofs << presetFileText; // Write the text to file
// 	ofs.close();

/*
	A preset sequencer is exposed via OSC through a SequenceServer object.
	By default it accepts a string with the name of the sequence to play on the
	OSC address /sequence.
	To play the "demo.sequence" sequence, send an OSC message like:
	/sequence demo

	The OSC address can be changed with the setAddress() function.
*/
	sequenceServer << presetSequencer;
/*
 	Show the details from the sequence server. In this case it prints:
Sequence server listening on: 127.0.0.1:9012
Communicating on path: /sequence
*/
	sequenceServer.print();

	MyApp().start();
	return 0;
}
