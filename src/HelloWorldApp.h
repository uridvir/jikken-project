#ifndef INCLUDED_HELLOWORLDAPP_H
#define INCLUDED_HELLOWORLDAPP_H

// The HelloWorldApp class. This class shows a window
// containing a statusbar with the text "Hello World"
class HelloWorldApp : public wxApp
{
public:
	virtual bool OnInit();
};

DECLARE_APP(HelloWorldApp)

/*
int main(int argc, char** argv) { 
	std::cout << "Hello World!" << std::endl;
	return 0;
}
*/

#endif // INCLUDED_HELLOWORLDAPP_H