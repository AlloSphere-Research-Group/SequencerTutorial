
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

/*
	A PresetSequencer object can read sequences that trigger presets over time.
*/
static PresetSequencer presetSequencer;

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
	To play a sequence, call the playSequence() function of a presetSequencer
	object. The argument to the function is the name of the sequence. Although
	the file on disk is called "demo.sequence", you don't need to add the
	extension, so you can pass "demo" as the name, and the extension is added
	automatically.

	Only a single sequence can be playing at a time on a PresetSequencer object.
	If you call playSequence() again and the sequence hasn't finished, the
	running sequence will be stopped and the new one will be started.
*/
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
/*
	After you group parameters together in a preset handler, you need to
	register it with the sequencer. The PresetSequencer object reads sequence
	files and then commands the PresetHandler to load them at the right times.
*/
	presetSequencer << presetHandler;

/*
	Now, we'll create a sequence file. This assumes that you have created
	at least three presets called 1,2 and 3 (You can create them with the
	earlier programs in this tutorial).

	Sequence files are stored in a simple plain text format. Each line
	corresponds to a command lo load a preset, with the preset name first,
	followed by the morph time and the wait time, separated by colons (without
	any spaces).

	The morph time in this case determines the time it takes to get to the
	preset and the wait time determines the time to wait after the morph has
	completed.

	For example, the sequence below sets preset "1" immediately (no morph time)
	and keeps it for 3 seconds. Then it takes 4 seconds to morph to preset "2".
	After these 4 seconds, there is a 2 second wait, after which it will take 1
	second to get to preset "3".

	The double colons ("::") mark the end of the sequence.
*/
	std::string presetFileText = R"(1:0.0:3.0
2:4.0:2.0
3:1.0:1.0
::
)";

/*
	The PresetSequencer will use the same path as the PresetHandler it is
	related to. You can query the PresetHandler's path using the getCurrentPath()
	function.
*/

	std::ofstream ofs(presetHandler.getCurrentPath() + std::string("demo.sequence"), std::ofstream::out);
    ofs << presetFileText; // Write the text to file
	ofs.close();

	MyApp().start();
	return 0;
}
