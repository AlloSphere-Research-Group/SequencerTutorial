
#include "allocore/io/al_App.hpp"
#include "allocore/ui/al_Parameter.hpp"

using namespace al;

/*
	The Parameter class encapsulates a single data value that can be shared
	safely between contexts (e.g. the graphics and audio threads) and that
	can be exposed to external control.
	The parameter constructor takes the following:

	Parameter(std::string parameterName, std::string Group,
		  float defaultValue,
		  std::string prefix = "",
		  float min = -99999.0,
		  float max = 99999.0
		);

		The parameter's name is the first argument, followed by the name of the
		group it belongs to. The group is used in particular by OSC to construct
		the access address. The "prefix" is similar to group in this respect
		as is prefixes the text to the address.
*/

static Parameter X("X", "Position", 0.0, "", -1.0, 1.0);
static Parameter Y("Y", "Position", 0.0, "", -1.0, 1.0);
static Parameter Size("Scale", "Size", 1.0, "", 0.1, 3.0);

/* Once the parameters have been declared they can easily be exposed via OSC
 using the ParameterServer class. You can specify the IP Address and the
 network port to listen on.

 To register the parameters to the parameter server, you need to add them
 using the << streaming operator. See the main function below.
*/

static ParameterServer paramServer("127.0.0.1", 9010);

/* A simple App that draws a cone on the screen */

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
/*
	You can get a parameter's value using the get() member function
*/
		g.translate(X.get(), Y.get(), 0);
		g.scale(Size.get());
		g.draw(g.mesh());
		g.popMatrix();
	}
private:
	Light light;
};


int main(int argc, char *argv[])
{
/*
	Parameters need to be added to the ParameterServer by using the
	streaming operator <<.
*/

	paramServer << X << Y << Size; // Add parameters to parameter server

/*
	The print function of the ParameterServer object provides information
	on the server, including the paths to all registered parameters.

	You will see a cone on the screen. I will not move until you send OSC
	messages with values to any of these addresses:

	Parameter X : /Position/X
	Parameter Y : /Position/Y
	Parameter Scale : /Size/Scale
*/
	paramServer.print();
	MyApp().start();
	return 0;
}
