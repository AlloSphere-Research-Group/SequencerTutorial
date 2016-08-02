
#include "allocore/io/al_App.hpp"
#include "allocore/ui/al_Parameter.hpp"

using namespace al;

// Parameter declaration
static Parameter X("X", "Position", 0.0, "", -1.0, 1.0);
static Parameter Y("Y", "Position", 0.0, "", -1.0, 1.0);
static Parameter Size("Scale", "Size", 1.0, "", 0.1, 3.0);
// ParameterServer declaration
static ParameterServer paramServer("127.0.0.1", 9010);

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
 	You can set the values of parameters using the set() function or
	you can assign a value directly to the parameter object. In this case
	the keys '[' and ']' will change the X parameter (the x position of the
	cone).

	Note that values are set here and can also be set using OSC through the
	parameter server.

	Also notice that the Parameter object will clamp the values within the
	minimum and maximum values set in the constructor.
*/
	virtual void onKeyDown(const Keyboard& k)
	{
		if (k.key() == '[') {
			X = X.get() - 0.1;
		} else if (k.key() == ']') {
 			X = X.get() + 0.1;
		}
	}
private:
	Light light;
};


int main(int argc, char *argv[])
{
	paramServer << X << Y << Size;
/*
 	You can add an OSC listener to a parameter server. Any change to any
	parameter will be broadcast to the OSC address and port registered using the
	addListener() function.
*/
	paramServer.addListener("127.0.0.1", 13560);
	MyApp().start();
	return 0;
}
